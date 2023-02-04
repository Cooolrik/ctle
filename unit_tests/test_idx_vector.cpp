// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/idx_vector.h"

using namespace ctle;

TEST( idx_vector , basic_test )
	{
	idx_vector<u64> vec;
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );

	for( size_t i = 0; i < 10; ++i )
		{
		vec.index().emplace_back( (i32)i * 2 + 0 );
		vec.index().emplace_back( (i32)i * 2 + 1 );
		vec.values().emplace_back( random_value<u64>() );
		}
	EXPECT_EQ( vec.index().size() , 20 );
	EXPECT_EQ( vec.values().size() , 10 );

	idx_vector<u64> vec2 = std::move( vec );
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );
	EXPECT_EQ( vec2.index().size() , 20 );
	EXPECT_EQ( vec2.values().size() , 10 );
	EXPECT_NE( vec.index() , vec2.index() );
	EXPECT_NE( vec.values() , vec2.values() );
	EXPECT_TRUE( vec != vec2 );
	EXPECT_TRUE( !(vec == vec2) );

	vec = vec2;
	EXPECT_EQ( vec.index().size() , 20 );
	EXPECT_EQ( vec.values().size() , 10 );
	EXPECT_EQ( vec.index().size() , vec2.index().size() );
	EXPECT_EQ( vec.values().size() , vec2.values().size() );
	EXPECT_EQ( vec.index() , vec2.index() );
	EXPECT_EQ( vec.values() , vec2.values() );
	EXPECT_TRUE( vec == vec2 );
	EXPECT_TRUE( !(vec != vec2) );

	vec.clear();
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );
	EXPECT_NE( vec.index() , vec2.index() );
	EXPECT_NE( vec.values() , vec2.values() );

	vec2.clear();
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );
	EXPECT_EQ( vec2.index().size() , 0 );
	EXPECT_EQ( vec2.values().size() , 0 );
	EXPECT_EQ( vec.index() , vec2.index() );
	EXPECT_EQ( vec.values() , vec2.values() );
	EXPECT_TRUE( vec == vec2 );
	EXPECT_TRUE( !(vec != vec2) );
	}

	