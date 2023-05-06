// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#ifndef PCH_H
#define PCH_H

#ifdef WIN32
#include "Windows.h"
#endif//WIN32

#include <cstdint>

#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include <gtest/gtest.h>

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

#endif //PCH_H
