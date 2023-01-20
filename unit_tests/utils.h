#pragma once

#ifdef _WINDOWS_
	#include <Windows.h>
#endif


#include <cinttypes>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

template<class T> T random_value();

#define AssertExpectException( expected_exception , statement ) \
Assert::ExpectException<expected_exception>( [&]() { statement } );

#define STANDARD_TEST_INIT() \
	TEST_METHOD_INITIALIZE( InitMethod )\
		{\
		setup_random_seed();\
		}