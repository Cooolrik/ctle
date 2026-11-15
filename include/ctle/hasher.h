// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_HASHER_H_
#define _CTLE_HASHER_H_

/// @file hasher.h
/// @brief Hasher function wrappers for hashing data streams.
/// 
/// The hasher_[...] classes are wrappers around popular hash functions, and provide a simple interface for hashing data streams.
/// 
/// Each hasher class implements the following functions:
/// - ctor() to initialize the hasher
/// - update() to update the hash with a block of bytes
/// - finish(), to end the hashed stream, and return the final hash
/// @note The hashers are implemented using external libraries. All declarations exist, but to implement a specific library, include 
/// the library header before including hasher.h in the implementation source file. (see the example implementation in the 
/// documentation for ctle.h for more information).
/// @note Resource-owning hashers are move-only. After a move, update() and finish() on the source return
/// status::not_initialized until a new hasher is move-assigned to it. hasher_noop remains copyable.

#include <memory>

#include "digest.h"
#include "status.h"
#include "status_return.h"

namespace ctle
{
namespace detail
{
	struct sha256_state;
	struct xxh3_state;
}

/// @brief Implementation of a no-operation hasher class template, for use when hashing is not needed. Always returns a zero hash on finish.
/// @tparam _Size the size of the hash in bits, must be 64, 128, 256 or 512.
/// @note This hasher is used when hashing is not needed, but the interface requires a hasher to be passed, and can be used as a template default parameter.
template <size_t _Size>
class hasher_noop
{
	status _update(const uint8_t* /*data*/, size_t /*size*/) { return status::ok; }

public:
	hasher_noop() {};
	~hasher_noop() {};
	using hash_type = digest<_Size>;

	/// @brief Update the hash with a block of bytes.
	/// @param data the data to add to the hash
	/// @param size the size of the data in bytes
	/// @return status::ok if the update was successful	
	status update(const uint8_t* data, size_t size) { return _update(data,size); }

	/// @brief Finish the hash generation and return the final hash value.
	/// @return status::ok if the update was successful, and the final hash value	
	status_return<status, digest<_Size>> finish() { return digest<_Size>(); }
};

/// @brief Implementation of a SHA-256 hasher, using picosha2.
/// @note To use, include picosha2 in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_sha256
{
public:
	hasher_sha256();
	~hasher_sha256();
	hasher_sha256( const hasher_sha256 & ) = delete;
	hasher_sha256 &operator=( const hasher_sha256 & ) = delete;
	/// @brief Transfer hash state, leaving the source uninitialized.
	hasher_sha256( hasher_sha256 &&other ) noexcept;
	/// @brief Replace hash state by transferring ownership from the source.
	hasher_sha256 &operator=( hasher_sha256 &&other ) noexcept;
	using hash_type = digest<256>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<256>> finish();

private:
	std::unique_ptr<detail::sha256_state> context;
};

/// @brief Implementation of XXH3 XXH64 hasher, using xxHash.
/// @note To use, include xxHash in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_xxh64
{
public:
	hasher_xxh64();
	~hasher_xxh64();
	hasher_xxh64( const hasher_xxh64 & ) = delete;
	hasher_xxh64 &operator=( const hasher_xxh64 & ) = delete;
	/// @brief Transfer hash state, leaving the source uninitialized.
	hasher_xxh64( hasher_xxh64 &&other ) noexcept;
	/// @brief Replace hash state by transferring ownership from the source.
	hasher_xxh64 &operator=( hasher_xxh64 &&other ) noexcept;
	using hash_type = digest<64>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<64>> finish();

private:
	std::unique_ptr<detail::xxh3_state> context;
};

/// @brief Implementation of XXH3 XXH128 hasher, using xxHash.
/// @note To use, include xxHash in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_xxh128
{
public:
	hasher_xxh128();
	~hasher_xxh128();
	hasher_xxh128( const hasher_xxh128 & ) = delete;
	hasher_xxh128 &operator=( const hasher_xxh128 & ) = delete;
	/// @brief Transfer hash state, leaving the source uninitialized.
	hasher_xxh128( hasher_xxh128 &&other ) noexcept;
	/// @brief Replace hash state by transferring ownership from the source.
	hasher_xxh128 &operator=( hasher_xxh128 &&other ) noexcept;
	using hash_type = digest<128>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<128>> finish();

private:
	std::unique_ptr<detail::xxh3_state> context;
};

/// @brief Implementation of XXH3 XXH128 hasher, using xxHash, concatenated into one 256 bit hash. Note that the second part of the hash is generated from an added salt of dcb7be9cd0fcf505, (big endian).
/// @note To use, include xxHash in the build before hasher.h to implement (see the example implementation in the documentation for ctle.h).
class hasher_2x_xxh128_dcb7be9cd0fcf505 
{
public:
	hasher_2x_xxh128_dcb7be9cd0fcf505();
	~hasher_2x_xxh128_dcb7be9cd0fcf505();
	hasher_2x_xxh128_dcb7be9cd0fcf505( const hasher_2x_xxh128_dcb7be9cd0fcf505 & ) = delete;
	hasher_2x_xxh128_dcb7be9cd0fcf505 &operator=( const hasher_2x_xxh128_dcb7be9cd0fcf505 & ) = delete;
	/// @brief Transfer hash state, leaving the source uninitialized.
	hasher_2x_xxh128_dcb7be9cd0fcf505( hasher_2x_xxh128_dcb7be9cd0fcf505 &&other ) noexcept;
	/// @brief Replace hash state by transferring ownership from the source.
	hasher_2x_xxh128_dcb7be9cd0fcf505 &operator=( hasher_2x_xxh128_dcb7be9cd0fcf505 &&other ) noexcept;
	using hash_type = digest<256>;

