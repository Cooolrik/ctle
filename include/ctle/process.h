// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_PROCESS_H_
#define _CTLE_PROCESS_H_

/// @file process.h
/// @brief Start and manage external processes.

#include <chrono>
#include <climits>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "fwd.h"
#include "status.h"
#include "status_return.h"

namespace ctle
{

/// @brief Get the full path of the current executable as a UTF-8 string.
std::string get_current_executable_path();

/// @brief Own a child process and its redirected streams.
/// @note Synchronize lifecycle methods externally. Callbacks must not destroy their process owner.
class process
{
public:
	using output_callback = std::function<bool( const std::vector<uint8_t> &data )>;

	/// @brief Start a process in the supplied working directory.
	/// @param command_arguments Executable name or path followed by its arguments.
	/// @param work_directory Working directory for the process.
	/// @param stdout_callback Receives stdout data; null discards stdout.
	/// @param stderr_callback Receives stderr data; null discards stderr.
	/// @param use_stdin Enable put_stdin(); otherwise the child reads EOF from stdin.
	/// @note Callbacks run on separate threads. Return false to close the corresponding pipe.
	/// @note Windows arguments use Microsoft CRT quoting, not shell syntax; pass unquoted argument values.
	static value_return<std::unique_ptr<process>> start(
		const std::vector<std::string> &command_arguments,
		const std::string &work_directory,
		output_callback stdout_callback = nullptr,
		output_callback stderr_callback = nullptr,
		bool use_stdin = false );

	/// @brief Stop and collect the direct child, then join its I/O threads.
	~process();

	/// @brief Wait for exit, returning not_ready on timeout. Zero polls; max waits indefinitely.
	/// @param exit_code Receives the code only when exit has been observed.
	/// @note Linux reports INT_MIN for signal termination. Output callbacks may still be draining.
	status wait( std::chrono::milliseconds time_out = std::chrono::milliseconds::max(),
		int *exit_code = nullptr );

	/// @brief Request termination of the child.
	/// @param exit_code Requested Windows exit code; Linux uses SIGTERM.
	/// @note Success means the request was sent, not that the child exited. Use wait() to observe exit.
	status terminate( int exit_code = 0 );

	/// @brief Get the exit code, or not_ready while the child is still running.
	status get_exit_code( int &exit_code );

	/// @brief Poll whether the child is still running.
	bool is_running();

	/// @brief Poll whether the child has exited.
	bool has_exited();

	/// @brief Queue bytes for the child's stdin.
	status put_stdin( const std::vector<uint8_t> &data );

private:
	process();
	struct os_data;

	std::unique_ptr<os_data> process_data;
	std::thread stdin_thread;
	std::thread stdout_thread;
	std::thread stderr_thread;

	enum _process_state { running, exited } process_state = running;
	int exit_code = -1;
	bool use_stdin = false;
	std::mutex stdin_mutex;
	std::vector<uint8_t> stdin_buffer;
	std::condition_variable stdin_cv;
	bool shutdown_stdin_thread = false;

	bool fetch_stdin_data( std::vector<uint8_t> &dest );
	void notify_stdin_thread_shutdown() noexcept;
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include <algorithm>
#include <system_error>
#include <utility>

#if defined(_WIN32)
#define _ADD_CTLE_HEADERS_WIN_STD
#elif defined(__linux__)
#define _ADD_CTLE_HEADERS_LINUX_STD
#include <pthread.h>
#include <signal.h>
#endif
#include "os.inl"

#include "string_funcs.h"
#include "log.h"
#include "_macros.inl"

namespace ctle
{

struct process::os_data
{
#if defined(_WIN32)
	using pipe_ref = windows_handle_ref;
	windows_handle_ref process_handle;
#elif defined(__linux__)
	using pipe_ref = linux_file_ref;
	pid_t process_id = -1;
#endif
	pipe_ref stdin_write;
	pipe_ref stdout_read;
	pipe_ref stderr_read;

	~os_data() { this->shutdown(); }
	status start( const std::vector<std::string> &arguments, const std::string &directory,
		bool input, bool output, bool error );
	status wait( std::chrono::milliseconds time_out, int &exit_code );
	status terminate( int exit_code );
	void shutdown() noexcept;

