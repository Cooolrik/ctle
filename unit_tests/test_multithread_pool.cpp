// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include <ctle/multithread_pool.h>
#include <ctle/status_return.h>

#include <future>
#include <cstdlib>

using namespace ctle;

TEST( multithread_pool, deinitialize_destroys_previous_output_after_unlocking )
{
	// Isolate a possible deadlock in a child with a deadline, rather than hanging the suite.
	GTEST_FLAG_SET( death_test_style, "threadsafe" );
	ASSERT_EXIT( ( []()
	{
		std::thread( []()
		{
			std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
			std::_Exit( 2 );
		} ).detach();

		struct observed_item
		{
			std::function<void()> on_destroy;
			~observed_item()
			{
				if( on_destroy )
					on_destroy();
			}
		};

		{
			multithread_pool<observed_item> pool;
			bool destroyed = false;
			std::vector<std::unique_ptr<observed_item>> objects;
			objects.emplace_back( new observed_item() );
			observed_item *loan = objects.front().get();
			pool.initialize( objects );
			ASSERT_EQ( pool.borrow_item(), loan );
			objects.emplace_back( new observed_item{ [&]()
			{
				destroyed = true;
				EXPECT_EQ( pool.available_count(), 0 );
				EXPECT_FALSE( pool.item_borrowed() );
				EXPECT_EQ( pool.borrow_item(), nullptr );
				EXPECT_FALSE( pool.return_item( loan ) );
			} } );

			EXPECT_FALSE( pool.deinitialize( objects ) );
			EXPECT_TRUE( destroyed );
			ASSERT_EQ( objects.size(), 1 );
			EXPECT_EQ( objects.front().get(), loan );
		}
		std::_Exit( ::testing::Test::HasFailure() ? 1 : 0 );
	} )(), ::testing::ExitedWithCode( 0 ), "" );
}

TEST( multithread_pool, deinitialize_detaches_all_lending_state )
{
	for( const bool borrow : { false, true } )
	{
		multithread_pool<int> pool;
		std::vector<std::unique_ptr<int>> objects;
		objects.emplace_back( new int( 1 ) );
		objects.emplace_back( new int( 2 ) );
		int *second = objects.back().get();
		pool.initialize( objects );
		if( borrow )
		{
			ASSERT_EQ( pool.borrow_item(), second );
		}

		EXPECT_EQ( pool.deinitialize( objects ), !borrow );
		ASSERT_EQ( objects.size(), 2 );
		EXPECT_EQ( objects.back().get(), second );
		EXPECT_FALSE( pool.item_available() );
		EXPECT_EQ( pool.available_count(), 0 );
		EXPECT_FALSE( pool.item_borrowed() );
		EXPECT_EQ( pool.borrow_item(), nullptr );
		EXPECT_FALSE( pool.return_item( second ) );

		std::vector<std::unique_ptr<int>> empty;
		EXPECT_TRUE( pool.deinitialize( empty ) );
		EXPECT_TRUE( empty.empty() );
	}
}

TEST( multithread_pool, initialize_replaces_previous_loans_and_available_items )
{
	for( const bool empty : { false, true } )
	{
		multithread_pool<int> pool;
		std::vector<std::unique_ptr<int>> original;
		original.emplace_back( new int( 1 ) );
		original.emplace_back( new int( 2 ) );
		pool.initialize( original );
		ASSERT_NE( pool.borrow_item(), nullptr );

		std::vector<std::unique_ptr<int>> replacement;
		if( !empty )
			replacement.emplace_back( new int( 9 ) );
		pool.initialize( replacement );
		EXPECT_TRUE( replacement.empty() );
		EXPECT_FALSE( pool.item_borrowed() );
		EXPECT_EQ( pool.available_count(), empty ? 0 : 1 );
		if( !empty )
		{
			int *item = pool.borrow_item();
			ASSERT_NE( item, nullptr );
			EXPECT_EQ( *item, 9 );
			EXPECT_EQ( pool.borrow_item(), nullptr );
			EXPECT_TRUE( pool.return_item( item ) );
		}

		EXPECT_TRUE( pool.deinitialize( replacement ) );
		EXPECT_EQ( replacement.size(), empty ? 0 : 1 );
	}
}

