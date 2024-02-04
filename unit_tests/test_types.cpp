// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <glm/glm.hpp>

#include <type_traits>


#include "../ctle/types.h"
#include "../ctle/ntup.h"

#include "unit_tests.h"

using namespace ctle;

template<class _Ty, size_t _Size> void TestTuple()
{
	n_tup<_Ty, _Size> tuple = {};

	// init with random values
	for( size_t i = 0; i < _Size; ++i )
	{
		tuple[i] = random_value<_Ty>();
	}

	// convert to string
	const std::string str = to_string( tuple );

	// read back from string
	auto tuple2 = from_string<n_tup<_Ty, _Size>>( str );

	// make sure they match
	for( size_t i = 0; i < _Size; ++i )
	{
		EXPECT_EQ( tuple[i], tuple2[i] );
	}

	EXPECT_EQ( tuple, tuple2 );
	EXPECT_FALSE( tuple < tuple2 );
	EXPECT_FALSE( tuple > tuple2 );
}

TEST( types, basic_test )
{
	TestTuple<i8, 1>();
	TestTuple<i8, 2>();
	TestTuple<i8, 3>();
	TestTuple<i8, 4>();
	TestTuple<u8, 1>();
	TestTuple<u8, 2>();
	TestTuple<u8, 3>();
	TestTuple<u8, 4>();
	TestTuple<i16, 1>();
	TestTuple<i16, 2>();
	TestTuple<i16, 3>();
	TestTuple<i16, 4>();
	TestTuple<u16, 1>();
	TestTuple<u16, 2>();
	TestTuple<u16, 3>();
	TestTuple<u16, 4>();
	TestTuple<i32, 1>();
	TestTuple<i32, 2>();
	TestTuple<i32, 3>();
	TestTuple<i32, 4>();
	TestTuple<u32, 1>();
	TestTuple<u32, 2>();
	TestTuple<u32, 3>();
	TestTuple<u32, 4>();
	TestTuple<i64, 1>();
	TestTuple<i64, 2>();
	TestTuple<i64, 3>();
	TestTuple<i64, 4>();
	TestTuple<u64, 1>();
	TestTuple<u64, 2>();
	TestTuple<u64, 3>();
	TestTuple<u64, 4>();
	TestTuple<float, 1>();
	TestTuple<float, 2>();
	TestTuple<float, 3>();
	TestTuple<float, 4>();
	TestTuple<double, 1>();
	TestTuple<double, 2>();
	TestTuple<double, 3>();
	TestTuple<double, 4>();
}

template<class _Ty> _Ty random_tuple()
{
	_Ty tup = {};

	for( size_t i = 0; i < _Ty::dimensions; ++i )
	{
		tup[i] = random_value<typename _Ty::value_type>();
	}

	return tup;
}

template<class _Ty, size_t _Size> void TestTupleOfTuple()
{
	nm_tup<_Ty, _Size> tupletuple = {};

	// init with random values
	for( size_t i = 0; i < _Size; ++i )
	{
		tupletuple[i] = random_tuple<_Ty>();
	}

	// convert to string
	const std::string str = to_string( tupletuple );

	// read back from string
	auto tupletuple2 = from_string<nm_tup<_Ty, _Size>>( str );

	// make sure they match
	for( size_t i = 0; i < _Size; ++i )
	{
		EXPECT_EQ( tupletuple[i], tupletuple2[i] );
	}

	EXPECT_EQ( tupletuple, tupletuple2 );
	EXPECT_FALSE( tupletuple < tupletuple2 );
	EXPECT_FALSE( tupletuple > tupletuple2 );
}

template<class _Ty, size_t _Size> void TestNMTuple()
{
	TestTupleOfTuple<n_tup<_Ty,_Size>,1>();
	TestTupleOfTuple<n_tup<_Ty,_Size>,2>();
	TestTupleOfTuple<n_tup<_Ty,_Size>,3>();
	TestTupleOfTuple<n_tup<_Ty,_Size>,4>();
}

TEST( types, tuple_of_tuple_test )
{
	TestNMTuple<i8, 1>();
	TestNMTuple<i8, 2>();
	TestNMTuple<i8, 3>();
	TestNMTuple<i8, 4>();
	TestNMTuple<u8, 1>();
	TestNMTuple<u8, 2>();
	TestNMTuple<u8, 3>();
	TestNMTuple<u8, 4>();
	TestNMTuple<i16, 1>();
	TestNMTuple<i16, 2>();
	TestNMTuple<i16, 3>();
	TestNMTuple<i16, 4>();
	TestNMTuple<u16, 1>();
	TestNMTuple<u16, 2>();
	TestNMTuple<u16, 3>();
	TestNMTuple<u16, 4>();
	TestNMTuple<i32, 1>();
	TestNMTuple<i32, 2>();
	TestNMTuple<i32, 3>();
	TestNMTuple<i32, 4>();
	TestNMTuple<u32, 1>();
	TestNMTuple<u32, 2>();
	TestNMTuple<u32, 3>();
	TestNMTuple<u32, 4>();
	TestNMTuple<i64, 1>();
	TestNMTuple<i64, 2>();
	TestNMTuple<i64, 3>();
	TestNMTuple<i64, 4>();
	TestNMTuple<u64, 1>();
	TestNMTuple<u64, 2>();
	TestNMTuple<u64, 3>();
	TestNMTuple<u64, 4>();
	TestNMTuple<float, 1>();
	TestNMTuple<float, 2>();
	TestNMTuple<float, 3>();
	TestNMTuple<float, 4>();
	TestNMTuple<double, 1>();
	TestNMTuple<double, 2>();
	TestNMTuple<double, 3>();
	TestNMTuple<double, 4>();
}