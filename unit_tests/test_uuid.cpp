// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/uuid.h>
#include <ctle/string_funcs.h>

#include "unit_tests.h"

using namespace ctle;

constexpr const ctle::uuid uuid_zero = ctle::uuid::zero();
constexpr const ctle::uuid uuid_inf = ctle::uuid::inf();
constexpr const ctle::uuid uuid_sup = ctle::uuid::sup();

TEST( uuid, basic_test )
{
	const uuid ido0 = from_hex_string<uuid>( "00000000-0000-0000-0000-000000000000" ); // lowest
	const uuid ido1 = from_hex_string<uuid>( "00000000-0000-0001-0000-000000000000" );
	const uuid ido2 = from_hex_string<uuid>( "00000200-0000-0000-0000-000000000000" );
	const uuid ido3 = from_hex_string<uuid>( "ffffffff-ffff-ffff-0000-000000000000" ); // highest

	uuid id = uuid::generate();

	std::stringstream sts;
	sts << id;
	uuid idfromstring = from_string<uuid>(sts.str());
	EXPECT_TRUE( id == idfromstring );

	std::string idstr = to_hex_string( id );
	uuid id2 = from_hex_string<uuid>( idstr.c_str() );
	EXPECT_TRUE( id == id2 );

	EXPECT_TRUE( id != uuid::nil );
	EXPECT_TRUE( id2 != uuid::nil );

	EXPECT_TRUE( id != uuid_zero );
	EXPECT_TRUE( id < uuid_sup );
	EXPECT_TRUE( uuid_inf < id );

	uuid id_nil = from_hex_string<uuid>( "00000000-0000-0000-0000-000000000000" );
	EXPECT_TRUE( id_nil == uuid::nil );

	EXPECT_TRUE( id_nil == uuid_zero );
	EXPECT_TRUE( id < uuid_sup );
	EXPECT_TRUE( uuid_inf == id_nil );

	std::stringstream str;
	str << id;
	uuid id3 = from_hex_string<uuid>( str.str().c_str() );
	EXPECT_TRUE( id == id3 );

	id2 = id3;
	id = uuid( id2 );
	EXPECT_TRUE( id == id3 );

	std::vector<uuid> a = { uuid::generate() , uuid::generate() };
	std::vector<uuid> b;
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
		EXPECT_TRUE( ido0._data_q[0] == 0 && ido0._data_q[1] == 0 );
		EXPECT_TRUE( ido1._data_q[0] != 0 && ido1._data_q[1] == 0 );
		EXPECT_TRUE( ido2._data_q[0] != 0 && ido2._data_q[1] == 0 );
		EXPECT_TRUE( ido3._data_q[0] == 0xffffffffffffffff && ido3._data_q[1] == 0 );

		EXPECT_TRUE( ido0 != ido1 );
		EXPECT_TRUE( ido1 != ido0 );
		EXPECT_TRUE( ido0 < ido1 );
		EXPECT_FALSE( ido1 < ido0 );
		EXPECT_FALSE( ido0 == ido1 );
		EXPECT_FALSE( ido1 == ido0 );

		EXPECT_TRUE( std::hash<uuid>{}( ido0 ) != std::hash<uuid>{}( ido1 ) );
		EXPECT_TRUE( std::hash<uuid>{}( ido1 ) != std::hash<uuid>{}( ido0 ) );
	}

	// test lookup in map
	if( true )
	{
		std::map<uuid, std::string> idstrmap;
		idstrmap.insert( std::pair<uuid, std::string>( ido0, "ido0" ) );
		idstrmap.insert( std::pair<uuid, std::string>( ido1, "ido1" ) );
		idstrmap.insert( std::pair<uuid, std::string>( ido2, "ido2" ) );
		idstrmap.insert( std::pair<uuid, std::string>( ido3, "ido3" ) );
		EXPECT_EQ( idstrmap.size(), 4 );

		EXPECT_EQ( idstrmap.find( ido0 )->second, "ido0" );
		EXPECT_EQ( idstrmap.find( ido1 )->second, "ido1" );
		EXPECT_EQ( idstrmap.find( ido2 )->second, "ido2" );
		EXPECT_EQ( idstrmap.find( ido3 )->second, "ido3" );
	}

	// insert a number of generated values.
	if( true )
	{
		std::map<uuid, uuid> idmap;
		for( size_t inx = 0; inx < 1000; ++inx )
		{
			uuid myid = uuid::generate();
			idmap[myid] = myid;
		}
		EXPECT_EQ( idmap.size(), 1000 );
	}
}
