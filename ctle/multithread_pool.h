// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <vector>
#include <unordered_set>
#include "readers_writer_lock.h"

namespace ctle
{

// The multithread_pool template is used to create a multithread pool, a pool of objects which
// can be shared by multiple threads. The objects are assumed to be expensive to allocate or
// have allocated, so it makes more sense to share them among multiple threads/jobs, instead
// of having one pool per thread. On init, a vector of preallocated objects are inserted to the pool
// and on deinit, the list of objects are returned, so that the caller can deallocate the objects.
template<class _Ty>
class multithread_pool
{
private:
	// the pool of objects
	std::vector<std::unique_ptr<_Ty>> pool;

	// the access mutex
	readers_writer_lock accessLock;

	// the available objects
	std::vector<_Ty *> available;

	// the used objects
	std::unordered_set<_Ty *> borrowed;

public:
	using value_type = _Ty;

	// Initialize the pool, and hand over a list of preallocated objects.
	// Note! Not thread safe, this method is assumed to only be called on setup, so is not guarded
	void initialize( std::vector<std::unique_ptr<_Ty>> &objectList )
	{
		this->pool = std::move( objectList );
		this->available.resize( this->pool.size() );
		for( size_t inx = 0; inx < this->pool.size(); ++inx )
		{
			this->available[inx] = this->pool[inx].get();
		}
	}

	// clears the pool, and returns all objects back to the caller
	// Note! Any non-returned items will also be moved back to the caller
	void deinitialize( std::vector<std::unique_ptr<_Ty>> &objectList )
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// move all pool objects to the return object list
		objectList = std::move( this->pool );
	}

	// returns true if there is an item available in the pool
	bool item_available()
	{
		readers_writer_lock::read_guard guard( this->accessLock );

		return !available.empty();
	}

	// borrow an item from the pool
	_Ty *borrow_item()
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// item available?
		if( available.empty() )
			return nullptr;

		// move the last available item into the locked set
		_Ty *ret = this->available.back();
		this->available.pop_back();
		this->borrowed.emplace( ret );

		return ret;
	}

	// return an item to the pool
	bool return_item( _Ty *item )
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// check that we have the item
		auto it = this->borrowed.find( item );
		if( it == this->borrowed.end() )
			return false;

		// found, return to available list
		this->available.emplace_back( *it );
		this->borrowed.erase( it );

		return true;
	}

};

}
//namespace ctle
