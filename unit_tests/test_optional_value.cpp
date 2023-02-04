// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/optional_value.h"

using namespace ctle;

TEST( optional_value , basic_test )
	{
	optional_value<int> opt( 0x1337 );
	EXPECT_TRUE( opt.has_value() );
	EXPECT_TRUE( opt == 0x1337 );
	EXPECT_FALSE( opt != 0x1337 );
	optional_value<int> opt2 = std::move( opt );
	EXPECT_THROW( opt.value(), ctle::bad_optional_value_access );
	EXPECT_FALSE( opt.has_value() );
	EXPECT_TRUE( opt != 0x1337 );
	EXPECT_FALSE( opt == 0x1337 );
	EXPECT_TRUE( opt != 0 );
	EXPECT_TRUE( opt2.has_value() );
	EXPECT_TRUE( opt2 == 0x1337 );
	EXPECT_TRUE( opt != opt2 );
	EXPECT_FALSE( opt == opt2 );
	opt.set( opt2 );
	EXPECT_TRUE( opt.has_value() );
	EXPECT_TRUE( opt2.has_value() );
	EXPECT_TRUE( opt == opt2 );
	opt2.reset();
	EXPECT_TRUE( opt.has_value() );
	EXPECT_FALSE( opt2.has_value() );
	EXPECT_TRUE( opt != opt2 );
	opt2.set();
	EXPECT_TRUE( opt != opt2 );
	opt2.value() = opt.value();
	EXPECT_TRUE( opt == opt2 );
	opt.set( 0x13337 );
	EXPECT_TRUE( opt != opt2 );
	EXPECT_FALSE( opt == opt2 );
	EXPECT_FALSE( opt == opt2.value() );
	EXPECT_TRUE( opt != opt2.value() );
	EXPECT_TRUE( opt.value() != opt2.value() );
	EXPECT_TRUE( opt.has_value() );
	EXPECT_TRUE( opt2.has_value() );
	opt.reset();
	opt2.reset();
	EXPECT_TRUE( opt == opt2 );
	}