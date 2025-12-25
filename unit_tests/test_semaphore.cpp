// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/semaphore.h>

#include "unit_tests.h"

using namespace ctle;
using namespace std::chrono_literals;

TEST(semaphore, blocks_acquire) 
{
    semaphore sem(0);

    // Start a thread that will try to acquire
    bool acquired = false;
    std::thread t([&] {
        sem.acquire();
        acquired = true;
    });

    // Give the thread time to block
    std::this_thread::sleep_for(100ms);
    EXPECT_FALSE(acquired);

    // Release once, should unblock
    sem.release();
    t.join();
    EXPECT_TRUE(acquired);
}

TEST(semaphore, allows_acquire) 
{
    semaphore sem(1);

    // acquire should succeed immediately
    auto status = sem.acquire();
    EXPECT_EQ(status, status::ok); 
}

TEST(semaphore, multi_release_multi_acquire) 
{
    semaphore sem(0);

    sem.release();
    sem.release();

    // Two acquires should succeed without blocking
    auto s1 = sem.acquire();
    auto s2 = sem.acquire();

    EXPECT_EQ(s1, status::ok);
    EXPECT_EQ(s2, status::ok);
}

TEST(semaphore, multiple_threads) 
{
    semaphore sem(0);

	std::vector<std::thread> threads;
	std::vector<bool> acquired_flags( 50, false );

	// Start multiple threads that will acquire the semaphore
	for( size_t i = 0; i < 50; ++i )
    {
        threads.push_back( std::thread([&]( size_t index ) {
            sem.acquire();
		    // Simulate some work
			acquired_flags[index] = true;
		    sem.release();
	    }, i ) );
    }

	// Give the threads time to block
	std::this_thread::sleep_for( 100ms );

	// Release the semaphore once to unblock one thread, this will cause a chain reaction
	sem.release();

	// Wait for all threads to finish
	for( auto &t : threads )
	{
		t.join();
	}

	// Check that all threads acquired the semaphore
    for( size_t i = 0; i < acquired_flags.size(); ++i )
    {
        EXPECT_TRUE( acquired_flags[i] ) << "Thread " << i << " did not acquire the semaphore.";
	}

	// After all threads have run, we should be able to acquire once more
    auto res = sem.acquire();
    EXPECT_EQ(res, status::ok);
}
