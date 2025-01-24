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

/// @brief nil object with a static allocation
/// @details nil_object is a static class with an allocated object, which can be 
/// used to point to or reference an invalid object, when nullptr is not applicable or allowed.
class nil_object
{
	static void* _ptr() noexcept;
	static bool _is_nil( const void *ptr ) noexcept;

public:
	// get a nil ptr and compare ptr to nil ptr
	template<class _Ty = void> static _Ty *ptr() noexcept { return (_Ty*)nil_object::_ptr(); }
	template<class _Ty> static bool is_nil(_Ty* ptr) noexcept { return nil_object::_is_nil(ptr); }
	template<class _Ty> static bool is_nil(const _Ty* ptr) noexcept { return nil_object::_is_nil(ptr); }
			 
	// get a nil ref and compare ref to nil ref
	template<class _Ty> static _Ty &ref() noexcept { return *((_Ty*)nil_object::_ptr()); }
	template<class _Ty> static bool is_nil( _Ty &ref ) noexcept { return nil_object::_is_nil(&ref); }
	template<class _Ty> static bool is_nil( const _Ty &ref ) noexcept { return nil_object::_is_nil(&ref); }
};

#ifdef CTLE_IMPLEMENTATION

static int64_t nil_object_mem;

void* nil_object::_ptr() noexcept
{
	return &nil_object_mem;
}

bool nil_object::_is_nil( const void *ptr ) noexcept
{
	return &nil_object_mem == ptr;
}

#endif//CTLE_IMPLEMENTATION

}
//namespace ctle
#endif//_CTLE_UTIL_H_
