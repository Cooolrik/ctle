// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include "../ctle/string_funcs.h"

#include "unit_tests.h"

using namespace ctle;

template<class _Ty> string_span<_Ty> get_span( const std::basic_string<_Ty> &str )
{
	return { str.data(), str.data() + str.size() };
}

TEST( string_funcs, basic_test )
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
		const size_t len = ( rand() % 10 ) + 1;
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
