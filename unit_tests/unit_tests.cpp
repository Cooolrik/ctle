// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#define CTLE_IMPLEMENTATION

#include "unit_tests.h"

#include "../ctle/ctle.h"

// just some value
const i64 global_random_seed = 12876123876;

inline void setup_random_seed()
{
	i64 seed = (global_random_seed == -1) ? i64( time( nullptr ) ) : global_random_seed;
	srand(u32( seed & 0xffffffff ));
}

// add headers that you want to pre-compile here
inline u8 u8_rand() { return (u8)(rand() & 0xff); } 
inline u16 u16_rand() { return u16(rand() << 4) ^ u16(rand()); } 
inline u32 u32_rand() { return u32(u16_rand()) << 16 | u32(u16_rand()); } 
inline u64 u64_rand() { return u64(u32_rand()) << 32 | u64(u32_rand()); } 

template<> bool random_value<bool>() { return (bool)(rand() & 0x1); } 

template<> i8 random_value<i8>() { return u8_rand(); }
template<> i16 random_value<i16>() { return u16_rand(); }
template<> i32 random_value<i32>() { return u32_rand(); }
template<> i64 random_value<i64>() { return u64_rand(); }

template<> u8 random_value<u8>() { return u8_rand(); }
template<> u16 random_value<u16>() { return u16_rand(); }
template<> u32 random_value<u32>() { return u32_rand(); }
template<> u64 random_value<u64>() { return u64_rand(); }

template<> double random_value<double>() { return (double)((i64)u64_rand()) / ((double)u32_rand() + (double)0xffffffff); }
template<> float random_value<float>() { return (float)u64_rand() ; }

