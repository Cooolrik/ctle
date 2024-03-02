// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include "status.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <vector>

namespace ctle
{
enum class access_mode : unsigned int
{
	no_access = 0x0,	// only checks for existance of file
	read = 0x2,			// read access
	write = 0x4,		// write access
	read_write = 0x6,	// read and write access
};

// check if file exists
bool file_exists( const char *path );
bool file_exists( const std::string &path );

// check if a file exists, and can be accessed with a specified access mode
status file_access( const char *path, access_mode amode );
status file_access( const std::string &path, access_mode amode );

// read a file in binary mode into a vector of bytes
status read_file( const std::string &filepath, std::vector<uint8_t> &dest );

// Write a file in binary mode from a pointer to or a container.
// If overwrite_existing is false, the return value will be status::already_exists if the file already exists.
status write_file( const std::string &filepath, const void *src, size_t src_size, bool overwrite_existing = false );
template<class _Ty> inline status write_file( const std::string &filepath, const _Ty &src, bool overwrite_existing = false )
{
	return write_file( filepath, (const void *)src.data(), src.size() * sizeof( typename _Ty::value_type ), overwrite_existing );
}

}
//namespace ctle

#ifdef CTLE_IMPLEMENTATION

namespace ctle
{
bool file_exists( const char *path )
{
	return file_access( path, access_mode::no_access );
}

bool file_exists( const std::string &path )
{
	return file_access( path, access_mode::no_access );
}

status file_access( const char *path, access_mode amode )
{
	if( !path )
		return status::invalid_param;
	return file_access( std::string( path ), amode );
}
}
//namespace ctle

#if defined(_MSC_VER)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <io.h>

