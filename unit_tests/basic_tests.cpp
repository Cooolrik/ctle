// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "unit_tests.h"

using namespace ctle;

TEST( basic_tests , Test_idx_vector )
	{
	idx_vector<u64> vec;
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );

	for( size_t i = 0; i < 10; ++i )
		{
		vec.index().emplace_back( (i32)i * 2 + 0 );
		vec.index().emplace_back( (i32)i * 2 + 1 );
		vec.values().emplace_back( random_value<u64>() );
		}
	EXPECT_EQ( vec.index().size() , 20 );
	EXPECT_EQ( vec.values().size() , 10 );

	idx_vector<u64> vec2 = std::move( vec );
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );
	EXPECT_EQ( vec2.index().size() , 20 );
	EXPECT_EQ( vec2.values().size() , 10 );
	EXPECT_NE( vec.index() , vec2.index() );
	EXPECT_NE( vec.values() , vec2.values() );
	EXPECT_TRUE( vec != vec2 );
	EXPECT_TRUE( !(vec == vec2) );

	vec = vec2;
	EXPECT_EQ( vec.index().size() , 20 );
	EXPECT_EQ( vec.values().size() , 10 );
	EXPECT_EQ( vec.index().size() , vec2.index().size() );
	EXPECT_EQ( vec.values().size() , vec2.values().size() );
	EXPECT_EQ( vec.index() , vec2.index() );
	EXPECT_EQ( vec.values() , vec2.values() );
	EXPECT_TRUE( vec == vec2 );
	EXPECT_TRUE( !(vec != vec2) );

	vec.clear();
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );
	EXPECT_NE( vec.index() , vec2.index() );
	EXPECT_NE( vec.values() , vec2.values() );

	vec2.clear();
	EXPECT_EQ( vec.index().size() , 0 );
	EXPECT_EQ( vec.values().size() , 0 );
	EXPECT_EQ( vec2.index().size() , 0 );
	EXPECT_EQ( vec2.values().size() , 0 );
	EXPECT_EQ( vec.index() , vec2.index() );
	EXPECT_EQ( vec.values() , vec2.values() );
	EXPECT_TRUE( vec == vec2 );
	EXPECT_TRUE( !(vec != vec2) );
	}

TEST( basic_tests , Test_optional_idx_vector )
	{
	optional_idx_vector<u64> vec;
	EXPECT_THROW( vec.index() , ctle::bad_optional_value_access );
	EXPECT_THROW( vec.values() , ctle::bad_optional_value_access );
	EXPECT_TRUE( vec == optional_idx_vector<u64>() );

	vec.set();
	for( size_t i = 0; i < 10; ++i )
		{
		vec.index().emplace_back( (i32)i * 2 + 0 );
		vec.index().emplace_back( (i32)i * 2 + 1 );
		vec.values().emplace_back( random_value<u64>() );
		}
	EXPECT_TRUE( vec.index().size() == 20 );
	EXPECT_TRUE( vec.values().size() == 10 );

	optional_idx_vector<u64> vec2 = std::move( vec );
	EXPECT_TRUE( vec2.index().size() == 20 );
	EXPECT_TRUE( vec2.values().size() == 10 );
	EXPECT_TRUE( vec != vec2 );

	vec = vec2;
	EXPECT_TRUE( vec.index().size() == 20 );
	EXPECT_TRUE( vec.values().size() == 10 );
	EXPECT_TRUE( vec.index().size() == vec2.index().size() );
	EXPECT_TRUE( vec.values().size() == vec2.values().size() );
	EXPECT_TRUE( vec.index() == vec2.index() );
	EXPECT_TRUE( vec.values() == vec2.values() );

	vec.vector().clear();
	EXPECT_TRUE( vec.index().size() == 0 );
	EXPECT_TRUE( vec.values().size() == 0 );
	EXPECT_TRUE( vec.index() != vec2.index() );
	EXPECT_TRUE( vec.values() != vec2.values() );

	vec2.vector().clear();
	EXPECT_TRUE( vec.index().size() == 0 );
	EXPECT_TRUE( vec.values().size() == 0 );
	EXPECT_TRUE( vec2.index().size() == 0 );
	EXPECT_TRUE( vec2.values().size() == 0 );
	EXPECT_TRUE( vec.index() == vec2.index() );
	EXPECT_TRUE( vec.values() == vec2.values() );
	}

