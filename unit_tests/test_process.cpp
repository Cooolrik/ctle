// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/process.h>

#include "unit_tests.h"

using namespace ctle;

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
