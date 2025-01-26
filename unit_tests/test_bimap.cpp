// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/bimap.h>
#include <ctle/uuid.h>

#include "unit_tests.h"

using namespace ctle;

TEST( bimap, basic_test )
{
	bimap<u32, std::string> uint_string_bimap;
	// insert two values: 0<->A and 1<->B
	uint_string_bimap.insert( 0, "A" );
	uint_string_bimap.insert( 1, "B" );
	EXPECT_TRUE( uint_string_bimap.size() == 2 );
	// make sure all keys and values are there
	EXPECT_TRUE( uint_string_bimap.contains_key( 0 ) );
	EXPECT_TRUE( uint_string_bimap.contains_key( 1 ) );
	EXPECT_TRUE( uint_string_bimap.contains_value( "A" ) );
	EXPECT_TRUE( uint_string_bimap.contains_value( "B" ) );
	EXPECT_TRUE( uint_string_bimap.get_key( "A" ).second );
	EXPECT_TRUE( uint_string_bimap.get_value( 1 ).second );
	// now, insert 1<->A instead, this should remove the previous two mappings, and replace with one mapping
	uint_string_bimap.insert( 1, "A" );
	EXPECT_TRUE( uint_string_bimap.size() == 1 );
	// make the correct values are left
	EXPECT_FALSE( uint_string_bimap.contains_key( 0 ) );
	EXPECT_TRUE( uint_string_bimap.contains_key( 1 ) );
	EXPECT_TRUE( uint_string_bimap.contains_value( "A" ) );
	EXPECT_FALSE( uint_string_bimap.contains_value( "B" ) );
	// remove the key, there should be no items left
	uint_string_bimap.erase_by_key( 1 );
	EXPECT_TRUE( uint_string_bimap.size() == 0 );
	// insert random uints and strings. cap the size to ensure some conflicts
	for( size_t i = 0; i < 100; ++i )
	{
		u8 key = random_value<u8>();
		std::string value;
		value += ( random_value<u8>() % 25 ) + 'A';
		uint_string_bimap.insert( key, value );
	}
	// for each key, make sure the value maps back to the key
	for( auto val : uint_string_bimap )
	{
		EXPECT_TRUE( uint_string_bimap.contains_key( val.first ) );
		EXPECT_TRUE( uint_string_bimap.contains_value( val.second ) );
		EXPECT_TRUE( uint_string_bimap.at_key( val.first ) == val.second );
		EXPECT_TRUE( uint_string_bimap.at_value( val.second ) == val.first );
	}
}

TEST( bimap, uuid_test )
{
	bimap<uuid, uuid> uuid_uuid_bimap;

	// build std map and bimap
	std::map<uuid,uuid> kvmap;
	for( size_t inx=0; inx<100; ++inx )
	{
		auto k = uuid::generate();
		auto v = uuid::generate();

		kvmap.emplace(k,v);	
		uuid_uuid_bimap.insert(k,v);
	}

	// make sure that the values are correctly mapped backwards
	for( const auto &kvpair: kvmap )
	{
		auto k = kvpair.first;
		auto v = kvpair.second;

		auto kres = uuid_uuid_bimap.get_key( v );
		EXPECT_TRUE( kres.second );
		EXPECT_EQ( kres.first, k );
		auto vres = uuid_uuid_bimap.get_value( kvpair.first );
		EXPECT_TRUE( vres.second );
		EXPECT_EQ( vres.first, v );
	}


}