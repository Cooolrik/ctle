// ctle Copyright (c) 2026 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_PROCESS_WINDOWS_ARGUMENTS_INL_
#define _CTLE_PROCESS_WINDOWS_ARGUMENTS_INL_

#include <string>
#include <vector>

namespace ctle
{
namespace detail
{
	// Requires validated arguments and an executable path without double quotes.
	inline std::string windows_command_line( const std::vector<std::string> &arguments )
	{
		// The CRT parses argv[0] as a quoted path, without the later arguments' escape rules.
		std::string command = "\"" + arguments.front() + "\"";
		for( size_t index = 1; index < arguments.size(); ++index )
		{
			command += " \"";
			size_t backslashes = 0;
			for( const char character : arguments[index] )
			{
				if( character == '\\' )
				{
					++backslashes;
					continue;
				}
				// Before a literal quote, encode each backslash twice and escape the quote.
				command.append( character == '"' ? 2 * backslashes + 1 : backslashes, '\\' );
				command += character;
				backslashes = 0;
			}
			// Trailing backslashes must not escape the closing delimiter.
			command.append( 2 * backslashes, '\\' );
			command += '"';
		}
		return command;
	}
}
// namespace detail
}
// namespace ctle

#endif//_CTLE_PROCESS_WINDOWS_ARGUMENTS_INL_
