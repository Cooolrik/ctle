// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_HASH_H_
#define _CTLE_HASH_H_

#include <cstdint>
#include <functional>
#include <iosfwd>

#include "status.h"

namespace ctle
{
	
/// @brief A hash digest structure.
/// 
/// The hash structure encapsulates a hash value, and adds comparison operators.
/// Ctle also provides stream output of hash digests, as well as conversion to(ctle::to_string) and from(ctle::from_string) strings.
/// @tparam _Size the size of the hash in bits, must be 64, 128, 256 or 512.
/// @note The hash values are stored and compared big-endian, so most significant byte at index 0, least significant byte at the last index (7, 15, 31 or 63).
template<size_t _Size>
struct hash
{
	static_assert( _Size == 64 || _Size == 128 || _Size == 256 || _Size == 512 , "Hash size must be one of 64, 128, 256 or 512");
	static constexpr const size_t hash_size = _Size;

	/// @brief The data storage for the hash value. 
	union
	{
		/// @brief The data as bytes, stored as most significant byte at index 0, least significant byte at the last index (7, 15, 31 or 63).
		uint8_t data[_Size / 8];
		uint64_t _data_q[_Size / 64] = {};
	};

	// compare operators
	bool operator<( const hash &other ) const noexcept;
	bool operator==( const hash &other ) const noexcept;
	bool operator!=( const hash &other ) const noexcept;
};

template<size_t _Size>
inline bool hash<_Size>::operator<( const hash &right ) const noexcept
{
	const uint8_t *u1 = this->data;
	const uint8_t *u2 = right.data;

	// hash values are stored big-endian, so MSB is first byte (index 0), LSB is last byte (index 7, 15, 31 or 63)
	size_t n = _Size/8;
	do
	{
		if( *u1 != *u2 )	// not equal, early exit, check if more or less than
		{
			if( *u1 < *u2 )
				return true;	// less than
			else
				return false;	// more than
		}
		++u1;
		++u2;
		--n;
	} while( n>0 );

	return false; // equal, so not less
};

template<size_t _Size>
inline bool hash<_Size>::operator==( const hash &right ) const noexcept
{
	const uint64_t *u1 = this->_data_q;
	const uint64_t *u2 = right._data_q;

	// compare the 64bit values
	size_t n = _Size/64;
	do
	{
		if( *u1 != *u2 )	// not equal, return false
		{
			return false;	
		}
		++u1;
		++u2;
		--n;
	} while( n>0 );

	return true; // equal
};

template<size_t _Size>
inline bool hash<_Size>::operator!=( const hash &right ) const noexcept
{
	return !this->operator==( right );
};

template<size_t _Size>
inline size_t calculate_size_hash( const hash<_Size> &value )
{
	static_assert( sizeof( std::size_t ) == sizeof( std::uint64_t ), "The hashing code only works for 64bit size_t" );
	size_t hval = value._data_q[0];
	for( size_t inx=1; inx<(_Size/64); ++inx )
	{
		hval ^= value._data_q[inx];
	}
	return hval;
}

}
//namespace ctle

////////////////////////////////////////

