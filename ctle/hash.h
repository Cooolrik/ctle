// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

#include <cstdint>
#include <functional>
#include <iosfwd>

#include "status.h"

namespace ctle
{
	
// define hash for message digests, either 256 or 512 bits in size (32 or 64 bytes)
template<size_t _Size>
struct hash
{
	static_assert( _Size == 256 || _Size == 512 , "Hash size must be 256 or 512");
	static constexpr const size_t hash_size = _Size;

	union
	{
		uint64_t _data_q[_Size/64] = {};
		uint8_t data[_Size/8];
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

	// hash values are stored big-endian, so MSB is first byte (index 0), LSB is last byte (index 31 or 63)
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

	// hash values are stored big-endian, so MSB is first byte (index 0), LSB is last byte (index 31 or 63)
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

status calculate_sha256_hash( uint8_t destDigest[32], const uint8_t *srcData, size_t srcDataLength );
status calculate_sha256_hash( hash<256> &destHash, const uint8_t *srcData, size_t srcDataLength );

}
//namespace ctle

namespace std
{

template <>
struct hash<ctle::hash<256>>
{
	size_t operator()( const ctle::hash<256> &val ) const noexcept
	{
		return ctle::calculate_size_hash<256>( val );
	}
};

template <>
struct hash<ctle::hash<512>>
{
	size_t operator()( const ctle::hash<512> &val ) const noexcept
	{
		return ctle::calculate_size_hash<512>( val );
	}
};

std::ostream &operator<<( std::ostream &os, const ctle::hash<256> &_hash );
std::ostream &operator<<( std::ostream &os, const ctle::hash<512> &_hash );

}

#ifdef CTLE_IMPLEMENTATION

#include <random>
#include <algorithm>
#include <memory>
#include <array>

#include "string_funcs.h"

namespace ctle
{

template <> std::string to_string<hash<256>>( const hash<256> &value )
{
	static_assert( sizeof( value ) == 32, "Error: hash<256> is assumed to be of size 32." );
	return bytes_to_hex_string( &value, 32 );
}

template <> std::string to_string<hash<512>>( const hash<512> &value )
{
	static_assert( sizeof( value ) == 64, "Error: hash<512> is assumed to be of size 64." );
	return bytes_to_hex_string( &value, 64 );
}

template <> hash<256> ctle::from_string<hash<256>>( const string_span<char> &str, bool &success ) noexcept
{
	if( (str.end - str.start) != 64 )
	{
		success = false;
		return {};
	}

	hash<256> value;
	static_assert( sizeof( value ) == 32, "Error: hash<256> is assumed to be of size 32." );
	hex_string_to_bytes( &value, str.start, 32, success );
	return value;
}

template <> hash<256> ctle::from_string<hash<256>>( const string_span<char> &str )
{
	bool success = true;
	auto value = ctle::from_string<hash<256>>( str, success );
	if( !success )
		throw std::runtime_error("Could not convert the string into a hash<256> value, it is ill-formatted.");
	return value;
}

template <> hash<256> hex_string_to_value<hash<256>>( const char *hex_string, bool &success ) noexcept
{
	return ctle::from_string<hash<256>>( string_span<char>(hex_string,hex_string+64), success );
}

template <> hash<512> ctle::from_string<hash<512>>( const string_span<char> &str, bool &success ) noexcept
{
	if( (str.end - str.start) != 128 )
	{
		success = false;
		return {};
	}

	hash<512> value;
	static_assert( sizeof( value ) == 64, "Error: hash<512> is assumed to be of size 64." );
	hex_string_to_bytes( &value, str.start, 64, success );
	return value;
}

template <> hash<512> hex_string_to_value<hash<512>>( const char *hex_string, bool &success ) noexcept
{
	return ctle::from_string<hash<512>>( string_span<char>(hex_string,hex_string+128), success );
}

template <> hash<512> ctle::from_string<hash<512>>( const string_span<char> &str )
{
	bool success = true;
	auto value = ctle::from_string<hash<512>>( str, success );
	if( !success )
		throw std::runtime_error("Could not convert the string into a hash<512> value, it is ill-formatted.");
	return value;
}

// if picosha-2 is included, implement the hash generation function for hash<256>
#ifdef PICOSHA2_H

status calculate_sha256_hash( uint8_t destDigest[32], const uint8_t *srcData, size_t srcDataLength )
{
	picosha2::hash256_one_by_one hasher;

	hasher.process( srcData, srcData + srcDataLength );
	hasher.finish();
	hasher.get_hash_bytes( destDigest, destDigest + 32 );

	return status::ok;
}

status calculate_sha256_hash( hash<256> &destHash, const uint8_t *srcData, size_t srcDataLength )
{
	return calculate_sha256_hash( destHash.data, srcData, srcDataLength );
}
#endif

}
//namespace ctle

namespace std
{

std::ostream &operator<<( std::ostream &os, const ctle::hash<256> &_hash )
{
	os << ctle::to_string( _hash );
	return os;
}

std::ostream &operator<<( std::ostream &os, const ctle::hash<512> &_hash )
{
	os << ctle::to_string( _hash );
	return os;
}

}

#endif
