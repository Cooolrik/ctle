// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_PROCESS_H_
#define _CTLE_PROCESS_H_

/// @file process.h 
/// @brief process handling for ctle, for starting and managing external processes.

#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <climits>

#include "fwd.h"
#include "status.h"
#include "status_return.h"

namespace ctle
{

/// @brief Get the full path of the current executable as a utf-8 string
std::string get_current_executable_path();

/// @brief Class for starting and managing external processes
class process
{
public:
	using output_callback = std::function<bool( const std::vector<uint8_t> &data )>;

	/// @brief create a new process instance and start the process
	/// @param command_arguments list of arguments to the process, the first one must be the name or path of the process
	/// @param work_directory the working directory for the process
	/// @param stdout_callback is called when data is available from the process stdout. if nullptr, stdout is not connected.
	/// @param stderr_callback is called when data is available from the process stderr. if nullptr, stderr is not connected.
	/// @param use_stdin if true, use put_stdin to send data to the process stdin. if false, stdin is not connected.
	/// @note the callbacks are called from separate threads, so make sure to synchronize access to shared data.
	static value_return<std::unique_ptr<process>> start(
		const std::vector<std::string> &command_arguments,
		const std::string &work_directory,
		output_callback stdout_callback = nullptr,
		output_callback stderr_callback = nullptr,
		bool use_stdin = false
	);
	~process();

	/// @brief wait for the process to exit, at most time_out ms. defaults to waiting indefinitely.
	/// @note if time_out is zero, the function will return immediately
	/// @note this function blocks the calling thread until the process exits or the timeout is reached
	/// @note on some platforms, the maximum time_out value may be limited by the underlying OS API to 32-bit milliseconds (approx 49 days). To rellay wait indefinitely, use the default parameter value of max().
	/// @param time_out maximum time to wait in milliseconds
	/// @param exit_code optional pointer to an int that receives the exit code of the process if it has finished
	/// @return status::ok if the process exited, status::not_ready if the wait timed out, error code on failure
	status wait( std::chrono::milliseconds time_out = std::chrono::milliseconds::max(), int *exit_code = nullptr );

	/// @brief terminate the process
	/// @param exit_code the exit code to set for the process
	/// @return status::ok on success, error code on failure
	status terminate( int exit_code = 0 );

	/// @brief get the exit code of the process
	/// @param exit_code set to the exit code of the process
	/// @return status::ok on success, error code on failure
	status get_exit_code( int &exit_code );

	/// @brief checks if the process is still running
	/// @return true if the process is running, false if it has fisnished, or has not been started yet
	bool is_running();

	/// @brief checks if a process is finished
	/// @return true if the process has been started and has run, false if the process has not been started, or is still running
	bool has_exited();

	/// @brief put_stdin. Send data to the process stdin.
	status put_stdin( const std::vector<uint8_t> &data );

private:
	process();

	struct os_data;
	friend struct os_data;

	std::thread stdin_thread;
	std::thread stdout_thread;
	std::thread stderr_thread;

	enum _process_state
	{
		running,
		exited
	} process_state = {};

	int exit_code = -1;

	bool use_stdin = false;
	std::mutex stdin_mutex;
	std::vector<uint8_t> stdin_buffer;
	std::condition_variable stdin_cv;
	bool shutdown_stdin_thread = false;
	bool fetch_stdin_data( std::vector<uint8_t> &dest );
	void notify_stdin_thread_shutdown() noexcept;

