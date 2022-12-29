// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <sstream>
#include <iostream>
#include <unordered_map>

#ifdef CTLE_IMPLEMENT_EXAMPLE_LOG_MACROS

#define ctle_log( msg_level )\
	if( ctle::log_level::##msg_level <= ctle::get_global_log_level() ) {\
		ctle::log_msg _ctle_log_entry(ctle::log_level::##msg_level,__FILE__,__LINE__,__FUNCSIG__); _ctle_log_entry.message()

#define ctle_log_error ctle_log( error )
#define ctle_log_warning ctle_log( warning )
#define ctle_log_info ctle_log( info )
#define ctle_log_debug ctle_log( debug )
#define ctle_log_verbose ctle_log( verbose )

#define ctle_log_end ""; }

#endif//CTLE_IMPLEMENT_EXAMPLE_LOG_MACROS

namespace ctle
	{
	enum class log_level : unsigned int
		{
		error = 0,		// log errors
		warning = 1,	// log warnings
		info = 2,		// log important info, (default level)
		debug = 3,		// log debug messages 
		verbose = 4		// log extra verbose debug info 
		};

	// set/get the global log level, used to filter logs
	void set_global_log_level( log_level level );
	log_level get_global_log_level();

	// get the log_level value as a string
	const char *get_log_level_as_string( log_level level );

	// log a message when the object goes out of scope
	class log_msg
		{
		private:
			log_level level;
			std::stringstream msg;
			const char *func_sig;
			const char *file_name;
			int file_line;

			// enters the message into the log
			void enter()
				{
				std::cout << get_log_level_as_string( this->level ) << " log: " << (func_sig?func_sig:"Global scope") << ": " << this->msg.str() << std::endl;
				}

		public:
			log_msg( log_level _level , const char *_file_name = nullptr , int _file_line = 0, const char *_func_sig = nullptr ) noexcept : 
				level( _level ) , file_name( _file_name ) , file_line( _file_line ) , func_sig( _func_sig )
				{}
			~log_msg()
				{
				this->enter();
				}

			// access the message member to construct the message
			std::stringstream &message() { return msg; }
		};

#ifdef CTLE_IMPLEMENTATION
	static log_level global_log_level = log_level::info;

	void set_global_log_level( log_level level ) { global_log_level = level; }
	log_level get_global_log_level() { return global_log_level; }

	static const std::unordered_map<log_level, const char *> log_level_to_string
		{
			{ log_level::error , "Error" },
			{ log_level::warning , "Warning" },
			{ log_level::info , "Info" },
			{ log_level::debug , "Debug" },
			{ log_level::verbose , "Verbose" }
		};

	const char *get_log_level_as_string( log_level level )
		{
		std::unordered_map<log_level, const char *>::const_iterator it = log_level_to_string.find( level );
		if( it == log_level_to_string.end() )
			return nullptr;
		return it->second;
		}
#endif//CTLE_IMPLEMENTATION
	}