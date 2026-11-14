// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

namespace ctle
{
namespace detail
{
	struct process_handle_attributes
	{
		std::vector<uint8_t> storage;
		LPPROC_THREAD_ATTRIBUTE_LIST value = nullptr;

		process_handle_attributes() = default;
		process_handle_attributes( const process_handle_attributes & ) = delete;
		process_handle_attributes &operator=( const process_handle_attributes & ) = delete;

		~process_handle_attributes()
		{
			if( value )
				DeleteProcThreadAttributeList( value );
		}

		status initialize( HANDLE *handles, size_t count )
		{
			SIZE_T bytes = 0;
			InitializeProcThreadAttributeList( nullptr, 1, 0, &bytes );
			if( bytes == 0 )
				return status::cant_allocate;
			this->storage.resize( bytes );
			auto attributes = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>( this->storage.data() );
			if( !InitializeProcThreadAttributeList( attributes, 1, 0, &bytes ) )
				return status::cant_allocate;
			this->value = attributes;
			if( !UpdateProcThreadAttribute( value, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
				handles, sizeof(HANDLE) * count, nullptr, nullptr ) )
				return status::cant_access;
			return status::ok;
		}
	};

	status redirect_process_stream( windows_handle_ref &parent, windows_handle_ref &child,
		bool connected, bool input )
	{
		SECURITY_ATTRIBUTES security = { sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE };
		if( !connected )
		{
			child.reset( CreateFileW( L"NUL", input ? GENERIC_READ : GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_WRITE, &security, OPEN_EXISTING, 0, nullptr ) );
			return child.get_handle() != INVALID_HANDLE_VALUE ? status::ok : status::cant_open;
		}
		HANDLE read_handle = nullptr;
		HANDLE write_handle = nullptr;
		if( !CreatePipe( &read_handle, &write_handle, &security, 0 ) )
			return status::cant_allocate;
		windows_handle_ref read_end( read_handle );
		windows_handle_ref write_end( write_handle );
		parent = std::move( input ? write_end : read_end );
		child = std::move( input ? read_end : write_end );
		if( !SetHandleInformation( parent.get_handle(), HANDLE_FLAG_INHERIT, 0 ) )
			return status::cant_access;
		return status::ok;
	}
}

std::string get_current_executable_path()
{
	std::vector<wchar_t> buffer( MAX_PATH );
	DWORD size = 0;
	for( ;; )
	{
		size = GetModuleFileNameW( nullptr, buffer.data(), (DWORD)buffer.size() );
		if( size < buffer.size() )
			break;
		if( size == 0 )
		{
			ctLogError << "get_current_executable_path: GetModuleFileNameW failed: "
				<< GetLastError() << ctLogEnd;
			return {};
		}
		buffer.resize( buffer.size() * 2 );
	}
	std::string path;
	if( !wstring_to_string( std::wstring( buffer.data(), size ), path ) )
		return {};
	for( auto &character : path )
	{
		if( character == '\\' )
			character = '/';
	}
	return path;
}

status process::os_data::start( const std::vector<std::string> &arguments,
	const std::string &directory, bool input, bool output, bool error )
{
	std::string command_line;
	for( const auto &argument : arguments )
	{
		if( !command_line.empty() )
			command_line += ' ';
		command_line += quote_string_t<quote_string_options::whitespace>( argument );
	}
	std::wstring command;
	std::wstring working_directory;
	if( !string_to_wstring( command_line, command ) || !string_to_wstring( directory, working_directory ) )
		return status::invalid_param;

	windows_handle_ref stdin_read;
	windows_handle_ref stdout_write;
	windows_handle_ref stderr_write;
	ctStatusCall( detail::redirect_process_stream( this->stdin_write, stdin_read, input, true ) );
	ctStatusCall( detail::redirect_process_stream( this->stdout_read, stdout_write, output, false ) );
	ctStatusCall( detail::redirect_process_stream( this->stderr_read, stderr_write, error, false ) );

	HANDLE handles[] = { stdin_read.get_handle(), stdout_write.get_handle(), stderr_write.get_handle() };
	detail::process_handle_attributes attributes;
	ctStatusCall( attributes.initialize( handles, 3 ) );
	STARTUPINFOEXW startup = {};
	startup.StartupInfo.cb = sizeof(startup);
	startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
	startup.StartupInfo.hStdInput = handles[0];
	startup.StartupInfo.hStdOutput = handles[1];
	startup.StartupInfo.hStdError = handles[2];
	startup.lpAttributeList = attributes.value;
	PROCESS_INFORMATION information = {};
	if( !CreateProcessW( nullptr, &command[0], nullptr, nullptr, TRUE,
		EXTENDED_STARTUPINFO_PRESENT, nullptr, working_directory.c_str(),
		&startup.StartupInfo, &information ) )
	{
		ctLogError << "process::start: CreateProcessW failed: " << GetLastError() << ctLogEnd;
		return status::cant_access;
	}
	this->process_handle.reset( information.hProcess );
	windows_handle_ref main_thread( information.hThread );
	return status::ok;
}

status process::os_data::wait( std::chrono::milliseconds time_out, int &exit_code )
{
	const DWORD milliseconds = time_out == std::chrono::milliseconds::max()
		? INFINITE : (DWORD)std::min( time_out, std::chrono::milliseconds( UINT_MAX - 1 ) ).count();
	const DWORD result = WaitForSingleObject( this->process_handle.get_handle(), milliseconds );
	if( result == WAIT_TIMEOUT )
		return status::not_ready;
	if( result != WAIT_OBJECT_0 )
	{
		ctLogError << "process::wait: WaitForSingleObject failed: " << GetLastError() << ctLogEnd;
		return status::undefined_error;
	}
	DWORD code = 0;
	if( !GetExitCodeProcess( this->process_handle.get_handle(), &code ) )
	{
		ctLogError << "process::wait: GetExitCodeProcess failed: " << GetLastError() << ctLogEnd;
		return status::undefined_error;
	}
	exit_code = (int)code;
	this->process_handle.reset();
	return status::ok;
}

status process::os_data::terminate( int exit_code )
{
	if( !TerminateProcess( this->process_handle.get_handle(), exit_code ) )
	{
		ctLogError << "process::terminate: TerminateProcess failed: " << GetLastError() << ctLogEnd;
		return status::cant_access;
	}
	return status::ok;
}

void process::os_data::shutdown() noexcept
{
	if( this->process_handle.get_handle() == INVALID_HANDLE_VALUE )
		return;
	if( WaitForSingleObject( this->process_handle.get_handle(), 0 ) == WAIT_TIMEOUT )
		TerminateProcess( this->process_handle.get_handle(), 0 );
	WaitForSingleObject( this->process_handle.get_handle(), INFINITE );
	this->process_handle.reset();
}

value_return<size_t> process::os_data::read_pipe( const pipe_ref &pipe, uint8_t *data, size_t size )
{
	DWORD count = 0;
	if( !ReadFile( pipe.get_handle(), data, (DWORD)std::min<size_t>( size, MAXDWORD ), &count, nullptr ) )
	{
		if( GetLastError() == ERROR_BROKEN_PIPE )
			return size_t( 0 );
		return status::cant_read;
	}
	return (size_t)count;
}

value_return<size_t> process::os_data::write_pipe( const pipe_ref &pipe, const uint8_t *data, size_t size )
{
	DWORD count = 0;
	if( !WriteFile( pipe.get_handle(), data, (DWORD)std::min<size_t>( size, MAXDWORD ), &count, nullptr ) )
		return status::cant_write;
	return (size_t)count;
}
}
// namespace ctle