namespace ctle
{
status file_access( const std::string &path, access_mode amode )
{
	auto res = _access_s( path.c_str(), (int)amode );
	if( res == 0 )
		return status::ok;
	else if( res == EACCES )
		return status::cant_access;
	else if( res == ENOENT )
		return status::not_found;
	else if( res == EINVAL )
		return status::invalid_param;
	else
		return status::undefined_error;
}

static std::wstring utf8string_to_wstringfullpath( std::string utf8str )
{
	// convert utf-8 string to wstring
	int req_chars = ::MultiByteToWideChar( CP_UTF8, 0, utf8str.c_str(), -1, nullptr, 0 );
	std::wstring wstr( req_chars, L'\0' );
	DWORD conv_chars = ::MultiByteToWideChar( CP_UTF8, 0, utf8str.c_str(), -1, &wstr[0], req_chars );
	wstr.resize( conv_chars );

	// make wstring path into fullpath 
	constexpr const DWORD max_path_len = 32768;
	std::wstring wfullpath( max_path_len, L'\0' );
	DWORD actual_len = GetFullPathNameW( wstr.c_str(), max_path_len, &wfullpath[0], nullptr );
	wfullpath.resize( actual_len );

	return wfullpath;
}

status read_file( const std::string &filepath, std::vector<uint8_t> &dest )
{
	// convert the utf8 string to wstring fullpath for the API call
	auto wpath = utf8string_to_wstringfullpath( filepath );

	// open the file
	HANDLE file_handle = ::CreateFileW( wpath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr );
	if( file_handle == INVALID_HANDLE_VALUE )
	{
		// failed to open the file
		return status::cant_open;
	}

	// get the size
	LARGE_INTEGER dfilesize = {};
	if( !::GetFileSizeEx( file_handle, &dfilesize ) )
	{
		// failed to get the size
		return status::corrupted;
	}
	uint64_t total_bytes_to_read = dfilesize.QuadPart;

	// read in all of the file
	dest.resize( total_bytes_to_read );
	if( dest.size() != total_bytes_to_read )
	{
		// failed to allocate the memory
		return status::cant_allocate;
	}
	uint8_t *buffer = dest.data();

	uint64_t bytes_read = 0;
	while( bytes_read < total_bytes_to_read )
	{
		// check how much to read and cap each read at UINT_MAX
		uint64_t bytes_left = total_bytes_to_read - bytes_read;
		uint32_t bytes_to_read_this_time = UINT_MAX;
		if( bytes_left < UINT_MAX )
			bytes_to_read_this_time = (uint32_t)bytes_left;

		// read in bytes into the memory allocation
		DWORD bytes_that_were_read = 0;
		if( !::ReadFile( file_handle, &buffer[bytes_read], bytes_to_read_this_time, &bytes_that_were_read, nullptr ) )
		{
			// failed to read
			return status::cant_read;
		}

		// update number of bytes that were read
		bytes_read += bytes_that_were_read;
	}

	::CloseHandle( file_handle );
	return status::ok;
}

status write_file( const std::string &filepath, const void *src, size_t src_size, bool overwrite_existing )
{
	// src can only be nullptr if src_size is 0
	if( !src && src_size > 0 )
		return status::invalid_param;

	// convert the utf8 string to wstring fullpath for the API call
	auto wpath = utf8string_to_wstringfullpath( filepath );

	// create the file
	HANDLE fileHandle = ::CreateFileW(
		wpath.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		nullptr,
		( overwrite_existing ) ? ( CREATE_ALWAYS ) : ( CREATE_NEW ),
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
	if( fileHandle == INVALID_HANDLE_VALUE )
	{
		// file open failed. return reason in error code
		DWORD errorCode = GetLastError();
		if( errorCode == ERROR_FILE_EXISTS )
		{
			return status::already_exists;
		}
		else
		{
			return status::cant_write;
		}
	}

	// write the file
	uint8_t *writeBuffer = (uint8_t *)src;
	uint64_t bytesWritten = 0;
	uint64_t totalBytesToWrite = src_size;
	while( bytesWritten < totalBytesToWrite )
	{
		// check how much to write, capped at UINT_MAX
		uint64_t bytesToWrite = std::min<uint64_t>( totalBytesToWrite - bytesWritten, UINT_MAX );

		// write the bytes to file
		DWORD numBytesWritten = 0;
		if( !::WriteFile( fileHandle, &writeBuffer[bytesWritten], (DWORD)bytesToWrite, &numBytesWritten, nullptr ) )
		{
			// failed to write
			return status::cant_write;
		}

		// update number of bytes that were read
		bytesWritten += numBytesWritten;
	}

	// done
	::CloseHandle( fileHandle );
	return status::ok;
}

}
//namespace ctle

#elif defined(__GNUC__)

#include <unistd.h>

namespace ctle
{
status file_access( const std::string &path, access_mode amode )
{
	auto res = access( path.c_str(), (int)amode );
	if( res == 0 )
		return status::ok;
	else if( res == EACCES )
		return status::cant_access;
	else if( res == ENOENT )
		return status::not_found;
	else if( res == EINVAL )
		return status::invalid_param;
	else
		return status::undefined_error;
}


status read_file( const std::string &filepath, std::vector<uint8_t> &dest )
{
	// open the file at the end
	std::ifstream file( filepath.c_str(), std::ios::in | std::ios::binary | std::ios::ate );
	if( !file.is_open() )
	{
		// failed to open the file
		return status::cant_open;
	}

	// get the size of the file, and move back to the beginning
	uint64_t total_bytes_to_read = file.tellg();
	file.seekg( 0, std::ios::beg );

	// allocate the data
	dest.resize( total_bytes_to_read );
	if( dest.size() != total_bytes_to_read )
	{
		// failed to allocate the memory
		return status::cant_allocate;
	}

	// read the data to the dest
	file.read( (char *)dest.data(), total_bytes_to_read );
	if( (uint64_t)file.tellg() != total_bytes_to_read )
	{
		// failed to read full file
		return status::cant_read;
	}

	file.close();
	return status::ok;
}

status write_file( const std::string &filepath, const void *src, size_t src_size, bool overwrite_existing )
{
	// src can only be nullptr if src_size is 0
	if( !src && src_size > 0 )
		return status::invalid_param;

	// if we cant overwrite an existing file, fail if the file exists
	if( !overwrite_existing && file_exists( filepath ) )
		return status::already_exists;

	// create the file
	std::ofstream file( filepath.c_str(), std::ios::out | std::ios::binary );
	if( !file.is_open() )
	{
		// failed to open the file
		return status::cant_write;
	}

	// write the file
	file.write( (const char *)src, src_size );
	if( (size_t)file.tellp() != src_size )
	{
		// failed to write full file
		return status::cant_write;
	}

	// done
	file.close();
	return status::ok;
}
}
//namespace ctle

#endif// defined(_MSC_VER) elif defined(__GNUC__)

#endif//CTLE_IMPLEMENTATION
