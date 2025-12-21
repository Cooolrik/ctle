// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_LOG_SYSTEM_H_
#define _CTLE_LOG_SYSTEM_H_

#include <thread>
#include <vector>

/// @file log_system.h
/// @brief 

#include "fwd.h"
#include "log.h"
#include "status.h"

namespace ctle
{
namespace log_system
{

struct message
{
	log_level level = {};
	
	uint64_t timestamp = 0;
	uint process_id = 0;
	uint thread_id = 0;

	const char *function_name;
	const char *file_name;
	uint file_line = 0;

	std::string text;
};

/// @brief
class handler
{
private:
	std::thread flush_thread;
	void flush_thread_func();

	// if set, messages from different threads are sorted each flush
	bool sort_messages = false;

	// log message options
	bool add_level = true;
	bool add_timestamp = true;
	bool add_process_id = true;
	bool add_thread_id = true;
	bool add_function_name = true;
	bool add_file_name = true;
	bool add_file_line = true;
		
	// if writing to file
	std::unique_ptr<_file_object> log_file;

	// format a log message into a string based on the current decoration flags
	std::string format_message( const message &msg ) const;

public:
	handler();
	~handler();
};

}
// namespace log_system
}
// namespace ctle

#ifdef CTLE_IMPLEMENTATION

#include <unordered_set>

namespace ctle
{
namespace log_system
{

class thread_log_queue;

// global data, protected by global_mutex
static std::mutex global_mutex;
static std::unordered_set<thread_log_queue*> thread_queues;
static std::vector<std::unique_ptr<message>> spillover_messages;
static size_t total_slow_path_messages = 0; 

// set at initialization/deinitialization
static log_function previous_log_function = nullptr;
static handler *global_message_sink = nullptr;
static std::atomic<bool> is_running = false;

#ifdef CTLE_LOG_SYSTEM_OVERRIDE_CAPACITY
static inline constexpr const size_t thread_log_queue_capacity = CTLE_LOG_SYSTEM_OVERRIDE_CAPACITY;
#else
static inline constexpr const size_t thread_log_queue_capacity = 32;
#endif

/// @brief per-thread log queue, which collects log messages for the thread
class thread_log_queue
{
	std::array<std::unique_ptr<message>, thread_log_queue_capacity> msg_buffer; // ring buffer for log messages
	std::atomic<u32> write_inx = 0; // index for writing new messages
	std::atomic<u32> flush_inx = 0; // index for flushing messages

public:
	thread_log_queue();
	~thread_log_queue();

	// add a log message to the queue
	void log_message( std::unique_ptr<message> msg )
	{
		// fetch the current write and flush indices in the ring buffer
		const u32 w_inx = this->write_inx.load(std::memory_order_relaxed);
		const u32 f_inx = this->flush_inx.load(std::memory_order_acquire);

		// calculate the next write index, wrapping around the buffer size
		const u32 n_inx = (w_inx + 1) % (u32)this->msg_buffer.size();

		// check if the next write index would collide with the flush index (buffer full)
		// and if so, use the slower & global spillover buffer
		if( n_inx == f_inx )
		{
			std::lock_guard<std::mutex> lock( global_mutex );
			spillover_messages.push_back( std::move( msg ) );
			++total_slow_path_messages;
			return;
		}

		// fast path: there is space in the ring buffer, so store the message
		this->msg_buffer[w_inx] = std::move( msg );
		this->write_inx.store(n_inx, std::memory_order_release);
	}

