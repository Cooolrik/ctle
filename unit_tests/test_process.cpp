// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/process.h>

#include "unit_tests.h"

#include <future>
#if defined(_WIN32)
#define _ADD_CTLE_HEADERS_WIN_STD
#include <ctle/os.inl>
#elif defined(__linux__)
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

using namespace ctle;

int run_process_test_child( const std::string &mode )
{
	if( mode == "unconnected-streams" )
	{
		const std::string output( 1024 * 1024, 'x' );
		std::cout << output << std::flush;
		std::cerr << output << std::flush;
		return std::cin.get() == std::char_traits<char>::eof() ? 73 : 1;
	}
	if( mode == "identified-child" || mode == "ignore-termination" || mode == "closed-stdin" )
	{
#if defined(_WIN32)
		if( mode == "closed-stdin" )
			_close( _fileno( stdin ) );
		std::cout << GetCurrentProcessId() << '\n' << std::flush;
#elif defined(__linux__)
		if( mode == "ignore-termination" )
			signal( SIGTERM, SIG_IGN );
		if( mode == "closed-stdin" )
			close( STDIN_FILENO );
		std::cout << getpid() << '\n' << std::flush;
#endif
		if( mode == "closed-stdin" )
			std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
		else
		{
			std::string input;
			std::getline( std::cin, input );
		}
		return 37;
	}
	if( mode == "bulk-input" )
	{
		const std::string expected( 1024 * 1024, 'x' );
		std::string input;
		std::getline( std::cin, input );
		return input == expected ? 64 : 1;
	}
#if defined(__linux__)
	if( mode == "deferred-exit" )
	{
		signal( SIGTERM, SIG_IGN );
		std::cout << "ready" << std::flush;
		std::string input;
		std::getline( std::cin, input );
		return 37;
	}
#endif
	return 2;
}

static process::output_callback capture_child_id( std::promise<u32> &identity )
{
	return [&identity, text = std::string()]( const std::vector<uint8_t> &data ) mutable
	{
		text.append( data.begin(), data.end() );
		if( text.find( '\n' ) == std::string::npos )
			return true;
		identity.set_value( (u32)std::stoul( text ) );
		return false;
	};
}

static void expect_child_collected( u32 identity )
{
#if defined(_WIN32)
	HANDLE child = OpenProcess( SYNCHRONIZE, FALSE, identity );
	if( child )
	{
		EXPECT_EQ( WaitForSingleObject( child, 0 ), WAIT_OBJECT_0 );
		CloseHandle( child );
	}
#elif defined(__linux__)
	int child_status = 0;
	errno = 0;
	EXPECT_EQ( waitpid( (pid_t)identity, &child_status, WNOHANG ), -1 );
	EXPECT_EQ( errno, ECHILD );
#endif
}

static size_t process_resource_count()
{
#if defined(_WIN32)
	DWORD count = 0;
	if( !GetProcessHandleCount( GetCurrentProcess(), &count ) )
		throw std::runtime_error( "Could not count process handles" );
	return count;
#elif defined(__linux__)
	DIR *directory = opendir( "/proc/self/fd" );
	if( !directory )
		throw std::runtime_error( "Could not count file descriptors" );
	size_t count = 0;
	while( readdir( directory ) )
		++count;
	closedir( directory );
	return count;
#endif
}

TEST( process, unconnected_streams_do_not_block )
{
	auto result = process::start(
		{ get_current_executable_path(), "--process-test", "unconnected-streams" }, "." );
	ASSERT_EQ( result.status(), status::ok );
	auto child = std::move( result.value() );
	int exit_code = -1;
	ASSERT_EQ( child->wait( std::chrono::seconds( 2 ), &exit_code ), status::ok );
	EXPECT_EQ( exit_code, 73 );
}

