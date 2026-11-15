// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_WRITE_STREAM_H_
#define _CTLE_WRITE_STREAM_H_

#include <cstring>
#include <type_traits>
#include <vector>

#include "fwd.h"
#include "status.h"
#include "status_return.h"
#include "hasher.h"
#include "file_funcs.h"

namespace ctle
{
/// @brief Buffered sequential output with optional hashing of bytes accepted by the destination.
template<class _DataDestTy, class _HashTy /* = hasher_noop<64> */>
class write_stream
{
	const size_t buffer_size = 2 * 1024 * 1024;

public:
	write_stream( _DataDestTy &_data_dest );
	/// @brief Best-effort end without throwing. Call end() explicitly to observe failures.
	~write_stream();

	using data_destination_type = _DataDestTy;
	using hasher_type = _HashTy;
	using hash_type = typename _HashTy::hash_type;

	/// @brief Number of accepted bytes, including buffered bytes and confirmed partial direct writes.
	u64 get_position() const { return this->current_position; };
	
	// Writes a single value to the stream. (Note that for writing multiple values, the write method which takes a pointer and a count is much more efficient.)
	// Caveat! The type must be trivially copyable, memcpy is used since the dest cache data values may not be correctly aligned
	// Caveat! Make sure the type is correctly packed, since any alignment byte will also be written to the stream
	template<class _DataTy> status write(const _DataTy& src);

	// Write a number of values to the stream. Returns status::ok on success, or an error value on erro
	// Caveat! The type must be trivially copyable, memcpy is used since the dest cache data values may not be correctly aligned
	// Caveat! Make sure the type is correctly packed, since any alignment byte will also be written to the stream
	template<class _DataTy> status write(const _DataTy* src, size_t count = 1);

	/// @brief Write bytes, buffering small inputs and sending large inputs directly.
	/// @note After a retryable partial write, use the position change to locate the unaccepted input suffix.
	status write_bytes( const u8* src, size_t count);

	/// @brief Drain the buffer and finalize the hash once. Repeated successful calls are no-ops.
	/// @note Zero-progress writes can be retried; destination errors and hash failures are terminal.
	status end();

	/// @brief Get the cached final digest, not_ready while open, or the terminal failure status.
	status_return<status,hash_type> get_digest() const
	{
		if( this->stream_state == state::failed )
			return this->failure_status;
		if( this->stream_state != state::ended )
			return status::not_ready;
		return this->hash_digest;
	}

private:
	enum class state { open, ended, failed };
	state stream_state = state::open;
	status failure_status = status_code::ok;
	u64 current_position = 0;
	size_t buffer_offset = 0;
	size_t buffer_position = 0;
	std::vector<u8> buffer;

	data_destination_type &data_dest;
	hasher_type hasher;
	hash_type hash_digest;

	void write_to_buffer( const u8 *src, size_t count );
	status write_to_destination( const u8 *src, size_t count, size_t &written_count );
	status flush_buffer();
	status fail( status error )
	{
		this->stream_state = state::failed;
		this->failure_status = error;
		return error;
	}
};

}

#include "log.h"
#include "_macros.inl"

namespace ctle
{

template<class _DataDestTy, class _HashTy>
inline write_stream<_DataDestTy,_HashTy>::write_stream( _DataDestTy &_data_dest ) 
	: data_dest(_data_dest)
{
	this->buffer.resize(buffer_size);
}

template<class _DataDestTy, class _HashTy>
inline write_stream<_DataDestTy,_HashTy>::~write_stream()
{
	try
	{
		this->end();
	}
	catch( ... )
	{
		// Destruction cannot report errors; callers must explicitly check end().
	}
}

template<class _DataDestTy, class _HashTy>
template<class _DataTy>
inline status write_stream<_DataDestTy,_HashTy>::write( const _DataTy& src )
{
	return this->write( &src, 1);
}

template<class _DataDestTy, class _HashTy>
template<class _DataTy>
inline status write_stream<_DataDestTy,_HashTy>::write( const _DataTy* src, size_t count)
{
	static_assert( std::is_trivially_copyable<_DataTy>(), "_DataTy data type must be trivially copyable" );

	ctStatusCall( this->write_bytes( (const u8*)src, sizeof(_DataTy)*count) );
	return status::ok;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::write_bytes(const u8* src, size_t count)
{
	if( this->stream_state == state::failed )
		return this->failure_status;
	if( this->stream_state != state::open )
		return status::not_ready;

	if( count >= buffer_size - this->buffer_position )
		ctStatusCall( this->flush_buffer() );

	if( count < buffer_size )
	{
		this->write_to_buffer( src, count );
		this->current_position += count;
		return status::ok;
	}

	size_t written_count = 0;
	status result;
	try
	{
		result = this->write_to_destination( src, count, written_count );
	}
	catch( ... )
	{
		this->current_position += written_count;
		throw;
	}
	this->current_position += written_count;
	return result;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::end() try
{
	if( this->stream_state == state::failed )
		return this->failure_status;
	if( this->stream_state == state::ended )
		return status::ok;
	ctStatusCall(this->flush_buffer());
	const auto result = this->hasher.finish();
	if( !result )
		return this->fail( result.status() );
	this->hash_digest = result.value();
	this->stream_state = state::ended;
	return status::ok;
}
catch( ... )
{
	this->fail( status::undefined_error );
	throw;
}

template<class _DataDestTy, class _HashTy>
inline void write_stream<_DataDestTy,_HashTy>::write_to_buffer( const u8 *src, size_t count )
{
	u8* const dest = this->buffer.data();
	memcpy(&dest[this->buffer_position], src, count);
	this->buffer_position += count;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::write_to_destination(
	const u8 *src, size_t count, size_t &written_count ) try
{
	while( written_count < count )
	{
		const auto result = this->data_dest.write( src + written_count, count - written_count );
		if( !result )
			return this->fail( result.status() );
		if( result.value() > count - written_count )
			return this->fail( status::corrupted );
		const size_t accepted = (size_t)result.value();
		if( accepted == 0 )
			return status::cant_write;
		const u8 *accepted_data = src + written_count;
		written_count += accepted;
		const status hash_result = this->hasher.update( accepted_data, accepted );
		if( !hash_result )
			return this->fail( hash_result );
	}
	return status::ok;
}
catch( ... )
{
	this->fail( status::undefined_error );
	throw;
}

template<class _DataDestTy, class _HashTy>
inline status write_stream<_DataDestTy,_HashTy>::flush_buffer()
{
	if( this->buffer_position > 0 )
		ctStatusCall( this->write_to_destination( this->buffer.data(), this->buffer_position, this->buffer_offset ) );
	this->buffer_offset = 0;
	this->buffer_position = 0;

	return status::ok;
}

}
// namespace ctle

#include "_undef_macros.inl"
#endif//_CTLE_WRITE_STREAM_H_
