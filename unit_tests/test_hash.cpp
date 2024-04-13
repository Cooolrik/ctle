// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/hash.h"
#include "../ctle/string_funcs.h"

#include "unit_tests.h"

#include <map>
#include <unordered_map>

using namespace ctle;

template<size_t _Size>
static hash<_Size> random_hash()
{
	hash<_Size> val;
	for( size_t inx=0; inx<_Size/64; ++inx )
	{
		val._data_q[inx] = random_value<uint64_t>();
	}
	return val;
}

TEST( hash, basic_test )
{
	using hash = ctle::hash<256>;

	const hash hsh0 = from_string<hash>( "0000000000000000000000000000000000000000000000000000000000000000" ); // lowest
	const hash hsh1 = from_string<hash>( "0000000000000000ffffffffffffffffffffffffffffffffffffffffffffffff" );
	const hash hsh2 = from_string<hash>( "00000200ffffffffffffffffffffffffffffffffffffffffffffffffffffffff" );
	const hash hsh3 = from_string<hash>( "ffffffffffffffffffffffffffffffffffff0000000000000000000000000000" ); // highest

	std::string hshstr = to_string( hsh1 );
	auto hshval2 = from_string<hash>( hshstr.c_str() );
	EXPECT_TRUE( hsh1 == hshval2 );

	std::stringstream str;
	str << hsh2;
	auto hshval3 = from_string<hash>( str.str().c_str() );
	EXPECT_TRUE( hsh2 == hshval3 );

	auto h2 = hsh3;
	EXPECT_TRUE( h2 == hsh3 );

	std::vector<hash> a = { random_hash<hash::hash_size>() , random_hash<hash::hash_size>() };
	std::vector<hash> b;
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( b.empty() );

	b = std::move( a );
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( a.empty() );

	a = b;
	EXPECT_TRUE( a == b );
	EXPECT_FALSE( a != b );

	// test ordering
	if( true )
	{
		EXPECT_TRUE( hsh0._data_q[0] == 0 && hsh0._data_q[1] == 0 );
		EXPECT_TRUE( hsh1._data_q[0] == 0 && hsh1._data_q[1] != 0 );
		EXPECT_TRUE( hsh2._data_q[0] != 0 && hsh2._data_q[1] != 0 );
		EXPECT_TRUE( hsh3._data_q[0] == 0xffffffffffffffff && hsh3._data_q[3] == 0 );

		EXPECT_TRUE( hsh0 != hsh1 );
		EXPECT_TRUE( hsh1 != hsh0 );
		EXPECT_TRUE( hsh0 < hsh1 );
		EXPECT_FALSE( hsh1 < hsh0 );
		EXPECT_FALSE( hsh0 == hsh1 );
		EXPECT_FALSE( hsh1 == hsh0 );

		EXPECT_TRUE( std::hash<hash>{}( hsh0 ) != std::hash<hash>{}( hsh1 ) );
		EXPECT_TRUE( std::hash<hash>{}( hsh1 ) != std::hash<hash>{}( hsh0 ) );
	}

	// test lookup in map
	if( true )
	{
		std::map<hash, std::string> idstrmap;
		idstrmap.insert( std::pair<hash, std::string>( hsh0, "hsh0" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh1, "hsh1" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh2, "hsh2" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh3, "hsh3" ) );
		EXPECT_EQ( idstrmap.size(), 4 );

		EXPECT_EQ( idstrmap.find( hsh0 )->second, "hsh0" );
		EXPECT_EQ( idstrmap.find( hsh1 )->second, "hsh1" );
		EXPECT_EQ( idstrmap.find( hsh2 )->second, "hsh2" );
		EXPECT_EQ( idstrmap.find( hsh3 )->second, "hsh3" );
	}

	// test lookup in unordered_map
	if( true )
	{
		std::unordered_map<hash, std::string> idstrmap;
		idstrmap.insert( std::pair<hash, std::string>( hsh0, "hsh0" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh1, "hsh1" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh2, "hsh2" ) );
		idstrmap.insert( std::pair<hash, std::string>( hsh3, "hsh3" ) );
		EXPECT_EQ( idstrmap.size(), 4 );

		EXPECT_EQ( idstrmap.find( hsh0 )->second, "hsh0" );
		EXPECT_EQ( idstrmap.find( hsh1 )->second, "hsh1" );
		EXPECT_EQ( idstrmap.find( hsh2 )->second, "hsh2" );
		EXPECT_EQ( idstrmap.find( hsh3 )->second, "hsh3" );
	}
	
	// insert a number of generated values.
	if( true )
	{
		std::map<hash, hash> idmap;
		for( size_t inx = 0; inx < 1000; ++inx )
		{
			hash myid = random_hash<hash::hash_size>();
			idmap[myid] = myid;
		}
		EXPECT_EQ( idmap.size(), 1000 );
	}
}

TEST( hash, sha256_hashing )
{
	using hash = ctle::hash<256>;

	if( true )
	{
		hash sha;

		u8 testdata[] = {
			0x34,0x2b,0x1f,0x3e,0x61,
			0x4b,0x03,0x4b,0x02,0x36,
			0x05,0x5c,0x17,0x29,0x3d,
			0x53,0x0e,0x5e,0x5b,0x4d,
			0x52,0x5f,0x12,0x20,0x0a,
			0x56,0x31,0x3b,0x2c,0x06,
			0x51,0x28,0x28,0x5d,0x05,
			0x59,0x2b,0x41,0x0d,0x1f,
			0x01,0x01,0x1b,0x1f,0x09,
			0x2c,0x13,0x01,0x46,0x19,
			0x05,0x3e,0x3c,0x2d,0x58,
			0x16,0x5f,0x19,0x0f,0x07,
			0x39,0x48,0x46,0x4b,0x23,
			0x06,0x15,0x0b,0x44,0x18,
			0x0e,0x38,0x56,0x0e,0x0a,
			0x0e,0x54,0x43,0x0a,0x31,
			0x2d,0x51,0x0d,0x2a,0x5a,
			0x09,0x06,0x10,0x23,0x24,
			0x23,0x33,0x2e,0x1d,0x56,
			0x48,0x2f,0x4a,0x33,0x06
		};

		ctle::calculate_sha256_hash( sha, testdata, sizeof( testdata ) );

		u8 expected_hash[32] = {
			0xf6,0x48,0x54,0x2d,0xf8,0xcc,0xf2,0x1f,
			0xd3,0x4e,0x95,0xf6,0x7d,0xf5,0xf2,0xb4,
			0xf2,0x72,0x72,0xaa,0x14,0xf5,0x03,0x09,
			0x0c,0xc4,0x76,0x6f,0xe2,0x78,0xc4,0xb5
		};

		EXPECT_EQ( memcmp( sha.data, expected_hash, 32 ), 0 );

		std::stringstream sts;
		sts << sha;
		hash shafromstring = from_string<hash>(sts.str());
		EXPECT_TRUE( sha == shafromstring );

	}
}
