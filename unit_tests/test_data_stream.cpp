// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#include <ctle/sockets.h>

#include "unit_tests.h"

#include <future>
#include <thread>

#include <ctle/read_stream.h>
#include <ctle/data_source.h>
#include <ctle/write_stream.h>
#include <ctle/data_destination.h>
#include <ctle/ntup.h>
#include <ctle/string_funcs.h>

#include <algorithm>
#include <limits>

using namespace ctle;

namespace
{
	class chunked_destination
	{
	public:
		std::vector<u8> bytes;
		size_t write_limit = std::numeric_limits<size_t>::max();
		size_t remaining_capacity = std::numeric_limits<size_t>::max();

		value_return<size_t> write( const u8 *src, size_t count )
		{
			const size_t accepted = std::min( count, std::min( this->write_limit, this->remaining_capacity ) );
			this->bytes.insert( this->bytes.end(), src, src + accepted );
			this->remaining_capacity -= accepted;
			return accepted;
		}
	};

	class uncertain_destination
	{
	public:
		std::vector<u8> bytes;

		value_return<size_t> write( const u8 *src, size_t /*count*/ )
		{
			this->bytes.push_back( *src );
			return status::cant_write;
		}
	};

	class failing_update_hasher
	{
	public:
		using hash_type = digest<64>;
		status update( const u8 *, size_t ) { return status::corrupted; }
		value_return<hash_type> finish() { return hash_type{}; }
	};

	class failing_finish_hasher
	{
	public:
		using hash_type = digest<64>;
		bool first_finish = true;
		status update( const u8 *, size_t ) { return status::ok; }
		value_return<hash_type> finish()
		{
			if( this->first_finish )
			{
				this->first_finish = false;
				return status::corrupted;
			}
			return hash_type{};
		}
	};

	class throwing_destination
	{
	public:
		std::vector<u8> bytes;
		bool throw_on_write = true;
		size_t successful_writes = 0;

		value_return<size_t> write( const u8 *src, size_t /*count*/ )
		{
			this->bytes.push_back( *src );
			if( this->throw_on_write && this->bytes.size() > this->successful_writes )
				throw std::runtime_error( "Destination failed after accepting a byte" );
			return size_t(1);
		}
	};

	class overreporting_destination
	{
	public:
		value_return<size_t> write( const u8 *, size_t count ) { return count + 1; }
	};

	class throwing_finish_hasher : public hasher_noop<64>
	{
	public:
		bool first_finish = true;
		value_return<hash_type> finish()
		{
			if( this->first_finish )
			{
				this->first_finish = false;
				throw std::runtime_error( "Hasher failed during finalization" );
			}
			return hash_type{};
		}
	};

	const std::vector<u8> stream_test_data = { 'a', 'b', 'c' };
	const char *stream_test_sha256 = "ba7816bf8f01cfea414140de5dae2223"
		"b00361a396177a9cb410ff61f20015ad";
}

TEST( data_stream, short_writes_commit_and_hash_each_byte_once )
{
	chunked_destination destination;
	destination.write_limit = 2;
	write_stream<chunked_destination, hasher_sha256> stream( destination );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	ASSERT_EQ( stream.end(), status::ok );
	EXPECT_EQ( destination.bytes, stream_test_data );
	EXPECT_EQ( stream.get_position(), stream_test_data.size() );
	ASSERT_EQ( stream.get_digest().status(), status::ok );
	EXPECT_EQ( stream.get_digest().value(), from_string<digest<256>>( stream_test_sha256 ) );
}

TEST( data_stream, end_retries_only_the_unwritten_buffer_suffix )
{
	chunked_destination destination;
	destination.remaining_capacity = 1;
	write_stream<chunked_destination, hasher_sha256> stream( destination );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	ASSERT_EQ( stream.end(), status::cant_write );
	EXPECT_EQ( stream.get_digest().status(), status::not_ready );
	EXPECT_EQ( destination.bytes, std::vector<u8>( { 'a' } ) );
	EXPECT_EQ( stream.get_position(), stream_test_data.size() );

	destination.remaining_capacity = 2;
	ASSERT_EQ( stream.end(), status::ok );
	EXPECT_EQ( destination.bytes, stream_test_data );
	EXPECT_EQ( stream.get_digest().value(), from_string<digest<256>>( stream_test_sha256 ) );
}

