// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_READERS_WRITER_LOCK_H_
#define _CTLE_READERS_WRITER_LOCK_H_

#include <mutex>
#include <atomic>
#include <thread>

namespace ctle
{


// readers_writer_lock.
// allows concurrent access for read-only operations. write operations is given exclusive access.
// use read_lock/read_unlock for read operations, and write_lock/write_unlock for write operations
// (the write code uses a spin lock (but calls yield()) for efficiency, so assumes the read operations are fast)
class readers_writer_lock
{
private:
	std::atomic<unsigned int> numReaders;
	std::atomic<unsigned int> numWriters;
	std::mutex writeMutex;

public:
	readers_writer_lock() 
		: numReaders( 0 )
		, numWriters( 0 ) 	
	{}

	// lock before reading 
	inline void read_lock()
	{
		// increase number of readers
		++this->numReaders;

		// if there is an active writer, we have to wait for it
		if( this->numWriters != 0 )
		{
			// remove us from active readers again, until the writer is done
			--this->numReaders;

			// wait for writer finish, by locking the write mutex
			this->writeMutex.lock();

			// the writer has relased the mutex, so we can go ahead and become active
			++this->numReaders;

			// we can now release the write mutex again
			this->writeMutex.unlock();
		}
	}

	// unlock after reading 
	inline void read_unlock()
	{
		// just remove from count
		--this->numReaders;
	}

	// lock before writing 
	inline void write_lock()
	{
		// lock the write mutex, so we have unique access to writing 
		this->writeMutex.lock();

		// increase the number of writers
		// this will block any new readers from reading (writers are already blocked by the mutex)
		++this->numWriters;

		// let any reader finish before writing
		while( this->numReaders != 0 )
		{
			std::this_thread::yield();
		}

		// done, we now have a unique write lock
	}

	// unlock after writing 
	inline void write_unlock()
	{
		// we are done, so remove from number of writers again
		--this->numWriters;

		// unlock the write lock, so anyone waiting (reader or writer) gets access again
#ifdef _MSC_VER
		_Requires_lock_held_( this->writeMutex ) // markup for VS SCA
#endif
			this->writeMutex.unlock();
	}

	// read_lock class locks for read while in scope
	class read_guard
	{
	private:
		readers_writer_lock &myLock;

	public:
		read_guard( readers_writer_lock &my_lock ) :
			myLock( my_lock )
		{
			this->myLock.read_lock();
		}

		~read_guard()
		{
			this->myLock.read_unlock();
		}
	};

	// read_lock class locks for write while in scope
	class write_guard
	{
	private:
		readers_writer_lock &myLock;

	public:
		write_guard( readers_writer_lock &my_lock ) :
			myLock( my_lock )
		{
			this->myLock.write_lock();
		}

		~write_guard()
		{
			this->myLock.write_unlock();
		}
	};

};


}
//namespace ctle

#endif//_CTLE_READERS_WRITER_LOCK_H_
