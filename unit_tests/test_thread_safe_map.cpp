// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

#include "../ctle/thread_safe_map.h"

using namespace ctle;

class uint_string_map_test_class
	{
	ctle::thread_safe_map<u32, std::string> *object = nullptr;
	u32 index = 0;
	static const u32 num_iters = 10;
	bool done = false;
	public:
		static void uint_string_map_test_thread( void *ptr )
			{
			std::vector<u32> list( num_iters );
			uint_string_map_test_class *pthis = (uint_string_map_test_class *)ptr;
			// insert values into list
			for( u32 i = 0; i < num_iters; ++i )
				{
				list[i] = rand() << 16 | pthis->index;
				char num[20];
#ifdef _MSC_VER
				sprintf_s( num, "%d", list[i] );
#else
				sprintf( num, "%d", list[i] );
#endif
				std::string str = num;
				pthis->object->insert( std::move( std::pair<u32, std::string>( list[i], str ) ) );
				}
			// make sure that the values exist, (do random lookup)
			for( u32 i = 0; i < num_iters; ++i )
				{
				u32 look_for = rand() % num_iters;
				char num[20];
#ifdef _MSC_VER				
				sprintf_s( num, "%d", list[look_for] );
#else
				sprintf( num, "%d", list[look_for] );
#endif				
				std::string str = num;
				auto val = pthis->object->get( list[look_for] );
				EXPECT_TRUE( val.second ); // make sure we found it
				EXPECT_TRUE( str == val.first );
				}
			// erase all values
			for( u32 i = 0; i < num_iters; ++i )
				{
				pthis->object->erase( list[i] );
				}
			pthis->done = true;
			}
		std::thread run( u32 _index, thread_safe_map<u32, std::string> *_object )
			{
			this->done = false;
			this->index = _index;
			this->object = _object;
			return std::thread( &uint_string_map_test_thread, (void *)this );
			}
		bool is_done() const
			{
			return this->done;
			}
	};

TEST( thread_safe_map , basic_test )
	{
	thread_safe_map<u32, std::string> uint_string_map;
	std::vector<uint_string_map_test_class> test_objects( 40 );
	std::vector<std::thread> threads( test_objects.size() );
	// run all threads
	for( size_t i = 0; i < test_objects.size(); ++i )
		{
		threads[i] = test_objects[i].run( (u32)i, &uint_string_map );
		}
	// wait for all threads to finish
	for( size_t i = 0; i < test_objects.size(); ++i )
		{
		threads[i].join();
		}
	}
