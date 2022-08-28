// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <mutex>

namespace ctle
	{
	// readers_writer_lock.
	// allows concurrent access for read-only operations. write operations is given exclusive access.
	// use read_lock/read_unlock for read operations, and write_lock/write_unlock for write operations
    // (the code uses a spin lock for efficiency, so assumes the operations are fast)
	class readers_writer_lock
		{
		private:
            std::atomic<unsigned int> numReaders = 0;
            std::atomic<unsigned int> numWriters = 0;
            std::mutex writeMutex;

		public:
            // lock before reading 
            inline void read_lock()
                {
                // increase number of readers
                ++this->numReaders;

                // if there is a writer, we have to wait for it
                if( this->numWriters != 0 )
                    {
                    // remove us again, until the writer is done
                    --this->numReaders;

                    // wait for writer to be done, by locking the write mutex
                    this->writeMutex.lock();

                    // we have the lock, add to readers again
                    ++this->numReaders;

                    // done with the lock
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
                // lock the write thread
                this->writeMutex.lock();

                // increase the number of writers
                // this will block any new readers for reading
                ++this->numWriters;

                // let any reader finish before writing
                while( this->numReaders != 0 )
                    {
                    std::this_thread::yield();
                    }
                }
            
            // unlock after writing 
            inline void write_unlock()
                {
                // lower the number of writers again
                --this->numWriters;

                // unlock the write lock, so any waiting reader or writer gets access again
                _Requires_lock_held_(this->writeMutex)
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
	};
