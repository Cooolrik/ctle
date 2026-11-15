// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/file_funcs.h>
#include <ctle/uuid.h>
#include <ctle/string_funcs.h>

#include "unit_tests.h"

#include <cstdio>
#include <type_traits>

using namespace ctle;

class file_object_test : public ::testing::Test
{
protected:
	const std::string filepath = to_hex_string( uuid::generate() );
	const std::string other_filepath = to_hex_string( uuid::generate() );

	void TearDown() override
	{
		std::remove( filepath.c_str() );
		std::remove( other_filepath.c_str() );
	}
};

TEST_F( file_object_test, read_handle_stays_open_until_closed )
{
	const std::vector<u8> expected = { 10, 20, 30 };
	ASSERT_EQ( write_file( filepath, expected ), status::ok );

	_file_object file;
	ASSERT_EQ( file.open_read( filepath ), status::ok );
	ASSERT_TRUE( file.is_open() );
	EXPECT_EQ( file.size(), expected.size() );

	std::vector<u8> actual( expected.size() );
	ASSERT_EQ( file.read( actual.data(), actual.size() ), status::ok );
	EXPECT_EQ( actual, expected );

	ASSERT_EQ( file.close(), status::ok );
	EXPECT_FALSE( file.is_open() );
	EXPECT_EQ( file.size(), 0 );
}

TEST_F( file_object_test, moving_transfers_position_and_releases_previous_file )
{
	EXPECT_FALSE( std::is_copy_constructible<_file_object>::value );
	EXPECT_FALSE( std::is_copy_assignable<_file_object>::value );
	EXPECT_TRUE( std::is_nothrow_move_constructible<_file_object>::value );
	EXPECT_TRUE( std::is_nothrow_move_assignable<_file_object>::value );

	const std::vector<u8> expected = { 10, 20, 30 };
	ASSERT_EQ( write_file( filepath, expected ), status::ok );
	_file_object source;
	ASSERT_EQ( source.open_read( filepath ), status::ok );
	u8 value = 0;
	ASSERT_EQ( source.read( &value, 1 ), status::ok );
	EXPECT_EQ( value, 10 );

	_file_object moved( std::move( source ) );
	EXPECT_FALSE( source.is_open() );
	EXPECT_EQ( source.size(), 0 );
	ASSERT_EQ( source.close(), status::ok );
	ASSERT_EQ( moved.read( &value, 1 ), status::ok );
	EXPECT_EQ( value, 20 );

	_file_object destination;
	ASSERT_EQ( destination.open_write( other_filepath ), status::ok );
	const u8 output = 40;
	ASSERT_EQ( destination.write( &output, 1 ), status::ok );
	destination = std::move( moved );
	EXPECT_FALSE( moved.is_open() );
	EXPECT_EQ( moved.size(), 0 );
	EXPECT_EQ( destination.size(), expected.size() );
	ASSERT_EQ( destination.read( &value, 1 ), status::ok );
	EXPECT_EQ( value, 30 );

	std::vector<u8> actual;
	ASSERT_EQ( read_file( other_filepath, actual ), status::ok );
	EXPECT_EQ( actual, std::vector<u8>( { 40 } ) );

	ASSERT_EQ( source.open_read( filepath ), status::ok );
	ASSERT_EQ( source.read( &value, 1 ), status::ok );
	EXPECT_EQ( value, 10 );
}

static void testReadWriteAccess()
{
	std::vector<uint8_t> cont = {};

	// allocate a random size in range 1M-2M, and write random bytes
	size_t siz = 1000000 + ( random_value<size_t>() % 1000000 );
	cont.resize( siz );
	for( size_t inx = 0; inx < siz; ++inx )
	{
		cont[inx] = random_value<uint8_t>();
	}

	// generate a unique local file name using a uuid
	std::string filename = to_hex_string( uuid::generate() );

	// make sure file does not exists 
	EXPECT_FALSE( file_exists( filename.c_str() ) );

	// write to the file
	EXPECT_TRUE( write_file( filename, cont ) );

	// make sure file now exists
	EXPECT_TRUE( file_exists( filename.c_str() ) );

	// read from the file
	std::vector<uint8_t> dest = {};
	EXPECT_TRUE( read_file( filename, dest ) );

	// make sure the containers are the same
	EXPECT_TRUE( cont == dest );
}

TEST( file_funcs, basic_test )
{
	testReadWriteAccess();
}
