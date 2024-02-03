// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/multithread_pool.h"
#include "../ctle/status_return.h"

#include "unit_tests.h"

using namespace ctle;

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
