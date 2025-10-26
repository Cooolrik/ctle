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

#include "status.h"

namespace ctle
{

/// @brief Get the full path of the current executable as a utf-8 string
std::string get_current_executable_path();

/// @brief Class for starting and managing external processes
class process
{
public:
	using input_callback = std::function<bool( std::vector<uint8_t> &data )>;
	using output_callback = std::function<bool( const std::vector<uint8_t> &data )>;

	/// @brief create a process object
	/// @param command_line the command line to execute
	/// @param work_directory the working directory for the process
	/// @param stdin_callback is called to get data to write to the process stdin. if nullptr, stdin is not connected.
	/// @param stdout_callback is called when data is available from the process stdout. if nullptr, stdout is not connected.
	/// @param stderr_callback is called when data is available from the process stderr. if nullptr, stderr is not connected.
	/// @note the callbacks are called from separate threads, so make sure to synchronize access to shared data.
	process( const std::string &command_line, 
		     const std::string &work_directory, 
			 input_callback stdin_callback = nullptr,
			 output_callback stdout_callback = nullptr,
			 output_callback stderr_callback = nullptr );
	~process();

	/// @brief start the process, asychronously
	/// @return status::ok on success, error code on failure
	status start();

	/// @brief wait for the process to exit, at most time_out milliseconds. if time_out is 0, wait indefinitely
	/// @param time_out maximum time to wait in milliseconds
	/// @return status::ok if the process exited, status::not_ready if the wait timed out, error code on failure
	status wait( std::chrono::milliseconds time_out = std::chrono::milliseconds(0) );

	/// @brief terminate the process
	/// @param exit_code the exit code to set for the process
	/// @return status::ok on success, error code on failure
	status terminate( int exit_code = 0 );

	/// @brief get the exit code of the process
	/// @param exit_code set to the exit code of the process
	/// @return status::ok on success, error code on failure
	status get_exit_code( int &exit_code );

private:
	struct os_data;
	
	std::string command_line;
	std::string work_directory;

	input_callback stdin_callback;
	output_callback stdout_callback;
	output_callback stderr_callback;

