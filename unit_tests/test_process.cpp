// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/process.h>

#include "unit_tests.h"

void printHandleCount();

using namespace ctle;

TEST( process, basic_test )
{
	auto exepath = get_current_executable_path();

	static const char pingback_msg[] = "Process received PING\n";

	std::string received_data;

	auto proc = std::make_unique<process>( 
		exepath + " --pingback", 
		".",
		[]( std::vector<uint8_t> &data ) -> bool
		{
			data = std::vector<uint8_t>( pingback_msg, pingback_msg + sizeof(pingback_msg) );
			return false;
		},
		[&]( const std::vector<uint8_t> &data ) -> bool
		{
			received_data.insert( received_data.end(), data.begin(), data.end() );
			return true;
		},
		[]( const std::vector<uint8_t> &data ) -> bool
		{
			return false;
		}
	);

	auto res = proc->start();
	ASSERT_EQ( res, status::ok );
	res = proc->wait();
	ASSERT_EQ( res, status::ok );
	int exit_code = 0;
	res = proc->get_exit_code( exit_code );
	ASSERT_EQ( res, status::ok );
	EXPECT_EQ( exit_code, -123 );
	proc.reset();

	const std::string exp = std::string( pingback_msg, 21 );
	const std::string rcv = received_data.substr( 6, 21 );
	EXPECT_EQ( exp, rcv );
}