	static value_return<size_t> read_pipe( const pipe_ref &pipe, uint8_t *data, size_t size );
	static value_return<size_t> write_pipe( const pipe_ref &pipe, const uint8_t *data, size_t size );
	static void read_from_pipe( pipe_ref pipe, output_callback callback );
	static void write_to_pipe( pipe_ref pipe, process *owner );
};

}
// namespace ctle

#if defined(_WIN32)
#include "process_windows.inl"
#elif defined(__linux__)
#include "process_linux.inl"
#endif

namespace ctle
{

process::process() = default;

process::~process()
{
	this->notify_stdin_thread_shutdown();
	if( this->process_data )
		this->process_data->shutdown();
	if( this->stdin_thread.joinable() )
		this->stdin_thread.join();
	if( this->stdout_thread.joinable() )
		this->stdout_thread.join();
	if( this->stderr_thread.joinable() )
		this->stderr_thread.join();
}

value_return<std::unique_ptr<process>> process::start(
	const std::vector<std::string> &command_arguments, const std::string &work_directory,
	output_callback stdout_callback, output_callback stderr_callback, bool _use_stdin )
{
	if( command_arguments.empty() || command_arguments.front().empty()
		|| work_directory.find( '\0' ) != std::string::npos )
		return status::invalid_param;
	for( const auto &argument : command_arguments )
	{
		if( argument.find( '\0' ) != std::string::npos )
			return status::invalid_param;
	}

	auto child = std::unique_ptr<process>( new process() );
	child->process_data = std::make_unique<os_data>();
	ctStatusCall( child->process_data->start( command_arguments, work_directory,
		_use_stdin, bool( stdout_callback ), bool( stderr_callback ) ) );
	child->use_stdin = _use_stdin;

	try
	{
		if( _use_stdin )
			child->stdin_thread = std::thread( os_data::write_to_pipe,
				std::move( child->process_data->stdin_write ), child.get() );
		if( stdout_callback )
			child->stdout_thread = std::thread( os_data::read_from_pipe,
				std::move( child->process_data->stdout_read ), std::move( stdout_callback ) );
		if( stderr_callback )
			child->stderr_thread = std::thread( os_data::read_from_pipe,
				std::move( child->process_data->stderr_read ), std::move( stderr_callback ) );
	}
	catch( const std::system_error & )
	{
		return status::cant_allocate;
	}
	return child;
}

status process::wait( std::chrono::milliseconds time_out, int *_exit_code )
{
	if( time_out < std::chrono::milliseconds::zero() )
		return status::invalid_param;
	if( this->process_state != exited )
	{
		const status result = this->process_data->wait( time_out, this->exit_code );
		if( !result )
			return result;
		this->process_state = exited;
		this->notify_stdin_thread_shutdown();
	}
	if( _exit_code )
		*_exit_code = this->exit_code;
	return status::ok;
}

status process::terminate( int _exit_code )
{
	const status result = this->wait( std::chrono::milliseconds::zero() );
	if( result != status::not_ready )
		return result;
	return this->process_data->terminate( _exit_code );
}

status process::get_exit_code( int &_exit_code )
{
	return this->wait( std::chrono::milliseconds::zero(), &_exit_code );
}

bool process::is_running()
{
	this->wait( std::chrono::milliseconds::zero() );
	return this->process_state == running;
}

bool process::has_exited()
{
	return !this->is_running();
}

status process::put_stdin( const std::vector<uint8_t> &data )
{
	if( !this->use_stdin )
		return status::invalid_param;
	{
		std::lock_guard<std::mutex> lock( this->stdin_mutex );
		if( this->shutdown_stdin_thread )
			return status::not_ready;
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
		this->stdin_buffer.clear();
	}
	this->stdin_cv.notify_one();
}

void process::os_data::read_from_pipe( pipe_ref pipe, output_callback callback )
{
	std::vector<uint8_t> buffer( 4096 );
	for( ;; )
	{
		auto result = read_pipe( pipe, buffer.data(), buffer.size() );
		if( !result || result.value() == 0 )
			return;
		if( !callback( std::vector<uint8_t>( buffer.begin(), buffer.begin() + result.value() ) ) )
			return;
	}
}

void process::os_data::write_to_pipe( pipe_ref pipe, process *owner )
{
#if defined(__linux__)
	// SIGPIPE belongs to this I/O thread; do not change the host application's signal handlers.
	sigset_t signals;
	sigemptyset( &signals );
	sigaddset( &signals, SIGPIPE );
	if( pthread_sigmask( SIG_BLOCK, &signals, nullptr ) != 0 )
	{
		owner->notify_stdin_thread_shutdown();
		return;
	}
#endif
	std::vector<uint8_t> buffer;
	while( owner->fetch_stdin_data( buffer ) )
	{
		size_t offset = 0;
		while( offset < buffer.size() )
		{
			auto result = write_pipe( pipe, buffer.data() + offset, buffer.size() - offset );
			if( !result || result.value() == 0 )
			{
				owner->notify_stdin_thread_shutdown();
				return;
			}
			offset += result.value();
		}
		buffer.clear();
	}
}

}
// namespace ctle

#include "_undef_macros.inl"
#endif//CTLE_IMPLEMENTATION
#endif//_CTLE_PROCESS_H_
