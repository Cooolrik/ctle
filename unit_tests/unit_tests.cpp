// ctle Copyright (c) 2023 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#include "unit_tests.h"

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

template <> bool random_value<bool>() { return (bool)(rand() & 0x1); } 

template <> i8 random_value<i8>() { return u8_rand() ; }
template <> i16 random_value<i16>() { return u16_rand() ; }
template <> i32 random_value<i32>() { return u32_rand() ; }
template <> i64 random_value<i64>() { return u64_rand() ; }

template <> u8 random_value<u8>() { return u8_rand() ; }
template <> u16 random_value<u16>() { return u16_rand() ; }
template <> u32 random_value<u32>() { return u32_rand() ; }
template <> u64 random_value<u64>() { return u64_rand() ; }