	// flush all available log messages to the dest vector
	bool flush_messages( std::vector<std::unique_ptr<message>> &dest )
	{
		const u32 w_inx = this->write_inx.load(std::memory_order_acquire);
		u32 f_inx = this->flush_inx.load(std::memory_order_relaxed);
		if( f_inx == w_inx )
			return false; // no messages to flush

		// calculate total messages to flush, accounting for wrap-around in the ring buffer
		const u32 total_msgs = (w_inx >= f_inx) ? (w_inx - f_inx) : ((u32)this->msg_buffer.size() - f_inx + w_inx);

		// reserve space in the dest vector, if needed
		if( dest.capacity() < (dest.size() + total_msgs) )
			dest.reserve( dest.size() + total_msgs );

		// move messages from the ring buffer to the dest vector
		while( f_inx != w_inx )
		{
			dest.push_back( std::move( this->msg_buffer[f_inx] ) );
			f_inx = (f_inx + 1) % (u32)this->msg_buffer.size();
		}

		// update the flush index to the new position
	    this->flush_inx.store(f_inx, std::memory_order_release);
		return true;
	}

};

thread_log_queue::thread_log_queue()
{
	std::lock_guard<std::mutex> lock( global_mutex );

	// register this thread log queue
	thread_queues.insert( this );
}

thread_log_queue::~thread_log_queue()
{
	std::lock_guard<std::mutex> lock( global_mutex );

	// unregister this thread log queue
	thread_queues.erase( this );

	// flush any remaining messages in message ring buffer to the spillover queue
	std::vector<std::unique_ptr<message>> flush;
	if( this->flush_messages(flush) )
	{
		for( auto &msg : flush )
			spillover_messages.push_back( std::move( msg ) );
	}
}

// per-thread log queue instance
static thread_local std::unique_ptr<thread_log_queue> log_queue_ = std::make_unique<thread_log_queue>();

static void log_system_logger( log_level level, const char *function_name, const char *text )
{
	auto log_msg = std::make_unique<message>();
	log_msg->level = level;
	log_msg->function_name = function_name;
	if( text )
		log_msg->text = std::string(text);

	log_queue_->log_message( std::move( log_msg ) );
}

// thread which periodically flushes log messages from all thread queues to the sink
void handler::flush_thread_func()
{
	std::vector<std::unique_ptr<message>> flush_msgs;

	do 
	{
		std::lock_guard<std::mutex> lock( global_mutex );

		// clear previous flush messages
		flush_msgs.clear();

		// fetch messages from all thread queues
		for( auto thread_queue : thread_queues )
			thread_queue->flush_messages( flush_msgs );
						
		// also collect spillover messages, if any
		if( !spillover_messages.empty() )
		{
			for( auto &msg : spillover_messages )
				flush_msgs.push_back( std::move( msg ) );
			spillover_messages.clear();
		}

		// sort messages by timestamp using stable sort to maintain order of same-timestamp messages
		std::stable_sort( flush_msgs.begin(), flush_msgs.end(), []( const std::unique_ptr<message> &a, const std::unique_ptr<message> &b )
			{
				return a->timestamp < b->timestamp;
			}
		);


		// sleep for a short duration before next flush
		std::this_thread::sleep_for( std::chrono::milliseconds(100) );
	} while( is_running.load() );
}

//std::string handler::format_message( const message &msg ) const
//{
//	std::string out;
//
//	if( this->decoration_flags & message_decoration_flags::)
//
//}

handler::handler()
{
	if( global_message_sink != nullptr )
	{
		throw std::runtime_error( "A log_system handler is already registered, please remove the previous one before adding a new" );
		return;
	}
	global_message_sink = this;

	// replace the global log function with our own
	previous_log_function = get_global_log_function();
	set_global_log_function( log_system_logger );

	// start the flush thread
	is_running.store( true );
	this->flush_thread = std::thread( &handler::flush_thread_func , this );
}

handler::~handler()
{
	// this should never happen, since it should be blocked in the ctor, but check anyway
	if( global_message_sink != this )
		return;

	// restore the previous log function
	set_global_log_function( previous_log_function );
	previous_log_function = nullptr;

	// stop the flush thread
	is_running.store( false );
	if( this->flush_thread.joinable() )
		this->flush_thread.join();

	global_message_sink = nullptr;
}

//status handler::get_messages( std::vector<std::unique_ptr<message>> &out_messages )
//{
//	std::lock_guard<std::mutex> lock( global_mutex );
//
//	out_messages.clear();
//	out_messages.swap( this->flush_msgs );
//
//	return status::ok;
//}

}
// namespace log_system
}
// namespace ctle

#endif//CTLE_IMPLEMENTATION

#endif//_CTLE_LOG_SYSTEM_H_
