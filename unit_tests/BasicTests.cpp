#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace ctle;

namespace ctletests
{
	TEST_CLASS(BasicTests)
	{
	public:
		TEST_METHOD( Test_idx_vector )
			{
			idx_vector<u64> vec;
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );

			for( size_t i = 0; i < 10; ++i )
				{
				vec.index().emplace_back( (i32)i*2+0 );
				vec.index().emplace_back( (i32)i*2+1 );
				vec.values().emplace_back( random_value<u64>() );
				}
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );

			idx_vector<u64> vec2 = std::move(vec);
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.index().size() == 20 );
			Assert::IsTrue( vec2.values().size() == 10 );
			Assert::IsTrue( vec.index() != vec2.index() );
			Assert::IsTrue( vec.values() != vec2.values() );
			Assert::IsTrue( vec != vec2 );
			Assert::IsTrue( !(vec == vec2) );

			vec = vec2;
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );
			Assert::IsTrue( vec.index().size() == vec2.index().size() );
			Assert::IsTrue( vec.values().size() == vec2.values().size() );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
			Assert::IsTrue( vec == vec2 );
			Assert::IsTrue( !(vec != vec2) );

			vec.clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec.index() != vec2.index() );
			Assert::IsTrue( vec.values() != vec2.values() );

			vec2.clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.index().size() == 0 );
			Assert::IsTrue( vec2.values().size() == 0 );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
			Assert::IsTrue( vec == vec2 );
			Assert::IsTrue( !(vec != vec2) );
			}

		TEST_METHOD( Test_optional_idx_vector )
			{
			optional_idx_vector<u64> vec;
			AssertExpectException( ctle::bad_optional_value_access , vec.index(); );
			AssertExpectException( ctle::bad_optional_value_access , vec.values(); );
			Assert::IsTrue( vec == optional_idx_vector<u64>() );

			vec.set();
			for( size_t i = 0; i < 10; ++i )
				{
				vec.index().emplace_back( (i32)i*2+0 );
				vec.index().emplace_back( (i32)i*2+1 );
				vec.values().emplace_back( random_value<u64>() );
				}
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );

			optional_idx_vector<u64> vec2 = std::move(vec);
			Assert::IsTrue( vec2.index().size() == 20 );
			Assert::IsTrue( vec2.values().size() == 10 );
			Assert::IsTrue( vec != vec2 );

			vec = vec2;
			Assert::IsTrue( vec.index().size() == 20 );
			Assert::IsTrue( vec.values().size() == 10 );
			Assert::IsTrue( vec.index().size() == vec2.index().size() );
			Assert::IsTrue( vec.values().size() == vec2.values().size() );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );

			vec.vector().clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec.index() != vec2.index() );
			Assert::IsTrue( vec.values() != vec2.values() );

			vec2.vector().clear();
			Assert::IsTrue( vec.index().size() == 0 );
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.index().size() == 0 );
			Assert::IsTrue( vec2.values().size() == 0 );
			Assert::IsTrue( vec.index() == vec2.index() );
			Assert::IsTrue( vec.values() == vec2.values() );
			}

		TEST_METHOD( Test_optional_value )
			{
			optional_value<int> opt( 0x1337 );
			Assert::IsTrue( opt.has_value() );
			Assert::IsTrue( opt == 0x1337 );
			Assert::IsFalse( opt != 0x1337 );

			optional_value<int> opt2 = std::move( opt );
			AssertExpectException( ctle::bad_optional_value_access , opt.value(); );
			Assert::IsFalse( opt.has_value() );
			Assert::IsTrue( opt != 0x1337 );
			Assert::IsFalse( opt == 0x1337 );
			Assert::IsTrue( opt != 0 );
			Assert::IsTrue( opt2.has_value() );
			Assert::IsTrue( opt2 == 0x1337 );
			Assert::IsTrue( opt != opt2 );
			Assert::IsFalse( opt == opt2 );

			opt.set( opt2 );
			Assert::IsTrue( opt.has_value() );
			Assert::IsTrue( opt2.has_value() );
			Assert::IsTrue( opt == opt2 );

			opt2.reset();
			Assert::IsTrue( opt.has_value() );
			Assert::IsFalse( opt2.has_value() );
			Assert::IsTrue( opt != opt2 );

			opt2.set();
			Assert::IsTrue( opt != opt2 );
			opt2.value() = opt.value();
			Assert::IsTrue( opt == opt2 );
			opt.set( 0x13337 );
			Assert::IsTrue( opt != opt2 );
			Assert::IsFalse( opt == opt2 );
			Assert::IsFalse( opt == opt2.value() );
			Assert::IsTrue( opt != opt2.value() );
			Assert::IsTrue( opt.value() != opt2.value() );
			Assert::IsTrue( opt.has_value() );
			Assert::IsTrue( opt2.has_value() );

			opt.reset();
			opt2.reset();
			Assert::IsTrue( opt == opt2 );
			}

		TEST_METHOD( Test_optional_vector )
			{
			optional_vector<u64> vec;
			AssertExpectException( ctle::bad_optional_value_access , vec.vector(); );
			Assert::IsTrue( vec == optional_vector<u64>() );

			vec.set();
			for( size_t i = 0; i < 10; ++i )
				{
				vec.values().emplace_back( random_value<u64>() );
				}
			Assert::IsTrue( vec.values().size() == 10 );

			optional_vector<u64> vec2 = std::move(vec);
			AssertExpectException( ctle::bad_optional_value_access , vec.values(); );
			Assert::IsTrue( vec2.values().size() == 10 );
			Assert::IsTrue( vec != vec2 );

			vec = vec2;
			Assert::IsTrue( vec.values().size() == 10 );
			Assert::IsTrue( vec.values().size() == vec2.values().size() );
			Assert::IsTrue( vec.values() == vec2.values() );

			vec.vector().clear();
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec.values() != vec2.values() );

			vec2.vector().clear();
			Assert::IsTrue( vec.values().size() == 0 );
			Assert::IsTrue( vec2.values().size() == 0 );
			Assert::IsTrue( vec.values() == vec2.values() );
			}


		class uint_string_map_test_class
			{
			ctle::thread_safe_map<u32, std::string> *object = nullptr;
			u32 index = 0;
			static const u32 num_iters = 10;
			bool done = false;

			public:
				static void uint_string_map_test_thread( void * ptr )
					{
					std::vector<u32> list(num_iters);
					uint_string_map_test_class *pthis = (uint_string_map_test_class *)ptr;

					// insert values into list
					for( u32 i = 0; i < num_iters; ++i )
						{
						list[i] = rand() << 16 | pthis->index;
						char num[20];
						sprintf_s( num, "%d", list[i] );
						std::string str = num;
						pthis->object->insert( std::pair<u32,std::string>(list[i],str));
						}

					// make sure that the values exist, (do random lookup)
					for( u32 i = 0; i < num_iters; ++i )
						{
						u32 look_for = rand() % num_iters;
						char num[20];
						sprintf_s( num, "%d", list[look_for] );
						std::string str = num;
						auto val = pthis->object->get( list[look_for] );
						Assert::IsTrue( val.second ); // make sure we found it
						Assert::IsTrue( str == val.first );
						}

					// erase all values
					for( u32 i = 0; i < num_iters; ++i )
						{
						pthis->object->erase( list[i] );
						}

					pthis->done = true;
					}

				HANDLE run( u32 _index , thread_safe_map<u32, std::string> *_object )
					{
					this->done = false;
					this->index = _index;
					this->object = _object;
					return (HANDLE)_beginthread( &uint_string_map_test_thread, 0, (void *)this );
					}

				bool is_done() const
					{
					return this->done;
					}
			};

		TEST_METHOD( Test_thread_safe_map )
			{
			thread_safe_map<u32, std::string> uint_string_map;
			std::vector<uint_string_map_test_class> test_objects( 40 );
			std::vector<HANDLE> thread_handles( test_objects.size() );
			for( size_t i = 0; i < test_objects.size(); ++i )
				{
				thread_handles[i] = test_objects[i].run( (u32)i, &uint_string_map );
				}
			DWORD val = WaitForMultipleObjects( (u32)thread_handles.size(), thread_handles.data(), TRUE, INFINITE );
			Assert::IsTrue( val == 0 );
			for( size_t i = 0; i < test_objects.size(); ++i )
				{
				Assert::IsTrue( test_objects[i].is_done() );
				}
			}

	};
}
