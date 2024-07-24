// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#ifndef PCH_H
#define PCH_H

#include <cstdint>

#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

// include external headers
// silence warning we can't control
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4456 )
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#endif

#include <gtest/gtest.h>
#include <picosha2.h>
#include <xxhash.h>
#include <functional>

// re-enable warnings again
#ifdef _MSC_VER
#pragma warning( pop )
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

template<class T> T random_value();

#define STANDARD_TEST_INIT() \
	TEST_METHOD_INITIALIZE( InitMethod )\
		{\
		setup_random_seed();\
		}

// runs a function with a timeout. 
// waits for the function to return true within the timeout in milliseconds, else returns false
template <typename Func>
bool run_function_with_timeout(Func func, long long timeout_ms) 
{
    const auto start = std::chrono::high_resolution_clock::now();
    bool result = false;

    while( true )
    {
        result = func();
        if( result )
            return true;

        const auto now = std::chrono::high_resolution_clock::now();
        const auto current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        if(current_ms >= timeout_ms) 
            return false;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

#endif //PCH_H
