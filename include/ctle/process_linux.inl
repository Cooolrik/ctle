// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

namespace ctle
{
namespace detail
{
	struct process_spawn_actions
	{
		posix_spawn_file_actions_t value;
		const int error = posix_spawn_file_actions_init( &value );

		process_spawn_actions() = default;
		process_spawn_actions( const process_spawn_actions & ) = delete;
		process_spawn_actions &operator=( const process_spawn_actions & ) = delete;

		~process_spawn_actions()
		{
			if( !error )
				posix_spawn_file_actions_destroy( &value );
		}
	};

	status redirect_process_stream( posix_spawn_file_actions_t &actions, int target,
		linux_file_ref &parent, linux_file_ref &child, bool connected, bool input )
	{
		if( !connected )
		{
			return posix_spawn_file_actions_addopen( &actions, target, "/dev/null",
				input ? O_RDONLY : O_WRONLY, 0 ) == 0 ? status::ok : status::cant_access;
		}

		int descriptors[2];
		if( pipe2( descriptors, O_CLOEXEC ) != 0 )
			return status::cant_allocate;
		linux_file_ref read_end( descriptors[0] );
		linux_file_ref write_end( descriptors[1] );
		for( auto *end : { &read_end, &write_end } )
		{
			// A parent may have closed a standard stream. Keep pipe endpoints above fd 2.
			if( end->get_handle() <= STDERR_FILENO )
			{
				int descriptor = fcntl( end->get_handle(), F_DUPFD_CLOEXEC, STDERR_FILENO + 1 );
				if( descriptor == -1 )
					return status::cant_allocate;
				end->reset( descriptor );
			}
		}
		parent = std::move( input ? write_end : read_end );
		child = std::move( input ? read_end : write_end );
		if( posix_spawn_file_actions_adddup2( &actions, child.get_handle(), target ) != 0
			|| posix_spawn_file_actions_addclose( &actions, child.get_handle() ) != 0 )
			return status::cant_access;
		return status::ok;
	}
}

std::string get_current_executable_path()
{
	std::vector<char> buffer( 1024 );
	ssize_t length = 0;
	for( ;; )
	{
		length = readlink( "/proc/self/exe", buffer.data(), buffer.size() - 1 );
		if( (size_t)length < buffer.size() )
			break;
		if( length == -1 )
		{
			ctLogError << "get_current_executable_path: readlink failed: " << errno << ctLogEnd;
			return {};
		}
		buffer.resize( buffer.size() * 2 );
	}
	buffer[length] = '\0';
	return std::string( buffer.data(), length );
}

status process::os_data::start( const std::vector<std::string> &arguments,
	const std::string &directory, bool input, bool output, bool error )
{
	std::vector<char *> argv;
	argv.reserve( arguments.size() + 1 );
	for( const auto &argument : arguments )
	{
		// posix_spawnp does not modify argv strings despite the mutable C signature.
		argv.push_back( const_cast<char *>( argument.c_str() ) );
	}
	argv.push_back( nullptr );

	detail::process_spawn_actions actions;
	if( actions.error )
		return status::cant_allocate;
	if( posix_spawn_file_actions_addchdir_np( &actions.value, directory.c_str() ) != 0 )
		return status::cant_access;
	linux_file_ref stdin_read;
	linux_file_ref stdout_write;
	linux_file_ref stderr_write;
	ctStatusCall( detail::redirect_process_stream( actions.value, STDIN_FILENO,
		this->stdin_write, stdin_read, input, true ) );
	ctStatusCall( detail::redirect_process_stream( actions.value, STDOUT_FILENO,
		this->stdout_read, stdout_write, output, false ) );
	ctStatusCall( detail::redirect_process_stream( actions.value, STDERR_FILENO,
		this->stderr_read, stderr_write, error, false ) );

	const int result = posix_spawnp( &this->process_id, argv[0], &actions.value,
		nullptr, argv.data(), environ );
	if( result != 0 )
	{
		this->process_id = -1;
		ctLogError << "process::start: posix_spawnp failed: " << strerror( result ) << ctLogEnd;
		return status::cant_access;
	}
	return status::ok;
}

status process::os_data::wait( std::chrono::milliseconds time_out, int &exit_code )
{
	int child_status = 0;
	const auto start_time = std::chrono::steady_clock::now();
	const bool indefinite = time_out == std::chrono::milliseconds::max();
	for( ;; )
	{
		const pid_t result = waitpid( this->process_id, &child_status, indefinite ? 0 : WNOHANG );
		if( result > 0 )
			break;
		if( result == -1 )
		{
			if( errno == EINTR )
				continue;
			ctLogError << "process::wait: waitpid failed: " << strerror( errno ) << ctLogEnd;
			return status::undefined_error;
		}
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - start_time );
		if( elapsed >= time_out )
			return status::not_ready;
		std::this_thread::sleep_for( std::min( std::chrono::milliseconds( 10 ), time_out - elapsed ) );
	}
	this->process_id = -1;
	exit_code = WIFEXITED( child_status ) ? WEXITSTATUS( child_status ) : INT_MIN;
	return status::ok;
}

status process::os_data::terminate( int /*exit_code*/ )
{
	if( kill( this->process_id, SIGTERM ) != 0 )
	{
		ctLogError << "process::terminate: kill failed: " << strerror( errno ) << ctLogEnd;
		return status::cant_access;
	}
	return status::ok;
}

void process::os_data::shutdown() noexcept
{
	if( this->process_id <= 0 )
		return;
	kill( this->process_id, SIGKILL );
	int child_status = 0;
	while( waitpid( this->process_id, &child_status, 0 ) == -1 && errno == EINTR )
	{}
	this->process_id = -1;
}

value_return<size_t> process::os_data::read_pipe( const pipe_ref &pipe, uint8_t *data, size_t size )
{
	ssize_t count;
	do
	{
		count = read( pipe.get_handle(), data, size );
	} while( count == -1 && errno == EINTR );
	if( count < 0 )
		return status::cant_read;
	return (size_t)count;
}

value_return<size_t> process::os_data::write_pipe( const pipe_ref &pipe, const uint8_t *data, size_t size )
{
	ssize_t count;
	do
	{
		count = write( pipe.get_handle(), data, size );
	} while( count == -1 && errno == EINTR );
	if( count < 0 )
		return status::cant_write;
	return (size_t)count;
}
}
// namespace ctle
