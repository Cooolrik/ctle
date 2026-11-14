// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_FILE_FUNCS_H_
#define _CTLE_FILE_FUNCS_H_

/// @file file_funcs.h
/// @brief Functions for file handling.

#include <utility>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "fwd.h"
#include "status.h"

namespace ctle
{

/// @brief Enum class for file access modes in file_access() function.
enum class access_mode : unsigned int
{
	no_access = 0x0,	// only checks for existence of file
	read = 0x2,			// read access
	write = 0x4,		// write access
	read_write = 0x6,	// read and write access
};

/// @brief Check if a file exists
/// @param path the file path
/// @return true if the file exists
bool file_exists(const char* path);
bool file_exists(const std::string& path); ///< @copydoc ctle::file_exists

/// @brief Check if a file can be accessed with a specified access mode
/// @param path the file path
/// @param amode the access mode
/// @return 
///	- status::ok if the file can be accessed using the specified access mode
///	- status::cant_access if the file can't be accessed 
///	- status::not_found if the file doesn't exist
///	- status::invalid_param if the path is nullptr
status file_access(const char* path, access_mode amode);
status file_access(const std::string& path, access_mode amode); ///< @copydoc ctle::file_access

/// @brief Read a file in binary mode into a vector of bytes
/// @param filepath the source file path
/// @param dest the destination vector
/// @return 
/// - status::ok if the file was read successfully
/// - status::cant_allocate if the vector could not be allocated
/// - status::cant_read if the file could not be read
status read_file(const std::string & filepath, std::vector<uint8_t>&dest);

/// @brief Write a file in binary mode from a pointer to or a container.
/// @param filepath the destination file path
/// @param src the source data 
/// @param src_size the source data size
/// @param overwrite_existing if false, the file will not be overwritten if it already exists, and the function will return status::already_exists
/// @return 
/// - status::ok if the file was written successfully
/// - status::cant_write if the file could not be written
/// - status::already_exists if the file already exists and overwrite_existing is false
status write_file(const std::string& filepath, const void* src, size_t src_size, bool overwrite_existing = false);

/// @brief Write a file in binary mode from a pointer to or a container.
/// @tparam _Ty the type of the source data(container)
/// @param filepath the destination file path
/// @param src the source data 
/// @param overwrite_existing if false, the file will not be overwritten if it already exists, and the function will return status::already_exists
/// @return 
/// - status::ok if the file was written successfully
/// - status::cant_write if the file could not be written
/// - status::already_exists if the file already exists and overwrite_existing is false
template<class _Ty> inline status write_file(const std::string& filepath, const _Ty& src, bool overwrite_existing = false)
{
	return write_file( filepath, (const void *)src.data(), src.size() * sizeof( typename _Ty::value_type ), overwrite_existing );
}

/// @brief Class for file reading/writing, encapsulating a file object.
/// @details This class is portable, but uses native interfaces when possible. Mainly for internal use, but can be used directly.
/// @note Move-only. A moved-from object is closed and can be reopened.
class _file_object
{
private:
	struct os_data;
	std::unique_ptr<os_data> data;
	
public:
	_file_object();
	~_file_object();
	_file_object( const _file_object & ) = delete;
	_file_object &operator=( const _file_object & ) = delete;

	/// @brief Transfer the open file and its position, leaving the source closed.
	_file_object( _file_object &&other ) noexcept;

	/// @brief Close the current file and transfer ownership, leaving the source closed.
	_file_object &operator=( _file_object &&other ) noexcept;

	/// @brief Open a file for reading
	/// @param filepath the file path
	/// @return 
	/// - status::ok if the file was opened successfully
	/// - status::cant_open if the file could not be opened
	/// - status::corrupted if the file size could not be determined
	status open_read(const std::string & filepath);

	/// @brief Open a file for writing
	/// @param filepath the file path
	/// @param overwrite_existing if false, the file will not be overwritten if it already exists, and the function will return status::already_exists
	/// @return 
	/// - status::ok if the file was opened successfully
	/// - status::cant_write if the file could not be opened
	/// - status::already_exists if the file already exists and overwrite_existing is false
	status open_write(const std::string & filepath, bool overwrite_existing = false);