namespace std
{

// hash<64>
template <>
struct hash<ctle::hash<64>>
{
	size_t operator()( const ctle::hash<64> &val ) const noexcept
	{
		return ctle::calculate_size_hash<64>( val );
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::hash<64>& _hash);

// hash<128>
template <>
struct hash<ctle::hash<128>>
{
	size_t operator()( const ctle::hash<128> &val ) const noexcept
	{
		return ctle::calculate_size_hash<128>( val );
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::hash<128>& _hash);

// hash<256>
template <>
struct hash<ctle::hash<256>>
{
	size_t operator()( const ctle::hash<256> &val ) const noexcept
	{
		return ctle::calculate_size_hash<256>( val );
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::hash<256>& _hash);

// hash<512>
template <>
struct hash<ctle::hash<512>>
{
	size_t operator()( const ctle::hash<512> &val ) const noexcept
	{
		return ctle::calculate_size_hash<512>( val );
	}
};
std::ostream& operator<<(std::ostream& os, const ctle::hash<512>& _hash);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CTLE_IMPLEMENTATION

#include <random>
#include <algorithm>
#include <memory>
#include <array>

#include "string_funcs.h"

namespace ctle
{

template <> std::string to_string<hash<64>>(const hash<64>& value) 		{ return bytes_to_hex_string( &value, 8 ); }
template <> std::string to_string<hash<128>>(const hash<128>& value) 	{ return bytes_to_hex_string( &value, 16 ); }
template <> std::string to_string<hash<256>>( const hash<256> &value ) 	{ return bytes_to_hex_string( &value, 32 ); }
template <> std::string to_string<hash<512>>( const hash<512> &value ) 	{ return bytes_to_hex_string( &value, 64 ); }

// error return version
template <size_t _Size>
inline hash<_Size> hash_from_string(const string_span<char>& str, bool& success) noexcept
{
	constexpr const size_t byte_size = _Size / 8;
	static_assert( sizeof( hash<_Size> ) == byte_size, "Error: hash<> invalid size." );

	// must be exactly 2 hex values per byte 
	if( (str.end - str.start) != (byte_size*2) )
	{
		success = false;
		return hash<_Size>();
	}	

	hash<_Size> value;
	hex_string_to_bytes( &value, str.start, byte_size, success );
	return value;
}

// exception throwing version
template <size_t _Size>
inline hash<_Size> hash_from_string(const string_span<char>& str)
{
	bool success = true;
	auto value = ctle::from_string<hash<_Size>>( str, success );
	if( !success )
		throw std::runtime_error("Could not convert the string into a hash<> value, it is ill-formatted.");
	return value;
}

template <> hash<64> ctle::from_string<hash<64>>(const string_span<char>& str, bool& success) noexcept { return hash_from_string<64>(str, success); }
template <> hash<128> ctle::from_string<hash<128>>(const string_span<char>& str, bool& success) noexcept { return hash_from_string<128>(str, success); }
template <> hash<256> ctle::from_string<hash<256>>(const string_span<char>& str, bool& success) noexcept { return hash_from_string<256>(str, success); }
template <> hash<512> ctle::from_string<hash<512>>(const string_span<char>& str, bool& success) noexcept { return hash_from_string<512>(str, success); }

template <> hash<64> hex_string_to_value<hash<64>>( const char *hex_string, bool &success ) noexcept { return ctle::from_string<hash<64>>( string_span<char>(hex_string,hex_string+16), success ); }
template <> hash<128> hex_string_to_value<hash<128>>( const char *hex_string, bool &success ) noexcept { return ctle::from_string<hash<128>>( string_span<char>(hex_string,hex_string+32), success ); }
template <> hash<256> hex_string_to_value<hash<256>>( const char *hex_string, bool &success ) noexcept { return ctle::from_string<hash<256>>( string_span<char>(hex_string,hex_string+64), success ); }
template <> hash<512> hex_string_to_value<hash<512>>( const char *hex_string, bool &success ) noexcept { return ctle::from_string<hash<512>>( string_span<char>(hex_string,hex_string+128), success ); }

template <> hash<64> ctle::from_string<hash<64>>(const string_span<char>& str) noexcept { return hash_from_string<64>(str); }
template <> hash<128> ctle::from_string<hash<128>>(const string_span<char>& str) noexcept { return hash_from_string<128>(str); }
template <> hash<256> ctle::from_string<hash<256>>(const string_span<char>& str) noexcept { return hash_from_string<256>(str); }
template <> hash<512> ctle::from_string<hash<512>>(const string_span<char>& str) noexcept { return hash_from_string<512>(str); }

}
//namespace ctle

////////////////////////////////////////

namespace std
{

std::ostream& operator<<(std::ostream& os, const ctle::hash<64>& _hash) { os << ctle::to_string(_hash); return os; }
std::ostream &operator<<( std::ostream &os, const ctle::hash<128> &_hash ) { os << ctle::to_string( _hash ); return os; }
std::ostream& operator<<(std::ostream& os, const ctle::hash<256>& _hash) { os << ctle::to_string(_hash); return os; }
std::ostream &operator<<( std::ostream &os, const ctle::hash<512> &_hash ) { os << ctle::to_string( _hash ); return os; }

}

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_HASH_H_
