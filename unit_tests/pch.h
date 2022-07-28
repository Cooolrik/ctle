#ifndef PCH_H
#define PCH_H

#include <Windows.h>
#include <process.h>
#include <stddef.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#include "../idx_vector.h"
#include "../optional_idx_vector.h"
#include "../optional_value.h"
#include "../optional_vector.h"

#include "../thread_safe_map.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

template<class T> T random_value();

#define AssertExpectException( expected_exception , statement ) Assert::ExpectException<expected_exception>( [&]() { statement } );

#define STANDARD_TEST_INIT() \
	TEST_METHOD_INITIALIZE( InitMethod )\
		{\
		setup_random_seed();\
		}




#endif //PCH_H
