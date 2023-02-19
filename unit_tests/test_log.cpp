// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/log.h"

using namespace ctle;

TEST( log , basic_test )
	{		
	set_global_log_level( log_level::warning );
	EXPECT_TRUE( get_global_log_level() == log_level::warning );

	ctle_log_error << "log test" << ctle_log_end;

	}