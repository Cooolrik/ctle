// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_LOG_SYSTEM_H_
#define _CTLE_LOG_SYSTEM_H_

/**
 * @file log_system.h
 * @brief 
 */

#include "fwd.h"
#include "log.h"
#include "status.h"

namespace ctle
{

struct log_system_msg
{
	log_level level = {};
	
	uint64_t timestamp = 0;
	uint process_id = 0;
	uint thread_id = 0;

	std::string function_name;
	uint file_line = 0;

	std::string message;
};

/// @brief
class log_system
{
private:
	log_function previous_log_function = nullptr;

public:
	log_system();
	~log_system();
};

}
// namespace ctle

/*
#ifdef CTLE_IMPLEMENTATION

#include <unordered_set>

namespace ctle
{

class thread_log_queue;

static std::mutex log_system_mutex;
static std::coun
static std::unordered_set<thread_log_queue*> log_system_thread_queues;

class thread_log_queue
{
	std::vector<std::unique_ptr<log_system_msg>> messages;
	std::vector<std::unique_ptr<log_system_msg>> flushed_messages;
	std::atomic<bool> is_flushing = false;

public:
	thread_log_queue();
	~thread_log_queue();

	// add a log message to the queue
	void log_message( std::unique_ptr<log_system_msg> msg )
	{
		messages.push_back( std::move( msg ) );
		if( messages.size() >= 100 )
			this->flush_messages();
	}

	// flush the log messages to the flushed_messages vector
	void flush_messages()
	{
		// make sure that a previous flush is not ongoing, and if so, just return
		if( this->is_flushing.load(std::memory_order_acquire) ) 
	        return;
	
		// no flush ongoing, so swap buffer and start the flush
		this->messages.swap(this->flushed_messages);
		this->is_flushing.store(true, std::memory_order_release);
	}




};

thread_log_queue::thread_log_queue()
{
	// register this thread log queue
	std::lock_guard<std::mutex> lock( log_system_mutex );
	log_system_thread_queues.insert( this );
}

thread_log_queue::~thread_log_queue()
{
	// unregister this thread log queue
	std::lock_guard<std::mutex> lock( log_system_mutex );
	log_system_thread_queues.erase( this );

	// flush any remaining messages




}

// this gets automatically created per thread
static thread_local std::unique_ptr<thread_log_queue> log_queue_ = std::make_unique<log_queue>();

static void log_system_logger( log_level level, const char *function_name, const char *message )
{
	auto log_msg = std::make_unique<log_system_msg>();
	log_msg->level = level;
	if( function_name )
		log_msg->function_name = std::string(function_name);
	if( message )
		log_msg->message = std::string(message);




}

log_system::log_system()
{
	// replace the global log function with our own
	this->previous_log_function = get_global_log_function();
	set_global_log_function( log_system_logger );
}

log_system::~log_system()
{
	// restore the previous log function
	set_global_log_function( this->previous_log_function );
}

}
// namespace ctle

#endif//CTLE_IMPLEMENTATION
*/

#endif//_CTLE_LOG_SYSTEM_H_
