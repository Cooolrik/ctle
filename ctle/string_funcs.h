// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <utility>

namespace ctle
	{
	// a span of characters, with start and end
	template<class _Ty> struct string_span
		{
		const _Ty *start = {}; // first char of span
		const _Ty *end = {}; // the character after the last char of the span

		// make a copy to a string
		operator std::basic_string<_Ty>() { return std::basic_string<_Ty>( this->start, this->end ); }
		};

	// get length of span of characters between start and end, which are in the control list of chars
	// Caveat: The chars in control are required to be in the lower set (not extended characters)
	template<class _Ty> size_t strspn_t( const _Ty *start, const _Ty *end, const _Ty *control )
		{
		const _Ty *ptr = start;

		// scan until end at maximum
		while( ptr < end )
			{
			// look through delimiters for a match
			bool found = false;
			for( const _Ty *d = control; *d != _Ty( 0 ); ++d )
				{
				if( *d == *ptr )
					{
					found = true;
					break;
					}
				}

			// if not found, we are at the end of the span
			if( !found )
				{
				break;
				}

			// step
			++ptr;
			}

		// at the end
		return ptr - start;
		}
	template<class _Ty> size_t strspn_t( const string_span<_Ty> &span, const _Ty *control )
		{
		return strspn_t( span.start, span.end, control );
		}

	// get length of span of characters between start and end, until first occurance of a character from the control list
	// Caveat: The chars in control are required to be in the lower set (not extended characters)
	template<class _Ty> size_t strcspn_t( const _Ty *start, const _Ty *end, const _Ty *control )
		{
		const _Ty *ptr = start;

		// scan until end at maximum
		while( ptr < end )
			{
			// look through delimiters for a match
			for( const _Ty *d = control; *d != _Ty( 0 ); ++d )
				{
				if( *d == *ptr )
					{
					// we have a match, early out
					// return the span to before this pointer
					return ptr - start;
					}
				}

			// no match, step one
			++ptr;
			}

		// not found, return the full span from start to end
		return ptr - start;
		}
	template<class _Ty> size_t strcspn_t( const string_span<_Ty> &span, const _Ty *control )
		{
		return strcspn_t( span.start, span.end, control );
		}

	// given a start and end pointer to a string, and a list of deliminators, return the first token.
	// the implementation does not modify the input string, and does not hold any internal state.
	// Caveat: The chars in delims are required to be in the lower set (not extended characters)
	template<class _Ty> string_span<_Ty> strtok_t( const _Ty *start, const _Ty *end, const _Ty *delims )
		{
		// skip over initial delimiters
		size_t span = strspn_t<_Ty>( start, end, delims );
		const _Ty *tokStart = start + span;

		// get span until next occurance of delimiter
		span = strcspn_t<_Ty>( tokStart, end, delims );
		const _Ty *tokEnd = tokStart + span;

		return {tokStart, tokEnd};
		}
	template<class _Ty> string_span<_Ty> strtok_t( const string_span<_Ty> &span, const _Ty *delims )
		{
		return strtok_t( span.start, span.end, delims );
		}

	// given a start and end pointer to a string, parse an unsigned decimal number.
	// if values other than numbers are found, the parsing stops and the value is returned, so trim any preceeding white space
	// the implementation does not modify the input string, and does not hold any internal state.
	template<class _Ty> uint64_t stou64_t( const _Ty *start, const _Ty *end )
		{
		const _Ty *ptr = start;
		uint64_t value = 0;

		if( ptr >= end )
			return 0;

		while( ptr < end )
			{
			size_t c = *ptr - _Ty( '0' );
			if( c >= 10 )
				break; // not a digit

			value = value * 10 + c;
			++ptr;
			}

		return value;
		}
	template<class _Ty> uint64_t stou64_t( const string_span<_Ty> &span )
		{
		return stou64_t( span.start, span.end );
		}

	// given a start and end pointer to a string, parse a signed decimal number.
	// if values other than numbers are found, the parsing stops and the value is returned, so trim any preceeding white space
	// the implementation does not modify the input string, and does not hold any internal state.
	template<class _Ty> int64_t stoi64_t( const _Ty *start, const _Ty *end )
		{
		const _Ty *ptr = start;
		bool sign = false;

		if( ptr >= end )
			return 0;

		// check for preceeding sign
		if( *ptr == _Ty( '-' ) )
			{
			sign = true;
			++ptr;
			}

		int64_t value = int64_t( stou64_t( ptr, end ) );
		return sign ? -value : value;
		}
	template<class _Ty> int64_t stoi64_t( const string_span<_Ty> &span )
		{
		return stoi64_t( span.start, span.end );
		}

	// given a start and end pointer to a string, lex into a vector of tokens. the lexer handles
	// whitespace and quoted strings, as well as separator characters. 
	// (pre-reserve in the vector the expected nunber of tokens)
	// Caveat: The lexer will not handle multi-char separators, but will instead return each
	// char in the separators set as an individual token
	template<class _Ty> bool lex_t( std::vector<string_span<_Ty>> *dest, const _Ty *start, const _Ty *end, const _Ty *separators = ",/*()=[]{}", const _Ty *quotes = "'\"", const _Ty *whitespaces = " \t\r\n" )
		{
		const _Ty *ptr = start;

		// lambda to skip over any whitespace 
		auto ws_skip = [&]()
			{
			for( ; ptr < end; ++ptr )
				{
				bool found = false;
				for( const _Ty *d = whitespaces; *d != _Ty( 0 ); ++d )
					{
					if( *d == *ptr )
						{
						found = true;
						break;
						}
					}
				if( !found )
					return true;
				}
			return false;
			};

		// check if in list
		auto is_a = [&]( const _Ty *char_list )
			{
			for( const _Ty *d = char_list; *d != _Ty( 0 ); ++d )
				{
				if( *d == *ptr )
					return *ptr;
				}
			return _Ty( 0 );
			};

		// main parse loop
		while( ptr < end )
			{
			// skip whitespace, break if nothing left
			if( !ws_skip() )
				break;

			// if this is a separator, output as a specific token, and continue
			if( is_a( separators ) )
				{
				dest->emplace_back( string_span<_Ty>({ptr,ptr + 1}) );
				++ptr;
				continue;
				}

			// if this is the start of a string in quotes, parse the whole string as a token
			if( _Ty c = is_a( quotes ) )
				{
				string_span<_Ty> str_span;

				// scan until we find another of the same quotation mark, (skip the marks in the token span)
				++ptr;
				str_span.start = ptr;
				for(;;)
					{
					if( ptr >= end )
						return false; // error: reached end of string without an end quote
					if( *ptr == c )
						{
						str_span.end = ptr;
						break;
						}
					++ptr;
					}

				dest->emplace_back( str_span );
				++ptr;
				continue;
				}

			// not a string or separator, parse as token until we reach any other character
			string_span<_Ty> token_span;
			token_span.start = ptr;
			++ptr;
			for(;;)
				{
				if( ptr >= end
					|| is_a( separators )
					|| is_a( whitespaces )
					|| is_a( quotes ) )
					{
					token_span.end = ptr;
					dest->emplace_back( token_span );
					break;
					}
				++ptr;
				}
			}

		return true;
		}
	template<class _Ty> bool lex_t( std::vector<string_span<_Ty>> *dest, const string_span<_Ty> &span, const _Ty *separators = ",/*()=[]{}", const _Ty *quotes = "'\"", const _Ty *whitespaces = " \t\r\n" )
		{
		return lex_t( dest, span.start, span.end, separators, quotes, whitespaces );
		}

	};
