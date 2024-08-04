// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

#include "status.h"
#include "status_return.h"
#include "file_funcs.h"

namespace ctle
{

// data destination objects implement a write method, and can be used for streaming data classes, e.g. write_stream 
//
// the method which needs to be implemented is:
//
// status_return<status, u64> write(const u8* src_buffer, u64 write_count)
//	- Blocking call which writes to the destination from a src_buffer, until the write_count bytes have been written, or an error occurs. 
//	  On succes, the method must return status::ok, and the actual number of bytes written to the destination.

class file_data_destination
{
public:
	file_data_destination( const std::string &filepath, bool overwrite_existing = true );
	~file_data_destination();

	// write from source buffer into destination, return number of bytes actually written
	status_return<status, u64> write(const u8* src_buffer, u64 write_count);

private:
	u64 file_position = 0;
	_file_object file;
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include "log.h"
#include "_macros.inl"

namespace ctle
{

file_data_destination::file_data_destination( const std::string &filepath, bool overwrite_existing )
{
	ctStatusCallThrow(this->file.open_write(filepath,overwrite_existing));
}

file_data_destination::~file_data_destination()
{
	this->file.close();
}

status_return<status, u64> file_data_destination::write(const u8* src_buffer, u64 write_count)
{
	if( write_count > 0 )
	{
		ctStatusCall(this->file.write(src_buffer, write_count));
		this->file_position += write_count;
	}

	return write_count;
}

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION