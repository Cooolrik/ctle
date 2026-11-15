// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <cstdlib>
#include <limits.h>

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

	/// @brief Run the resolved file through /bin/sh, preserving the original argument boundaries.
	int spawn_process_script( pid_t &process_id, const char *path,
		const std::vector<char *> &argv, const posix_spawn_file_actions_t &actions )
	{
		// Give the shell a filename, not a "-c" command string: argument contents must not
		// become shell syntax. "--" also protects filenames that begin with a dash.
		std::vector<char *> shell_argv = {
			const_cast<char *>( "/bin/sh" ), const_cast<char *>( "--" ), const_cast<char *>( path )
		};

		// The resolved filename replaces the original argv[0]. Keep all remaining arguments,
		// including empty strings, and copy the existing null terminator.
		shell_argv.insert( shell_argv.end(), argv.begin() + 1, argv.end() );

		// Reuse the original working-directory and stream redirections. Argument storage is
		// prepared here in the parent; posix_spawn handles setup and execution in the child.
		return posix_spawn( &process_id, "/bin/sh", &actions, nullptr, shell_argv.data(), environ );
	}

	/// @brief Spawn with execvp-style shell fallback for unrecognized executable formats.
	/// @return Zero after a successful launch, or the POSIX error number on failure.
	int spawn_process( pid_t &process_id, const std::vector<char *> &argv,
		const posix_spawn_file_actions_t &actions )
	{
		// Leave ordinary launches and their errors unchanged. ENOEXEC means the kernel did
		// not recognize the file's executable format, for example a script without "#!".
		// execvp would then try /bin/sh, but posix_spawnp does not provide that fallback.
		int result = posix_spawnp( &process_id, argv[0], &actions, nullptr, argv.data(), environ );
		if( result != ENOEXEC )
			return result;

		// A name containing a slash bypasses PATH lookup, so the failing filename is known.
		const std::string executable = argv[0];
		if( executable.find( '/' ) != std::string::npos )
			return spawn_process_script( process_id, argv[0], argv, actions );

		// For a bare name, posix_spawnp does not reveal which PATH candidate failed.
		// Passing that bare name to the shell could open a different file in the working
		// directory. Repeat the search to obtain the filename to pass to /bin/sh.
		// An unset PATH uses the system default; an explicitly empty PATH must stay empty.
		std::string search_path;
		if( const char *path = std::getenv( "PATH" ) )
			search_path = path;
		else
		{
			// confstr includes the trailing null in its size. Reserve room for it, then
			// exclude it from the C++ string before splitting the search path.
			const size_t size = confstr( _CS_PATH, nullptr, 0 );
			if( size == 0 )
				return ENOENT;
			search_path.resize( size );
			if( confstr( _CS_PATH, &search_path[0], size ) == 0 )
				return ENOENT;
			search_path.pop_back();
		}

		bool access_denied = false;
		size_t begin = 0;
		// Include empty entries, even after a trailing colon or when PATH itself is empty.
		// Each empty entry means the child's working directory.
		while( begin <= search_path.size() )
		{
			size_t end = search_path.find( ':', begin );
			if( end == std::string::npos )
				end = search_path.size();
			const std::string directory = search_path.substr( begin, end - begin );
			begin = end + 1;

			// Match glibc's search-buffer limit: an oversized entry must not prevent
			// trying a later valid directory.
			if( directory.size() >= PATH_MAX )
				continue;
			const std::string path = directory.empty() ? executable : directory + '/' + executable;

			// Resolve and check the candidate through spawn, using the original file actions.
			// Parent-side file checks would resolve relative paths in the wrong directory.
			result = posix_spawn( &process_id, path.c_str(), &actions, nullptr, argv.data(), environ );
			if( result == ENOEXEC )
				return spawn_process_script( process_id, path.c_str(), argv, actions );

			// Like execvp, continue past lookup/access errors but stop on other failures.
			// A retry can also succeed if files changed since the first search; return zero
			// immediately in that case rather than launching another process.
			switch( result )
			{
			case EACCES:
				access_denied = true;
				break;
			case ENOENT:
			case ENOTDIR:
			case ESTALE:
			case ENODEV:
			case ETIMEDOUT:
				break;
			default:
				return result;
			}
		}
		// If no candidate launched, a permission denial takes precedence over later
		// "not found" errors, matching execvp's result for an exhausted PATH search.
		return access_denied ? EACCES : result;
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

	pid_t process_id = -1;
	const int result = detail::spawn_process( process_id, argv, actions.value );
	if( result != 0 )
	{
		ctLogError << "process::start: spawn failed: " << strerror( result ) << ctLogEnd;
		return status::cant_access;
	}
	this->process_id = process_id;
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
