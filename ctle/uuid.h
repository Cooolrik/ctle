// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

#include <cstdint>
#include <functional>
#include <iosfwd>

namespace ctle
{
// uuid implements a portable, variant 1, version 4 (RNG generated) of uuid
// stored big-endian on all platforms
struct uuid
{
	union
	{
		uint64_t _data_q[2] = {};
		uint8_t data[16];
	};

	// the constant nil object
	static const uuid nil;

	// compare operators
	bool operator==( const ctle::uuid &other ) const noexcept;
	bool operator!=( const ctle::uuid &other ) const noexcept;
	bool operator<( const ctle::uuid &other ) const noexcept;

	// generates an uuid using the mt19937 seeded from random_device
	// the call is thread-safe and uses a mutex to avoid collisions
	static uuid generate();
};

inline bool uuid::operator<( const ctle::uuid &right ) const noexcept
{
	const uint8_t *u1 = this->data;
	const uint8_t *u2 = right.data;

	// uuid is stored big-endian, so MSB is first byte, LSB is last byte
	size_t n = 16;
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

inline bool uuid::operator==( const ctle::uuid &right ) const noexcept
{
	return ( this->_data_q[0] == right._data_q[0] )
		&& ( this->_data_q[1] == right._data_q[1] );
};

inline bool uuid::operator!=( const ctle::uuid &right ) const noexcept
{
	return ( this->_data_q[0] != right._data_q[0] )
		|| ( this->_data_q[1] != right._data_q[1] );
};

}
//namespace ctle

template <>
struct std::hash<ctle::uuid>
{
	std::size_t operator()( ctle::uuid const &val ) const noexcept
	{
		static_assert( sizeof( std::size_t ) == sizeof( std::uint64_t ), "The hashing code only works for 64bit size_t" );
		static_assert( sizeof( ctle::uuid ) == 16, "The uuid must be 16 bytes in size" );
		return val._data_q[0] ^ val._data_q[1];
	}
};

std::ostream &operator<<( std::ostream &os, const ctle::uuid &_uuid );

//namespace std

#ifdef CTLE_IMPLEMENTATION

#include <random>
#include <algorithm>
#include <memory>
#include <array>
#include <mutex>

#include "string_funcs.h"

namespace ctle
{
const uuid uuid::nil;

template <> std::string value_to_hex_string( const uuid &value )
{
	std::string ret;

	// format: nnnnnnnn-nnnn-nnnn-nnnn-nnnnnnnnnnnn , 36 characters long (32 hex + 4 dash)
	ret += bytes_to_hex_string( &value.data[0], 4 );
	ret += "-";
	ret += bytes_to_hex_string( &value.data[4], 2 );
	ret += "-";
	ret += bytes_to_hex_string( &value.data[6], 2 );
	ret += "-";
	ret += bytes_to_hex_string( &value.data[8], 2 );
	ret += "-";
	ret += bytes_to_hex_string( &value.data[10], 6 );

	return ret;
}

template <> uuid hex_string_to_value<uuid>( const char *hex_string )
{
	if( hex_string[8] != '-'
		|| hex_string[13] != '-'
		|| hex_string[18] != '-'
		|| hex_string[23] != '-' )
	{
		throw std::runtime_error( "hex_string_to_value: ill-formated hex_string" );
	}

	uuid value;

	hex_string_to_bytes( &value.data[0], &hex_string[0], 4 );
	hex_string_to_bytes( &value.data[4], &hex_string[9], 2 );
	hex_string_to_bytes( &value.data[6], &hex_string[14], 2 );
	hex_string_to_bytes( &value.data[8], &hex_string[19], 2 );
	hex_string_to_bytes( &value.data[10], &hex_string[24], 6 );

	return value;
}

uuid uuid::generate()
{
	static std::mutex generate_mutex;
	static std::unique_ptr<std::mt19937> generator;

	// make it thread safe
	const std::lock_guard<std::mutex> lock( generate_mutex );

	// in first call, set up generator
	if( !generator )
	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate( std::begin( seed_data ), std::end( seed_data ), std::ref( rd ) );
		std::seed_seq seq( std::begin( seed_data ), std::end( seed_data ) );
		generator = std::unique_ptr<std::mt19937>( new std::mt19937( seq ) );
	}

	// generate random values
	uint64_t v0 = ( *generator.get() )( );
	uint64_t v1 = ( *generator.get() )( );
	uint64_t v2 = ( *generator.get() )( );
	uint64_t v3 = ( *generator.get() )( );

	uuid value;
	value._data_q[0] = ( v0 << 32 | v1 );
	value._data_q[1] = ( v2 << 32 | v3 );

	// set the version and variant values
	// version byte is: 0100xxxx , (version 4)
	value.data[6] &= 0x4F;
	value.data[6] |= 0x40;

	// variant byte is: 10xxxxxx , (variant 1)
	value.data[8] &= 0xBF;
	value.data[8] |= 0x80;

	return value;
}

}
//namespace ctle

std::ostream &operator<<( std::ostream &os, const ctle::uuid &_uuid )
{
	os << ctle::value_to_hex_string( _uuid );
	return os;
}

#endif