	/// @brief Close the file
	status close();

	/// @brief Check if the file is open
	bool is_open() const;

	/// @brief Get the size of the file
	u64 size() const;

	/// @brief Read data from the file
	/// @param dest the destination buffer
	/// @param size the number of bytes to read
	/// @return 
	/// - status::ok if the data was read successfully
	/// - status::cant_read if the data could not be read
	status read(u8 * dest, const u64 size);

	/// @brief Write data to the file
	/// @param src the source buffer
	/// @param size the number of bytes to write
	/// @return 
	/// - status::ok if the data was written successfully
	/// - status::cant_write if the data could not be written
	status write(const u8 * src, const u64 size);
};

}
//namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include "log.h"
#include "_macros.inl"

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

status read_file(const std::string& filepath, std::vector<uint8_t>& dest)
{
	_file_object f;
	ctStatusCall(f.open_read(filepath));

	dest.resize( f.size() );
	if( dest.size() != f.size() )
	{
		return status::cant_allocate;
	}

	ctStatusCall(f.read(dest.data(), dest.size()));
	ctStatusCall(f.close());
	return status::ok;
}

status write_file( const std::string &filepath, const void *src, size_t src_size, bool overwrite_existing )
{
	// src can only be nullptr if src_size is 0
	if( !src && src_size > 0 )
		return status::invalid_param;
	
	_file_object f;
	ctStatusCall(f.open_write(filepath,overwrite_existing));
	ctStatusCall(f.write( (u8*)src, src_size));
	ctStatusCall(f.close());
	return status::ok;
}

}
//namespace ctle

#if defined(_WIN32)

#define _ADD_CTLE_HEADERS_WIN_STD
#include "os.inl"

namespace ctle
{
struct _file_object::os_data
{
	windows_handle_ref file_handle;
	u64 file_size = 0;

	os_data( const std::wstring &path, DWORD access, DWORD disposition, DWORD attributes )
		: file_handle( ::CreateFileW( path.c_str(), access,
			FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, nullptr,
			disposition, attributes, nullptr ) )
	{}
};

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

status _file_object::open_read(const std::string& filepath)
{
	this->close();

	// convert the utf8 string to wstring fullpath for the API call
	const auto wpath = utf8string_to_wstringfullpath(filepath);

	auto opened = std::make_unique<os_data>( wpath, GENERIC_READ, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY );
	if (opened->file_handle.get_handle() == INVALID_HANDLE_VALUE)
	{
		// failed to open the file
		return status::cant_open;
	}

	// get the size
	LARGE_INTEGER dfilesize = {};
	if (!::GetFileSizeEx(opened->file_handle.get_handle(), &dfilesize))
	{
		// failed to get the size
		return status::corrupted;
	}
	opened->file_size = dfilesize.QuadPart;
	this->data = std::move( opened );

	return status::ok;
}

status _file_object::open_write(const std::string& filepath, bool overwrite_existing)
{
	this->close();

	// convert the utf8 string to wstring fullpath for the API call
	const auto wpath = utf8string_to_wstringfullpath(filepath);
	auto opened = std::make_unique<os_data>( wpath, GENERIC_WRITE,
		overwrite_existing ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL );
	if( opened->file_handle.get_handle() == INVALID_HANDLE_VALUE )
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

	this->data = std::move( opened );
	return status::ok;
}

status _file_object::read(u8* dest, const u64 size)
{
	ctValidate(this->is_open(), status::not_ready) << "The file stream is not open" << ctValidateEnd;

	u64 bytes_read = 0;
	while( bytes_read < size )
	{
		// check how much to read and cap each read at UINT_MAX
		const u64 bytes_left = size - bytes_read;
		const DWORD bytes_to_read_this_time = (bytes_left < UINT_MAX) ? ((DWORD)bytes_left) : (UINT_MAX);

		// read in bytes into the memory allocation
		DWORD bytes_that_were_read = 0;
		if( !::ReadFile( this->data->file_handle.get_handle(), &dest[bytes_read], bytes_to_read_this_time, &bytes_that_were_read, nullptr ) )
		{
			// failed to read from the file
			return status::cant_read;
		}
		if( bytes_that_were_read == 0 )
		{
			break; // EOF reached
		}

		// update number of bytes that were read
		bytes_read += bytes_that_were_read;
	}

	return status::ok;
}

status _file_object::write(const u8* src, const u64 size)
{
	ctValidate(this->is_open(), status::not_ready) << "The file stream is not open" << ctValidateEnd;

	// write the file
	u64 bytes_written = 0;
	while( bytes_written < size )
	{
		// check how much to write, capped at UINT_MAX
		const u64 bytes_left = size - bytes_written;
		const DWORD bytes_to_write_this_time = (bytes_left < UINT_MAX) ? ((DWORD)bytes_left) : (UINT_MAX);

		// write the bytes to file
		DWORD bytes_that_were_written = 0;
		if( !::WriteFile( this->data->file_handle.get_handle(), &src[bytes_written], (DWORD)bytes_to_write_this_time, &bytes_that_were_written, nullptr ) )
		{
			// failed to write to file
			return status::cant_write;
		}
		ctSanityCheck( bytes_that_were_written != 0 ); // this should not happen with a regular file

		// update number of bytes that were written
		bytes_written += bytes_that_were_written;
	}

	return status::ok;
}

}
//namespace ctle

