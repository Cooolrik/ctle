// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_MULTITHREAD_POOL_H_
#define _CTLE_MULTITHREAD_POOL_H_

/// @file multithread_pool.h
/// @brief A pool of objects which can be shared by tasks in multiple threads.

#include <vector>
#include <memory>
#include <unordered_set>
#include "readers_writer_lock.h"

namespace ctle
{

/// @brief A multithread pool of objects, which can be shared by tasks in multiple threads.
/// @details The multithread_pool template is used to create a pool of objects which
/// can be shared by multiple threads. The objects are assumed to be expensive to allocate or
/// have allocated, so it makes more sense to share them among multiple threads/tasks, instead
/// of having one pool per thread. On init, a vector of preallocated objects are inserted to the pool
/// and when the pool is deinitialized, the list of objects is returned, so that the caller 
/// can deallocate the objects in a correct fashion. If the objects automatically clean up, 
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

	/// @brief Initialize the pool, and hand over a list of preallocated objects.
	/// @details Call during initial setup before workers start, or to reinitialize while worker access is paused.
	/// Before reinitializing, wait for all use and return of previous loans to finish.
	/// Prevent new borrow/return calls until this method returns, and serialize initialize/deinitialize calls.
	/// @note Do not call while workers are still using borrowed objects; replacing the pool destroys those objects.
	/// The internal lock protects bookkeeping, not the lifetime of pointers already handed to workers.
	/// @param objectList Preallocated objects whose ownership is transferred; do not modify this vector concurrently.
	/// @note Allocation failure leaves the caller's objects and the existing pool unchanged.
	void initialize( std::vector<std::unique_ptr<_Ty>> &objectList )
	{
		// Build the index first so allocation failure leaves the caller and current pool unchanged.
		std::vector<_Ty *> new_available( objectList.size() );
		for( size_t inx = 0; inx < objectList.size(); ++inx )
		{
			new_available[inx] = objectList[inx].get();
		}

		// Keep old objects outside the lock scope: their destructors may call back into the pool.
		std::vector<std::unique_ptr<_Ty>> previous_pool;
		{
			readers_writer_lock::write_guard guard( this->accessLock );

			// Transfer ownership without allocating or destroying pooled objects.
			this->pool.swap( previous_pool ); // Move old objects aside; pool is now empty.
			this->pool.swap( objectList ); // Take the caller's objects; objectList is now empty.

			// Publish the matching index and clear old loan tracking under the same lock.
			this->available.swap( new_available );
			this->borrowed.clear();
		}
	}

	/// @brief Clears the pool, and returns all objects back to the caller.
	/// @details Call during shutdown, normally after stopping new borrows and waiting for existing loans to return.
	/// Serialize initialize/deinitialize calls. This method does not wait for borrowers or revoke their pointers.
	/// @note All objects are transferred and both indexes cleared, including when loans are outstanding.
	/// Outstanding loans are allowed only if the caller keeps the transferred objects alive until their users finish.
	/// Late return_item() calls are rejected once loans are detached. Keep this pool alive until those calls finish,
	/// and do not reinitialize it while old workers may still access it.
	/// @param objectList Receives all owned objects, replacing its previous contents; do not modify it concurrently.
	/// Previous contents are destroyed after releasing the lock, so their destructors may query the pool.
	/// @return false if loans were outstanding when ownership was detached; otherwise true.
	/// A false result does not cancel the ownership transfer.
	bool deinitialize(std::vector<std::unique_ptr<_Ty>>& objectList)
	{
		std::vector<std::unique_ptr<_Ty>> detached_pool;
		bool all_returned = false;
		{
			// Hold the lock across bookkeeping cleanup and detachment so no partial state is visible.
			readers_writer_lock::write_guard guard( this->accessLock );

			// Remember outstanding loans before clearing them; they determine the return value.
			all_returned = this->borrowed.empty();

			// Stop lending detached objects and prevent late returns from making them available again.
			this->available.clear();
			this->borrowed.clear();

			// Detach every object, even if loans remain. The caller must keep those objects alive.
			this->pool.swap( detached_pool );
		}

		// Replacing the caller's old objects may run destructors that call back into the pool.
		objectList = std::move( detached_pool );
		return all_returned;
	}

	/// @brief returns true if there is an item available in the pool
	bool item_available()
	{
		readers_writer_lock::read_guard guard( this->accessLock );
		return !available.empty();
	}

	/// @brief returns the number of items available in the pool
	size_t available_count()
	{
		readers_writer_lock::read_guard guard( this->accessLock );
		return available.size();
	}

	/// @brief returns true if any item is borrowed from the pool
	bool item_borrowed()
	{
		readers_writer_lock::read_guard guard( this->accessLock );
		return !borrowed.empty();
	}

	/// @brief borrow an item from the pool
	/// @return a pointer to the item, or nullptr if no item is available
	_Ty* borrow_item()
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

	/// @brief return an item to the pool
	/// @param item the item to return, must be a valid item from the pool
	/// @return true if the item was returned, false if the item was not borrowed from the pool, and cannot be returned
	bool return_item(_Ty* item)
	{
		readers_writer_lock::write_guard guard( this->accessLock );

		// check that we have the item in the pool
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

#endif//_CTLE_MULTITHREAD_POOL_H_