#if defined(__linux__)
TEST( process, termination_request_does_not_report_exit )
{
	std::promise<void> ready;
	auto result = process::start(
		{ get_current_executable_path(), "--process-test", "deferred-exit" }, ".",
		[&]( const std::vector<uint8_t> & )
		{
			ready.set_value();
			return false;
		}, nullptr, true );
	ASSERT_EQ( result.status(), status::ok );
	auto child = std::move( result.value() );
	ASSERT_EQ( ready.get_future().wait_for( std::chrono::seconds( 2 ) ), std::future_status::ready );

	ASSERT_EQ( child->terminate( 91 ), status::ok );
	int exit_code = -1;
	ASSERT_EQ( child->wait( std::chrono::milliseconds( 20 ), &exit_code ), status::not_ready );
	EXPECT_EQ( exit_code, -1 );
	EXPECT_TRUE( child->is_running() );
	EXPECT_FALSE( child->has_exited() );
	EXPECT_EQ( child->get_exit_code( exit_code ), status::not_ready );

	ASSERT_EQ( child->put_stdin( { '\n' } ), status::ok );
	ASSERT_EQ( child->wait( std::chrono::seconds( 2 ), &exit_code ), status::ok );
	EXPECT_EQ( exit_code, 37 );
	ASSERT_EQ( child->terminate( 99 ), status::ok );
	ASSERT_EQ( child->get_exit_code( exit_code ), status::ok );
	EXPECT_EQ( exit_code, 37 );
}
#endif

TEST( process, wait_collects_terminated_child )
{
	std::promise<u32> identity;
	auto ready = identity.get_future();
	auto result = process::start(
		{ get_current_executable_path(), "--process-test", "identified-child" }, ".",
		capture_child_id( identity ), nullptr, true );
	ASSERT_EQ( result.status(), status::ok );
	auto child = std::move( result.value() );
	ASSERT_EQ( ready.wait_for( std::chrono::seconds( 2 ) ), std::future_status::ready );
	const u32 child_id = ready.get();
	ASSERT_EQ( child->terminate( 83 ), status::ok );
	int exit_code = -1;
	ASSERT_EQ( child->wait( std::chrono::seconds( 2 ), &exit_code ), status::ok );
#if defined(_WIN32)
	EXPECT_EQ( exit_code, 83 );
#elif defined(__linux__)
	EXPECT_EQ( exit_code, INT_MIN );
#endif
	expect_child_collected( child_id );
	int cached_code = -1;
	ASSERT_EQ( child->get_exit_code( cached_code ), status::ok );
	EXPECT_EQ( cached_code, exit_code );
	EXPECT_TRUE( child->has_exited() );
	EXPECT_EQ( child->put_stdin( { 'x' } ), status::not_ready );
}

TEST( process, destruction_collects_child_even_after_ignored_termination )
{
	std::promise<u32> identity;
	auto ready = identity.get_future();
	auto result = process::start(
		{ get_current_executable_path(), "--process-test", "ignore-termination" }, ".",
		capture_child_id( identity ), nullptr, true );
	ASSERT_EQ( result.status(), status::ok );
	auto child = std::move( result.value() );
	ASSERT_EQ( ready.wait_for( std::chrono::seconds( 2 ) ), std::future_status::ready );
	const u32 child_id = ready.get();
	ASSERT_EQ( child->terminate(), status::ok );
	const auto before = std::chrono::steady_clock::now();
	child.reset();
	EXPECT_LT( std::chrono::steady_clock::now() - before, std::chrono::seconds( 2 ) );
	expect_child_collected( child_id );
}

TEST( process, failed_start_does_not_leak_streams )
{
	const auto discard = []( const std::vector<uint8_t> & ) { return true; };
	const size_t before = process_resource_count();
	for( int attempt = 0; attempt < 8; ++attempt )
	{
		auto result = process::start(
			{ get_current_executable_path(), "--process-test", "unconnected-streams" },
			"ctle-nonexistent-directory/child", discard, discard, true );
		EXPECT_EQ( result.status(), status::cant_access );
		EXPECT_EQ( result.value(), nullptr );
	}
	EXPECT_EQ( process_resource_count(), before );

	EXPECT_EQ( process::start( {}, "." ).status(), status::invalid_param );
	EXPECT_EQ( process::start( { "" }, "." ).status(), status::invalid_param );
	EXPECT_EQ( process::start( { "ctle-nonexistent-directory/executable" }, "." ).status(),
		status::cant_access );
}