	std::unique_ptr<os_data> process_data; // platform specific data
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include <string>
#include <atomic>
#include <utility>
#include <mutex>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#if defined(_WIN32)
#define _ADD_CTLE_HEADERS_WIN_SOCKETS
#elif defined(linux)
#define _ADD_CTLE_HEADERS_LINUX_SOCKETS
#endif
#include "os.inl"

#include "string_funcs.h"
#include "log.h"
#include "_macros.inl"

namespace ctle
{

process::process() = default;

process::~process()
{
	// if the process is still running, terminate it
	if( this->is_running() )
	{
		this->terminate();
	}

	// wait for the IO threads to finish
	this->notify_stdin_thread_shutdown();
	if( this->stdin_thread.joinable() )
		this->stdin_thread.join();
	if( this->stdout_thread.joinable() )
		this->stdout_thread.join();
	if( this->stderr_thread.joinable() )
		this->stderr_thread.join();
}

status process::put_stdin( const std::vector<uint8_t> &data )
{
	ctValidate( this->use_stdin, status::invalid_param ) << "process::put_stdin: process was not started with stdin enabled." << ctValidateEnd;
	{
		std::lock_guard<std::mutex> lock( this->stdin_mutex );
		this->stdin_buffer.insert( this->stdin_buffer.end(), data.begin(), data.end() );
	}
	this->stdin_cv.notify_one();
	return status::ok;
}

bool process::fetch_stdin_data( std::vector<uint8_t> &dest )
{
	std::unique_lock<std::mutex> lock( this->stdin_mutex );
	this->stdin_cv.wait( lock, [this]() { return !this->stdin_buffer.empty() || this->shutdown_stdin_thread; } );
	if( this->shutdown_stdin_thread )
		return false;
	dest.swap( this->stdin_buffer );
	return true;
}

void process::notify_stdin_thread_shutdown() noexcept 
{ 
	{
		std::lock_guard<std::mutex> lock( this->stdin_mutex );
		this->shutdown_stdin_thread = true;
	}
	this->stdin_cv.notify_one();
}

#ifdef _WIN32

std::string get_current_executable_path() 
{
    std::vector<wchar_t> buffer(MAX_PATH);
    DWORD size = 0;

    for(;;)
	{
		// try to get the module file name
		size = GetModuleFileNameW( nullptr, buffer.data(), (DWORD)buffer.size() );
		if( size < buffer.size() )
			break;

		// the call failed, check why
		if( size == 0 )
		{
			ctLogError  << "get_current_executable_path: GetModuleFileNameW failed with error code: " << GetLastError() << ctLogEnd;
			return {};
		}

		// the buffer was too small, increase size and retry
		buffer.resize( buffer.size() * 2 );
	}

	// Successfully retrieved the path, convert to utf-8 string
	std::wstring wpath( buffer.data(), size );
	std::string utfpath;
	if( !ctle::wstring_to_string( wpath, utfpath ) )
	{
		ctLogError << "get_current_executable_path: could not convert wide string path to utf-8 string." << ctLogEnd;
		return {};
	}

	// now convert it to a generic utf-8 path (with forward slashes)
	for( auto &c : utfpath )
	{
		if( c == '\\' )
			c = '/';
	}

	// conversion succeeded, return the utf-8 path
	return utfpath;
}

struct process::os_data
{
	std::unique_ptr<windows_handle_ref> process_handle;
	std::unique_ptr<windows_handle_ref> process_thread_handle;
	std::unique_ptr<windows_handle_ref> stdin_write;
	std::unique_ptr<windows_handle_ref> stdout_read;
	std::unique_ptr<windows_handle_ref> stderr_read;

