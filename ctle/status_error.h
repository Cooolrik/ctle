// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include "status.h"
#include <stdexcept>

namespace ctle
{
	class status_error : public std::runtime_error
	{
		public:
			status value;
			explicit status_error( status _value, char const* const _Message = "" ) noexcept : std::runtime_error(_Message), value(_value) {}
	};				  
}
// namespace ctle