// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/readers_writer_lock.h"
#include <future>

using namespace ctle;

static uint32_t ProtectedValue = 0;
static readers_writer_lock AccessLock;

TEST( readers_writer_lock , basic_test )
	{
	std::vector<std::future<void>> tasks(10);

	// spawn 10 treads which all read and write using the reader writer lock
	// each tread reads/writes randomly, but when it has written 100 times, it ends
	for( size_t inx=0; inx<10; ++inx )
		{
		tasks[inx] = std::async( std::launch::async, []{ 
			int total_read = 0;
			int total_written = 0;

			while(true)
				{
				if( rand() % 10 == 0 )
					{
					readers_writer_lock::read_guard guard( AccessLock );
					total_read += ProtectedValue;
					}
				else
					{
					readers_writer_lock::write_guard guard( AccessLock );
					ProtectedValue++;
					total_written++;
					if( total_written == 10 )
						return;
					}
				}
			});
		}

	for( size_t inx=0; inx<10; ++inx )
		{
		tasks[inx].wait();
		}

	// make sure there were 100 values written (10 threads * 10 writes per thread)
	ASSERT_EQ( ProtectedValue , 100 );
	}