// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

// os.inl - includes os-specific files.

// each time a set of includes are needed, define the correct define and include os.inl, this will include the defined files
// you can define multiple defines at once and then include os.inl, and the defined are reset each time they are found


// ----------------------------------------------------
// Windows standard headers
#ifdef _ADD_CTLE_HEADERS_WIN_STD

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif//WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif//NOMINMAX

#include <Windows.h>
#include <io.h>

#undef _ADD_CTLE_HEADERS_WIN_STD
#endif//_ADD_CTLE_HEADERS_WIN_STD



// ----------------------------------------------------
// Linux standard headers
#ifdef _ADD_CTLE_HEADERS_LINUX_STD

#include <unistd.h>
#include <locale.h>

#undef _ADD_CTLE_HEADERS_LINUX_STD
#endif//_ADD_CTLE_HEADERS_LINUX_STD



// ----------------------------------------------------
// Windows socket headers
#ifdef _ADD_CTLE_HEADERS_WIN_SOCKETS

#include <WinSock2.h>
#include <WS2tcpip.h>

#undef _ADD_CTLE_HEADERS_WIN_SOCKETS
#endif//_ADD_CTLE_HEADERS_WIN_SOCKETS



// ----------------------------------------------------
// Linux socket headers
#ifdef _ADD_CTLE_HEADERS_LINUX_SOCKETS

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#undef _ADD_CTLE_HEADERS_LINUX_SOCKETS
#endif//_ADD_CTLE_HEADERS_LINUX_SOCKETS