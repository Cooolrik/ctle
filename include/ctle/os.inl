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

namespace ctle
{

// RAII wrapper for Windows HANDLE
class windows_handle_ref
{
public:
	windows_handle_ref( HANDLE _handle ) : handle( _handle ) {}
	~windows_handle_ref() { if( handle!=INVALID_HANDLE_VALUE ) { ::CloseHandle( handle ); } }
	HANDLE get_handle() const noexcept { return this->handle; }
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