	std::thread stdin_thread;
	std::thread stdout_thread;
	std::thread stderr_thread;

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
	static void write_to_pipe( HANDLE pipe, process::input_callback cb );
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

void process::os_data::write_to_pipe( HANDLE pipe, process::input_callback cb )
{
	std::vector<uint8_t> buffer;
	DWORD bytes_written = 0;

	ctLogDebug << "process::os_data::write_to_pipe: starting write loop. pipe:" << pipe << ctLogEnd;
	bool cont = true;
	while( cont )
	{
		cont = cb( buffer );
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

process::process( const std::string &_command_line,
				  const std::string &_work_directory,
				  input_callback _stdin_callback,
				  output_callback _stdout_callback,
				  output_callback _stderr_callback )
	: command_line( _command_line )
	, work_directory( _work_directory )
	, stdin_callback( _stdin_callback )
	, stdout_callback( _stdout_callback )
	, stderr_callback( _stderr_callback )
{
}

process::~process()
{
	// if the process is still running, terminate it
	if( this->process_data != nullptr )
	{
		int exit_code = 0;
		this->get_exit_code( exit_code );
		if( exit_code == STILL_ACTIVE )
			this->terminate();
	}

	// wait for the IO threads to finish
	if( this->stdin_thread.joinable() )
		this->stdin_thread.join();
	if( this->stdout_thread.joinable() )
		this->stdout_thread.join();
	if( this->stderr_thread.joinable() )
		this->stderr_thread.join();
}

status process::start()
{
	ctValidate( this->process_data == nullptr, status::already_initialized ) 
		<< "process::terminate: process already started."
		<< ctValidateEnd;

	// make sure we can convert command line and work directory to wide strings
	std::wstring wcmd = {};
	std::wstring wdir = {};
	if( !ctle::string_to_wstring( this->command_line, wcmd ) 
	 || !ctle::string_to_wstring( this->work_directory, wdir ) )
	{
		ctLogError << "process::start: when trying to run: '" << this->command_line << "' "
				   << "in work directory: '" << this->work_directory << "' "
				   << "could not convert command line and/or work directory to wide strings." 
				   << ctLogEnd;
		return status::invalid_param;
	}
	this->process_data = std::make_unique<os_data>();

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

	// store the handles we are going to keep open
	this->process_data->stdin_write = std::make_unique<windows_handle_ref>( _stdin_write );
	this->process_data->stdout_read = std::make_unique<windows_handle_ref>( _stdout_read );
	this->process_data->stderr_read = std::make_unique<windows_handle_ref>( _stderr_read );

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
		<< "process::start: when trying to run: '" << this->command_line << "' "
		<< "in work directory: '" << this->work_directory << "' "
		<< "the CreateProcessW function failed with error code: " << GetLastError()
		<< ctValidateEnd;

	// process has successfully started, store the process and thread handles as well
	this->process_data->process_handle = std::make_unique<windows_handle_ref>( pi.hProcess );
	this->process_data->process_thread_handle = std::make_unique<windows_handle_ref>( pi.hThread );

	if( this->stdin_callback )
		this->stdin_thread = std::thread( process::os_data::write_to_pipe, _stdin_write, this->stdin_callback );

	if( this->stdout_callback )
		this->stdout_thread = std::thread( process::os_data::read_from_pipe, _stdout_read, this->stdout_callback );

	if( this->stderr_callback )
		this->stderr_thread = std::thread( process::os_data::read_from_pipe, _stderr_read, this->stderr_callback );

	ctLogVerbose << "process::start: running: '" << this->command_line << "' "
				 << "in work directory: '" << this->work_directory << "' "
				 << ctLogEnd;

	return status::ok;
}

status process::wait( std::chrono::milliseconds time_out )
{
	ctValidate( this->process_data != nullptr, status::not_initialized ) 
		<< "process::terminate: process not started or already terminated." 
		<< ctValidateEnd;
	const DWORD wait_time = (time_out.count() == 0)?(INFINITE):((DWORD)time_out.count());
	auto res = ::WaitForSingleObject( this->process_data->process_handle->get_handle(), wait_time );
	ctValidate( res != WAIT_FAILED, status::undefined_error ) << "process::wait: WaitForSingleObject failed with error code: " << GetLastError() << ctValidateEnd;
	if( res == WAIT_TIMEOUT )
		return status::not_ready;
	return status::ok;
}

status process::terminate( int exit_code )
{
	ctValidate( this->process_data != nullptr, status::not_initialized ) 
		<< "process::terminate: process not started or already terminated." 
		<< ctValidateEnd;
	auto res = ::TerminateProcess( this->process_data->process_handle->get_handle(), exit_code );
	ctValidate( res != 0, status::cant_access ) 
		<< "process::terminate: TerminateProcess failed with error code: " << GetLastError() 
		<< ctValidateEnd;
	return status::ok;
}

status process::get_exit_code( int &exit_code )
{
	ctValidate( this->process_data != nullptr, status::not_initialized ) 
		<< "process::get_exit_code: process not started or already terminated." 
		<< ctValidateEnd;
	DWORD code = 0;
	auto res = ::GetExitCodeProcess( this->process_data->process_handle->get_handle(), &code );
	ctValidate( res != 0, status::undefined_error ) 
		<< "process::get_exit_code: GetExitCodeProcess failed with error code: " << GetLastError() 
		<< ctValidateEnd;
	exit_code = (int)( code );
	return status::ok;
}

#elif defined(linux)

struct process::os_data
{
	//std::unique_ptr<windows_handle_ref> process_handle;
	//std::unique_ptr<windows_handle_ref> process_thread_handle;
	//std::unique_ptr<windows_handle_ref> stdin_write;
	//std::unique_ptr<windows_handle_ref> stdout_read;
	//std::unique_ptr<windows_handle_ref> stderr_read;

	static void read_from_pipe( HANDLE pipe, process::output_callback cb );
	static void write_to_pipe( HANDLE pipe, process::input_callback cb );
};


////#ifdef _WIN32
//	void *hProcess = nullptr;
//	void *hThread = nullptr;
//	void *hStdinWrite = nullptr;
//	void *hStdoutRead = nullptr;
//	void *hStderrRead = nullptr;
//
//	void readStream( void *handle, OutputCallback callback );
////#else
////	pid_t pid_ = -1;
////	int stdinPipe_[2];
////	int stdoutPipe_[2];
////	int stderrPipe_[2];
////
////	void readStream( int fd, OutputCallback callback );
////#endif
//
//
//
//LPVOID process::buildEnvironmentBlock()
//{
//	if( environment_.empty() ) return nullptr;
//	std::wstring block;
//	for( const auto &e:environment_ ) block += toUtf16( e )+L'\0';
//	block += L'\0';
//	return (LPVOID)block.c_str();
//}



//
//bool process::start()
//{
//	pipe( stdinPipe_ );
//	pipe( stdoutPipe_ );
//	pipe( stderrPipe_ );
//
//	pid_ = fork();
//	if( pid_==0 )
//	{
//		dup2( stdinPipe_[0], STDIN_FILENO );
//		dup2( stdoutPipe_[1], STDOUT_FILENO );
//		dup2( stderrPipe_[1], STDERR_FILENO );
//
//		close( stdinPipe_[1] );
//		close( stdoutPipe_[0] );
//		close( stderrPipe_[0] );
//
//		if( !environment_.empty() )
//		{
//			std::vector<char *> envp;
//			for( auto &e:environment_ ) envp.push_back( const_cast<char *>(e.c_str()) );
//			envp.push_back( nullptr );
//			execl( "/bin/sh", "sh", "-c", commandLine_.c_str(), nullptr, envp.data() );
//		}
//		else
//		{
//			execl( "/bin/sh", "sh", "-c", commandLine_.c_str(), nullptr );
//		}
//		_exit( 1 );
//	}
//
//	close( stdinPipe_[0] );
//	close( stdoutPipe_[1] );
//	close( stderrPipe_[1] );
//
//	if( !input_.empty() )
//	{
//		write( stdinPipe_[1], input_.c_str(), input_.size() );
//		close( stdinPipe_[1] );
//	}
//
//	stdoutThread_ = std::thread( [this]
//		{
//			readStream( stdoutPipe_[0], onStdout_ );
//		} );
//	stderrThread_ = std::thread( [this]
//		{
//			readStream( stderrPipe_[0], onStderr_ );
//		} );
//
//	return true;
//}
//
//void process::readStream( int fd, OutputCallback callback )
//{
//	char buffer[4096];
//	while( true )
//	{
//		ssize_t count = read( fd, buffer, sizeof( buffer ) );
//		if( count<=0 ) break;
//		if( callback ) callback( std::string( buffer, count ) );
//	}
//	close( fd );
//}
//
//void process::wait()
//{
//	waitpid( pid_, nullptr, 0 );
//	if( stdoutThread_.joinable() ) stdoutThread_.join();
//	if( stderrThread_.joinable() ) stderrThread_.join();
//}
//
//void process::terminate()
//{
//	kill( pid_, SIGTERM );
//}
//
//process::~process()
//{
//	// No handles to clean up on Linux
//}
//
#endif//defined(_WIN32) elif defined(linux)

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_PROCESS_H_
