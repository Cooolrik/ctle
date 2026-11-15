// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <cctype>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <new>
#include <stdexcept>
#include <type_traits>

#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L
#include <charconv>
#endif

namespace ctle
{
namespace detail
{
	enum class parse_error { none, invalid_input, out_of_range, allocation_failure };

	template<class T>
	struct number_result
	{
		T value = {};
		parse_error error = parse_error::none;
	};

	inline parse_error parse_magnitude( const char *start, const char *end, uint64_t &value ) noexcept
	{
#if (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L
		const auto result = std::from_chars( start, end, value );
		if( result.ec == std::errc::invalid_argument )
			return parse_error::invalid_input;
		if( result.ec == std::errc::result_out_of_range )
			return parse_error::out_of_range;
		return parse_error::none;
#else
		if( start == end || *start < '0' || *start > '9' )
			return parse_error::invalid_input;
		constexpr uint64_t limit = std::numeric_limits<uint64_t>::max();
		for( ; start != end; ++start )
		{
			const unsigned digit = static_cast<unsigned>( *start - '0' );
			if( digit > 9 )
				break;
			if( value > limit / 10 || (value == limit / 10 && digit > limit % 10) )
				return parse_error::out_of_range;
			value = value * 10 + digit;
		}
		return parse_error::none;
#endif
	}

	template<class T>
	number_result<T> parse_integer( const string_span<char> &text ) noexcept
	{
		if( !text.start || !text.end || text.start >= text.end )
			return { {}, parse_error::invalid_input };
		const char *start = text.start;
		while( start != text.end && std::isspace( static_cast<unsigned char>( *start ) ) )
			++start;
		if( start == text.end )
			return { {}, parse_error::invalid_input };
		const bool negative = *start == '-';
		if( negative || *start == '+' )
			++start;

		uint64_t magnitude = 0;
		const auto error = parse_magnitude( start, text.end, magnitude );
		if( error != parse_error::none )
			return { {}, error };

		constexpr uint64_t maximum = static_cast<uint64_t>( std::numeric_limits<T>::max() );
		if( std::is_signed<T>::value )
		{
			if( magnitude > maximum + uint64_t( negative ) )
				return { {}, parse_error::out_of_range };
			if( negative && magnitude == maximum + 1 )
				return { std::numeric_limits<T>::min(), parse_error::none };
			const T value = static_cast<T>( magnitude );
			return { static_cast<T>( negative ? -value : value ), parse_error::none };
		}

		// Preserve stoull's unsigned negation before checking narrower destination types.
		const uint64_t value = negative ? uint64_t(0) - magnitude : magnitude;
		if( value > maximum )
			return { {}, parse_error::out_of_range };
		return { static_cast<T>( value ), parse_error::none };
	}

	template<class T>
	T parse_real_value( const char *text, char **end ) noexcept;

	template<> inline float parse_real_value<float>( const char *text, char **end ) noexcept
	{
		return std::strtof( text, end );
	}

	template<> inline double parse_real_value<double>( const char *text, char **end ) noexcept
	{
		return std::strtod( text, end );
	}

	template<class T>
	number_result<T> parse_real_terminated( const char *text ) noexcept
	{
		const int previous_errno = errno;
		errno = 0;
		char *end = nullptr;
		const T value = parse_real_value<T>( text, &end );
		const int error = errno;
		if( error == 0 )
			errno = previous_errno;
		if( end == text )
			return { {}, parse_error::invalid_input };
		if( error == ERANGE )
			return { {}, parse_error::out_of_range };
		return { value, parse_error::none };
	}

	template<class T>
	number_result<T> parse_real( const string_span<char> &text ) noexcept
	{
		if( !text.start || !text.end || text.start >= text.end )
			return { {}, parse_error::invalid_input };
		try
		{
			return parse_real_terminated<T>( std::string( text.start, text.end ).c_str() );
		}
		catch( const std::bad_alloc & )
		{
			return { {}, parse_error::allocation_failure };
		}
		catch( const std::length_error & )
		{
			return { {}, parse_error::allocation_failure };
		}
	}

	template<class T>
	T number_or_throw( const number_result<T> &result )
	{
		if( result.error == parse_error::invalid_input )
			throw std::invalid_argument( "Could not parse numeric input." );
		if( result.error == parse_error::out_of_range )
			throw std::out_of_range( "Numeric input is outside the destination range." );
		if( result.error == parse_error::allocation_failure )
			throw std::bad_alloc();
		return result.value;
	}

	template<class T>
	T number_or_flag( const number_result<T> &result, bool &success ) noexcept
	{
		if( result.error != parse_error::none )
			success = false;
		return result.value;
	}
}
// namespace detail
}
// namespace ctle
