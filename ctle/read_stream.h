// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

#include <vector>

#include "status.h"
#include "status_return.h"
#include "types.h"
#include "hash.h"
#include "file_funcs.h"

namespace ctle
{
constexpr const size_t data_stream_default_buffer_size = 2 * 1024 * 1024;

class _data_stream_hasher;

// base class for a read_stream, a read-only input stream which is designed for streaming data sequentially, using a memory buffer. 
// the read stream can optionally calculate an SHA256 hash on the read data, if picosha2 is included in the build. 
// NOTE to implement hash calculation, make sure to include picosha2 before including ctle in the file which implements ctle
class read_stream
{
public:
	virtual ~read_stream();

	// get the number of bytes read up until the current position
	// NOTE this does not include the read look-ahead buffer, but represents the number of bytes read out from the stream by the user
	u64 get_position() const { return this->current_position; };

	// peek at the next single value in the stream, but don't advance the stream pointer. if at end of the stream, an empty value is returned
	template<class _Ty> _Ty peek();

	// read a single value from the stream. if at end of the stream, an empty value is returned. 
	template<class _Ty> _Ty read();

	// read a number of values from the stream. if at end of the stream, method returns an error
	template<class _Ty> status read(_Ty* dest, size_t count = 1);

	// read the raw byte stream into a memory area
	status read_bytes(u8* dest, size_t count);

	// returns true if the stream has ended (eos/eof)
	bool has_ended() const;

	// get the hash digest from the stream, after stream has ended
	// NOTE the method will only be available if picosha2 is included in the build
	status_return<status,hash<256>> get_digest() const;

protected:
	read_stream();
	status initialize(bool calculate_hash, size_t buffer_size);
	virtual status_return<status, u64> read_data(u8* dest, u64 size) = 0;

private:
	u64 current_position = 0;
	size_t buffer_position = 0;
	size_t buffer_end = 0;
	std::vector<u8> buffer;

	std::unique_ptr<_data_stream_hasher> hasher;

	// fills the read buffer, and updates the hash
	status fill_buffer();
};

}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include "log.h"
#include "_macros.inl"

namespace ctle
{

#ifdef PICOSHA2_H

// internal wrapper of picosha2
class _data_stream_hasher
{
private:
	picosha2::hash256_one_by_one hasher;
	hash<256> digest;

public:
	_data_stream_hasher() { hasher.init(); }
	void process(const u8* data, size_t size) { if (size > 0) { hasher.process(data, data + size); } }
	void finish() { hasher.finish(); hasher.get_hash_bytes(digest.data, digest.data + 32); }
	hash<256> get_digest() const { return this->digest; };
};

#endif//PICOSHA2_H

read_stream::read_stream() 
{
}

read_stream::~read_stream()
{
}

status read_stream::initialize(bool calculate_hash, size_t buffer_size)
{
	// allocate hasher and buffer
	if (calculate_hash)
	{
		this->hasher = std::make_unique<_data_stream_hasher>();
	}
	this->buffer.resize(buffer_size);

	// fill the buffer from the stream
	ctStatusCall(this->fill_buffer());
	return status::ok;
}

status read_stream::read_bytes(u8* dest, size_t count)
{
	const u8* src = this->buffer.data();

	size_t read_bytes = 0;
	while( read_bytes < count )
	{
		ctValidate(!this->has_ended(), status::cant_read) << "The stream ended before reading the desired data count" << ctValidateEnd;

		const size_t in_buffer = this->buffer_end - this->buffer_position;
		const size_t to_read = std::min(count - read_bytes , in_buffer);
		
		// copy from the buffer, and step up counter
		memcpy(&dest[read_bytes], &src[this->buffer_position], to_read);
		this->buffer_position += to_read;
		read_bytes += to_read;

		// if we are at the end of the buffer, fill it
		if (this->buffer_position >= this->buffer_end)
			ctStatusCall(this->fill_buffer());
	}

	return status::ok;
}

bool read_stream::has_ended() const
{
	if (this->buffer_end == this->buffer.size())
		return false;
	return this->buffer_position >= this->buffer_end;
}

status_return<status,hash<256>> read_stream::get_digest() const
{
	ctValidate(this->hasher, status::not_initialized) << "The stream is not set to calculating a hash" << ctValidateEnd;
	ctValidate(this->has_ended(), status::not_ready) << "The stream has not ended, the hash is only avaliable after the end" << ctValidateEnd;

	return this->hasher->get_digest();
}

status read_stream::fill_buffer()
{
	u8* const buffer_data = this->buffer.data();
	const size_t buffer_count = buffer_end - buffer_position;

	// move whatever is left in the buffer to the beginning
	if (buffer_count > 0)
		memcpy( (void*)buffer_data, (void*)&buffer_data[buffer_position], buffer_count);
	buffer_position = 0;
	buffer_end = buffer_count;

	// fill up with new data. 
	const size_t fill_start = buffer_end;
	const size_t fill_count = this->buffer.size() - buffer_end;
	ctStatusAutoReturnCall(read_count, this->read_data(&buffer_data[fill_start], fill_count));
	buffer_end += read_count;

	// update hash digest
	if (this->hasher)
	{
		this->hasher->process(&buffer_data[fill_start], read_count);

		// if at the end of the stream, get the hash value
		if (this->has_ended())
			this->hasher->finish();
	}
	
	return status::ok;
}

}
// namespace ctle

#include "_undef_macros.inl"

#endif//CTLE_IMPLEMENTATION