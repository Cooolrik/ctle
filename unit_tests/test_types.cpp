// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <glm/glm.hpp>

#include <type_traits>


#include "../ctle/types.h"
#include "../ctle/ntup.h"

#include "unit_tests.h"

using namespace ctle;

template<class _Ty, size_t _Size> void CompareExact( const n_tup<_Ty, _Size> &tuple, const n_tup<_Ty, _Size> &tuple2 )
{
	// make sure they match exactly
	for( size_t i = 0; i < _Size; ++i )
	{
		EXPECT_EQ( tuple[i], tuple2[i] );
	}

	EXPECT_EQ( tuple, tuple2 );
	EXPECT_FALSE( tuple < tuple2 );
	EXPECT_FALSE( tuple > tuple2 );
}

template<class _Ty, size_t _Size> void CompareNear( const n_tup<_Ty, _Size> &tuple, const n_tup<_Ty, _Size> &tuple2 )
{
	// make sure they are near
	for( size_t i = 0; i < _Size; ++i )
	{
		EXPECT_NEAR( (double)tuple[i], (double)tuple2[i], 0.00001 );
	}
}

template<class _Ty, size_t _Size, bool _Exact> void TestTuple()
{
	n_tup<_Ty, _Size> tuple = {};

	static_assert( sizeof(tuple) == sizeof(_Ty)*_Size );

	// init with random values
	for( size_t i = 0; i < _Size; ++i )
	{
		tuple[i] = random_value<_Ty>();
	}

	// convert to string
	const std::string str = to_string( tuple );

	// read back from string
	auto tuple2 = from_string<n_tup<_Ty, _Size>>( str );

	if( _Exact )
	{
		CompareExact<_Ty,_Size>( tuple, tuple2 );
	}
	else
	{
		CompareNear<_Ty,_Size>( tuple, tuple2 );
	}
}

TEST( types, basic_test )
{
	TestTuple<i8, 1, true>();
	TestTuple<i8, 2, true>();
	TestTuple<i8, 3, true>();
	TestTuple<i8, 4, true>();
	TestTuple<u8, 1, true>();
	TestTuple<u8, 2, true>();
	TestTuple<u8, 3, true>();
	TestTuple<u8, 4, true>();
	TestTuple<i16, 1, true>();
	TestTuple<i16, 2, true>();
	TestTuple<i16, 3, true>();
	TestTuple<i16, 4, true>();
	TestTuple<u16, 1, true>();
	TestTuple<u16, 2, true>();
	TestTuple<u16, 3, true>();
	TestTuple<u16, 4, true>();
	TestTuple<i32, 1, true>();
	TestTuple<i32, 2, true>();
	TestTuple<i32, 3, true>();
	TestTuple<i32, 4, true>();
	TestTuple<u32, 1, true>();
	TestTuple<u32, 2, true>();
	TestTuple<u32, 3, true>();
	TestTuple<u32, 4, true>();
	TestTuple<i64, 1, true>();
	TestTuple<i64, 2, true>();
	TestTuple<i64, 3, true>();
	TestTuple<i64, 4, true>();
	TestTuple<u64, 1, true>();
	TestTuple<u64, 2, true>();
	TestTuple<u64, 3, true>();
	TestTuple<u64, 4, true>();
	TestTuple<float, 1, false>();
	TestTuple<float, 2, false>();
	TestTuple<float, 3, false>();
	TestTuple<float, 4, false>();
	TestTuple<double, 1, false>();
	TestTuple<double, 2, false>();
	TestTuple<double, 3, false>();
	TestTuple<double, 4, false>();
}

template<class _Ty, size_t _Size, bool _Exact> void TestTupleOfTuple()
{
	nm_tup<_Ty, _Size> tupletuple = {};

	static_assert( sizeof(tupletuple) == sizeof(_Ty) * _Size );

	// init with random values
	for( size_t i = 0; i < _Size; ++i )
	{
		tupletuple[i] = random_value<_Ty>();
	}

	// convert to string
	const std::string str = to_string( tupletuple );

	// read back from string
	auto tupletuple2 = from_string<nm_tup<_Ty, _Size>>( str );

	// make sure they match
	for( size_t i = 0; i < _Size; ++i )
	{
		if( _Exact )
		{
			CompareExact<_Ty::value_type,_Ty::dimensions>( tupletuple[i], tupletuple2[i] );
		}
		else
		{
			CompareNear<_Ty::value_type,_Ty::dimensions>( tupletuple[i], tupletuple2[i] );
		}
	}

}

template<class _Ty, size_t _Size, bool _Exact> void TestNMTuple()
{
	TestTupleOfTuple<n_tup<_Ty,_Size>,1,_Exact>();
	TestTupleOfTuple<n_tup<_Ty,_Size>,2,_Exact>();
	TestTupleOfTuple<n_tup<_Ty,_Size>,3,_Exact>();
	TestTupleOfTuple<n_tup<_Ty,_Size>,4,_Exact>();
}

TEST( types, tuple_of_tuple_test )
{
	TestNMTuple<i8, 1, true>();
	TestNMTuple<i8, 2, true>();
	TestNMTuple<i8, 3, true>();
	TestNMTuple<i8, 4, true>();
	TestNMTuple<u8, 1, true>();
	TestNMTuple<u8, 2, true>();
	TestNMTuple<u8, 3, true>();
	TestNMTuple<u8, 4, true>();
	TestNMTuple<i16, 1, true>();
	TestNMTuple<i16, 2, true>();
	TestNMTuple<i16, 3, true>();
	TestNMTuple<i16, 4, true>();
	TestNMTuple<u16, 1, true>();
	TestNMTuple<u16, 2, true>();
	TestNMTuple<u16, 3, true>();
	TestNMTuple<u16, 4, true>();
	TestNMTuple<i32, 1, true>();
	TestNMTuple<i32, 2, true>();
	TestNMTuple<i32, 3, true>();
	TestNMTuple<i32, 4, true>();
	TestNMTuple<u32, 1, true>();
	TestNMTuple<u32, 2, true>();
	TestNMTuple<u32, 3, true>();
	TestNMTuple<u32, 4, true>();
	TestNMTuple<i64, 1, true>();
	TestNMTuple<i64, 2, true>();
	TestNMTuple<i64, 3, true>();
	TestNMTuple<i64, 4, true>();
	TestNMTuple<u64, 1, true>();
	TestNMTuple<u64, 2, true>();
	TestNMTuple<u64, 3, true>();
	TestNMTuple<u64, 4, true>();
	TestNMTuple<float, 1, false>();
	TestNMTuple<float, 2, false>();
	TestNMTuple<float, 3, false>();
	TestNMTuple<float, 4, false>();
	TestNMTuple<double, 1, false>();
	TestNMTuple<double, 2, false>();
	TestNMTuple<double, 3, false>();
	TestNMTuple<double, 4, false>();
}