TEST( basic_tests , Test_optional_value )
	{
	optional_value<int> opt( 0x1337 );
	EXPECT_TRUE( opt.has_value() );
	EXPECT_TRUE( opt == 0x1337 );
	EXPECT_FALSE( opt != 0x1337 );
	optional_value<int> opt2 = std::move( opt );
	EXPECT_THROW( opt.value(), ctle::bad_optional_value_access );
	EXPECT_FALSE( opt.has_value() );
	EXPECT_TRUE( opt != 0x1337 );
	EXPECT_FALSE( opt == 0x1337 );
	EXPECT_TRUE( opt != 0 );
	EXPECT_TRUE( opt2.has_value() );
	EXPECT_TRUE( opt2 == 0x1337 );
	EXPECT_TRUE( opt != opt2 );
	EXPECT_FALSE( opt == opt2 );
	opt.set( opt2 );
	EXPECT_TRUE( opt.has_value() );
	EXPECT_TRUE( opt2.has_value() );
	EXPECT_TRUE( opt == opt2 );
	opt2.reset();
	EXPECT_TRUE( opt.has_value() );
	EXPECT_FALSE( opt2.has_value() );
	EXPECT_TRUE( opt != opt2 );
	opt2.set();
	EXPECT_TRUE( opt != opt2 );
	opt2.value() = opt.value();
	EXPECT_TRUE( opt == opt2 );
	opt.set( 0x13337 );
	EXPECT_TRUE( opt != opt2 );
	EXPECT_FALSE( opt == opt2 );
	EXPECT_FALSE( opt == opt2.value() );
	EXPECT_TRUE( opt != opt2.value() );
	EXPECT_TRUE( opt.value() != opt2.value() );
	EXPECT_TRUE( opt.has_value() );
	EXPECT_TRUE( opt2.has_value() );
	opt.reset();
	opt2.reset();
	EXPECT_TRUE( opt == opt2 );
	}

TEST( basic_tests , Test_optional_vector )
	{
	optional_vector<u64> vec;
	EXPECT_THROW( vec.vector() , ctle::bad_optional_value_access );
	EXPECT_TRUE( vec == optional_vector<u64>() );
	vec.set();
	for( size_t i = 0; i < 10; ++i )
		{
		vec.values().emplace_back( random_value<u64>() );
		}
	EXPECT_TRUE( vec.values().size() == 10 );
	optional_vector<u64> vec2 = std::move( vec );
	EXPECT_THROW( vec.values() , ctle::bad_optional_value_access );
	EXPECT_TRUE( vec2.values().size() == 10 );
	EXPECT_TRUE( vec != vec2 );
	vec = vec2;
	EXPECT_TRUE( vec.values().size() == 10 );
	EXPECT_TRUE( vec.values().size() == vec2.values().size() );
	EXPECT_TRUE( vec.values() == vec2.values() );
	vec.vector().clear();
	EXPECT_TRUE( vec.values().size() == 0 );
	EXPECT_TRUE( vec.values() != vec2.values() );
	vec2.vector().clear();
	EXPECT_TRUE( vec.values().size() == 0 );
	EXPECT_TRUE( vec2.values().size() == 0 );
	EXPECT_TRUE( vec.values() == vec2.values() );
	}
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
TEST( basic_tests , Test_thread_safe_map )
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

status_return<bool,std::string> TestReturnString()
	{
	return { true , "hej" };
	}
enum class TestEnum : uint32_t
	{
	val0 = 0,
	val15534 = 15534,
	val242398723 = 242398723
	};
status_return<TestEnum,std::unique_ptr<int>> TestReturnUniquePtr()
	{
	auto ptr = std::make_unique<int>(100);
	return { TestEnum::val242398723 , std::move(ptr) };
	}