	/// @copydoc hasher_noop::update
	status update(const uint8_t* data, size_t size);

	/// @copydoc hasher_noop::finish
	status_return<status, digest<256>> finish();

private:
	std::unique_ptr<detail::xxh3_state> context;
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CTLE_IMPLEMENTATION

#include <cstring>
#include <new>

namespace ctle
{

////////////////////////////////////////

#ifdef PICOSHA2_H

struct detail::sha256_state
{
	picosha2::hash256_one_by_one value;
};

hasher_sha256::hasher_sha256()
	: context( std::make_unique<detail::sha256_state>() )
{
}

hasher_sha256::~hasher_sha256() = default;
hasher_sha256::hasher_sha256( hasher_sha256 &&other ) noexcept = default;
hasher_sha256 &hasher_sha256::operator=( hasher_sha256 &&other ) noexcept = default;

status hasher_sha256::update(const uint8_t* data, size_t size)
{
	if( !this->context )
		return status::not_initialized;
	this->context->value.process( data, data + size );
	return status::ok;
}

status_return<status,digest<256>> hasher_sha256::finish()
{
	if( !this->context )
		return status::not_initialized;
	digest<256> ret;
	this->context->value.finish();
	this->context->value.get_hash_bytes( ret.data, ret.data+32 );
	return ret;
}

#endif//PICOSHA2_H

////////////////////////////////////////

#ifdef XXHASH_H_5627135585666179

struct detail::xxh3_state
{
	struct deleter
	{
		void operator()( XXH3_state_t *value ) const noexcept
		{
			XXH3_freeState( value );
		}
	};

	std::unique_ptr<XXH3_state_t, deleter> value{ XXH3_createState() };

