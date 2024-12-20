// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_HASHER_H_
#define _CTLE_HASHER_H_

#include "hash.h"
#include "status.h"
#include "status_return.h"

namespace ctle
{
	
// hasher function wrappers. all hashers auto-initializes in the ctor, and implements:
// - update() to update the hash with a block of bytes
// - finish(), to end the hashed stream, and return the final hash

// implementation of SHA-256 hasher, using picosha2 (include picosha2 in the build before ctle to implement)
class hasher_sha256
{
public:
	hasher_sha256();
	~hasher_sha256();
	using hash_type = hash<256>;

	status update(const uint8_t* data, size_t size);
	status_return<status,hash<256>> finish();

private:
	void *context = nullptr;
};

// implementation of XXH3 XXH64 hasher, using xxHash (include xxHash in the build before ctle to implement)
class hasher_xxh64
{
public:
	hasher_xxh64();
	~hasher_xxh64();
	using hash_type = hash<64>;

	status update(const uint8_t* data, size_t size);
	status_return<status,hash<64>> finish();

private:
	void *context = nullptr;
};

// implementation of XXH3 XXH128 hasher, using xxHash (include xxHash in the build before ctle to implement)
class hasher_xxh128
{
public:
	hasher_xxh128();
	~hasher_xxh128();
	using hash_type = hash<128>;

	status update(const uint8_t* data, size_t size);
	status_return<status,hash<128>> finish();

private:
	void *context = nullptr;
};

// implementation of no-operation hasher class template, for use when hashing is not needed. always returns a zero hash 
template <size_t _Size> 
class hasher_noop
{
public:
	hasher_noop() {};
	~hasher_noop() {};
	using hash_type = hash<_Size>;

	status update(const uint8_t* /*data*/, size_t /*size*/) { return status::ok; }
	status_return<status,hash<_Size>> finish() { return hash<_Size>(); }
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CTLE_IMPLEMENTATION

namespace ctle
{

////////////////////////////////////////

#ifdef PICOSHA2_H

hasher_sha256::hasher_sha256()
{
	this->context = (void*) new picosha2::hash256_one_by_one();
}

hasher_sha256::~hasher_sha256()
{
	delete ((picosha2::hash256_one_by_one*)this->context);
}

status hasher_sha256::update(const uint8_t* data, size_t size)
{
	((picosha2::hash256_one_by_one*)this->context)->process( data, data + size );
	return status::ok;
}

status_return<status,hash<256>> hasher_sha256::finish()
{
	hash<256> ret;
	((picosha2::hash256_one_by_one*)this->context)->finish();
	((picosha2::hash256_one_by_one*)this->context)->get_hash_bytes( ret.data, ret.data+32 );
	return ret;
}

#endif//PICOSHA2_H

////////////////////////////////////////

#ifdef XXHASH_H_5627135585666179

hasher_xxh64::hasher_xxh64()
{
	this->context = (void*)XXH3_createState();
	XXH3_64bits_reset((XXH3_state_t*)this->context);
}

hasher_xxh64::~hasher_xxh64()
{
	XXH3_freeState((XXH3_state_t*)this->context);
}

status hasher_xxh64::update(const uint8_t* data, size_t size)
{
	XXH3_64bits_update((XXH3_state_t*)this->context, data, size);
	return status::ok;
}

status_return<status,hash<64>> hasher_xxh64::finish()
{
	XXH64_hash_t result = XXH3_64bits_digest((XXH3_state_t*)this->context);

	XXH64_canonical_t canonical;
	XXH64_canonicalFromHash( &canonical, result );

	hash<64> ret;
	memcpy( ret.data, canonical.digest, sizeof(ret.data) );
	return ret;
}

///////////////////

hasher_xxh128::hasher_xxh128()
{
	this->context = (void*)XXH3_createState();
	XXH3_128bits_reset((XXH3_state_t*)this->context);
}

hasher_xxh128::~hasher_xxh128()
{
	XXH3_freeState((XXH3_state_t*)this->context);
}

status hasher_xxh128::update(const uint8_t* data, size_t size)
{
	XXH3_128bits_update((XXH3_state_t*)this->context, data, size);
	return status::ok;
}

status_return<status,hash<128>> hasher_xxh128::finish()
{
	XXH128_hash_t result = XXH3_128bits_digest((XXH3_state_t*)this->context);

	XXH128_canonical_t canonical;
	XXH128_canonicalFromHash( &canonical, result );

	hash<128> ret;
	memcpy( ret.data, canonical.digest, sizeof(ret.data) );
	return ret;
}

#endif//XXHASH_H_5627135585666179

////////////////////////////////////////

}

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_HASHER_H_