TEST( data_stream, direct_write_progress_identifies_the_remaining_input )
{
	const std::vector<u8> input( 2 * 1024 * 1024 + 3, 'x' );
	chunked_destination destination;
	destination.remaining_capacity = 5;
	write_stream<chunked_destination, hasher_xxh128> stream( destination );
	ASSERT_EQ( stream.write_bytes( input.data(), input.size() ), status::cant_write );
	ASSERT_EQ( stream.get_position(), 5 );
	EXPECT_EQ( destination.bytes, std::vector<u8>( 5, 'x' ) );

	const size_t accepted = (size_t)stream.get_position();
	destination.remaining_capacity = input.size() - accepted;
	ASSERT_EQ( stream.write_bytes( input.data() + accepted, input.size() - accepted ), status::ok );
	ASSERT_EQ( stream.end(), status::ok );
	EXPECT_EQ( destination.bytes, input );
	EXPECT_EQ( stream.get_position(), input.size() );

	hasher_xxh128 expected;
	ASSERT_EQ( expected.update( input.data(), input.size() ), status::ok );
	EXPECT_EQ( stream.get_digest().value(), expected.finish().value() );
}

TEST( data_stream, finalization_caches_digest_and_rejects_later_writes )
{
	chunked_destination destination;
	write_stream<chunked_destination, hasher_2x_xxh128_dcb7be9cd0fcf505> stream( destination );
	EXPECT_EQ( stream.get_digest().status(), status::not_ready );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	EXPECT_EQ( stream.get_digest().status(), status::not_ready );
	ASSERT_EQ( stream.end(), status::ok );
	const auto digest = stream.get_digest();
	ASSERT_EQ( digest.status(), status::ok );

	ASSERT_EQ( stream.end(), status::ok );
	EXPECT_EQ( stream.get_digest().value(), digest.value() );
	EXPECT_EQ( stream.write<u8>( 'x' ), status::not_ready );
	EXPECT_EQ( stream.write_bytes( nullptr, 0 ), status::not_ready );
	EXPECT_EQ( stream.get_position(), stream_test_data.size() );
	EXPECT_EQ( destination.bytes, stream_test_data );
}

TEST( data_stream, uncertain_destination_error_is_never_replayed )
{
	uncertain_destination destination;
	{
		write_stream<uncertain_destination, hasher_sha256> stream( destination );
		ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
		EXPECT_EQ( stream.end(), status::cant_write );
		EXPECT_EQ( stream.end(), status::cant_write );
		EXPECT_EQ( stream.write<u8>( 'x' ), status::cant_write );
		EXPECT_EQ( stream.get_digest().status(), status::cant_write );
	}
	EXPECT_EQ( destination.bytes, std::vector<u8>( { 'a' } ) );
}

TEST( data_stream, hash_update_failure_preserves_progress_without_replaying )
{
	for( const size_t size : { size_t(3), size_t(2 * 1024 * 1024) } )
	{
		const std::vector<u8> input( size, 'x' );
		chunked_destination destination;
		{
			write_stream<chunked_destination, failing_update_hasher> stream( destination );
			const auto result = stream.write_bytes( input.data(), input.size() );
			EXPECT_EQ( result, size == 3 ? status::ok : status::corrupted );
			EXPECT_EQ( stream.end(), status::corrupted );
			EXPECT_EQ( stream.end(), status::corrupted );
			EXPECT_EQ( stream.get_digest().status(), status::corrupted );
			EXPECT_EQ( stream.get_position(), input.size() );
		}
		EXPECT_EQ( destination.bytes, input );
	}
}

TEST( data_stream, hash_finalization_failure_is_not_retried )
{
	chunked_destination destination;
	write_stream<chunked_destination, failing_finish_hasher> stream( destination );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	EXPECT_EQ( stream.end(), status::corrupted );
	EXPECT_EQ( stream.end(), status::corrupted );
	EXPECT_EQ( stream.get_digest().status(), status::corrupted );
	EXPECT_EQ( stream.write<u8>( 'x' ), status::corrupted );
	EXPECT_EQ( destination.bytes, stream_test_data );
}

TEST( data_stream, destination_exception_leaves_the_stream_failed )
{
	throwing_destination destination;
	{
		write_stream<throwing_destination> stream( destination );
		ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
		EXPECT_THROW( stream.end(), std::runtime_error );
		destination.throw_on_write = false;
		EXPECT_EQ( stream.end(), status::undefined_error );
		EXPECT_EQ( stream.get_digest().status(), status::undefined_error );
	}
	EXPECT_EQ( destination.bytes, std::vector<u8>( { 'a' } ) );
}

TEST( data_stream, impossible_write_count_is_a_terminal_error )
{
	overreporting_destination destination;
	write_stream<overreporting_destination> stream( destination );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	EXPECT_EQ( stream.end(), status::corrupted );
	EXPECT_EQ( stream.end(), status::corrupted );
	EXPECT_EQ( stream.get_digest().status(), status::corrupted );
}

