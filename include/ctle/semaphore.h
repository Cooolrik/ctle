// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_SEMAPHORE_H_
#define _CTLE_SEMAPHORE_H_

#include "status.h"

/// @file semaphore.h
/// @brief 

namespace ctle
{

class semaphore 
{
public:
	/// @brief Initialize the semaphore with an initial count. Default initial count is zero, which 
	/// means that the semaphore will block acquire() until release() is called.
	/// @param initial_count The initial count of the semaphore. Set to non-zero to initialize the semaphore as non-blocking.
	/// @throws std::runtime_error if the semaphore could not be created.
	explicit semaphore( unsigned int initial_count = 0 );

	/// @brief Destroy the semaphore.
	/// @note Any threads waiting on the semaphore will be unblocked and may return an error.
	~semaphore();

	/// @brief Acquire the semaphore. (decrement the count). If the count is zero, block until another thread releases the semaphore.
	status acquire();

	/// @brief Release the semaphore. (increment the count). 
	status release();

private:
	void *handle = nullptr; // Opaque pointer to the platform-specific semaphore handle
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#if defined(_WIN32)
#define _ADD_CTLE_HEADERS_WIN_STD
#include "os.inl"
#else
#define _ADD_CTLE_HEADERS_LINUX_STD
#include "os.inl"
#endif

namespace ctle
{

semaphore::semaphore( unsigned int initial_count )
{
#if defined(_WIN32)
	// create a semaphore object, stored in handle
	this->handle = CreateSemaphore(
		nullptr,                // default security attributes
		initial_count,          // initial count param
		LONG_MAX,               // maximum count
		nullptr                 // unnamed semaphore
	);
	if( !this->handle )
		throw std::runtime_error( "Failed to initialize semaphore" );
#else
	// allocate a semaphore object, and initialize it
	this->handle = new sem_t;
	if( sem_init( (sem_t *)this->handle, 0, initial_count ) != 0 )
		throw std::runtime_error( "Failed to initialize semaphore" );
#endif
}

semaphore::~semaphore()
{
#if defined(_WIN32)
	if( this->handle )
		CloseHandle( this->handle );
#else
	sem_destroy( (sem_t *)this->handle );
	delete (sem_t *)this->handle;
#endif
}

status semaphore::acquire()
{
#if defined(_WIN32)
	// wait for the semaphore to be available, returns WAIT_OBJECT_0 on success
	auto res = WaitForSingleObject( this->handle, INFINITE );
	if( res == WAIT_OBJECT_0 )
		return status::ok;

	// failed to acquire semaphore, check reason
	if( res == WAIT_TIMEOUT )
		return status::not_ready;
	return status::invalid;
#else
	// wait for the semaphore to be available, returns 0 on success, and -1 on error
	while( sem_wait( (sem_t *)this->handle ) == -1 )
	{
		// retry if interrupted by signal
		if( errno == EINTR )
			continue;

		// other error, return invalid
		return status::invalid;
	}
	return status::ok;
#endif
}

status semaphore::release() 
{
#if defined(_WIN32)
	// release the semaphore, returns TRUE on success
	auto res = ReleaseSemaphore( this->handle, 1, nullptr );
	if( res )
		return status::ok;

	return status::invalid;
#else
	// release the semaphore, returns 0 on success
	auto res = sem_post( (sem_t *)this->handle );
	if( res == 0 )
		return status::ok;

	return status::invalid;
#endif
}

}
//namespace ctle

#endif CTLE_IMPLEMENTATION

#endif//_CTLE_SEMAPHORE_H_
