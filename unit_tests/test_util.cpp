// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/util.h"

#include "unit_tests.h"

using namespace ctle;

TEST( util , basic_test )
	{
	auto val = nil_object::ptr();
	EXPECT_EQ( nil_object::ptr() , val );
	auto pt = (int*)val;
	EXPECT_TRUE( nil_object::is_nil(val) );
	EXPECT_TRUE( nil_object::is_nil(pt) );

	auto &ref = nil_object::ref<int>();
	EXPECT_TRUE( nil_object::is_nil(ref) );

	auto &ref2 = nil_object::ref<std::vector<std::unique_ptr<int>>>();
	EXPECT_TRUE( nil_object::is_nil(ref2) );
	}
