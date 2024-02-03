// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/endianness.h"

#include "unit_tests.h"

using namespace ctle;

template<class _Ty> void test_bigendian_from_value( _Ty value, uint8_t *expected )
{
	uint8_t big_endian_dest[sizeof( _Ty )] = {};
	bigendian_from_value<_Ty>( big_endian_dest, value );
	for( size_t inx = 0; inx < sizeof( _Ty ); ++inx )
	{
		EXPECT_EQ( big_endian_dest[inx], expected[inx] );
	}
}

TEST( endianness, basic_test )
{
	uint8_t big_endian[8] = { 0x12 , 0x34 , 0x56 , 0x78 , 0x9a , 0xbc , 0xde , 0xf0 };

	EXPECT_EQ( value_from_bigendian<uint16_t>( big_endian ), ( (uint16_t)0x1234 ) );
	EXPECT_EQ( value_from_bigendian<uint32_t>( big_endian ), ( (uint32_t)0x12345678 ) );
	EXPECT_EQ( value_from_bigendian<uint64_t>( big_endian ), ( (uint64_t)0x123456789abcdef0 ) );

	test_bigendian_from_value<uint16_t>( 0x1234, big_endian );
	test_bigendian_from_value<uint32_t>( 0x12345678, big_endian );
	test_bigendian_from_value<uint64_t>( 0x123456789abcdef0, big_endian );

	uint16_t sb16 = 0x1234;
	swap_byte_order( &sb16 );
	EXPECT_EQ( sb16, (uint16_t)0x3412 );

	uint32_t sb32 = 0x12345678;
	swap_byte_order( &sb32 );
	EXPECT_EQ( sb32, (uint32_t)0x78563412 );

	uint64_t sb64 = 0x123456789abcdef0;
	swap_byte_order( &sb64 );
	EXPECT_EQ( sb64, (uint64_t)0xf0debc9a78563412 );
}