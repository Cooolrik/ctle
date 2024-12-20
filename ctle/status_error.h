// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_STATUS_ERROR_H_
#define _CTLE_STATUS_ERROR_H_

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
#endif//_CTLE_STATUS_ERROR_H_