template<> i8tup1 random_value<i8tup1>() { return i8tup1(random_value<i8>()); }
template<> i8tup2 random_value<i8tup2>() { return i8tup2(random_value<i8>(), random_value<i8>()); }
template<> i8tup3 random_value<i8tup3>() { return i8tup3(random_value<i8>(), random_value<i8>(), random_value<i8>()); }
template<> i8tup4 random_value<i8tup4>() { return i8tup4(random_value<i8>(), random_value<i8>(), random_value<i8>(), random_value<i8>()); }
template<> i8tup1x1 random_value<i8tup1x1>() { return i8tup1x1(random_value<i8tup1>()); }
template<> i8tup1x2 random_value<i8tup1x2>() { return i8tup1x2(random_value<i8tup1>(), random_value<i8tup1>()); }
template<> i8tup1x3 random_value<i8tup1x3>() { return i8tup1x3(random_value<i8tup1>(), random_value<i8tup1>(), random_value<i8tup1>()); }
template<> i8tup1x4 random_value<i8tup1x4>() { return i8tup1x4(random_value<i8tup1>(), random_value<i8tup1>(), random_value<i8tup1>(), random_value<i8tup1>()); }
template<> i8tup2x1 random_value<i8tup2x1>() { return i8tup2x1(random_value<i8tup2>()); }
template<> i8tup2x2 random_value<i8tup2x2>() { return i8tup2x2(random_value<i8tup2>(), random_value<i8tup2>()); }
template<> i8tup2x3 random_value<i8tup2x3>() { return i8tup2x3(random_value<i8tup2>(), random_value<i8tup2>(), random_value<i8tup2>()); }
template<> i8tup2x4 random_value<i8tup2x4>() { return i8tup2x4(random_value<i8tup2>(), random_value<i8tup2>(), random_value<i8tup2>(), random_value<i8tup2>()); }
template<> i8tup3x1 random_value<i8tup3x1>() { return i8tup3x1(random_value<i8tup3>()); }
template<> i8tup3x2 random_value<i8tup3x2>() { return i8tup3x2(random_value<i8tup3>(), random_value<i8tup3>()); }
template<> i8tup3x3 random_value<i8tup3x3>() { return i8tup3x3(random_value<i8tup3>(), random_value<i8tup3>(), random_value<i8tup3>()); }
template<> i8tup3x4 random_value<i8tup3x4>() { return i8tup3x4(random_value<i8tup3>(), random_value<i8tup3>(), random_value<i8tup3>(), random_value<i8tup3>()); }
template<> i8tup4x1 random_value<i8tup4x1>() { return i8tup4x1(random_value<i8tup4>()); }
template<> i8tup4x2 random_value<i8tup4x2>() { return i8tup4x2(random_value<i8tup4>(), random_value<i8tup4>()); }
template<> i8tup4x3 random_value<i8tup4x3>() { return i8tup4x3(random_value<i8tup4>(), random_value<i8tup4>(), random_value<i8tup4>()); }
template<> i8tup4x4 random_value<i8tup4x4>() { return i8tup4x4(random_value<i8tup4>(), random_value<i8tup4>(), random_value<i8tup4>(), random_value<i8tup4>()); }
template<> u8tup1 random_value<u8tup1>() { return u8tup1(random_value<u8>()); }
template<> u8tup2 random_value<u8tup2>() { return u8tup2(random_value<u8>(), random_value<u8>()); }
template<> u8tup3 random_value<u8tup3>() { return u8tup3(random_value<u8>(), random_value<u8>(), random_value<u8>()); }
template<> u8tup4 random_value<u8tup4>() { return u8tup4(random_value<u8>(), random_value<u8>(), random_value<u8>(), random_value<u8>()); }
template<> u8tup1x1 random_value<u8tup1x1>() { return u8tup1x1(random_value<u8tup1>()); }
template<> u8tup1x2 random_value<u8tup1x2>() { return u8tup1x2(random_value<u8tup1>(), random_value<u8tup1>()); }
template<> u8tup1x3 random_value<u8tup1x3>() { return u8tup1x3(random_value<u8tup1>(), random_value<u8tup1>(), random_value<u8tup1>()); }
template<> u8tup1x4 random_value<u8tup1x4>() { return u8tup1x4(random_value<u8tup1>(), random_value<u8tup1>(), random_value<u8tup1>(), random_value<u8tup1>()); }
template<> u8tup2x1 random_value<u8tup2x1>() { return u8tup2x1(random_value<u8tup2>()); }
template<> u8tup2x2 random_value<u8tup2x2>() { return u8tup2x2(random_value<u8tup2>(), random_value<u8tup2>()); }
template<> u8tup2x3 random_value<u8tup2x3>() { return u8tup2x3(random_value<u8tup2>(), random_value<u8tup2>(), random_value<u8tup2>()); }
template<> u8tup2x4 random_value<u8tup2x4>() { return u8tup2x4(random_value<u8tup2>(), random_value<u8tup2>(), random_value<u8tup2>(), random_value<u8tup2>()); }
template<> u8tup3x1 random_value<u8tup3x1>() { return u8tup3x1(random_value<u8tup3>()); }
template<> u8tup3x2 random_value<u8tup3x2>() { return u8tup3x2(random_value<u8tup3>(), random_value<u8tup3>()); }
template<> u8tup3x3 random_value<u8tup3x3>() { return u8tup3x3(random_value<u8tup3>(), random_value<u8tup3>(), random_value<u8tup3>()); }
template<> u8tup3x4 random_value<u8tup3x4>() { return u8tup3x4(random_value<u8tup3>(), random_value<u8tup3>(), random_value<u8tup3>(), random_value<u8tup3>()); }
template<> u8tup4x1 random_value<u8tup4x1>() { return u8tup4x1(random_value<u8tup4>()); }
template<> u8tup4x2 random_value<u8tup4x2>() { return u8tup4x2(random_value<u8tup4>(), random_value<u8tup4>()); }
template<> u8tup4x3 random_value<u8tup4x3>() { return u8tup4x3(random_value<u8tup4>(), random_value<u8tup4>(), random_value<u8tup4>()); }
template<> u8tup4x4 random_value<u8tup4x4>() { return u8tup4x4(random_value<u8tup4>(), random_value<u8tup4>(), random_value<u8tup4>(), random_value<u8tup4>()); }
template<> i16tup1 random_value<i16tup1>() { return i16tup1(random_value<i16>()); }
template<> i16tup2 random_value<i16tup2>() { return i16tup2(random_value<i16>(), random_value<i16>()); }
template<> i16tup3 random_value<i16tup3>() { return i16tup3(random_value<i16>(), random_value<i16>(), random_value<i16>()); }
template<> i16tup4 random_value<i16tup4>() { return i16tup4(random_value<i16>(), random_value<i16>(), random_value<i16>(), random_value<i16>()); }
template<> i16tup1x1 random_value<i16tup1x1>() { return i16tup1x1(random_value<i16tup1>()); }
template<> i16tup1x2 random_value<i16tup1x2>() { return i16tup1x2(random_value<i16tup1>(), random_value<i16tup1>()); }
template<> i16tup1x3 random_value<i16tup1x3>() { return i16tup1x3(random_value<i16tup1>(), random_value<i16tup1>(), random_value<i16tup1>()); }
template<> i16tup1x4 random_value<i16tup1x4>() { return i16tup1x4(random_value<i16tup1>(), random_value<i16tup1>(), random_value<i16tup1>(), random_value<i16tup1>()); }
template<> i16tup2x1 random_value<i16tup2x1>() { return i16tup2x1(random_value<i16tup2>()); }
template<> i16tup2x2 random_value<i16tup2x2>() { return i16tup2x2(random_value<i16tup2>(), random_value<i16tup2>()); }
template<> i16tup2x3 random_value<i16tup2x3>() { return i16tup2x3(random_value<i16tup2>(), random_value<i16tup2>(), random_value<i16tup2>()); }
template<> i16tup2x4 random_value<i16tup2x4>() { return i16tup2x4(random_value<i16tup2>(), random_value<i16tup2>(), random_value<i16tup2>(), random_value<i16tup2>()); }
template<> i16tup3x1 random_value<i16tup3x1>() { return i16tup3x1(random_value<i16tup3>()); }
template<> i16tup3x2 random_value<i16tup3x2>() { return i16tup3x2(random_value<i16tup3>(), random_value<i16tup3>()); }
template<> i16tup3x3 random_value<i16tup3x3>() { return i16tup3x3(random_value<i16tup3>(), random_value<i16tup3>(), random_value<i16tup3>()); }
template<> i16tup3x4 random_value<i16tup3x4>() { return i16tup3x4(random_value<i16tup3>(), random_value<i16tup3>(), random_value<i16tup3>(), random_value<i16tup3>()); }
template<> i16tup4x1 random_value<i16tup4x1>() { return i16tup4x1(random_value<i16tup4>()); }
template<> i16tup4x2 random_value<i16tup4x2>() { return i16tup4x2(random_value<i16tup4>(), random_value<i16tup4>()); }
template<> i16tup4x3 random_value<i16tup4x3>() { return i16tup4x3(random_value<i16tup4>(), random_value<i16tup4>(), random_value<i16tup4>()); }
template<> i16tup4x4 random_value<i16tup4x4>() { return i16tup4x4(random_value<i16tup4>(), random_value<i16tup4>(), random_value<i16tup4>(), random_value<i16tup4>()); }
template<> u16tup1 random_value<u16tup1>() { return u16tup1(random_value<u16>()); }
template<> u16tup2 random_value<u16tup2>() { return u16tup2(random_value<u16>(), random_value<u16>()); }
template<> u16tup3 random_value<u16tup3>() { return u16tup3(random_value<u16>(), random_value<u16>(), random_value<u16>()); }
template<> u16tup4 random_value<u16tup4>() { return u16tup4(random_value<u16>(), random_value<u16>(), random_value<u16>(), random_value<u16>()); }
template<> u16tup1x1 random_value<u16tup1x1>() { return u16tup1x1(random_value<u16tup1>()); }
template<> u16tup1x2 random_value<u16tup1x2>() { return u16tup1x2(random_value<u16tup1>(), random_value<u16tup1>()); }
template<> u16tup1x3 random_value<u16tup1x3>() { return u16tup1x3(random_value<u16tup1>(), random_value<u16tup1>(), random_value<u16tup1>()); }
template<> u16tup1x4 random_value<u16tup1x4>() { return u16tup1x4(random_value<u16tup1>(), random_value<u16tup1>(), random_value<u16tup1>(), random_value<u16tup1>()); }
template<> u16tup2x1 random_value<u16tup2x1>() { return u16tup2x1(random_value<u16tup2>()); }
template<> u16tup2x2 random_value<u16tup2x2>() { return u16tup2x2(random_value<u16tup2>(), random_value<u16tup2>()); }
template<> u16tup2x3 random_value<u16tup2x3>() { return u16tup2x3(random_value<u16tup2>(), random_value<u16tup2>(), random_value<u16tup2>()); }
template<> u16tup2x4 random_value<u16tup2x4>() { return u16tup2x4(random_value<u16tup2>(), random_value<u16tup2>(), random_value<u16tup2>(), random_value<u16tup2>()); }
template<> u16tup3x1 random_value<u16tup3x1>() { return u16tup3x1(random_value<u16tup3>()); }
template<> u16tup3x2 random_value<u16tup3x2>() { return u16tup3x2(random_value<u16tup3>(), random_value<u16tup3>()); }
template<> u16tup3x3 random_value<u16tup3x3>() { return u16tup3x3(random_value<u16tup3>(), random_value<u16tup3>(), random_value<u16tup3>()); }
template<> u16tup3x4 random_value<u16tup3x4>() { return u16tup3x4(random_value<u16tup3>(), random_value<u16tup3>(), random_value<u16tup3>(), random_value<u16tup3>()); }
template<> u16tup4x1 random_value<u16tup4x1>() { return u16tup4x1(random_value<u16tup4>()); }
template<> u16tup4x2 random_value<u16tup4x2>() { return u16tup4x2(random_value<u16tup4>(), random_value<u16tup4>()); }
template<> u16tup4x3 random_value<u16tup4x3>() { return u16tup4x3(random_value<u16tup4>(), random_value<u16tup4>(), random_value<u16tup4>()); }
template<> u16tup4x4 random_value<u16tup4x4>() { return u16tup4x4(random_value<u16tup4>(), random_value<u16tup4>(), random_value<u16tup4>(), random_value<u16tup4>()); }
template<> i32tup1 random_value<i32tup1>() { return i32tup1(random_value<i32>()); }
template<> i32tup2 random_value<i32tup2>() { return i32tup2(random_value<i32>(), random_value<i32>()); }
template<> i32tup3 random_value<i32tup3>() { return i32tup3(random_value<i32>(), random_value<i32>(), random_value<i32>()); }
template<> i32tup4 random_value<i32tup4>() { return i32tup4(random_value<i32>(), random_value<i32>(), random_value<i32>(), random_value<i32>()); }
template<> i32tup1x1 random_value<i32tup1x1>() { return i32tup1x1(random_value<i32tup1>()); }
template<> i32tup1x2 random_value<i32tup1x2>() { return i32tup1x2(random_value<i32tup1>(), random_value<i32tup1>()); }
template<> i32tup1x3 random_value<i32tup1x3>() { return i32tup1x3(random_value<i32tup1>(), random_value<i32tup1>(), random_value<i32tup1>()); }
template<> i32tup1x4 random_value<i32tup1x4>() { return i32tup1x4(random_value<i32tup1>(), random_value<i32tup1>(), random_value<i32tup1>(), random_value<i32tup1>()); }
template<> i32tup2x1 random_value<i32tup2x1>() { return i32tup2x1(random_value<i32tup2>()); }
template<> i32tup2x2 random_value<i32tup2x2>() { return i32tup2x2(random_value<i32tup2>(), random_value<i32tup2>()); }
template<> i32tup2x3 random_value<i32tup2x3>() { return i32tup2x3(random_value<i32tup2>(), random_value<i32tup2>(), random_value<i32tup2>()); }
template<> i32tup2x4 random_value<i32tup2x4>() { return i32tup2x4(random_value<i32tup2>(), random_value<i32tup2>(), random_value<i32tup2>(), random_value<i32tup2>()); }
template<> i32tup3x1 random_value<i32tup3x1>() { return i32tup3x1(random_value<i32tup3>()); }
template<> i32tup3x2 random_value<i32tup3x2>() { return i32tup3x2(random_value<i32tup3>(), random_value<i32tup3>()); }
template<> i32tup3x3 random_value<i32tup3x3>() { return i32tup3x3(random_value<i32tup3>(), random_value<i32tup3>(), random_value<i32tup3>()); }
template<> i32tup3x4 random_value<i32tup3x4>() { return i32tup3x4(random_value<i32tup3>(), random_value<i32tup3>(), random_value<i32tup3>(), random_value<i32tup3>()); }
template<> i32tup4x1 random_value<i32tup4x1>() { return i32tup4x1(random_value<i32tup4>()); }
template<> i32tup4x2 random_value<i32tup4x2>() { return i32tup4x2(random_value<i32tup4>(), random_value<i32tup4>()); }
template<> i32tup4x3 random_value<i32tup4x3>() { return i32tup4x3(random_value<i32tup4>(), random_value<i32tup4>(), random_value<i32tup4>()); }
template<> i32tup4x4 random_value<i32tup4x4>() { return i32tup4x4(random_value<i32tup4>(), random_value<i32tup4>(), random_value<i32tup4>(), random_value<i32tup4>()); }
template<> u32tup1 random_value<u32tup1>() { return u32tup1(random_value<u32>()); }
template<> u32tup2 random_value<u32tup2>() { return u32tup2(random_value<u32>(), random_value<u32>()); }
template<> u32tup3 random_value<u32tup3>() { return u32tup3(random_value<u32>(), random_value<u32>(), random_value<u32>()); }
template<> u32tup4 random_value<u32tup4>() { return u32tup4(random_value<u32>(), random_value<u32>(), random_value<u32>(), random_value<u32>()); }
template<> u32tup1x1 random_value<u32tup1x1>() { return u32tup1x1(random_value<u32tup1>()); }
template<> u32tup1x2 random_value<u32tup1x2>() { return u32tup1x2(random_value<u32tup1>(), random_value<u32tup1>()); }
template<> u32tup1x3 random_value<u32tup1x3>() { return u32tup1x3(random_value<u32tup1>(), random_value<u32tup1>(), random_value<u32tup1>()); }
template<> u32tup1x4 random_value<u32tup1x4>() { return u32tup1x4(random_value<u32tup1>(), random_value<u32tup1>(), random_value<u32tup1>(), random_value<u32tup1>()); }
template<> u32tup2x1 random_value<u32tup2x1>() { return u32tup2x1(random_value<u32tup2>()); }
template<> u32tup2x2 random_value<u32tup2x2>() { return u32tup2x2(random_value<u32tup2>(), random_value<u32tup2>()); }
template<> u32tup2x3 random_value<u32tup2x3>() { return u32tup2x3(random_value<u32tup2>(), random_value<u32tup2>(), random_value<u32tup2>()); }
template<> u32tup2x4 random_value<u32tup2x4>() { return u32tup2x4(random_value<u32tup2>(), random_value<u32tup2>(), random_value<u32tup2>(), random_value<u32tup2>()); }
template<> u32tup3x1 random_value<u32tup3x1>() { return u32tup3x1(random_value<u32tup3>()); }
template<> u32tup3x2 random_value<u32tup3x2>() { return u32tup3x2(random_value<u32tup3>(), random_value<u32tup3>()); }
template<> u32tup3x3 random_value<u32tup3x3>() { return u32tup3x3(random_value<u32tup3>(), random_value<u32tup3>(), random_value<u32tup3>()); }
template<> u32tup3x4 random_value<u32tup3x4>() { return u32tup3x4(random_value<u32tup3>(), random_value<u32tup3>(), random_value<u32tup3>(), random_value<u32tup3>()); }
template<> u32tup4x1 random_value<u32tup4x1>() { return u32tup4x1(random_value<u32tup4>()); }
template<> u32tup4x2 random_value<u32tup4x2>() { return u32tup4x2(random_value<u32tup4>(), random_value<u32tup4>()); }
template<> u32tup4x3 random_value<u32tup4x3>() { return u32tup4x3(random_value<u32tup4>(), random_value<u32tup4>(), random_value<u32tup4>()); }
template<> u32tup4x4 random_value<u32tup4x4>() { return u32tup4x4(random_value<u32tup4>(), random_value<u32tup4>(), random_value<u32tup4>(), random_value<u32tup4>()); }
template<> i64tup1 random_value<i64tup1>() { return i64tup1(random_value<i64>()); }
template<> i64tup2 random_value<i64tup2>() { return i64tup2(random_value<i64>(), random_value<i64>()); }
template<> i64tup3 random_value<i64tup3>() { return i64tup3(random_value<i64>(), random_value<i64>(), random_value<i64>()); }
template<> i64tup4 random_value<i64tup4>() { return i64tup4(random_value<i64>(), random_value<i64>(), random_value<i64>(), random_value<i64>()); }
template<> i64tup1x1 random_value<i64tup1x1>() { return i64tup1x1(random_value<i64tup1>()); }
template<> i64tup1x2 random_value<i64tup1x2>() { return i64tup1x2(random_value<i64tup1>(), random_value<i64tup1>()); }
template<> i64tup1x3 random_value<i64tup1x3>() { return i64tup1x3(random_value<i64tup1>(), random_value<i64tup1>(), random_value<i64tup1>()); }
template<> i64tup1x4 random_value<i64tup1x4>() { return i64tup1x4(random_value<i64tup1>(), random_value<i64tup1>(), random_value<i64tup1>(), random_value<i64tup1>()); }
template<> i64tup2x1 random_value<i64tup2x1>() { return i64tup2x1(random_value<i64tup2>()); }
template<> i64tup2x2 random_value<i64tup2x2>() { return i64tup2x2(random_value<i64tup2>(), random_value<i64tup2>()); }
template<> i64tup2x3 random_value<i64tup2x3>() { return i64tup2x3(random_value<i64tup2>(), random_value<i64tup2>(), random_value<i64tup2>()); }
template<> i64tup2x4 random_value<i64tup2x4>() { return i64tup2x4(random_value<i64tup2>(), random_value<i64tup2>(), random_value<i64tup2>(), random_value<i64tup2>()); }
template<> i64tup3x1 random_value<i64tup3x1>() { return i64tup3x1(random_value<i64tup3>()); }
template<> i64tup3x2 random_value<i64tup3x2>() { return i64tup3x2(random_value<i64tup3>(), random_value<i64tup3>()); }
template<> i64tup3x3 random_value<i64tup3x3>() { return i64tup3x3(random_value<i64tup3>(), random_value<i64tup3>(), random_value<i64tup3>()); }
template<> i64tup3x4 random_value<i64tup3x4>() { return i64tup3x4(random_value<i64tup3>(), random_value<i64tup3>(), random_value<i64tup3>(), random_value<i64tup3>()); }
template<> i64tup4x1 random_value<i64tup4x1>() { return i64tup4x1(random_value<i64tup4>()); }
template<> i64tup4x2 random_value<i64tup4x2>() { return i64tup4x2(random_value<i64tup4>(), random_value<i64tup4>()); }
template<> i64tup4x3 random_value<i64tup4x3>() { return i64tup4x3(random_value<i64tup4>(), random_value<i64tup4>(), random_value<i64tup4>()); }
template<> i64tup4x4 random_value<i64tup4x4>() { return i64tup4x4(random_value<i64tup4>(), random_value<i64tup4>(), random_value<i64tup4>(), random_value<i64tup4>()); }
template<> u64tup1 random_value<u64tup1>() { return u64tup1(random_value<u64>()); }
template<> u64tup2 random_value<u64tup2>() { return u64tup2(random_value<u64>(), random_value<u64>()); }
template<> u64tup3 random_value<u64tup3>() { return u64tup3(random_value<u64>(), random_value<u64>(), random_value<u64>()); }
template<> u64tup4 random_value<u64tup4>() { return u64tup4(random_value<u64>(), random_value<u64>(), random_value<u64>(), random_value<u64>()); }
template<> u64tup1x1 random_value<u64tup1x1>() { return u64tup1x1(random_value<u64tup1>()); }
template<> u64tup1x2 random_value<u64tup1x2>() { return u64tup1x2(random_value<u64tup1>(), random_value<u64tup1>()); }
template<> u64tup1x3 random_value<u64tup1x3>() { return u64tup1x3(random_value<u64tup1>(), random_value<u64tup1>(), random_value<u64tup1>()); }
template<> u64tup1x4 random_value<u64tup1x4>() { return u64tup1x4(random_value<u64tup1>(), random_value<u64tup1>(), random_value<u64tup1>(), random_value<u64tup1>()); }
template<> u64tup2x1 random_value<u64tup2x1>() { return u64tup2x1(random_value<u64tup2>()); }
template<> u64tup2x2 random_value<u64tup2x2>() { return u64tup2x2(random_value<u64tup2>(), random_value<u64tup2>()); }
template<> u64tup2x3 random_value<u64tup2x3>() { return u64tup2x3(random_value<u64tup2>(), random_value<u64tup2>(), random_value<u64tup2>()); }
template<> u64tup2x4 random_value<u64tup2x4>() { return u64tup2x4(random_value<u64tup2>(), random_value<u64tup2>(), random_value<u64tup2>(), random_value<u64tup2>()); }
template<> u64tup3x1 random_value<u64tup3x1>() { return u64tup3x1(random_value<u64tup3>()); }
template<> u64tup3x2 random_value<u64tup3x2>() { return u64tup3x2(random_value<u64tup3>(), random_value<u64tup3>()); }
template<> u64tup3x3 random_value<u64tup3x3>() { return u64tup3x3(random_value<u64tup3>(), random_value<u64tup3>(), random_value<u64tup3>()); }
template<> u64tup3x4 random_value<u64tup3x4>() { return u64tup3x4(random_value<u64tup3>(), random_value<u64tup3>(), random_value<u64tup3>(), random_value<u64tup3>()); }
template<> u64tup4x1 random_value<u64tup4x1>() { return u64tup4x1(random_value<u64tup4>()); }
template<> u64tup4x2 random_value<u64tup4x2>() { return u64tup4x2(random_value<u64tup4>(), random_value<u64tup4>()); }
template<> u64tup4x3 random_value<u64tup4x3>() { return u64tup4x3(random_value<u64tup4>(), random_value<u64tup4>(), random_value<u64tup4>()); }
template<> u64tup4x4 random_value<u64tup4x4>() { return u64tup4x4(random_value<u64tup4>(), random_value<u64tup4>(), random_value<u64tup4>(), random_value<u64tup4>()); }
template<> ftup1 random_value<ftup1>() { return ftup1(random_value<float>()); }
template<> ftup2 random_value<ftup2>() { return ftup2(random_value<float>(), random_value<float>()); }
template<> ftup3 random_value<ftup3>() { return ftup3(random_value<float>(), random_value<float>(), random_value<float>()); }
template<> ftup4 random_value<ftup4>() { return ftup4(random_value<float>(), random_value<float>(), random_value<float>(), random_value<float>()); }
template<> ftup1x1 random_value<ftup1x1>() { return ftup1x1(random_value<ftup1>()); }
template<> ftup1x2 random_value<ftup1x2>() { return ftup1x2(random_value<ftup1>(), random_value<ftup1>()); }
template<> ftup1x3 random_value<ftup1x3>() { return ftup1x3(random_value<ftup1>(), random_value<ftup1>(), random_value<ftup1>()); }
template<> ftup1x4 random_value<ftup1x4>() { return ftup1x4(random_value<ftup1>(), random_value<ftup1>(), random_value<ftup1>(), random_value<ftup1>()); }
template<> ftup2x1 random_value<ftup2x1>() { return ftup2x1(random_value<ftup2>()); }
template<> ftup2x2 random_value<ftup2x2>() { return ftup2x2(random_value<ftup2>(), random_value<ftup2>()); }
template<> ftup2x3 random_value<ftup2x3>() { return ftup2x3(random_value<ftup2>(), random_value<ftup2>(), random_value<ftup2>()); }
template<> ftup2x4 random_value<ftup2x4>() { return ftup2x4(random_value<ftup2>(), random_value<ftup2>(), random_value<ftup2>(), random_value<ftup2>()); }
template<> ftup3x1 random_value<ftup3x1>() { return ftup3x1(random_value<ftup3>()); }
template<> ftup3x2 random_value<ftup3x2>() { return ftup3x2(random_value<ftup3>(), random_value<ftup3>()); }
template<> ftup3x3 random_value<ftup3x3>() { return ftup3x3(random_value<ftup3>(), random_value<ftup3>(), random_value<ftup3>()); }
template<> ftup3x4 random_value<ftup3x4>() { return ftup3x4(random_value<ftup3>(), random_value<ftup3>(), random_value<ftup3>(), random_value<ftup3>()); }
template<> ftup4x1 random_value<ftup4x1>() { return ftup4x1(random_value<ftup4>()); }
template<> ftup4x2 random_value<ftup4x2>() { return ftup4x2(random_value<ftup4>(), random_value<ftup4>()); }
template<> ftup4x3 random_value<ftup4x3>() { return ftup4x3(random_value<ftup4>(), random_value<ftup4>(), random_value<ftup4>()); }
template<> ftup4x4 random_value<ftup4x4>() { return ftup4x4(random_value<ftup4>(), random_value<ftup4>(), random_value<ftup4>(), random_value<ftup4>()); }
template<> dtup1 random_value<dtup1>() { return dtup1(random_value<double>()); }
template<> dtup2 random_value<dtup2>() { return dtup2(random_value<double>(), random_value<double>()); }
template<> dtup3 random_value<dtup3>() { return dtup3(random_value<double>(), random_value<double>(), random_value<double>()); }
template<> dtup4 random_value<dtup4>() { return dtup4(random_value<double>(), random_value<double>(), random_value<double>(), random_value<double>()); }
template<> dtup1x1 random_value<dtup1x1>() { return dtup1x1(random_value<dtup1>()); }
template<> dtup1x2 random_value<dtup1x2>() { return dtup1x2(random_value<dtup1>(), random_value<dtup1>()); }
template<> dtup1x3 random_value<dtup1x3>() { return dtup1x3(random_value<dtup1>(), random_value<dtup1>(), random_value<dtup1>()); }
template<> dtup1x4 random_value<dtup1x4>() { return dtup1x4(random_value<dtup1>(), random_value<dtup1>(), random_value<dtup1>(), random_value<dtup1>()); }
template<> dtup2x1 random_value<dtup2x1>() { return dtup2x1(random_value<dtup2>()); }
template<> dtup2x2 random_value<dtup2x2>() { return dtup2x2(random_value<dtup2>(), random_value<dtup2>()); }
template<> dtup2x3 random_value<dtup2x3>() { return dtup2x3(random_value<dtup2>(), random_value<dtup2>(), random_value<dtup2>()); }
template<> dtup2x4 random_value<dtup2x4>() { return dtup2x4(random_value<dtup2>(), random_value<dtup2>(), random_value<dtup2>(), random_value<dtup2>()); }
template<> dtup3x1 random_value<dtup3x1>() { return dtup3x1(random_value<dtup3>()); }
template<> dtup3x2 random_value<dtup3x2>() { return dtup3x2(random_value<dtup3>(), random_value<dtup3>()); }
template<> dtup3x3 random_value<dtup3x3>() { return dtup3x3(random_value<dtup3>(), random_value<dtup3>(), random_value<dtup3>()); }
template<> dtup3x4 random_value<dtup3x4>() { return dtup3x4(random_value<dtup3>(), random_value<dtup3>(), random_value<dtup3>(), random_value<dtup3>()); }
template<> dtup4x1 random_value<dtup4x1>() { return dtup4x1(random_value<dtup4>()); }
template<> dtup4x2 random_value<dtup4x2>() { return dtup4x2(random_value<dtup4>(), random_value<dtup4>()); }
template<> dtup4x3 random_value<dtup4x3>() { return dtup4x3(random_value<dtup4>(), random_value<dtup4>(), random_value<dtup4>()); }
template<> dtup4x4 random_value<dtup4x4>() { return dtup4x4(random_value<dtup4>(), random_value<dtup4>(), random_value<dtup4>(), random_value<dtup4>()); }

