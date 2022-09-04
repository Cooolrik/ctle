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
						pthis->object->insert( std::move(std::pair<u32,std::string>(list[i],str)) );
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

				std::thread run( u32 _index , thread_safe_map<u32, std::string> *_object )
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

		TEST_METHOD( Test_thread_safe_map )
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

		TEST_METHOD( Test_bimap )
			{
			bimap<u32, std::string> uint_string_bimap;

			// insert two values: 0<->A and 1<->B
			uint_string_bimap.insert( 0, "A" );
			uint_string_bimap.insert( 1, "B" );
			Assert::IsTrue( uint_string_bimap.size() == 2 );

			// make sure all keys and values are there
			Assert::IsTrue( uint_string_bimap.contains_key( 0 ) );
			Assert::IsTrue( uint_string_bimap.contains_key( 1 ) );
			Assert::IsTrue( uint_string_bimap.contains_value( "A" ));
			Assert::IsTrue( uint_string_bimap.contains_value( "B" ));

			// now, insert 1<->A instead, this should remove the previous two mappings, and replace with one mapping
			uint_string_bimap.insert( 1, "A" );
			Assert::IsTrue( uint_string_bimap.size() == 1 );

			// make the correct values are left
			Assert::IsFalse( uint_string_bimap.contains_key( 0 ) );
			Assert::IsTrue( uint_string_bimap.contains_key( 1 ) );
			Assert::IsTrue( uint_string_bimap.contains_value( "A" ));
			Assert::IsFalse( uint_string_bimap.contains_value( "B" ));

			// remove the key, there should be no items left
			uint_string_bimap.erase_by_key( 1 );
			Assert::IsTrue( uint_string_bimap.size() == 0 );

			// insert random uints and strings. cap the size to ensure some conflicts
			for( size_t i = 0; i < 100; ++i )
				{
				u8 key = random_value<u8>();
				std::string value;
				value += (random_value<u8>() % 25) + 'A';

				uint_string_bimap.insert( key, value );
				}

			// for each key, make sure the value maps back to the key
			for( auto val : uint_string_bimap )
				{
				Assert::IsTrue( uint_string_bimap.contains_key( val.first ) );
				Assert::IsTrue( uint_string_bimap.contains_value( val.second ) );

				Assert::IsTrue( uint_string_bimap.at_key( val.first ) == val.second );
				Assert::IsTrue( uint_string_bimap.at_value( val.second ) == val.first );
				}

			}

		template<class _Ty> string_span<_Ty> get_span( const std::basic_string<_Ty> &str )
			{
			return {str.data(), str.data() + str.size()};
			}

		TEST_METHOD( Test_string_funcs )
			{
			const char *testString1_start = "123456789";
			const char *testString1_end = &testString1_start[strlen(testString1_start)];

			const char *testString2_start = " A BCD EFF GH 123 ";
			const char *testString2_end = &testString2_start[strlen(testString2_start)];

			const char *testString3_start = "-4332827     ";
			const char *testString3_end = &testString3_start[strlen(testString3_start)];

			const char *testString4_start = "   679873244321     ";
			const char *testString4_end = &testString4_start[strlen(testString4_start)];

			// generate some random values 
			const size_t random_vals_count = 30000;
			std::vector<i64> random_vals(random_vals_count);
			for( size_t i = 0; i < random_vals_count; ++i )
				{
				random_vals[i] = random_value<i64>();
				}

			// generate some random deliminator strings 
			const std::string source_delims = ",afgejsbdASDGes_ \t\r\n";
			const size_t random_delims_count = 300;
			std::vector<std::string> random_delims(random_delims_count);
			for( size_t i = 0; i < random_delims_count; ++i )
				{
				std::string delim_string;
				const size_t len = (rand() % 10) + 1;
				for( size_t q = 0; q < len; ++q )
					{
					delim_string += source_delims[rand() % source_delims.size()];
					}
				random_delims[i] = delim_string;
				}

			// we should never hit the char, so span all string
			Assert::IsTrue( strcspn_t( testString1_start, testString1_end, "abcdeFGH, " ) == 9 );

			// we don't span anything with this string
			Assert::IsTrue( strspn_t( testString1_start, testString1_end, "abcdeFGH, " ) == 0 );

			// we should hit character '3' at 2 steps in
			Assert::IsTrue( strcspn_t( testString1_start, testString1_end, "3" ) == 2 );

			// we should hit any of 9, 8, 7, e, H or ',' at 6 steps in (7)
			Assert::IsTrue( strcspn_t( testString1_start, testString1_end, "987eH," ) == 6 );

			// initial span of 123456_89 should be until the 6, so 6 steps in
			Assert::IsTrue( strspn_t( testString1_start, testString1_end, "123456_89" ) == 6 );

			// second span should be 89, 2 steps in after the 7 at index 6 (so beginning at index 7)
			Assert::IsTrue( strspn_t( &testString1_start[7], testString1_end, "123456_89" ) == 2 );

			// test string and wstring
			Assert::IsTrue( strspn_t( get_span<char>("qwerTY") , "ytrewq" ) == 4);
			Assert::IsTrue( strcspn_t( get_span<char>("qwerrrrrrtyTY") , "YTy" ) == 10);
			Assert::IsTrue( strspn_t( get_span<wchar_t>(L"qwertyTY") , L"ytrewq" ) == 6);
			Assert::IsTrue( strcspn_t( get_span<wchar_t>(L"qwertyTY") , L"YTy" ) == 5);

			// get the tokens of the string
			auto tok = strtok_t( testString2_start, testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ) == "A" );
			tok = strtok_t( &testString2_start[3], testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ) == "BCD" );
			tok = strtok_t( &testString2_start[6], testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ) == "EFF" );
			tok = strtok_t( &testString2_start[11], testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ) == "GH" );
			tok = strtok_t( &testString2_start[12], testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ) == "H" );
			tok = strtok_t( &testString2_start[13], testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ) == "123" );
			tok = strtok_t( &testString2_start[18], testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ).empty() );

			// test invalid & empty strings
			tok = strtok_t( testString2_end, testString2_start, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ).empty() );
			tok = strtok_t( testString2_start, testString2_start, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ).empty() );
			tok = strtok_t( testString2_end, testString2_end, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ).empty() );
			tok = strtok_t<char>( nullptr, nullptr, " " );
			Assert::IsTrue( std::string( tok.start, tok.end ).empty() );

			// test atou64
			Assert::IsTrue( stou64_t( testString1_start, testString1_end ) == 123456789 );
			Assert::IsTrue( stou64_t( testString2_start, testString2_end ) == 0 );
			Assert::IsTrue( stou64_t( testString3_start, testString3_end ) == 0 );
			Assert::IsTrue( stou64_t( testString4_start, testString4_end ) == 0 );
			Assert::IsTrue( stou64_t( &testString4_start[strspn_t(testString4_start, testString4_end, " \t")], testString4_end) == 679873244321);
							
			// test atoi64	
			Assert::IsTrue( stoi64_t( testString1_start, testString1_end ) == 123456789 );
			Assert::IsTrue( stoi64_t( testString2_start, testString2_end ) == 0 );
			Assert::IsTrue( stoi64_t( testString3_start, testString3_end ) == -4332827 );
			Assert::IsTrue( stoi64_t( get_span<wchar_t>(L"678876") ) == 678876 );
			Assert::IsTrue( stoi64_t( get_span<wchar_t>(L"-35678876") ) == -35678876 );

			// empty and invalid strings
			Assert::IsTrue( stou64_t<char>( nullptr, nullptr ) == 0 );
			Assert::IsTrue( stou64_t( testString1_start, testString1_start ) == 0 );
			Assert::IsTrue( stou64_t( testString1_end, testString1_start ) == 0 );

			// write random vals to string
			std::stringstream ss;
			for( size_t i = 0; i < random_vals_count; ++i )
				{
				ss << random_vals[i];
				ss << random_delims[i % random_delims_count];
				}
			const std::string tokens = ss.str();

			// tokenize and read back
			auto current_span = get_span( tokens );
			size_t token_index = 0;
			while( current_span.start < current_span.end )
				{
				// get the next token in the remaining span
				auto token = strtok_t( current_span, source_delims.c_str() );
				if( token.start == token.end )
					break; // no more tokens

				// get value, make sure it matches
				Assert::IsTrue( stoi64_t( token ) == random_vals[token_index] );

				// move the span to beyond the token
				current_span.start = token.end;
				++token_index;
				}
			Assert::IsTrue( token_index == random_vals_count );


			}
	};
}
