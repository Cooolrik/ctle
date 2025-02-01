// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_FWD_H_
#define _CTLE_FWD_H_

/// @file fwd.h
/// @brief Forward declarations of ctle classes

#include <cinttypes>
#include <cstddef>
#include <vector>

namespace ctle
{

// Standard integer and real values short-hand
using i8 = std::int8_t;
using u8 = std::uint8_t;
using i16 = std::int16_t;
using u16 = std::uint16_t;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using i64 = std::int64_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;

// from status.h
enum class status_code : int;
class status;

// from data_source.h
class file_data_source;

// from data_destination.h
class file_data_destination;

// from hasher.h
class hasher_sha256;
class hasher_xxh64;
class hasher_xxh128;
template <size_t _Size> class hasher_noop;

// from read_stream.h
template<class _DataSourceTy, class _HashTy = hasher_noop<64>> class read_stream;

// from write_stream.h
template<class _DataDestTy, class _HashTy = hasher_noop<64>> class write_stream;

// from ntup.h
template<class _Ty, size_t _Size> class n_tup;
template<class _Ty, size_t _InnerSize, size_t _OuterSize> class mn_tup;

// from bimap.h
template<class _Kty, class _Vty> class bimap;

// from bitmap_font.h
enum class bitmap_font_flags : int;

// from file_funcs.h
enum class access_mode : unsigned int;
class _file_object;

// from hash.h
template<size_t _Size> struct hash;

// from idx_vector.h
template <class _Ty, class _IdxTy = std::vector<i32>, class _VecTy = std::vector<_Ty>> class idx_vector;

// from string_funcs.h
template<class _Ty> struct string_span;


}
//namespace ctle

#endif//_CTLE_FWD_H_