status_return<TestEnum,std::unique_ptr<int>> TestDontReturnUniquePtr()
	{
	return { TestEnum::val0 };
	}
status_return<TestEnum,void> TestReturnJustStatus()
	{
	return TestEnum::val15534;
	}

TEST( basic_tests , Test_status_return )
	{
	auto ret = TestReturnString();
	EXPECT_TRUE( ret.status() );
	EXPECT_TRUE( ret.value() == "hej" );
	auto ret2 = TestReturnUniquePtr();
	EXPECT_TRUE( ret2.status() == TestEnum::val242398723 );
	EXPECT_TRUE( *(ret2.value().get()) == 100 );
	auto ret3 = TestDontReturnUniquePtr();
	EXPECT_TRUE( ret3.status() == TestEnum::val0 );
	EXPECT_TRUE( ret3.value().get() == nullptr );
	auto ret4 = TestReturnJustStatus();
	EXPECT_TRUE( ret4.status() == TestEnum::val15534 );
	}

TEST( basic_tests , Test_bimap )
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
		value += (random_value<u8>() % 25) + 'A';
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
template<class _Ty> string_span<_Ty> get_span( const std::basic_string<_Ty> &str )
	{
	return {str.data(), str.data() + str.size()};
	}
	
TEST( basic_tests , Test_string_funcs )
	{
	const char *testString1_start = "123456789";
	const char *testString1_end = &testString1_start[strlen( testString1_start )];
	const char *testString2_start = " A BCD EFF GH 123 ";
	const char *testString2_end = &testString2_start[strlen( testString2_start )];
	const char *testString3_start = "-4332827     ";
	const char *testString3_end = &testString3_start[strlen( testString3_start )];
	const char *testString4_start = "   679873244321     ";
	const char *testString4_end = &testString4_start[strlen( testString4_start )];
	// generate some random values 
	const size_t random_vals_count = 30000;
	std::vector<i64> random_vals( random_vals_count );
	for( size_t i = 0; i < random_vals_count; ++i )
		{
		random_vals[i] = random_value<i64>();
		}
	// generate some random deliminator strings 
	const std::string source_delims = ",afgejsbdASDGes_ \t\r\n";
	const size_t random_delims_count = 300;
	std::vector<std::string> random_delims( random_delims_count );
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
	std::vector<string_span<char>> lexed_tokens;
	// we should never hit the char, so span all string
	EXPECT_TRUE( strcspn_t( testString1_start, testString1_end, "abcdeFGH, " ) == 9 );
	// we don't span anything with this string
	EXPECT_TRUE( strspn_t( testString1_start, testString1_end, "abcdeFGH, " ) == 0 );
	// we should hit character '3' at 2 steps in
	EXPECT_TRUE( strcspn_t( testString1_start, testString1_end, "3" ) == 2 );
	// we should hit any of 9, 8, 7, e, H or ',' at 6 steps in (7)
	EXPECT_TRUE( strcspn_t( testString1_start, testString1_end, "987eH," ) == 6 );
	// initial span of 123456_89 should be until the 6, so 6 steps in
	EXPECT_TRUE( strspn_t( testString1_start, testString1_end, "123456_89" ) == 6 );
	// second span should be 89, 2 steps in after the 7 at index 6 (so beginning at index 7)
	EXPECT_TRUE( strspn_t( &testString1_start[7], testString1_end, "123456_89" ) == 2 );
	// test string and wstring
	EXPECT_TRUE( strspn_t( get_span<char>( "qwerTY" ), "ytrewq" ) == 4 );
	EXPECT_TRUE( strcspn_t( get_span<char>( "qwerrrrrrtyTY" ), "YTy" ) == 10 );
	EXPECT_TRUE( strspn_t( get_span<wchar_t>( L"qwertyTY" ), L"ytrewq" ) == 6 );
	EXPECT_TRUE( strcspn_t( get_span<wchar_t>( L"qwertyTY" ), L"YTy" ) == 5 );
	// get the tokens of the string
	auto tok = strtok_t( testString2_start, testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ) == "A" );
	tok = strtok_t( &testString2_start[3], testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ) == "BCD" );
	tok = strtok_t( &testString2_start[6], testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ) == "EFF" );
	tok = strtok_t( &testString2_start[11], testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ) == "GH" );
	tok = strtok_t( &testString2_start[12], testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ) == "H" );
	tok = strtok_t( &testString2_start[13], testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ) == "123" );
	tok = strtok_t( &testString2_start[18], testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ).empty() );
	// test invalid & empty strings
	tok = strtok_t( testString2_end, testString2_start, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ).empty() );
	tok = strtok_t( testString2_start, testString2_start, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ).empty() );
	tok = strtok_t( testString2_end, testString2_end, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ).empty() );
	tok = strtok_t<char>( nullptr, nullptr, " " );
	EXPECT_TRUE( std::string( tok.start, tok.end ).empty() );
	// test atou64
	EXPECT_TRUE( stou64_t( testString1_start, testString1_end ) == 123456789 );
	EXPECT_TRUE( stou64_t( testString2_start, testString2_end ) == 0 );
	EXPECT_TRUE( stou64_t( testString3_start, testString3_end ) == 0 );
	EXPECT_TRUE( stou64_t( testString4_start, testString4_end ) == 0 );
	EXPECT_TRUE( stou64_t( &testString4_start[strspn_t( testString4_start, testString4_end, " \t" )], testString4_end ) == 679873244321 );
	// test atoi64	
	EXPECT_TRUE( stoi64_t( testString1_start, testString1_end ) == 123456789 );
	EXPECT_TRUE( stoi64_t( testString2_start, testString2_end ) == 0 );
	EXPECT_TRUE( stoi64_t( testString3_start, testString3_end ) == -4332827 );
	EXPECT_TRUE( stoi64_t( get_span<wchar_t>( L"678876" ) ) == 678876 );
	EXPECT_TRUE( stoi64_t( get_span<wchar_t>( L"-35678876" ) ) == -35678876 );
	// empty and invalid strings
	EXPECT_TRUE( stou64_t<char>( nullptr, nullptr ) == 0 );
	EXPECT_TRUE( stou64_t( testString1_start, testString1_start ) == 0 );
	EXPECT_TRUE( stou64_t( testString1_end, testString1_start ) == 0 );
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
		EXPECT_TRUE( stoi64_t( token ) == random_vals[token_index] );
		// move the span to beyond the token
		current_span.start = token.end;
		++token_index;
		}
	EXPECT_TRUE( token_index == random_vals_count );
	// test simple lexing
	std::string lexTest1 = " \thej,salvan hur mfds det ?! ";
	lexed_tokens.clear();
	EXPECT_TRUE( lex_t( &lexed_tokens, get_span<char>( lexTest1 ), ",?" ) );
	EXPECT_TRUE( lexed_tokens.size() == 8 );
	EXPECT_TRUE( std::string( lexed_tokens[0] ) == "hej" );
	EXPECT_TRUE( std::string( lexed_tokens[1] ) == "," );
	EXPECT_TRUE( std::string( lexed_tokens[2] ) == "salvan" );
	EXPECT_TRUE( std::string( lexed_tokens[3] ) == "hur" );
	EXPECT_TRUE( std::string( lexed_tokens[4] ) == "mfds" );
	EXPECT_TRUE( std::string( lexed_tokens[5] ) == "det" );
	EXPECT_TRUE( std::string( lexed_tokens[6] ) == "?" );
	EXPECT_TRUE( std::string( lexed_tokens[7] ) == "!" );
	// test some more complex lexing
	std::string lexTest2 = "1/2/3 4//5 //6";
	lexed_tokens.clear();
	EXPECT_TRUE( lex_t( &lexed_tokens, get_span<char>( lexTest2 ), "/" ) );
	EXPECT_TRUE( lexed_tokens.size() == 12 );
	// test string lexing as well
	std::string lexTest3 = "foo/'bar'/\"bur\" @ my = \"123\" end";
	lexed_tokens.clear();
	EXPECT_TRUE( lex_t( &lexed_tokens, get_span<char>( lexTest3 ), "/=" ) );
	EXPECT_TRUE( lexed_tokens.size() == 10 );
	// test separators
	std::string lexTest4 = "float val = 1.f; auto val2 = {{ 1, 2, 3 }}; ";
	lexed_tokens.clear();
	EXPECT_TRUE( lex_t( &lexed_tokens, get_span<char>( lexTest4 ), ";{}," ) );
	EXPECT_TRUE( lexed_tokens.size() == 18 );
	EXPECT_TRUE( std::string( lexed_tokens[12] ) == "2" );
	EXPECT_TRUE( std::string( lexed_tokens[15] ) == "}" );
	// test fail, where string is not correctly ended
	std::string lexTest5 = " str = \"this string won't end well ";
	lexed_tokens.clear();
	EXPECT_FALSE( lex_t( &lexed_tokens, get_span<char>( lexTest5 ) ) );
	EXPECT_TRUE( lexed_tokens.size() == 2 );
	// test success, where string IS correctly ended (and single quotes)
	std::string lexTest6 = " str = 'this string does end well' ";
	lexed_tokens.clear();
	EXPECT_TRUE( lex_t( &lexed_tokens, get_span<char>( lexTest6 ) ) );
	EXPECT_TRUE( lexed_tokens.size() == 3 );
	EXPECT_TRUE( std::string( lexed_tokens[2] ) == "this string does end well" );
	}

