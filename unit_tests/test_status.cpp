// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/status.h"

#include "unit_tests.h"

using namespace ctle;

TEST( status, basic_test )
{
	status res = status_code::ok;

	EXPECT_TRUE( res == status_code::ok );
	EXPECT_FALSE( res != status_code::ok );

	EXPECT_TRUE( res != status_code::cant_allocate );
	EXPECT_FALSE( res == status_code::cant_allocate );

	EXPECT_TRUE( ( res ) );
	EXPECT_FALSE( !( res ) );

	res = status_code::cant_write;

	EXPECT_TRUE( res != status_code::ok );
	EXPECT_FALSE( res == status_code::ok );

	EXPECT_TRUE( res == status_code::cant_write );
	EXPECT_FALSE( res != status_code::cant_write );

	EXPECT_FALSE( ( res ) );
	EXPECT_TRUE( !( res ) );

	EXPECT_EQ( (int)res.value(), -109 );
	EXPECT_TRUE( res.name() == "cant_write" );
	EXPECT_TRUE( res.description() == "cant write to file or handle" );
}