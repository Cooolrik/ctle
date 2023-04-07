// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/optional_idx_vector.h"

#include "unit_tests.h"

using namespace ctle;

TEST( optional_idx_vector , basic_test )
	{
	optional_idx_vector<u64> vec;
	EXPECT_THROW( vec.index() , ctle::bad_optional_value_access );
	EXPECT_THROW( vec.values() , ctle::bad_optional_value_access );
	EXPECT_TRUE( vec == optional_idx_vector<u64>() );

	vec.set();
	for( size_t i = 0; i < 10; ++i )
		{
		vec.index().emplace_back( (i32)i * 2 + 0 );
		vec.index().emplace_back( (i32)i * 2 + 1 );
		vec.values().emplace_back( random_value<u64>() );
		}
	EXPECT_TRUE( vec.index().size() == 20 );
	EXPECT_TRUE( vec.values().size() == 10 );

	optional_idx_vector<u64> vec2 = std::move( vec );
	EXPECT_TRUE( vec2.index().size() == 20 );
	EXPECT_TRUE( vec2.values().size() == 10 );
	EXPECT_TRUE( vec != vec2 );

	vec = vec2;
	EXPECT_TRUE( vec.index().size() == 20 );
	EXPECT_TRUE( vec.values().size() == 10 );
	EXPECT_TRUE( vec.index().size() == vec2.index().size() );
	EXPECT_TRUE( vec.values().size() == vec2.values().size() );
	EXPECT_TRUE( vec.index() == vec2.index() );
	EXPECT_TRUE( vec.values() == vec2.values() );

	vec.vector().clear();
	EXPECT_TRUE( vec.index().size() == 0 );
	EXPECT_TRUE( vec.values().size() == 0 );
	EXPECT_TRUE( vec.index() != vec2.index() );
	EXPECT_TRUE( vec.values() != vec2.values() );

	vec2.vector().clear();
	EXPECT_TRUE( vec.index().size() == 0 );
	EXPECT_TRUE( vec.values().size() == 0 );
	EXPECT_TRUE( vec2.index().size() == 0 );
	EXPECT_TRUE( vec2.values().size() == 0 );
	EXPECT_TRUE( vec.index() == vec2.index() );
	EXPECT_TRUE( vec.values() == vec2.values() );
	}