TEST( basic_tests , Test_uuid )
	{
	const uuid ido0 = hex_string_to_value<uuid>("00000000-0000-0000-0000-000000000000"); // lowest
	const uuid ido1 = hex_string_to_value<uuid>("00000000-0000-0001-0000-000000000000");
	const uuid ido2 = hex_string_to_value<uuid>("00000200-0000-0000-0000-000000000000");
	const uuid ido3 = hex_string_to_value<uuid>("ffffffff-ffff-ffff-0000-000000000000"); // highest

	uuid id = uuid::generate();

	std::string idstr = value_to_hex_string(id);
	uuid id2 = hex_string_to_value<uuid>(idstr.c_str());
	EXPECT_TRUE( id == id2 );

	EXPECT_TRUE( id != uuid::nil );
	EXPECT_TRUE( id2 != uuid::nil );

	uuid id_nil = hex_string_to_value<uuid>("00000000-0000-0000-0000-000000000000");
	EXPECT_TRUE( id_nil == uuid::nil );

	std::stringstream str;
	str << id;
	uuid id3 = hex_string_to_value<uuid>(str.str().c_str());
	EXPECT_TRUE( id == id3 );

	id2 = id3;
	id = uuid(id2);
	EXPECT_TRUE( id == id3 );

	std::vector<uuid> a = { uuid::generate() , uuid::generate() };
	std::vector<uuid> b;
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( b.empty() );

	b = std::move(a);
	EXPECT_FALSE( a == b );
	EXPECT_TRUE( a != b );
	EXPECT_TRUE( a.empty() );

	a = b;
	EXPECT_TRUE( a == b );
	EXPECT_FALSE( a != b );

	// test ordering
	if( true )
		{
		EXPECT_TRUE( ido0._data_q[0] == 0 && ido0._data_q[1] == 0 );
		EXPECT_TRUE( ido1._data_q[0] != 0 && ido1._data_q[1] == 0 );
		EXPECT_TRUE( ido2._data_q[0] != 0 && ido2._data_q[1] == 0 );
		EXPECT_TRUE( ido3._data_q[0] == 0xffffffffffffffff && ido3._data_q[1] == 0 );

		EXPECT_TRUE( ido0 != ido1 );
		EXPECT_TRUE( ido1 != ido0 );
		EXPECT_TRUE( ido0 < ido1 );
		EXPECT_FALSE( ido1 < ido0 );
		EXPECT_FALSE( ido0 == ido1 );
		EXPECT_FALSE( ido1 == ido0 );

		EXPECT_TRUE( std::hash<uuid>{}(ido0) != std::hash<uuid>{}(ido1) );
		EXPECT_TRUE( std::hash<uuid>{}(ido1) != std::hash<uuid>{}(ido0) );
		}

	// test lookup in map
	if( true )
		{
		std::map<uuid,std::string> idstrmap;
		idstrmap.insert( std::pair<uuid,std::string>(ido0,"ido0") );
		idstrmap.insert( std::pair<uuid,std::string>(ido1,"ido1") );
		idstrmap.insert( std::pair<uuid,std::string>(ido2,"ido2") );
		idstrmap.insert( std::pair<uuid,std::string>(ido3,"ido3") );
		EXPECT_EQ( idstrmap.size() , 4 );

		EXPECT_EQ( idstrmap.find(ido0)->second , "ido0" );
		EXPECT_EQ( idstrmap.find(ido1)->second , "ido1" );
		EXPECT_EQ( idstrmap.find(ido2)->second , "ido2" );
		EXPECT_EQ( idstrmap.find(ido3)->second , "ido3" );
		}

	// insert a number of generated values.
	if( true )
		{
		std::map<uuid,uuid> idmap;
		for( size_t inx=0; inx<1000; ++inx )
			{
			uuid id = uuid::generate();
			idmap[id] = id;
			}
		EXPECT_EQ( idmap.size() , 1000 );
		}


	}