TEST( process, queued_input_survives_multiple_pipe_writes )
{
	auto result = process::start(
		{ get_current_executable_path(), "--process-test", "bulk-input" }, ".",
		nullptr, nullptr, true );
	ASSERT_EQ( result.status(), status::ok );
	auto child = std::move( result.value() );
	std::vector<uint8_t> input( 1024 * 1024, 'x' );
	input.push_back( '\n' );
	ASSERT_EQ( child->put_stdin( input ), status::ok );
	int exit_code = -1;
	ASSERT_EQ( child->wait( std::chrono::seconds( 2 ), &exit_code ), status::ok );
	EXPECT_EQ( exit_code, 64 );
}

TEST( process, closed_child_stdin_does_not_signal_parent )
{
	std::promise<u32> identity;
	auto ready = identity.get_future();
	auto result = process::start(
		{ get_current_executable_path(), "--process-test", "closed-stdin" }, ".",
		capture_child_id( identity ), nullptr, true );
	ASSERT_EQ( result.status(), status::ok );
	auto child = std::move( result.value() );
	ASSERT_EQ( ready.wait_for( std::chrono::seconds( 2 ) ), std::future_status::ready );
	const u32 child_id = ready.get();
	ASSERT_EQ( child->put_stdin( std::vector<uint8_t>( 1024 * 1024, 'x' ) ), status::ok );
	EXPECT_TRUE( run_function_with_timeout( [&]()
		{ return child->put_stdin( {} ) == status::not_ready; }, 2000 ) );
	child.reset();
	expect_child_collected( child_id );
}

TEST( process, basic_test )
{
	auto exepath = get_current_executable_path();

	static const char pingback_msg[] = "Process received PING";

	std::string received_data_stdout;
	std::string received_data_stderr;

	std::vector<std::string> args = { exepath , std::string("--pingback") };

	auto result = process::start(
		args, 
		".",
		[&]( const std::vector<uint8_t> &data ) -> bool
		{
			received_data_stdout.insert( received_data_stdout.end(), data.begin(), data.end() );
			return true;
		},
		[&]( const std::vector<uint8_t> &data ) -> bool
		{
			received_data_stderr.insert( received_data_stderr.end(), data.begin(), data.end() );
			return true;
		},
		true
	);
	ASSERT_EQ( result, status::ok );
	auto proc = std::move( result.value() );

	// send one char at a time, with a small delay between each char, to simulate slow input
	for( auto c : pingback_msg )
	{
		if( c == '\0' )
			break;
		ASSERT_EQ( proc->put_stdin( std::vector<uint8_t>( 1, (uint8_t)c ) ), status::ok );
		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	// make sure the process is still running, then send the last char (newline) to trigger 
	// the pingback, which will make the process exit with code 123
	ASSERT_TRUE( proc->is_running() );	
	ASSERT_TRUE( proc->put_stdin( std::vector<uint8_t>( 1, (uint8_t)'\n' ) ) );
	
	// wait for the process to exit
	auto res = proc->wait();
	ASSERT_EQ( res, status::ok );
	int exit_code = 0;
	res = proc->get_exit_code( exit_code );
	ASSERT_EQ( res, status::ok );
	EXPECT_EQ( exit_code, 123 );
	proc.reset();

	const std::string expstdout = std::string("Pong: ") + std::string( pingback_msg, 21 );
	const std::string expstderr = std::string("Pang: ") + std::string( pingback_msg, 21 );
	EXPECT_EQ( expstdout, received_data_stdout.substr(0,27) );
	EXPECT_EQ( expstderr, received_data_stderr.substr(0,27) );
}