	static void read_from_pipe( HANDLE pipe, process::output_callback cb );
	static void write_to_pipe( HANDLE pipe, process *pProcess );
};

void process::os_data::read_from_pipe( HANDLE pipe, process::output_callback cb )
{
	std::vector<uint8_t> buffer( 4096 );
	DWORD bytes_read = 0;

	ctLogDebug << "process::os_data::read_from_pipe: starting read loop. pipe:" << pipe << ctLogEnd;
	bool cont = true;
	while( cont )	
	{
		// read data from the pipe (blocking)
		const bool success = ReadFile( pipe, buffer.data(), (DWORD)buffer.size(), &bytes_read, nullptr ) != 0;
		if( !success )
		{
			const auto err = GetLastError();
			if( err == ERROR_BROKEN_PIPE )
			{
				ctLogDebug << "process::os_data::read_from_pipe: pipe closed. pipe:" << pipe << ctLogEnd;
				return;
			}
			else
			{
				ctLogError << "process::os_data::read_from_pipe: pipe:" << pipe << " ReadFile failed with error code: " << err << ctLogEnd;
				return;
			}
		}

		// call the callback with the data read
		cont = cb( std::vector<uint8_t>( buffer.data(), buffer.data() + bytes_read ) );
	} 

	ctLogDebug << "process::os_data::read_from_pipe: callback requested to stop. pipe:" << pipe << ctLogEnd;
}

void process::os_data::write_to_pipe( HANDLE pipe, process *pProcess )
{
	std::vector<uint8_t> buffer;
	DWORD bytes_written = 0;

	ctLogDebug << "process::os_data::write_to_pipe: starting write loop. pipe:" << pipe << ctLogEnd;
	bool cont = true;
	while( cont )
	{
		// fetch data to write to the pipe. blocking call, returns false if the thread should exit
		cont = pProcess->fetch_stdin_data( buffer );
		if( buffer.empty() ) 
			continue;

		// write data to the pipe 
		const bool success = WriteFile( pipe, buffer.data(), (DWORD)buffer.size(), &bytes_written, nullptr ) != 0;
		if( !success )
		{
			const auto err = GetLastError();
			if( err == ERROR_BROKEN_PIPE )
			{
				ctLogDebug << "process::os_data::write_to_pipe: pipe closed. pipe:" << pipe << ctLogEnd;
				return;
			}
			else
			{
				ctLogError << "process::os_data::write_to_pipe: pipe:" << pipe << " WriteFile failed with error code: " << err << ctLogEnd;
				return;
			}
		}
		buffer.clear();
	}

	ctLogDebug << "process::os_data::write_to_pipe: callback requested to stop. pipe:" << pipe << ctLogEnd;
}

value_return<std::unique_ptr<process>> process::start( const std::vector<std::string> &command_arguments,
													   const std::string &work_directory,
													   output_callback stdout_callback,
													   output_callback stderr_callback,
													   bool _use_stdin )
{
	// combine all command arguments into a single command line string
	std::string command_line;
	for( const auto &arg : command_arguments )
	{
		// add space separator if needed
		if( !command_line.empty() )
			command_line += " ";

		// quote the argument if needed (escape characters or whitespace)
		command_line += ctle::quote_string_t<quote_string_options::whitespace>( arg );
	}

	// make sure we can convert command line and work directory to wide strings
	std::wstring wcmd = {};
	std::wstring wdir = {};
	if( !ctle::string_to_wstring( command_line, wcmd ) 
	 || !ctle::string_to_wstring( work_directory, wdir ) )
	{
		ctLogError << "process::start: when trying to run: '" << command_arguments[0] << "' "
				   << "in work directory: '" << work_directory << "' "
				   << "could not convert command line arguments and/or work directory to wide strings." 
				   << ctLogEnd;
		return status::invalid_param;
	}
	
	// set up security attributes to allow handle inheritance
	SECURITY_ATTRIBUTES sa = {};
	sa.nLength = sizeof( SECURITY_ATTRIBUTES );
	sa.lpSecurityDescriptor = nullptr;
	sa.bInheritHandle = TRUE;
	
	// Create pipes for stdin, stdout, and stderr
	HANDLE _stdin_read = {}, _stdin_write = {};
	HANDLE _stdout_read = {}, _stdout_write = {};
	HANDLE _stderr_read = {}, _stderr_write = {};

	CreatePipe( &_stdin_read, &_stdin_write, &sa, 0 );
	CreatePipe( &_stdout_read, &_stdout_write, &sa, 0 );
	CreatePipe( &_stderr_read, &_stderr_write, &sa, 0 );

	// set up the handles for the child process
	STARTUPINFOW si = {};
	si.cb = sizeof( STARTUPINFOW );
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = _stdin_read;
	si.hStdOutput = _stdout_write;
	si.hStdError = _stderr_write;

	// create the process
	PROCESS_INFORMATION pi = {};
	BOOL success = CreateProcessW(
		nullptr,	 // application name
		wcmd.data(), // command line
		nullptr,	 // process security attributes
		nullptr,	 // primary thread security attributes
		TRUE,		 // handles are inherited
		0,			 // creation flags
		nullptr,	 // use parent's environment
		wdir.data(), // current directory
		&si,		 // startup info
		&pi			 // process information
	);

	// close the handles that we sent to the child process
	CloseHandle( _stdin_read );
	CloseHandle( _stdout_write );
	CloseHandle( _stderr_write );

	// check if the process was created successfully
	ctValidate( success, status::cant_access ) 
		<< "process::start: when trying to run: '" << command_arguments[0] << "' "
		<< "in work directory: '" << work_directory << "' "
		<< "the CreateProcessW function failed with error code: " << GetLastError()
		<< ctValidateEnd;

	// create the process instance
	auto pThis = std::unique_ptr<process>( new process() );
	pThis->process_data = std::make_unique<os_data>();
	
	// start the IO threads as needed
	pThis->use_stdin = _use_stdin;
	if( _use_stdin )
		pThis->stdin_thread = std::thread( process::os_data::write_to_pipe, _stdin_write, pThis.get() );
	if( stdout_callback )
		pThis->stdout_thread = std::thread( process::os_data::read_from_pipe, _stdout_read, stdout_callback );
	if( stderr_callback )
		pThis->stderr_thread = std::thread( process::os_data::read_from_pipe, _stderr_read, stderr_callback );

	ctLogVerbose << "process::start: running: '" << command_arguments[0] << "' "
				 << "in work directory: '" << work_directory << "' "
				 << ctLogEnd;

	// store away the needed handles
	pThis->process_data->stdin_write = std::make_unique<windows_handle_ref>( _stdin_write );
	pThis->process_data->stdout_read = std::make_unique<windows_handle_ref>( _stdout_read );
	pThis->process_data->stderr_read = std::make_unique<windows_handle_ref>( _stderr_read );
	pThis->process_data->process_handle = std::make_unique<windows_handle_ref>( pi.hProcess );
	pThis->process_data->process_thread_handle = std::make_unique<windows_handle_ref>( pi.hThread );

	pThis->process_state = running;
	return pThis;
}

status process::wait( std::chrono::milliseconds time_out, int *_exit_code )
{
	// check if the process has already exited
	if( this->process_state == exited )
	{
		if( _exit_code ) 
			*_exit_code = this->exit_code;
		return status::ok;
	}
	ctSanityCheck( this->process_state == running );

	// get the wait time in milliseconds
	const DWORD wait_time =
		(time_out == std::chrono::milliseconds::max()) ?
		(INFINITE) :
		((DWORD)std::min( time_out, std::chrono::milliseconds( UINT_MAX - 1 ) ).count());

	auto res = ::WaitForSingleObject( this->process_data->process_handle->get_handle(), wait_time );
	ctValidate( res != WAIT_FAILED, status::undefined_error ) << "process::wait: WaitForSingleObject failed with error code: " << GetLastError() << ctValidateEnd;
	if( res == WAIT_TIMEOUT )
		return status::not_ready;

	// process has exited, update the process state and get the exit code
	this->process_state = exited;

	res = ::GetExitCodeProcess( this->process_data->process_handle->get_handle(), (LPDWORD)(&this->exit_code) );
	ctValidate( res != 0, status::undefined_error ) 
		<< "process::get_exit_code: GetExitCodeProcess failed with error code: " << GetLastError() 
		<< ctValidateEnd;

	if( _exit_code ) 
		*_exit_code = this->exit_code;
	return status::ok;
}

status process::terminate( int _exit_code )
{
	// must be in state running or exited
	if( this->process_state == running )
	{
		// process is running, terminate it
		auto res = ::TerminateProcess( this->process_data->process_handle->get_handle(), _exit_code );
		ctValidate( res != 0, status::cant_access ) 
			<< "process::terminate: TerminateProcess failed with error code: " << GetLastError() 
			<< ctValidateEnd;

		this->process_state = exited;
	}
	ctSanityCheck( this->process_state == exited );

	this->exit_code = _exit_code;
	
	return status::ok;
}

status process::get_exit_code( int &_exit_code )
{
	// is still running, cannot get exit code
	if( this->is_running() )
		return status::not_ready;
	ctSanityCheck( this->process_state == exited );

	_exit_code = this->exit_code;
	return status::ok;
}

bool process::is_running()
{
	if( this->process_state == exited )
		return false;

	// may be running, or has just exited, update the process state by waiting with zero timeout, then check state
	this->wait( std::chrono::milliseconds( 0 ) );

	return (this->process_state == running);
}

bool process::has_exited()
{
	return !this->is_running();
}

#elif defined(linux)

std::string get_current_executable_path() 
{
    std::vector<char> buffer(1024);

	ssize_t len = 0;
    for(;;)
	{
		len = readlink("/proc/self/exe", buffer.data(), buffer.size()-1 );
		if( (size_t)len < buffer.size() )
			break;

		// if the call failed, return what error code is generated
		if( len == -1 )
		{
			ctLogError  << "get_current_executable_path: readlink failed with error code: " << errno << ctLogEnd;
			return {};
		}

		// the buffer was too small, increase size and retry
		buffer.resize( buffer.size() * 2 );
	}

	buffer[len] = '\0';
	return std::string( buffer.data(), len );
}

struct process::os_data
{
	pid_t pid = -1;
	std::unique_ptr<linux_file_ref> stdin_write;
	std::unique_ptr<linux_file_ref> stdout_read;
	std::unique_ptr<linux_file_ref> stderr_read;