#elif defined(__linux__)

#define _ADD_CTLE_HEADERS_LINUX_STD
#include "os.inl"

namespace ctle
{
struct _file_object::os_data
{
	std::fstream file;
	u64 file_size = 0;
};

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

status _file_object::open_read(const std::string& filepath)
{
	this->close();

	// open the file at the end, so we can tell() the size of the file
	auto opened = std::make_unique<os_data>();
	opened->file.open( filepath, std::ios::in | std::ios::binary | std::ios::ate );
	if( !opened->file.is_open() )
	{
		return status::cant_open;
	}

	// get the size of the file, and move back to the beginning
	const auto file_size = opened->file.tellg();
	if( file_size == std::streampos( -1 ) )
		return status::corrupted;
	opened->file.seekg( 0, std::ios::beg );
	if( opened->file.fail() )
		return status::corrupted;

	opened->file_size = (u64)file_size;
	this->data = std::move( opened );
	return status::ok;
}

status _file_object::open_write(const std::string& filepath, bool overwrite_existing)
{
	this->close();

	// if we can't overwrite an existing file, fail if the file exists
	if( !overwrite_existing && file_exists( filepath ) )
		return status::already_exists;

	// create the file
	auto opened = std::make_unique<os_data>();
	opened->file.open( filepath, std::ios::out | std::ios::binary );
	if( !opened->file.is_open() )
	{
		return status::cant_write;
	}

	this->data = std::move( opened );
	return status::ok;
}

status _file_object::read(u8* dest, const u64 size)
{
	ctValidate(this->is_open(), status::not_ready) << "The file stream is not open" << ctValidateEnd;

	// read the data to the dest
	this->data->file.read((char*)dest, size);
	if( this->data->file.fail() )
		return status::cant_read;

	return status::ok;
}

status _file_object::write(const u8* src, const u64 size)
{
	ctValidate(this->is_open(), status::not_ready) << "The file stream is not open" << ctValidateEnd;

	// read the data to the dest
	this->data->file.write((const char*)src, size);
	if( this->data->file.fail() )
		return status::cant_read;

	return status::ok;
}

}
//namespace ctle

#endif// defined(_MSC_VER) elif defined(__GNUC__)

namespace ctle
{
_file_object::_file_object() = default;
_file_object::~_file_object() = default;
_file_object::_file_object( _file_object &&other ) noexcept = default;
_file_object &_file_object::operator=( _file_object &&other ) noexcept = default;

status _file_object::close()
{
	this->data.reset();
	return status::ok;
}

bool _file_object::is_open() const
{
	return bool( this->data );
}

u64 _file_object::size() const
{
	return this->data ? this->data->file_size : 0;
}
}
//namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_FILE_FUNCS_H_