	xxh3_state()
	{
		if( !this->value )
			throw std::bad_alloc();
	}
};

hasher_xxh64::hasher_xxh64()
	: context( std::make_unique<detail::xxh3_state>() )
{
	XXH3_64bits_reset( this->context->value.get() );
}

hasher_xxh64::~hasher_xxh64() = default;
hasher_xxh64::hasher_xxh64( hasher_xxh64 &&other ) noexcept = default;
hasher_xxh64 &hasher_xxh64::operator=( hasher_xxh64 &&other ) noexcept = default;

status hasher_xxh64::update(const uint8_t* data, size_t size)
{
	if( !this->context )
		return status::not_initialized;
	XXH3_64bits_update( this->context->value.get(), data, size );
	return status::ok;
}

status_return<status,digest<64>> hasher_xxh64::finish()
{
	if( !this->context )
		return status::not_initialized;
	XXH64_hash_t result = XXH3_64bits_digest( this->context->value.get() );

	XXH64_canonical_t canonical;
	XXH64_canonicalFromHash( &canonical, result );

	digest<64> ret;
	memcpy( ret.data, canonical.digest, sizeof(ret.data) );
	return ret;
}

///////////////////

hasher_xxh128::hasher_xxh128()
	: context( std::make_unique<detail::xxh3_state>() )
{
	XXH3_128bits_reset( this->context->value.get() );
}

hasher_xxh128::~hasher_xxh128() = default;
hasher_xxh128::hasher_xxh128( hasher_xxh128 &&other ) noexcept = default;
hasher_xxh128 &hasher_xxh128::operator=( hasher_xxh128 &&other ) noexcept = default;

status hasher_xxh128::update(const uint8_t* data, size_t size)
{
	if( !this->context )
		return status::not_initialized;
	XXH3_128bits_update( this->context->value.get(), data, size );
	return status::ok;
}

status_return<status,digest<128>> hasher_xxh128::finish()
{
	if( !this->context )
		return status::not_initialized;
	XXH128_hash_t result = XXH3_128bits_digest( this->context->value.get() );

	XXH128_canonical_t canonical;
	XXH128_canonicalFromHash( &canonical, result );

	digest<128> ret;
	memcpy( ret.data, canonical.digest, sizeof(ret.data) );
	return ret;
}


///////////////////

hasher_2x_xxh128_dcb7be9cd0fcf505::hasher_2x_xxh128_dcb7be9cd0fcf505()
	: context( std::make_unique<detail::xxh3_state>() )
{
	XXH3_128bits_reset( this->context->value.get() );
}

hasher_2x_xxh128_dcb7be9cd0fcf505::~hasher_2x_xxh128_dcb7be9cd0fcf505() = default;
hasher_2x_xxh128_dcb7be9cd0fcf505::hasher_2x_xxh128_dcb7be9cd0fcf505(
	hasher_2x_xxh128_dcb7be9cd0fcf505 &&other ) noexcept = default;
hasher_2x_xxh128_dcb7be9cd0fcf505 &hasher_2x_xxh128_dcb7be9cd0fcf505::operator=(
	hasher_2x_xxh128_dcb7be9cd0fcf505 &&other ) noexcept = default;

status hasher_2x_xxh128_dcb7be9cd0fcf505::update(const uint8_t* data, size_t size)
{
	if( !this->context )
		return status::not_initialized;
	XXH3_128bits_update( this->context->value.get(), data, size );
	return status::ok;
}

status_return<status,digest<256>> hasher_2x_xxh128_dcb7be9cd0fcf505::finish()
{
	if( !this->context )
		return status::not_initialized;
	digest<256> ret;

	// get the first hash result, and copy to return
	XXH128_hash_t result = XXH3_128bits_digest( this->context->value.get() );
	XXH128_canonical_t canonical;
	XXH128_canonicalFromHash( &canonical, result );
	memcpy( &ret.data[0], canonical.digest, sizeof(canonical.digest) );

	// append the randomly generated salt 'dcb7be9cd0fcf505' to the state
	const static u8 salt[8] = {0xdc,0xb7,0xbe,0x9c,0xd0,0xfc,0xf5,0x05};
	XXH3_128bits_update( this->context->value.get(), salt, sizeof(salt) );

	// get the second hash result, and copy to second part of return
	result = XXH3_128bits_digest( this->context->value.get() );
	XXH128_canonicalFromHash( &canonical, result );
	memcpy( &ret.data[16], canonical.digest, sizeof(canonical.digest) );

	return ret;
}

#endif//XXHASH_H_5627135585666179

////////////////////////////////////////

}

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_HASHER_H_