	static void read_from_pipe( int pipe, process::output_callback cb );
	static void write_to_pipe( int pipe, process *pProcess );
};

void process::os_data::read_from_pipe( int pipe, process::output_callback cb )
{
	std::vector<uint8_t> buffer( 4096 );
	
	ctLogDebug << "process::os_data::read_from_pipe: starting read loop. pipe:" << pipe << ctLogEnd;
	bool cont = true;
	while( cont )	
	{
		// read data from the pipe (blocking)
		const ssize_t bytes_read = read( pipe, buffer.data(), buffer.size() );
		if( bytes_read == 0 )
		{
			ctLogDebug << "process::os_data::read_from_pipe: pipe closed. pipe:" << pipe << ctLogEnd;
			return;
		}
		else if( bytes_read < 0 )
		{
			ctLogError << "process::os_data::read_from_pipe: pipe:" << pipe << " read() failed with error code: " << errno << ctLogEnd;
			return;
		}

		// call the callback with the data read
		cont = cb( std::vector<uint8_t>( buffer.data(), buffer.data() + bytes_read ) );
	} 

	ctLogDebug << "process::os_data::read_from_pipe: callback requested to stop. pipe:" << pipe << ctLogEnd;
}

void process::os_data::write_to_pipe( int pipe, process *pProcess )
{
	std::vector<uint8_t> buffer;
	
	ctLogDebug << "process::os_data::write_to_pipe: starting write loop. pipe:" << pipe << ctLogEnd;
	bool cont = true;
	while( cont )
	{
		// fetch data to write to the pipe. blocking call, returns false if the thread should exit
		cont = pProcess->fetch_stdin_data( buffer );
		if( buffer.empty() ) 
			continue;

		// write data to the pipe 
		const ssize_t bytes_written = write( pipe, buffer.data(), buffer.size() );
		if( bytes_written == 0 
		 || (bytes_written < 0 && errno == EPIPE ) )
		{
			ctLogDebug << "process::os_data::write_to_pipe: pipe closed. pipe:" << pipe << ctLogEnd;
			return;
		}
		else if( bytes_written < 0 )
		{
			ctLogError << "process::os_data::write_to_pipe: pipe:" << pipe << " WriteFile failed with error code: " << errno << ctLogEnd;
			return;
		}
		buffer.clear();
	}

	ctLogDebug << "process::os_data::write_to_pipe: callback requested to stop. pipe:" << pipe << ctLogEnd;
}

value_return<std::unique_ptr<process>> process::start( const std::vector<std::string> &command_arguments,
													   const std::string &work_directory,
													   output_callback stdout_callback,
													   output_callback stderr_callback,
													   bool _use_stdin )
{
	int stdin_pipe[2];
	int stdout_pipe[2];
	int stderr_pipe[2];

	pipe( stdin_pipe );
	pipe( stdout_pipe );
	pipe( stderr_pipe );

	pid_t pid = vfork();
	ctValidate(pid != -1, status::cant_access) << "process::start: vfork failed." << ctValidateEnd;
	if (pid == 0) 
	{
		// Child process
		chdir( work_directory.c_str() );

		// Redirect stdin
		close(stdin_pipe[1]); // close write end
		dup2(stdin_pipe[0], STDIN_FILENO);
		close(stdin_pipe[0]);

		// Redirect stdout
		close(stdout_pipe[0]); // close read end
		dup2(stdout_pipe[1], STDOUT_FILENO);
		close(stdout_pipe[1]);

		// Redirect stderr
		close(stderr_pipe[0]); // close read end
		dup2(stderr_pipe[1], STDERR_FILENO);
		close(stderr_pipe[1]);

		// Build argv
		std::vector<std::string> args = command_arguments;
		std::vector<char*> argv;
		for (auto& arg : args) 
		{
			argv.emplace_back( arg.data() );
		}
		argv.push_back(nullptr);

		// Execute the command, this exits this function on success
		execvp(argv[0], argv.data());

		// If exec fails, exit with failure
		_exit(127);
	} 
	else 
	{
		// Parent process, close unused pipes
		close(stdin_pipe[0]);  // parent writes to child's stdin
		close(stdout_pipe[1]); // parent reads from child's stdout
		close(stderr_pipe[1]); // parent reads from child's stderr
	}
	
	auto pThis = std::unique_ptr<process>( new process );
	pThis->process_data = std::make_unique<os_data>();

	pThis->use_stdin = _use_stdin;
	if( _use_stdin )
		pThis->stdin_thread = std::thread( process::os_data::write_to_pipe, stdin_pipe[1], pThis.get() );
	if( stdout_callback )
		pThis->stdout_thread = std::thread( process::os_data::read_from_pipe, stdout_pipe[0], stdout_callback );
	if( stderr_callback )
		pThis->stderr_thread = std::thread( process::os_data::read_from_pipe, stderr_pipe[0], stderr_callback );

	// save the process id, and close unused ends in parent
	pThis->process_data->pid = pid;
	pThis->process_data->stdin_write = std::make_unique<linux_file_ref>( stdin_pipe[1] );
	pThis->process_data->stdout_read = std::make_unique<linux_file_ref>( stdout_pipe[0] );
	pThis->process_data->stderr_read = std::make_unique<linux_file_ref>( stderr_pipe[0] );

    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

	pThis->process_state = running;
	return pThis;
}

status process::wait( std::chrono::milliseconds time_out, int *_exit_code )
{
	// check if the process has already exited
	if( this->process_state == exited )
	{
		if( _exit_code ) 
			*_exit_code = this->exit_code;
		return status::ok;
	}
	ctSanityCheck( this->process_state == running );

	int proc_status = 0;
    auto start_time = std::chrono::steady_clock::now();

    while(true) 
	{
        pid_t result = waitpid(this->process_data->pid, &proc_status, WNOHANG);
		if(result > 0) 
			break;	
        ctValidate( result != -1, status::undefined_error ) << "process::wait: waitpid failed with error: " << strerror(errno) << ctValidateEnd;

		// if we are not waiting indefinitely, check time
		if( time_out != std::chrono::milliseconds::max() )
		{
			auto now_time = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now_time - start_time);
			if (elapsed >= time_out)
				return status::not_ready;
		}

		// sleep briefly to avoid busy-waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

	// process has exited, update the process state and get the exit code
	this->process_state = exited;

	// done, collect the exit code
	if( WIFEXITED(proc_status) ) 
	{
		this->exit_code = WEXITSTATUS(proc_status);
	}
	else
	{
		ctLogError << "process::get_exit_code: Process terminated abnormally, cannot fetch exit code, defaulting to INT_MIN." << ctLogEnd;
		this->exit_code = INT_MIN;
	}
	
	if( _exit_code ) 
		*_exit_code = this->exit_code;
	return status::ok;
}

status process::terminate( int _exit_code )
{
	if( this->process_state == running )
	{
		// wait for process to end
		kill( this->process_data->pid, SIGTERM );
		this->process_state = exited;
	}
	ctSanityCheck( this->process_state == exited );

	this->exit_code = _exit_code;
	
	return status::ok;
}

status process::get_exit_code( int &_exit_code )
{
	// if still running, cannot get exit code
	if( this->is_running() )
		return status::not_ready;
	ctSanityCheck( this->process_state == exited );

	_exit_code = this->exit_code;
	return status::ok;
}

bool process::is_running()
{
	if( this->process_state == exited )
		return false;

	// may be running, or has just exited, update the process state by waiting with zero timeout, then check state
	this->wait( std::chrono::milliseconds( 0 ) );

	return (this->process_state == running);
}

bool process::has_exited()
{
	return !this->is_running();
}

#endif//defined(_WIN32) elif defined(linux)

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_PROCESS_H_
