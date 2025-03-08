// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_UUID_H_
#define _CTLE_UUID_H_

/// @file uuid.h
/// @brief A portable, variant 1, version 4 (RNG generated) of uuid implementation.

#include <cstdint>
#include <functional>
#include <iosfwd>

namespace ctle
{
/// @brief uuid implements a portable, variant 1, version 4 (RNG generated) of uuid implementation.
/// @details The uuid is stored as big-endian 16 bytes, and can be compared, hashed, and converted to and from strings.
struct uuid
{
	union
	{
		uint8_t data[16];
		uint64_t _data_q[2] = {}; // a quadword memory overload, for faster comparisons
	};

	// the constant nil object
	static const uuid nil;

	// constexpr values for inferior/min (zero & inf) value or superior/max values
	static constexpr const uuid zero();
	static constexpr const uuid inf();
	static constexpr const uuid sup();

	// compare operators
	bool operator==( const ctle::uuid &other ) const noexcept;
	bool operator!=( const ctle::uuid &other ) const noexcept;
	bool operator<( const ctle::uuid &other ) const noexcept;

	/// @brief Generates an uuid using the mt19937 seeded from random_device.
	/// @details The generate call is thread-safe and uses a mutex to avoid collisions
	static uuid generate();
};

constexpr const uuid uuid::zero() { uuid ret; for( size_t inx=0; inx<2; ++inx) { ret._data_q[inx] = 0; } return ret; } 
constexpr const uuid uuid::inf() { return zero(); }
constexpr const uuid uuid::sup() { uuid ret; for( size_t inx=0; inx<2; ++inx) {	ret._data_q[inx] = UINT64_MAX; } return ret; } 

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

namespace std
{

template <>
struct hash<ctle::uuid>
{
	std::size_t operator()( ctle::uuid const &val ) const noexcept
	{
		static_assert( sizeof( std::size_t ) == sizeof( std::uint64_t ), "The hashing code only works for 64bit size_t" );
		static_assert( sizeof( ctle::uuid ) == 16, "The uuid must be 16 bytes in size" );
		return val._data_q[0] ^ val._data_q[1];
	}
};

std::ostream &operator<<( std::ostream &os, const ctle::uuid &_uuid );

}
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
const uuid uuid::nil = uuid::zero();

template <> std::string to_string( const uuid &value )
{
	std::string ret;

	// format: nnnnnnnn-nnnn-nnnn-nnnn-nnnnnnnnnnnn , 36 characters long (32 hex + 4 dash)
	ret += _bytes_to_hex_string( &value.data[0], 4 );
	ret += "-";
	ret += _bytes_to_hex_string( &value.data[4], 2 );
	ret += "-";
	ret += _bytes_to_hex_string( &value.data[6], 2 );
	ret += "-";
	ret += _bytes_to_hex_string( &value.data[8], 2 );
	ret += "-";
	ret += _bytes_to_hex_string( &value.data[10], 6 );

	return ret;
}

template <> std::string to_hex_string( const uuid &value )
{
	return to_string( value );
}

template <> uuid ctle::from_string<uuid>( const string_span<char> &str, bool &success ) noexcept
{
	if( (str.end - str.start) != 36
		|| str.start[8] != '-'
		|| str.start[13] != '-'
		|| str.start[18] != '-'
		|| str.start[23] != '-' )
	{
		success = false; // ill-formatted
		return uuid::nil;
	}

	uuid value;

	_bytes_from_hex_string( &value.data[0], 4, &str.start[0], success );
	_bytes_from_hex_string( &value.data[4], 2, &str.start[9], success );
	_bytes_from_hex_string( &value.data[6], 2, &str.start[14], success );
	_bytes_from_hex_string( &value.data[8], 2, &str.start[19], success );
	_bytes_from_hex_string( &value.data[10], 6, &str.start[24], success );

	return value;
}

template <> uuid ctle::from_string<uuid>( const string_span<char> &str )
{
	bool success = true;
	auto value = ctle::from_string<uuid>( str, success );
	if( !success )
		throw std::runtime_error("Could not convert the string into a uuid, it is ill-formatted.");
	return value;
}

template <> uuid ctle::from_hex_string<uuid>( const string_span<char>& str, bool &success ) noexcept
{
	return ctle::from_string<uuid>(str, success);
}

template <> uuid ctle::from_hex_string<uuid>( const string_span<char> &str )
{
	return ctle::from_string<uuid>(str);
}

uuid uuid::generate()
{
	static std::mutex generate_mutex;
	static std::mt19937 generator;
	static bool generator_initialized = false;

	// lock access to the generator
	const std::lock_guard<std::mutex> lock(generate_mutex);

	// If needed, set up the generator.
	// we use a generator which is setup once, and shared between threads. 
	// The assumption since the random_device setup might be costly and that thread collisions are not common.
	if( !generator_initialized )
	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate( std::begin( seed_data ), std::end( seed_data ), std::ref( rd ) );
		std::seed_seq seq( std::begin( seed_data ), std::end( seed_data ) );
		generator = std::mt19937(seq);
		generator_initialized = true;
	}

	// pack four 32-bit random numbers into the uuid (generator() outputs 32-bit values)
	uuid value;
	uint64_t v0 = generator();
	uint64_t v1 = generator();
	uint64_t v2 = generator();
	uint64_t v3 = generator();
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

namespace std
{

std::ostream &operator<<( std::ostream &os, const ctle::uuid &_uuid )
{
	os << ctle::to_string( _uuid );
	return os;
}

}
//namespace std

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_UUID_H_
