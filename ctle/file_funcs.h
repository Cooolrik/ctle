// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include "status.h"

#include <utility>
#include <iostream>
#include <fstream>

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
	status file_access( const char *path , access_mode amode );
	status file_access( const std::string &path , access_mode amode );

	// read whole file into a data container (like string or vector) in binary mode
	template<class _Ty> inline status read_file( const std::string &filepath, _Ty &dest )
		{
		static_assert( sizeof(typename _Ty::value_type) == 1 , "Can only be used with container types where value_type is one byte in size." );

		std::ifstream ifs(filepath, std::ios::binary);
		if( !ifs.is_open() )
			return status::cant_open;

		// get size of file
		ifs.seekg(0, std::ios::end);
		auto fsize = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		// allocate size and read in data
		dest.resize( (size_t)fsize );
		ifs.read((char*)dest.data(), fsize);
		ifs.close();

		return status::ok;
		}

	// write whole file from a data container (like string or vector) in binary mode
	template<class _Ty> inline status write_file( const std::string &filepath, const _Ty &src )
		{
		static_assert( sizeof(typename _Ty::value_type) == 1 , "Can only be used with container types where value_type is one byte in size." );

		std::ofstream ofs(filepath, std::ios::binary);
		if( !ofs.is_open() )
			return status::cant_open;

		// write the data
		ofs.write((const char*)src.data(), src.size());
		ofs.close();

		return status::ok;
		}
	};

#ifdef CTLE_IMPLEMENTATION

namespace ctle
	{
	bool file_exists( const char *path )
		{
		return file_access( path , access_mode::no_access );
		}

	bool file_exists( const std::string &path )
		{
		return file_access( path , access_mode::no_access );
		}

	status file_access( const char *path , access_mode amode )
		{
		if( !path )
			return status::invalid_param;
		return file_access( std::string(path), amode );
		}
	};

#if defined(_MSC_VER)
#include <io.h>

namespace ctle
	{
	status file_access( const std::string &path , access_mode amode )
		{
		auto res = _access_s(path.c_str(),(int)amode);
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
	};

#elif defined(__GNUC__)

#include <unistd.h>

namespace ctle
	{
	status file_access( const std::string &path , access_mode amode )
		{
		auto res = access(path.c_str(),(int)amode);
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
	};

#endif// defined(_MSC_VER) elif defined(__GNUC__)

#endif//CTLE_IMPLEMENTATION