TEST( multithread_pool, detached_loans_do_not_enter_a_reinitialized_pool )
{
	multithread_pool<int> pool;
	std::vector<std::unique_ptr<int>> detached;
	detached.emplace_back( new int( 1 ) );
	pool.initialize( detached );
	int *old_loan = pool.borrow_item();
	ASSERT_NE( old_loan, nullptr );
	EXPECT_FALSE( pool.deinitialize( detached ) );

	std::vector<std::unique_ptr<int>> replacement;
	replacement.emplace_back( new int( 9 ) );
	int *new_item = replacement.front().get();
	pool.initialize( replacement );
	EXPECT_FALSE( pool.return_item( old_loan ) );
	EXPECT_EQ( pool.available_count(), 1 );
	ASSERT_EQ( pool.borrow_item(), new_item );
	EXPECT_EQ( pool.borrow_item(), nullptr );
	EXPECT_TRUE( pool.return_item( new_item ) );
	EXPECT_TRUE( pool.deinitialize( replacement ) );
}

TEST( multithread_pool, basic_test )
{
	multithread_pool<int> mypool;

	std::vector<std::unique_ptr<int>> objlist;
	objlist.push_back( std::unique_ptr<int>( new int( 1 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 2 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 3 ) ) );

	mypool.initialize( objlist );
	EXPECT_TRUE( objlist.empty() );

	EXPECT_TRUE( mypool.item_available() );
	int *p3 = mypool.borrow_item();
	EXPECT_EQ( *p3, 3 );
	EXPECT_TRUE( mypool.item_available() );
	int *p2 = mypool.borrow_item();
	EXPECT_EQ( *p2, 2 );
	EXPECT_TRUE( mypool.item_available() );
	int *p1 = mypool.borrow_item();
	EXPECT_EQ( *p1, 1 );
	EXPECT_FALSE( mypool.item_available() );

	EXPECT_TRUE( mypool.return_item( p3 ) );
	EXPECT_TRUE( mypool.return_item( p2 ) );
	EXPECT_TRUE( mypool.return_item( p1 ) );

	int *pp1 = mypool.borrow_item();
	EXPECT_EQ( *pp1, 1 );
	int *pp2 = mypool.borrow_item();
	EXPECT_EQ( *pp2, 2 );
	int *pp3 = mypool.borrow_item();
	EXPECT_EQ( *pp3, 3 );

	mypool.deinitialize( objlist );
	EXPECT_EQ( objlist.size(), 3 );
}

TEST( multithread_pool, multithread_test )
{
	multithread_pool<int> mypool;
	std::vector<std::future<void>> tasks( 2 );

	std::vector<std::unique_ptr<int>> objlist;
	objlist.push_back( std::unique_ptr<int>( new int( 1 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 2 ) ) );
	objlist.push_back( std::unique_ptr<int>( new int( 3 ) ) );

	mypool.initialize( objlist );
	EXPECT_TRUE( objlist.empty() );

	// spawn 2 treads which use the pool to borrow and return items
	for (size_t inx = 0; inx < 2; ++inx)
	{
		tasks[inx] = std::async( std::launch::async, [&]
			{
				for( size_t inx = 0; inx < 100; ++inx )
				{
					// since we only have 2 threads, we should be able to borrow at 
					// all times, since each thread will return the item before borrowing again
					int* p = mypool.borrow_item();
					EXPECT_TRUE( p != nullptr );
					if (p)
					{
						*p = *p + 1;
						std::this_thread::yield();
						mypool.return_item(p);
					}
				}
			} );
	}
	for( size_t inx = 0; inx < 2; ++inx )
	{
		tasks[inx].wait();
	}
	
	EXPECT_FALSE(mypool.item_borrowed());

	mypool.deinitialize(objlist);

	// since we only borrowed from two threads, we should have 1 left at index 0, which was never borrowed
	EXPECT_EQ(*(objlist[0].get()), 1);

	// since the other two were borrowed 100 times each (on average), they should total 205 (=100+100+2+3)
	size_t total_for_borrowed = *(objlist[1].get()) + *(objlist[2].get());
	EXPECT_EQ(total_for_borrowed, 205); 

	EXPECT_EQ(objlist.size(), 3);
}