template<class _Ty> void test_bigendian_from_value( _Ty value , uint8_t *expected )
	{
	uint8_t big_endian_dest[sizeof(_Ty)] = {};
	bigendian_from_value<_Ty>( big_endian_dest , value );
	for( size_t inx=0; inx<sizeof(_Ty); ++inx )
		{
		EXPECT_EQ( big_endian_dest[inx] , expected[inx] );
		}
	}

TEST( basic_tests , Test_endianness )
	{
	uint8_t big_endian[8] = { 0x12 , 0x34 , 0x56 , 0x78 , 0x9a , 0xbc , 0xde , 0xf0 };

	EXPECT_EQ( value_from_bigendian<uint16_t>( big_endian ) , ((uint16_t)0x1234) );
	EXPECT_EQ( value_from_bigendian<uint32_t>( big_endian ) , ((uint32_t)0x12345678) );
	EXPECT_EQ( value_from_bigendian<uint64_t>( big_endian ) , ((uint64_t)0x123456789abcdef0) );

	test_bigendian_from_value<uint16_t>( 0x1234 , big_endian );
	test_bigendian_from_value<uint32_t>( 0x12345678 , big_endian );
	test_bigendian_from_value<uint64_t>( 0x123456789abcdef0 , big_endian );

	uint16_t sb16 = 0x1234;
	swap_byte_order( &sb16 );
	EXPECT_EQ( sb16 , (uint16_t)0x3412 );

	uint32_t sb32 = 0x12345678;
	swap_byte_order( &sb32 );
	EXPECT_EQ( sb32 , (uint32_t)0x78563412 );

	uint64_t sb64 = 0x123456789abcdef0;
	swap_byte_order( &sb64 );
	EXPECT_EQ( sb64 , (uint64_t)0xf0debc9a78563412 );
	}

TEST( basic_tests , Test_status )
	{	
	status res = status_code::ok;

	EXPECT_TRUE( res == status_code::ok );
	EXPECT_FALSE( res != status_code::ok );

	EXPECT_TRUE( res != status_code::cant_allocate );
	EXPECT_FALSE( res == status_code::cant_allocate );

	EXPECT_TRUE( (res) );
	EXPECT_FALSE( !(res) );

	res = status_code::cant_write;

	EXPECT_TRUE( res != status_code::ok );
	EXPECT_FALSE( res == status_code::ok );

	EXPECT_TRUE( res == status_code::cant_write );
	EXPECT_FALSE( res != status_code::cant_write );

	EXPECT_FALSE( (res) );
	EXPECT_TRUE( !(res) );

	EXPECT_EQ( (int)res.value() , -109 );
	EXPECT_TRUE( res.name() == "cant_write" );
	EXPECT_TRUE( res.description() == "cant write to file or handle" );
	}

TEST( basic_tests , Test_log )
	{		
	set_global_log_level( log_level::warning );
	EXPECT_TRUE( get_global_log_level() == log_level::warning );
	}