// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <glm/glm.hpp>

#include <type_traits>


#include "../ctle/types.h"

#include "unit_tests.h"

using namespace ctle;

template<class _Ty, size_t _Size> void TestTuple()
	{
	n_tup<_Ty,_Size> tuple = {};

	static_assert(std::is_same<decltype(tuple)::value_type, typename _Ty>::value, "value type does not match");

	// init with random values
	for( size_t i=0; i<_Size; ++i  )
		{
		tuple[i] = random_value<_Ty>();
		}

	// convert to string
	const std::string str = to_string( tuple );

	// read back from string
	auto tuple2 = from_string<n_tup<_Ty,_Size>>( str );

	// make sure they match
	for( size_t i=0; i<_Size; ++i  )
		{
		EXPECT_EQ( tuple[i] , tuple2[i] );
		}
	}

TEST( types , basic_test )
	{
	TestTuple<i8,1>();
	TestTuple<i8,2>();
	TestTuple<i8,3>();
	TestTuple<i8,4>();
	TestTuple<u8,1>();
	TestTuple<u8,2>();
	TestTuple<u8,3>();
	TestTuple<u8,4>();
	TestTuple<i16,1>();
	TestTuple<i16,2>();
	TestTuple<i16,3>();
	TestTuple<i16,4>();
	TestTuple<u16,1>();
	TestTuple<u16,2>();
	TestTuple<u16,3>();
	TestTuple<u16,4>();
	TestTuple<i32,1>();
	TestTuple<i32,2>();
	TestTuple<i32,3>();
	TestTuple<i32,4>();
	TestTuple<u32,1>();
	TestTuple<u32,2>();
	TestTuple<u32,3>();
	TestTuple<u32,4>();
	TestTuple<i64,1>();
	TestTuple<i64,2>();
	TestTuple<i64,3>();
	TestTuple<i64,4>();
	TestTuple<u64,1>();
	TestTuple<u64,2>();
	TestTuple<u64,3>();
	TestTuple<u64,4>();
	TestTuple<float,1>();
	TestTuple<float,2>();
	TestTuple<float,3>();
	TestTuple<float,4>();
	TestTuple<double,1> ();
	TestTuple<double,2> ();
	TestTuple<double,3> ();
	TestTuple<double,4> ();
	}
