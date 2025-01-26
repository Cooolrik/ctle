// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/file_funcs.h>
#include <ctle/uuid.h>
#include <ctle/string_funcs.h>

#include "unit_tests.h"

using namespace ctle;

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
