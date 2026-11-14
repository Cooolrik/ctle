// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_UTIL_H_
#define _CTLE_UTIL_H_

#include <type_traits>

namespace ctle
{

/// @brief assign a value to a variable if the variable is trivially default constructible
/// @details identity_assign_if_trivially_default_constructible is a conditional template function which:
///  - Initializes trivially constructable types by using the = {} assignment. 
///  - For all other types, the template is a noop and does nothing. 
/// This is to avoid applying the = {} where there is a complex default constructor, with e.g. default parameters.
template<typename _Ty, typename std::enable_if<std::is_trivially_default_constructible<_Ty>{},bool>::type = true> void identity_assign_if_trivially_default_constructible( _Ty &val ) { val = {}; }
template<typename _Ty, typename std::enable_if<!std::is_trivially_default_constructible<_Ty>{},bool>::type = true> void identity_assign_if_trivially_default_constructible( _Ty & ) { /*noop*/ }

}
//namespace ctle
#endif//_CTLE_UTIL_H_