TEST( data_stream, direct_write_exception_keeps_confirmed_position )
{
	const std::vector<u8> input( 2 * 1024 * 1024, 'x' );
	throwing_destination destination;
	destination.successful_writes = 1;
	write_stream<throwing_destination> stream( destination );
	EXPECT_THROW( stream.write_bytes( input.data(), input.size() ), std::runtime_error );
	EXPECT_EQ( stream.get_position(), 1 );
	EXPECT_EQ( stream.end(), status::undefined_error );
}

TEST( data_stream, finalization_exception_leaves_the_stream_failed )
{
	chunked_destination destination;
	write_stream<chunked_destination, throwing_finish_hasher> stream( destination );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	EXPECT_THROW( stream.end(), std::runtime_error );
	EXPECT_EQ( stream.end(), status::undefined_error );
	EXPECT_EQ( stream.get_digest().status(), status::undefined_error );
}

TEST( data_stream, destruction_suppresses_destination_exceptions )
{
	throwing_destination destination;
	{
		write_stream<throwing_destination> stream( destination );
		ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	}
	EXPECT_EQ( destination.bytes, std::vector<u8>( { 'a' } ) );
}

TEST( data_stream, failed_buffer_drain_does_not_accept_new_input )
{
	const std::vector<u8> input( 2 * 1024 * 1024, 'x' );
	chunked_destination destination;
	destination.remaining_capacity = 1;
	write_stream<chunked_destination, hasher_xxh128> stream( destination );
	ASSERT_EQ( stream.write_bytes( stream_test_data.data(), stream_test_data.size() ), status::ok );
	ASSERT_EQ( stream.write_bytes( input.data(), input.size() ), status::cant_write );
	EXPECT_EQ( stream.get_position(), stream_test_data.size() );
	EXPECT_EQ( destination.bytes, std::vector<u8>( { 'a' } ) );

	destination.remaining_capacity = input.size() + 2;
	ASSERT_EQ( stream.write_bytes( input.data(), input.size() ), status::ok );
	ASSERT_EQ( stream.end(), status::ok );
	std::vector<u8> expected = stream_test_data;
	expected.insert( expected.end(), input.begin(), input.end() );
	EXPECT_EQ( destination.bytes, expected );
	EXPECT_EQ( stream.get_position(), expected.size() );
}

TEST( data_stream, empty_stream_finalizes_once )
{
	chunked_destination destination;
	write_stream<chunked_destination, hasher_sha256> stream( destination );
	ASSERT_EQ( stream.end(), status::ok );
	const auto expected = from_string<digest<256>>(
		"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" );
	EXPECT_EQ( stream.get_digest().value(), expected );
	ASSERT_EQ( stream.end(), status::ok );
	EXPECT_EQ( stream.get_digest().value(), expected );
	EXPECT_TRUE( destination.bytes.empty() );
	EXPECT_EQ( stream.get_position(), 0 );
}

TEST( data_stream, basic_test )
{
	// set up a list of random values
	const size_t value_list_size = 200;
	std::vector<std::unique_ptr<variant>> value_list(value_list_size);
	std::vector<size_t> positions(value_list_size);
	for( size_t inx=0; inx<value_list_size; ++inx )
	{
		value_list[inx] = random_variant();
		value_list[inx]->random();
	}

	// write the data to a file
	digest<128> digest1;
	if( true )
	{
		file_data_destination dd("./data_stream_basic_test.dat");
		write_stream<file_data_destination,hasher_xxh128> ws(dd);
		for( size_t inx=0; inx<value_list_size; ++inx )
		{
			auto res = value_list[inx]->write_to_stream( ws );
			ASSERT_EQ( res, status::ok );
			positions[inx] = ws.get_position();
		}
		ASSERT_EQ( ws.end(), status::ok );
		digest1 = ws.get_digest().value();
	}

	// read the data back to a second list
	digest<128> digest2;
	std::vector<std::unique_ptr<variant>> value_list2(value_list_size);
	std::vector<size_t> positions2(value_list_size);
	if( true )
	{
		file_data_source ds("./data_stream_basic_test.dat");
		read_stream<file_data_source,hasher_xxh128> rs(ds);

		for( size_t inx=0; inx<value_list_size; ++inx )
		{
			value_list2[inx] = new_variant( value_list[inx]->type() );
			auto res = value_list2[inx]->read_from_stream( rs );
			ASSERT_EQ( res, status::ok );
			positions2[inx] = rs.get_position();
		}
		EXPECT_TRUE( rs.has_ended() );
		digest2 = rs.get_digest().value();
	}

	// make sure the lists agree
	for( size_t inx=0; inx<value_list_size; ++inx )
	{
		EXPECT_TRUE( value_list[inx]->is_equal( *(value_list2[inx].get()) ) );
		EXPECT_EQ( positions[inx], positions2[inx] );
	}

}
