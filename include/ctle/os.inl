// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

// os.inl - includes os-specific files.

// each time a set of includes are needed, define the correct define and include os.inl, this will include the defined files
// you can define multiple defines at once and then include os.inl, and the defined are reset each time they are found


// ----------------------------------------------------
// Windows standard headers
#ifdef _ADD_CTLE_HEADERS_WIN_STD
#ifndef _CTLE_HEADERS_WIN_STD_ADDED
#define _CTLE_HEADERS_WIN_STD_ADDED

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif//WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif//NOMINMAX

#include <Windows.h>
#include <io.h>
#include <utility>

namespace ctle
{

// RAII wrapper for Windows HANDLE
class windows_handle_ref
{
public:
	windows_handle_ref( HANDLE _handle = INVALID_HANDLE_VALUE ) : handle( _handle ) {}
	~windows_handle_ref() { this->reset(); }
	windows_handle_ref( const windows_handle_ref & ) = delete;
	const windows_handle_ref &operator = ( const windows_handle_ref & ) = delete;
	windows_handle_ref( windows_handle_ref &&other ) noexcept : handle( other.release() ) {}
	windows_handle_ref &operator=( windows_handle_ref &&other ) noexcept
	{
		if( this != &other )
			this->reset( other.release() );
		return *this;
	}
	HANDLE get_handle() const noexcept { return this->handle; }
	HANDLE release() noexcept { return std::exchange( this->handle, INVALID_HANDLE_VALUE ); }
	void reset( HANDLE value = INVALID_HANDLE_VALUE ) noexcept
	{
		if( this->handle != INVALID_HANDLE_VALUE && this->handle != nullptr )
			::CloseHandle( this->handle );
		this->handle = value;
	}
private:
	HANDLE handle;
};

}

#endif//_CTLE_HEADERS_WIN_STD_ADDED
#undef _ADD_CTLE_HEADERS_WIN_STD
#endif//_ADD_CTLE_HEADERS_WIN_STD


// ----------------------------------------------------
// Linux standard headers
#ifdef _ADD_CTLE_HEADERS_LINUX_STD
#ifndef _CTLE_HEADERS_LINUX_STD_ADDED
#define _CTLE_HEADERS_LINUX_STD_ADDED

#include <unistd.h>
#include <locale.h>
#include <unistd.h>
#include <spawn.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <cstring>
#include <utility>

// RAII wrapper for Linux file handles
class linux_file_ref
{
public:
	linux_file_ref( int _handle = -1 ) : handle( _handle ) {}
	~linux_file_ref() { this->reset(); }
	linux_file_ref( const linux_file_ref & ) = delete;
	const linux_file_ref &operator = ( const linux_file_ref & ) = delete;
	linux_file_ref( linux_file_ref &&other ) noexcept : handle( other.release() ) {}
	linux_file_ref &operator=( linux_file_ref &&other ) noexcept
	{
		if( this != &other )
			this->reset( other.release() );
		return *this;
	}
	int get_handle() const noexcept { return this->handle; }
	int release() noexcept { return std::exchange( this->handle, -1 ); }
	void reset( int value = -1 ) noexcept
	{
		if( this->handle != -1 )
			::close( this->handle );
		this->handle = value;
	}
private:
	int handle;
};

#endif//_CTLE_HEADERS_LINUX_STD_ADDED
#undef _ADD_CTLE_HEADERS_LINUX_STD
#endif//_ADD_CTLE_HEADERS_LINUX_STD



// ----------------------------------------------------
// Windows socket headers
#ifdef _ADD_CTLE_HEADERS_WIN_SOCKETS
#ifndef _CTLE_HEADERS_WIN_SOCKETS_ADDED
#define _CTLE_HEADERS_WIN_SOCKETS_ADDED

#include <WinSock2.h>
#include <WS2tcpip.h>

#endif//_CTLE_HEADERS_WIN_SOCKETS_ADDED
#undef _ADD_CTLE_HEADERS_WIN_SOCKETS
#endif//_ADD_CTLE_HEADERS_WIN_SOCKETS



// ----------------------------------------------------
// Linux socket headers
#ifdef _ADD_CTLE_HEADERS_LINUX_SOCKETS
#ifndef _CTLE_HEADERS_LINUX_SOCKETS_ADDED
#define _CTLE_HEADERS_LINUX_SOCKETS_ADDED

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#endif//_CTLE_HEADERS_LINUX_SOCKETS_ADDED
#undef _ADD_CTLE_HEADERS_LINUX_SOCKETS
#endif//_ADD_CTLE_HEADERS_LINUX_SOCKETS