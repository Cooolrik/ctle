// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#define CTLE_IMPLEMENT_EXAMPLE_LOG_MACROS

#include "../ctle/log.h"

#include "unit_tests.h"

using namespace ctle;

static std::string test_level;
static std::string test_func;
static std::string test_msg;

static void log_testing_function( log_level level , const char *function_name , const char *message )
	{
	test_level = std::string(get_log_level_as_string( level ));
	test_func = std::string(function_name);
	test_msg = std::string(message);
	}

static void logging_function()
	{
	ctle_log_error << "log test" << ctle_log_end;
	}

TEST( log , basic_test )
	{		
	set_global_log_level( log_level::warning );
	set_global_log_function( &log_testing_function );

	EXPECT_TRUE( get_global_log_level() == log_level::warning );

	logging_function();

	EXPECT_TRUE( test_level == "Error" );
	EXPECT_TRUE( test_func == "logging_function()" );
	EXPECT_TRUE( test_msg == "log test" );
	}