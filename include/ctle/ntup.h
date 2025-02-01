// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_NTUP_H_
#define _CTLE_NTUP_H_

/// @file ntup.h
/// @brief n-tuple (N - vectors) and mn-tuple (M x N - matrices) templates. 1-4 dimensions.
/// @details The ntup.h templates is a lightweight way to define vectors and matrices of 1-4 dimensions, with 
/// comparison operators and conversions to / from glm types. To enable glm conversions, include 
/// glm.hpp before including ntup.h in any file where the conversion is to be supported.

// Only one of CTLE_MN_TUP_COLUMNMAJOR and CTLE_MN_TUP_ROWMAJOR can be defined.
#if defined(CTLE_MN_TUP_COLUMNMAJOR) && defined(CTLE_MN_TUP_ROWMAJOR)
#error Invalid: cannot have CTLE_MN_TUP_COLUMNMAJOR and CTLE_MN_TUP_ROWMAJOR defined at the same time.
#endif//CTLE_MN_TUP_COLUMNMAJOR && CTLE_MN_TUP_ROWMAJOR

#include <cinttypes>
#include <limits>

#include "fwd.h"
#include "string_funcs.h"
#include "status.h"
#include "status_return.h"

namespace ctle
{
// 1-dimensional n-tuple template
template<class _Ty> class n_tup<_Ty,1>
{
public:
	using value_type = _Ty;
	static constexpr const size_t dims = 1;

#ifdef CTLE_N_TUP_ONLY_XYZW
	_Ty x; // [0]
#else//CTLE_N_TUP_ONLY_XYZW
	union { _Ty x; _Ty r; }; // [0]
#endif//CTLE_N_TUP_ONLY_XYZW

	// ctors/dtor
	n_tup() noexcept { x = 0; }
	n_tup( _Ty _x ) noexcept { x = _x; }
	n_tup( const _Ty *values ) noexcept { x = values[0]; }
	n_tup( const n_tup &other ) noexcept = default;
	n_tup &operator=( const n_tup &other ) noexcept = default;
	~n_tup() noexcept = default;

	// cmps ops, (order of significance: x>y>z>w)
	bool operator<( const n_tup &rhs ) const noexcept
	{
		if( this->x < rhs.x ) { return true; }
		return false;
	}
	bool operator>=( const n_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const n_tup &rhs ) const noexcept
	{
		if( this->x > rhs.x ) { return true; }
		return false;
	}
	bool operator<=( const n_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const n_tup &rhs ) const noexcept
	{
		if( this->x != rhs.x ) { return false; }
		return true;
	}
	bool operator!=( const n_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return &this->x; }
	_Ty *data() noexcept { return &this->x; }

	// index operator
	const _Ty &at( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &at( size_t index ) noexcept { return (&this->x)[index]; }
	const _Ty &operator[]( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &operator[]( size_t index ) noexcept { return (&this->x)[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	n_tup( const glm::vec<1,_Ty> &other ) noexcept { x = other.x; }
	n_tup &operator=( const glm::vec<1,_Ty> &other ) noexcept { x = other.x; return *this; }
	operator glm::vec<1,_Ty>() const noexcept { return glm::vec<1,_Ty>( x ); }
#endif//GLM_VERSION
};

// 2-dimensional n-tuple template
template<class _Ty> class n_tup<_Ty,2>
{
public:
	using value_type = _Ty;
	static constexpr const size_t dims = 2;

#ifdef CTLE_N_TUP_ONLY_XYZW
	_Ty x; // [0]
	_Ty y; // [1]
#else//CTLE_N_TUP_ONLY_XYZW
	union { _Ty x; _Ty r; }; // [0]
	union { _Ty y; _Ty g; }; // [1]
#endif//CTLE_N_TUP_ONLY_XYZW

	// ctors/dtor
	n_tup() noexcept { x = 0; y = 0; }
	n_tup( _Ty _x,_Ty _y ) noexcept { x = _x; y = _y; }
	n_tup( const _Ty *values ) noexcept { x = values[0]; y = values[1]; }
	n_tup( const n_tup &other ) noexcept = default;
	n_tup &operator=( const n_tup &other ) noexcept = default;
	~n_tup() noexcept = default;

	// cmps ops, (order of significance: x>y>z>w)
	bool operator<( const n_tup &rhs ) const noexcept
	{
		if( this->x < rhs.x ) { return true; }
		if( this->x > rhs.x ) { return false; }
		if( this->y < rhs.y ) { return true; }
		return false;
	}
	bool operator>=( const n_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const n_tup &rhs ) const noexcept
	{
		if( this->x > rhs.x ) { return true; }
		if( this->x < rhs.x ) { return false; }
		if( this->y > rhs.y ) { return true; }
		return false;
	}
	bool operator<=( const n_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const n_tup &rhs ) const noexcept
	{
		if( this->x != rhs.x ) { return false; }
		if( this->y != rhs.y ) { return false; }
		return true;
	}
	bool operator!=( const n_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return &this->x; }
	_Ty *data() noexcept { return &this->x; }

	// index operator
	const _Ty &at( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &at( size_t index ) noexcept { return (&this->x)[index]; }
	const _Ty &operator[]( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &operator[]( size_t index ) noexcept { return (&this->x)[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	n_tup( const glm::vec<2,_Ty> &other ) noexcept { x = other.x; y = other.y; }
	n_tup &operator=( const glm::vec<2,_Ty> &other ) noexcept { x = other.x; y = other.y; return *this; }
	operator glm::vec<2,_Ty>() const noexcept { return glm::vec<2,_Ty>( x , y ); }
#endif//GLM_VERSION
};

// 3-dimensional n-tuple template
template<class _Ty> class n_tup<_Ty,3>
{
public:
	using value_type = _Ty;
	static constexpr const size_t dims = 3;

#ifdef CTLE_N_TUP_ONLY_XYZW
	_Ty x; // [0]
	_Ty y; // [1]
	_Ty z; // [2]
#else//CTLE_N_TUP_ONLY_XYZW
	union { _Ty x; _Ty r; }; // [0]
	union { _Ty y; _Ty g; }; // [1]
	union { _Ty z; _Ty b; }; // [2]
#endif//CTLE_N_TUP_ONLY_XYZW

	// ctors/dtor
	n_tup() noexcept { x = 0; y = 0; z = 0; }
	n_tup( _Ty _x,_Ty _y,_Ty _z ) noexcept { x = _x; y = _y; z = _z; }
	n_tup( const _Ty *values ) noexcept { x = values[0]; y = values[1]; z = values[2]; }
	n_tup( const n_tup &other ) noexcept = default;
	n_tup &operator=( const n_tup &other ) noexcept = default;
	~n_tup() noexcept = default;

	// cmps ops, (order of significance: x>y>z>w)
	bool operator<( const n_tup &rhs ) const noexcept
	{
		if( this->x < rhs.x ) { return true; }
		if( this->x > rhs.x ) { return false; }
		if( this->y < rhs.y ) { return true; }
		if( this->y > rhs.y ) { return false; }
		if( this->z < rhs.z ) { return true; }
		return false;
	}
	bool operator>=( const n_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const n_tup &rhs ) const noexcept
	{
		if( this->x > rhs.x ) { return true; }
		if( this->x < rhs.x ) { return false; }
		if( this->y > rhs.y ) { return true; }
		if( this->y < rhs.y ) { return false; }
		if( this->z > rhs.z ) { return true; }
		return false;
	}
	bool operator<=( const n_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const n_tup &rhs ) const noexcept
	{
		if( this->x != rhs.x ) { return false; }
		if( this->y != rhs.y ) { return false; }
		if( this->z != rhs.z ) { return false; }
		return true;
	}
	bool operator!=( const n_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return &this->x; }
	_Ty *data() noexcept { return &this->x; }

	// index operator
	const _Ty &at( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &at( size_t index ) noexcept { return (&this->x)[index]; }
	const _Ty &operator[]( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &operator[]( size_t index ) noexcept { return (&this->x)[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	n_tup( const glm::vec<3,_Ty> &other ) noexcept { x = other.x; y = other.y; z = other.z; }
	n_tup &operator=( const glm::vec<3,_Ty> &other ) noexcept { x = other.x; y = other.y; z = other.z; return *this; }
	operator glm::vec<3,_Ty>() const noexcept { return glm::vec<3,_Ty>( x , y , z ); }
#endif//GLM_VERSION
};

// 4-dimensional n-tuple template
template<class _Ty> class n_tup<_Ty,4>
{
public:
	using value_type = _Ty;
	static constexpr const size_t dims = 4;

#ifdef CTLE_N_TUP_ONLY_XYZW
	_Ty x; // [0]
	_Ty y; // [1]
	_Ty z; // [2]
	_Ty w; // [3]
#else//CTLE_N_TUP_ONLY_XYZW
	union { _Ty x; _Ty r; }; // [0]
	union { _Ty y; _Ty g; }; // [1]
	union { _Ty z; _Ty b; }; // [2]
	union { _Ty w; _Ty a; }; // [3]
#endif//CTLE_N_TUP_ONLY_XYZW

	// ctors/dtor
	n_tup() noexcept { x = 0; y = 0; z = 0; w = 0; }
	n_tup( _Ty _x,_Ty _y,_Ty _z,_Ty _w ) noexcept { x = _x; y = _y; z = _z; w = _w; }
	n_tup( const _Ty *values ) noexcept { x = values[0]; y = values[1]; z = values[2]; w = values[3]; }
	n_tup( const n_tup &other ) noexcept = default;
	n_tup &operator=( const n_tup &other ) noexcept = default;
	~n_tup() noexcept = default;

	// cmps ops, (order of significance: x>y>z>w)
	bool operator<( const n_tup &rhs ) const noexcept
	{
		if( this->x < rhs.x ) { return true; }
		if( this->x > rhs.x ) { return false; }
		if( this->y < rhs.y ) { return true; }
		if( this->y > rhs.y ) { return false; }
		if( this->z < rhs.z ) { return true; }
		if( this->z > rhs.z ) { return false; }
		if( this->w < rhs.w ) { return true; }
		return false;
	}
	bool operator>=( const n_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const n_tup &rhs ) const noexcept
	{
		if( this->x > rhs.x ) { return true; }
		if( this->x < rhs.x ) { return false; }
		if( this->y > rhs.y ) { return true; }
		if( this->y < rhs.y ) { return false; }
		if( this->z > rhs.z ) { return true; }
		if( this->z < rhs.z ) { return false; }
		if( this->w > rhs.w ) { return true; }
		return false;
	}
	bool operator<=( const n_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const n_tup &rhs ) const noexcept
	{
		if( this->x != rhs.x ) { return false; }
		if( this->y != rhs.y ) { return false; }
		if( this->z != rhs.z ) { return false; }
		if( this->w != rhs.w ) { return false; }
		return true;
	}
	bool operator!=( const n_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return &this->x; }
	_Ty *data() noexcept { return &this->x; }

	// index operator
	const _Ty &at( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &at( size_t index ) noexcept { return (&this->x)[index]; }
	const _Ty &operator[]( size_t index ) const noexcept { return (&this->x)[index]; }
	_Ty &operator[]( size_t index ) noexcept { return (&this->x)[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	n_tup( const glm::vec<4,_Ty> &other ) noexcept { x = other.x; y = other.y; z = other.z; w = other.w; }
	n_tup &operator=( const glm::vec<4,_Ty> &other ) noexcept { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
	operator glm::vec<4,_Ty>() const noexcept { return glm::vec<4,_Ty>( x , y , z , w ); }
#endif//GLM_VERSION
};

// 1-dimensional tuple-of-tuples template
template<class _Ty, size_t _InnerSize> class mn_tup<_Ty,_InnerSize,1/*_OuterSize*/>
{
public:
	using inner_type = n_tup<_Ty,_InnerSize>; ///< subtuple type
	static constexpr const size_t inner_dims = _InnerSize; ///< size of each subtuple
	static constexpr const size_t outer_dims = 1; ///< number of subtuples

	// By default, mn_tup does not define row or column major order, but if
	// CTLE_MN_TUP_COLUMNMAJOR or CTLE_MN_TUP_ROWMAJOR is defined, also define
	// the number of columns and number of rows in the MxN matrix the tuple-of-tuples represents.
#if defined(CTLE_MN_TUP_COLUMNMAJOR)
	static constexpr const size_t num_rows = _InnerSize;
	static constexpr const size_t num_columns = 1;
#elif defined(CTLE_MN_TUP_ROWMAJOR)
	static constexpr const size_t num_rows = 1;
	static constexpr const size_t num_columns = _InnerSize;
#endif

	n_tup<_Ty,_InnerSize> values[1]; // 1 values of tuple type

	// ctors/dtor
	mn_tup( const n_tup<_Ty,_InnerSize> &value0 = {} ) noexcept { values[0] = value0; }
	mn_tup( const n_tup<_Ty,_InnerSize> *_values ) noexcept { values[0] = _values[0]; }
	mn_tup( const mn_tup &other ) noexcept = default;
	mn_tup &operator=( const mn_tup &other ) noexcept = default;
	~mn_tup() noexcept = default;

	// cmps ops, (order of significance: 0>1>2>3)
	bool operator<( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] < rhs.values[0] ) { return true; }
		return false;
	}
	bool operator>=( const mn_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] > rhs.values[0] ) { return true; }
		return false;
	}
	bool operator<=( const mn_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] != rhs.values[0] ) { return false; }
		return true;
	}
	bool operator!=( const mn_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return this->values->data(); }
	_Ty *data() noexcept { return this->values->data(); }

	// index operator
	const n_tup<_Ty,_InnerSize> &at( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &at( size_t index ) noexcept { return this->values[index]; }
	const n_tup<_Ty,_InnerSize> &operator[]( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &operator[]( size_t index ) noexcept { return this->values[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	mn_tup( const glm::mat<1, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); }
	mn_tup &operator=( const glm::mat<1, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); return *this; }
	operator glm::mat<1, _InnerSize, _Ty>() const noexcept { return glm::mat<1,_InnerSize,_Ty>( values[0]); }
#endif//GLM_VERSION
};

// 2-dimensional tuple-of-tuples template
template<class _Ty, size_t _InnerSize> class mn_tup<_Ty,_InnerSize,2/*_OuterSize*/>
{
public:
	using inner_type = n_tup<_Ty,_InnerSize>; ///< subtuple type
	static constexpr const size_t inner_dims = _InnerSize; ///< size of each subtuple
	static constexpr const size_t outer_dims = 2; ///< number of subtuples

	// By default, mn_tup does not define row or column major order, but if
	// CTLE_MN_TUP_COLUMNMAJOR or CTLE_MN_TUP_ROWMAJOR is defined, also define
	// the number of columns and number of rows in the MxN matrix the tuple-of-tuples represents.
#if defined(CTLE_MN_TUP_COLUMNMAJOR)
	static constexpr const size_t num_rows = _InnerSize;
	static constexpr const size_t num_columns = 2;
#elif defined(CTLE_MN_TUP_ROWMAJOR)
	static constexpr const size_t num_rows = 2;
	static constexpr const size_t num_columns = _InnerSize;
#endif

	n_tup<_Ty,_InnerSize> values[2]; // 2 values of tuple type

	// ctors/dtor
	mn_tup( const n_tup<_Ty,_InnerSize> &value0 = {},const n_tup<_Ty,_InnerSize> &value1 = {} ) noexcept { values[0] = value0; values[1] = value1; }
	mn_tup( const n_tup<_Ty,_InnerSize> *_values ) noexcept { values[0] = _values[0]; values[1] = _values[1]; }
	mn_tup( const mn_tup &other ) noexcept = default;
	mn_tup &operator=( const mn_tup &other ) noexcept = default;
	~mn_tup() noexcept = default;

	// cmps ops, (order of significance: 0>1>2>3)
	bool operator<( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] < rhs.values[0] ) { return true; }
		if( this->values[0] > rhs.values[0] ) { return false; }
		if( this->values[1] < rhs.values[1] ) { return true; }
		return false;
	}
	bool operator>=( const mn_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] > rhs.values[0] ) { return true; }
		if( this->values[0] < rhs.values[0] ) { return false; }
		if( this->values[1] > rhs.values[1] ) { return true; }
		return false;
	}
	bool operator<=( const mn_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] != rhs.values[0] ) { return false; }
		if( this->values[1] != rhs.values[1] ) { return false; }
		return true;
	}
	bool operator!=( const mn_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return this->values->data(); }
	_Ty *data() noexcept { return this->values->data(); }

	// index operator
	const n_tup<_Ty,_InnerSize> &at( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &at( size_t index ) noexcept { return this->values[index]; }
	const n_tup<_Ty,_InnerSize> &operator[]( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &operator[]( size_t index ) noexcept { return this->values[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	mn_tup( const glm::mat<2, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); values[1] = n_tup<_Ty,_InnerSize>(other[1]); }
	mn_tup &operator=( const glm::mat<2, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); values[1] = n_tup<_Ty,_InnerSize>(other[1]); return *this; }
	operator glm::mat<2, _InnerSize, _Ty>() const noexcept { return glm::mat<2,_InnerSize,_Ty>( values[0], values[1]); }
#endif//GLM_VERSION
};

// 3-dimensional tuple-of-tuples template
template<class _Ty, size_t _InnerSize> class mn_tup<_Ty,_InnerSize,3/*_OuterSize*/>
{
public:
	using inner_type = n_tup<_Ty,_InnerSize>; ///< subtuple type
	static constexpr const size_t inner_dims = _InnerSize; ///< size of each subtuple
	static constexpr const size_t outer_dims = 3; ///< number of subtuples

	// By default, mn_tup does not define row or column major order, but if
	// CTLE_MN_TUP_COLUMNMAJOR or CTLE_MN_TUP_ROWMAJOR is defined, also define
	// the number of columns and number of rows in the MxN matrix the tuple-of-tuples represents.
#if defined(CTLE_MN_TUP_COLUMNMAJOR)
	static constexpr const size_t num_rows = _InnerSize;
	static constexpr const size_t num_columns = 3;
#elif defined(CTLE_MN_TUP_ROWMAJOR)
	static constexpr const size_t num_rows = 3;
	static constexpr const size_t num_columns = _InnerSize;
#endif

	n_tup<_Ty,_InnerSize> values[3]; // 3 values of tuple type

	// ctors/dtor
	mn_tup( const n_tup<_Ty,_InnerSize> &value0 = {},const n_tup<_Ty,_InnerSize> &value1 = {},const n_tup<_Ty,_InnerSize> &value2 = {} ) noexcept { values[0] = value0; values[1] = value1; values[2] = value2; }
	mn_tup( const n_tup<_Ty,_InnerSize> *_values ) noexcept { values[0] = _values[0]; values[1] = _values[1]; values[2] = _values[2]; }
	mn_tup( const mn_tup &other ) noexcept = default;
	mn_tup &operator=( const mn_tup &other ) noexcept = default;
	~mn_tup() noexcept = default;

	// cmps ops, (order of significance: 0>1>2>3)
	bool operator<( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] < rhs.values[0] ) { return true; }
		if( this->values[0] > rhs.values[0] ) { return false; }
		if( this->values[1] < rhs.values[1] ) { return true; }
		if( this->values[1] > rhs.values[1] ) { return false; }
		if( this->values[2] < rhs.values[2] ) { return true; }
		return false;
	}
	bool operator>=( const mn_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] > rhs.values[0] ) { return true; }
		if( this->values[0] < rhs.values[0] ) { return false; }
		if( this->values[1] > rhs.values[1] ) { return true; }
		if( this->values[1] < rhs.values[1] ) { return false; }
		if( this->values[2] > rhs.values[2] ) { return true; }
		return false;
	}
	bool operator<=( const mn_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] != rhs.values[0] ) { return false; }
		if( this->values[1] != rhs.values[1] ) { return false; }
		if( this->values[2] != rhs.values[2] ) { return false; }
		return true;
	}
	bool operator!=( const mn_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return this->values->data(); }
	_Ty *data() noexcept { return this->values->data(); }

	// index operator
	const n_tup<_Ty,_InnerSize> &at( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &at( size_t index ) noexcept { return this->values[index]; }
	const n_tup<_Ty,_InnerSize> &operator[]( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &operator[]( size_t index ) noexcept { return this->values[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	mn_tup( const glm::mat<3, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); values[1] = n_tup<_Ty,_InnerSize>(other[1]); values[2] = n_tup<_Ty,_InnerSize>(other[2]); }
	mn_tup &operator=( const glm::mat<3, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); values[1] = n_tup<_Ty,_InnerSize>(other[1]); values[2] = n_tup<_Ty,_InnerSize>(other[2]); return *this; }
	operator glm::mat<3, _InnerSize, _Ty>() const noexcept { return glm::mat<3,_InnerSize,_Ty>( values[0], values[1], values[2]); }
#endif//GLM_VERSION
};

// 4-dimensional tuple-of-tuples template
template<class _Ty, size_t _InnerSize> class mn_tup<_Ty,_InnerSize,4/*_OuterSize*/>
{
public:
	using inner_type = n_tup<_Ty,_InnerSize>; ///< subtuple type
	static constexpr const size_t inner_dims = _InnerSize; ///< size of each subtuple
	static constexpr const size_t outer_dims = 4; ///< number of subtuples

	// By default, mn_tup does not define row or column major order, but if
	// CTLE_MN_TUP_COLUMNMAJOR or CTLE_MN_TUP_ROWMAJOR is defined, also define
	// the number of columns and number of rows in the MxN matrix the tuple-of-tuples represents.
#if defined(CTLE_MN_TUP_COLUMNMAJOR)
	static constexpr const size_t num_rows = _InnerSize;
	static constexpr const size_t num_columns = 4;
#elif defined(CTLE_MN_TUP_ROWMAJOR)
	static constexpr const size_t num_rows = 4;
	static constexpr const size_t num_columns = _InnerSize;
#endif

	n_tup<_Ty,_InnerSize> values[4]; // 4 values of tuple type

	// ctors/dtor
	mn_tup( const n_tup<_Ty,_InnerSize> &value0 = {},const n_tup<_Ty,_InnerSize> &value1 = {},const n_tup<_Ty,_InnerSize> &value2 = {},const n_tup<_Ty,_InnerSize> &value3 = {} ) noexcept { values[0] = value0; values[1] = value1; values[2] = value2; values[3] = value3; }
	mn_tup( const n_tup<_Ty,_InnerSize> *_values ) noexcept { values[0] = _values[0]; values[1] = _values[1]; values[2] = _values[2]; values[3] = _values[3]; }
	mn_tup( const mn_tup &other ) noexcept = default;
	mn_tup &operator=( const mn_tup &other ) noexcept = default;
	~mn_tup() noexcept = default;

	// cmps ops, (order of significance: 0>1>2>3)
	bool operator<( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] < rhs.values[0] ) { return true; }
		if( this->values[0] > rhs.values[0] ) { return false; }
		if( this->values[1] < rhs.values[1] ) { return true; }
		if( this->values[1] > rhs.values[1] ) { return false; }
		if( this->values[2] < rhs.values[2] ) { return true; }
		if( this->values[2] > rhs.values[2] ) { return false; }
		if( this->values[3] < rhs.values[3] ) { return true; }
		return false;
	}
	bool operator>=( const mn_tup &rhs ) const noexcept { return !this->operator<(rhs); }
	bool operator>( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] > rhs.values[0] ) { return true; }
		if( this->values[0] < rhs.values[0] ) { return false; }
		if( this->values[1] > rhs.values[1] ) { return true; }
		if( this->values[1] < rhs.values[1] ) { return false; }
		if( this->values[2] > rhs.values[2] ) { return true; }
		if( this->values[2] < rhs.values[2] ) { return false; }
		if( this->values[3] > rhs.values[3] ) { return true; }
		return false;
	}
	bool operator<=( const mn_tup &rhs ) const noexcept { return !this->operator>(rhs); }
	bool operator==( const mn_tup &rhs ) const noexcept
	{
		if( this->values[0] != rhs.values[0] ) { return false; }
		if( this->values[1] != rhs.values[1] ) { return false; }
		if( this->values[2] != rhs.values[2] ) { return false; }
		if( this->values[3] != rhs.values[3] ) { return false; }
		return true;
	}
	bool operator!=( const mn_tup &rhs ) const noexcept { return !this->operator==(rhs); }

	// data access
	const _Ty *data() const noexcept { return this->values->data(); }
	_Ty *data() noexcept { return this->values->data(); }

	// index operator
	const n_tup<_Ty,_InnerSize> &at( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &at( size_t index ) noexcept { return this->values[index]; }
	const n_tup<_Ty,_InnerSize> &operator[]( size_t index ) const noexcept { return this->values[index]; }
	n_tup<_Ty,_InnerSize> &operator[]( size_t index ) noexcept { return this->values[index]; }

#ifdef GLM_VERSION
	// conversions to/from glm (to enable, include glm.hpp before including ntup.h)
	mn_tup( const glm::mat<4, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); values[1] = n_tup<_Ty,_InnerSize>(other[1]); values[2] = n_tup<_Ty,_InnerSize>(other[2]); values[3] = n_tup<_Ty,_InnerSize>(other[3]); }
	mn_tup &operator=( const glm::mat<4, _InnerSize, _Ty> &other ) noexcept { values[0] = n_tup<_Ty,_InnerSize>(other[0]); values[1] = n_tup<_Ty,_InnerSize>(other[1]); values[2] = n_tup<_Ty,_InnerSize>(other[2]); values[3] = n_tup<_Ty,_InnerSize>(other[3]); return *this; }
	operator glm::mat<4, _InnerSize, _Ty>() const noexcept { return glm::mat<4,_InnerSize,_Ty>( values[0], values[1], values[2], values[3]); }
#endif//GLM_VERSION
};

#ifdef CTLE_IMPLEMENTATION

// Print types to strings.
template<> std::string to_string( const n_tup<i8,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<i8,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<i8,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<i8,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<i8,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i8,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i8,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i8,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i8,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i8,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i8,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i8,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i8,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i8,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i8,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i8,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i8,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i8,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i8,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i8,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<u8,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<u8,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<u8,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<u8,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<u8,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u8,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u8,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u8,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u8,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u8,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u8,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u8,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u8,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u8,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u8,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u8,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u8,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u8,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u8,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u8,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<i16,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<i16,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<i16,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<i16,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<i16,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i16,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i16,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i16,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i16,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i16,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i16,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i16,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i16,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i16,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i16,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i16,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i16,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i16,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i16,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i16,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<u16,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<u16,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<u16,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<u16,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<u16,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u16,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u16,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u16,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u16,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u16,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u16,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u16,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u16,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u16,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u16,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u16,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u16,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u16,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u16,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u16,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<i32,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<i32,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<i32,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<i32,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<i32,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i32,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i32,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i32,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i32,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i32,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i32,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i32,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i32,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i32,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i32,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i32,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i32,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i32,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i32,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i32,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<u32,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<u32,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<u32,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<u32,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<u32,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u32,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u32,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u32,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u32,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u32,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u32,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u32,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u32,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u32,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u32,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u32,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u32,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u32,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u32,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u32,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<i64,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<i64,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<i64,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<i64,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<i64,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i64,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i64,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i64,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i64,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i64,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i64,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i64,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i64,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i64,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i64,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i64,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<i64,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<i64,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<i64,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<i64,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<u64,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<u64,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<u64,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<u64,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<u64,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u64,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u64,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u64,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u64,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u64,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u64,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u64,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u64,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u64,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u64,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u64,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<u64,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<u64,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<u64,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<u64,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<f32,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<f32,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<f32,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<f32,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<f32,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f32,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f32,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f32,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<f32,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f32,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f32,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f32,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<f32,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f32,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f32,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f32,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<f32,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f32,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f32,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f32,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const n_tup<f64,1> &val ) { return to_string(val.x); }
template<> std::string to_string( const n_tup<f64,2> &val ) { return to_string(val.x) + " " + to_string(val.y); }
template<> std::string to_string( const n_tup<f64,3> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z); }
template<> std::string to_string( const n_tup<f64,4> &val ) { return to_string(val.x) + " " + to_string(val.y) + " " + to_string(val.z) + " " + to_string(val.w); }
template<> std::string to_string( const mn_tup<f64,1,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f64,1,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f64,1,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f64,1,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<f64,2,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f64,2,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f64,2,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f64,2,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<f64,3,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f64,3,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f64,3,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f64,3,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }
template<> std::string to_string( const mn_tup<f64,4,1> &val ) { return to_string(val.values[0]); }
template<> std::string to_string( const mn_tup<f64,4,2> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]); }
template<> std::string to_string( const mn_tup<f64,4,3> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]); }
template<> std::string to_string( const mn_tup<f64,4,4> &val ) { return to_string(val.values[0]) + " " + to_string(val.values[1]) + " " + to_string(val.values[2]) + " " + to_string(val.values[3]); }


template<class _Ty> inline _Ty _n_tup_from_string( const string_span<char> &str, bool &result ) noexcept
{
	using basic_type = typename _Ty::value_type; 

	std::vector<string_span<char>> toks;
	if( !lex_t<char>( &toks, str ) || toks.size() != _Ty::dims )
	{ 
		result = false;
		return {};
	}

	_Ty val = {};
	for( size_t inx=0; inx<_Ty::dims; ++inx )
	{
		// use the non-throwing conversion, and check the result
		val[inx] = from_string<basic_type>(toks[inx], result); 
		if( !result )
  		{
			return {}; // the conversion failed, return empty 
		}
	}
	return val;
}
template<class _Ty> inline _Ty _n_tup_from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = _n_tup_from_string<_Ty>(str, result);
	if( !result ) 
 	{
		throw std::out_of_range("In ctle::from_string(): Could not convert from string to value, either wrong number of items, or the values are out of range.");
	}
	return val;
}

template<class _Ty> inline _Ty _mn_tup_from_string( const string_span<char> &str, bool &result ) noexcept
{
	constexpr const size_t total_values = _Ty::outer_dims * _Ty::inner_dims;
	using basic_type = typename _Ty::inner_type::value_type; 

	std::vector<string_span<char>> toks;
	if( !lex_t<char>( &toks, str ) || toks.size() != total_values )
	{ 
		result = false;
		return {};
	}

	_Ty val = {};
	for( size_t inx=0; inx<_Ty::outer_dims; ++inx )
	{
		auto &tup = val.values[inx];
		for( size_t n=0; n<_Ty::inner_dims; ++n )
		{
			// use the non-throwing conversion, and check the result
			tup[n] = from_string<basic_type>(toks[(inx*_Ty::inner_dims) + n], result); 
			if( !result )
			{
				return {}; // the conversion failed, return empty 
			}   
		}
	}
	return val;
}
template<class _Ty> inline _Ty _mn_tup_from_string( const string_span<char> &str )
{
	bool result = true;
	auto val = _mn_tup_from_string<_Ty>(str, result);
	if( !result ) 
 	{
		throw std::out_of_range("In ctle::from_string(): Could not convert from string to value, either wrong number of items, or the values are out of range.");
	}
	return val;
}


template<> n_tup<i8,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i8,1>>(str, result); }
template<> n_tup<i8,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i8,1>>(str); }
template<> n_tup<i8,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i8,2>>(str, result); }
template<> n_tup<i8,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i8,2>>(str); }
template<> n_tup<i8,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i8,3>>(str, result); }
template<> n_tup<i8,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i8,3>>(str); }
template<> n_tup<i8,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i8,4>>(str, result); }
template<> n_tup<i8,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i8,4>>(str); }
template<> mn_tup<i8,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,1,1>>(str, result); }
template<> mn_tup<i8,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,1,1>>(str); }
template<> mn_tup<i8,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,1,2>>(str, result); }
template<> mn_tup<i8,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,1,2>>(str); }
template<> mn_tup<i8,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,1,3>>(str, result); }
template<> mn_tup<i8,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,1,3>>(str); }
template<> mn_tup<i8,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,1,4>>(str, result); }
template<> mn_tup<i8,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,1,4>>(str); }
template<> mn_tup<i8,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,2,1>>(str, result); }
template<> mn_tup<i8,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,2,1>>(str); }
template<> mn_tup<i8,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,2,2>>(str, result); }
template<> mn_tup<i8,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,2,2>>(str); }
template<> mn_tup<i8,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,2,3>>(str, result); }
template<> mn_tup<i8,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,2,3>>(str); }
template<> mn_tup<i8,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,2,4>>(str, result); }
template<> mn_tup<i8,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,2,4>>(str); }
template<> mn_tup<i8,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,3,1>>(str, result); }
template<> mn_tup<i8,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,3,1>>(str); }
template<> mn_tup<i8,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,3,2>>(str, result); }
template<> mn_tup<i8,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,3,2>>(str); }
template<> mn_tup<i8,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,3,3>>(str, result); }
template<> mn_tup<i8,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,3,3>>(str); }
template<> mn_tup<i8,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,3,4>>(str, result); }
template<> mn_tup<i8,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,3,4>>(str); }
template<> mn_tup<i8,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,4,1>>(str, result); }
template<> mn_tup<i8,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,4,1>>(str); }
template<> mn_tup<i8,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,4,2>>(str, result); }
template<> mn_tup<i8,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,4,2>>(str); }
template<> mn_tup<i8,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,4,3>>(str, result); }
template<> mn_tup<i8,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,4,3>>(str); }
template<> mn_tup<i8,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i8,4,4>>(str, result); }
template<> mn_tup<i8,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i8,4,4>>(str); }
template<> n_tup<u8,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u8,1>>(str, result); }
template<> n_tup<u8,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u8,1>>(str); }
template<> n_tup<u8,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u8,2>>(str, result); }
template<> n_tup<u8,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u8,2>>(str); }
template<> n_tup<u8,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u8,3>>(str, result); }
template<> n_tup<u8,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u8,3>>(str); }
template<> n_tup<u8,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u8,4>>(str, result); }
template<> n_tup<u8,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u8,4>>(str); }
template<> mn_tup<u8,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,1,1>>(str, result); }
template<> mn_tup<u8,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,1,1>>(str); }
template<> mn_tup<u8,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,1,2>>(str, result); }
template<> mn_tup<u8,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,1,2>>(str); }
template<> mn_tup<u8,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,1,3>>(str, result); }
template<> mn_tup<u8,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,1,3>>(str); }
template<> mn_tup<u8,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,1,4>>(str, result); }
template<> mn_tup<u8,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,1,4>>(str); }
template<> mn_tup<u8,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,2,1>>(str, result); }
template<> mn_tup<u8,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,2,1>>(str); }
template<> mn_tup<u8,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,2,2>>(str, result); }
template<> mn_tup<u8,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,2,2>>(str); }
template<> mn_tup<u8,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,2,3>>(str, result); }
template<> mn_tup<u8,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,2,3>>(str); }
template<> mn_tup<u8,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,2,4>>(str, result); }
template<> mn_tup<u8,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,2,4>>(str); }
template<> mn_tup<u8,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,3,1>>(str, result); }
template<> mn_tup<u8,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,3,1>>(str); }
template<> mn_tup<u8,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,3,2>>(str, result); }
template<> mn_tup<u8,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,3,2>>(str); }
template<> mn_tup<u8,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,3,3>>(str, result); }
template<> mn_tup<u8,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,3,3>>(str); }
template<> mn_tup<u8,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,3,4>>(str, result); }
template<> mn_tup<u8,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,3,4>>(str); }
template<> mn_tup<u8,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,4,1>>(str, result); }
template<> mn_tup<u8,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,4,1>>(str); }
template<> mn_tup<u8,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,4,2>>(str, result); }
template<> mn_tup<u8,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,4,2>>(str); }
template<> mn_tup<u8,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,4,3>>(str, result); }
template<> mn_tup<u8,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,4,3>>(str); }
template<> mn_tup<u8,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u8,4,4>>(str, result); }
template<> mn_tup<u8,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u8,4,4>>(str); }
template<> n_tup<i16,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i16,1>>(str, result); }
template<> n_tup<i16,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i16,1>>(str); }
template<> n_tup<i16,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i16,2>>(str, result); }
template<> n_tup<i16,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i16,2>>(str); }
template<> n_tup<i16,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i16,3>>(str, result); }
template<> n_tup<i16,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i16,3>>(str); }
template<> n_tup<i16,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i16,4>>(str, result); }
template<> n_tup<i16,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i16,4>>(str); }
template<> mn_tup<i16,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,1,1>>(str, result); }
template<> mn_tup<i16,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,1,1>>(str); }
template<> mn_tup<i16,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,1,2>>(str, result); }
template<> mn_tup<i16,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,1,2>>(str); }
template<> mn_tup<i16,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,1,3>>(str, result); }
template<> mn_tup<i16,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,1,3>>(str); }
template<> mn_tup<i16,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,1,4>>(str, result); }
template<> mn_tup<i16,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,1,4>>(str); }
template<> mn_tup<i16,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,2,1>>(str, result); }
template<> mn_tup<i16,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,2,1>>(str); }
template<> mn_tup<i16,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,2,2>>(str, result); }
template<> mn_tup<i16,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,2,2>>(str); }
template<> mn_tup<i16,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,2,3>>(str, result); }
template<> mn_tup<i16,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,2,3>>(str); }
template<> mn_tup<i16,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,2,4>>(str, result); }
template<> mn_tup<i16,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,2,4>>(str); }
template<> mn_tup<i16,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,3,1>>(str, result); }
template<> mn_tup<i16,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,3,1>>(str); }
template<> mn_tup<i16,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,3,2>>(str, result); }
template<> mn_tup<i16,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,3,2>>(str); }
template<> mn_tup<i16,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,3,3>>(str, result); }
template<> mn_tup<i16,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,3,3>>(str); }
template<> mn_tup<i16,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,3,4>>(str, result); }
template<> mn_tup<i16,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,3,4>>(str); }
template<> mn_tup<i16,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,4,1>>(str, result); }
template<> mn_tup<i16,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,4,1>>(str); }
template<> mn_tup<i16,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,4,2>>(str, result); }
template<> mn_tup<i16,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,4,2>>(str); }
template<> mn_tup<i16,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,4,3>>(str, result); }
template<> mn_tup<i16,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,4,3>>(str); }
template<> mn_tup<i16,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i16,4,4>>(str, result); }
template<> mn_tup<i16,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i16,4,4>>(str); }
template<> n_tup<u16,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u16,1>>(str, result); }
template<> n_tup<u16,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u16,1>>(str); }
template<> n_tup<u16,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u16,2>>(str, result); }
template<> n_tup<u16,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u16,2>>(str); }
template<> n_tup<u16,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u16,3>>(str, result); }
template<> n_tup<u16,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u16,3>>(str); }
template<> n_tup<u16,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u16,4>>(str, result); }
template<> n_tup<u16,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u16,4>>(str); }
template<> mn_tup<u16,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,1,1>>(str, result); }
template<> mn_tup<u16,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,1,1>>(str); }
template<> mn_tup<u16,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,1,2>>(str, result); }
template<> mn_tup<u16,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,1,2>>(str); }
template<> mn_tup<u16,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,1,3>>(str, result); }
template<> mn_tup<u16,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,1,3>>(str); }
template<> mn_tup<u16,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,1,4>>(str, result); }
template<> mn_tup<u16,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,1,4>>(str); }
template<> mn_tup<u16,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,2,1>>(str, result); }
template<> mn_tup<u16,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,2,1>>(str); }
template<> mn_tup<u16,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,2,2>>(str, result); }
template<> mn_tup<u16,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,2,2>>(str); }
template<> mn_tup<u16,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,2,3>>(str, result); }
template<> mn_tup<u16,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,2,3>>(str); }
template<> mn_tup<u16,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,2,4>>(str, result); }
template<> mn_tup<u16,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,2,4>>(str); }
template<> mn_tup<u16,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,3,1>>(str, result); }
template<> mn_tup<u16,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,3,1>>(str); }
template<> mn_tup<u16,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,3,2>>(str, result); }
template<> mn_tup<u16,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,3,2>>(str); }
template<> mn_tup<u16,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,3,3>>(str, result); }
template<> mn_tup<u16,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,3,3>>(str); }
template<> mn_tup<u16,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,3,4>>(str, result); }
template<> mn_tup<u16,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,3,4>>(str); }
template<> mn_tup<u16,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,4,1>>(str, result); }
template<> mn_tup<u16,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,4,1>>(str); }
template<> mn_tup<u16,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,4,2>>(str, result); }
template<> mn_tup<u16,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,4,2>>(str); }
template<> mn_tup<u16,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,4,3>>(str, result); }
template<> mn_tup<u16,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,4,3>>(str); }
template<> mn_tup<u16,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u16,4,4>>(str, result); }
template<> mn_tup<u16,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u16,4,4>>(str); }
template<> n_tup<i32,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i32,1>>(str, result); }
template<> n_tup<i32,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i32,1>>(str); }
template<> n_tup<i32,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i32,2>>(str, result); }
template<> n_tup<i32,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i32,2>>(str); }
template<> n_tup<i32,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i32,3>>(str, result); }
template<> n_tup<i32,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i32,3>>(str); }
template<> n_tup<i32,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i32,4>>(str, result); }
template<> n_tup<i32,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i32,4>>(str); }
template<> mn_tup<i32,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,1,1>>(str, result); }
template<> mn_tup<i32,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,1,1>>(str); }
template<> mn_tup<i32,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,1,2>>(str, result); }
template<> mn_tup<i32,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,1,2>>(str); }
template<> mn_tup<i32,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,1,3>>(str, result); }
template<> mn_tup<i32,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,1,3>>(str); }
template<> mn_tup<i32,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,1,4>>(str, result); }
template<> mn_tup<i32,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,1,4>>(str); }
template<> mn_tup<i32,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,2,1>>(str, result); }
template<> mn_tup<i32,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,2,1>>(str); }
template<> mn_tup<i32,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,2,2>>(str, result); }
template<> mn_tup<i32,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,2,2>>(str); }
template<> mn_tup<i32,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,2,3>>(str, result); }
template<> mn_tup<i32,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,2,3>>(str); }
template<> mn_tup<i32,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,2,4>>(str, result); }
template<> mn_tup<i32,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,2,4>>(str); }
template<> mn_tup<i32,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,3,1>>(str, result); }
template<> mn_tup<i32,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,3,1>>(str); }
template<> mn_tup<i32,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,3,2>>(str, result); }
template<> mn_tup<i32,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,3,2>>(str); }
template<> mn_tup<i32,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,3,3>>(str, result); }
template<> mn_tup<i32,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,3,3>>(str); }
template<> mn_tup<i32,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,3,4>>(str, result); }
template<> mn_tup<i32,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,3,4>>(str); }
template<> mn_tup<i32,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,4,1>>(str, result); }
template<> mn_tup<i32,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,4,1>>(str); }
template<> mn_tup<i32,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,4,2>>(str, result); }
template<> mn_tup<i32,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,4,2>>(str); }
template<> mn_tup<i32,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,4,3>>(str, result); }
template<> mn_tup<i32,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,4,3>>(str); }
template<> mn_tup<i32,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i32,4,4>>(str, result); }
template<> mn_tup<i32,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i32,4,4>>(str); }
template<> n_tup<u32,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u32,1>>(str, result); }
template<> n_tup<u32,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u32,1>>(str); }
template<> n_tup<u32,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u32,2>>(str, result); }
template<> n_tup<u32,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u32,2>>(str); }
template<> n_tup<u32,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u32,3>>(str, result); }
template<> n_tup<u32,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u32,3>>(str); }
template<> n_tup<u32,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u32,4>>(str, result); }
template<> n_tup<u32,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u32,4>>(str); }
template<> mn_tup<u32,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,1,1>>(str, result); }
template<> mn_tup<u32,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,1,1>>(str); }
template<> mn_tup<u32,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,1,2>>(str, result); }
template<> mn_tup<u32,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,1,2>>(str); }
template<> mn_tup<u32,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,1,3>>(str, result); }
template<> mn_tup<u32,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,1,3>>(str); }
template<> mn_tup<u32,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,1,4>>(str, result); }
template<> mn_tup<u32,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,1,4>>(str); }
template<> mn_tup<u32,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,2,1>>(str, result); }
template<> mn_tup<u32,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,2,1>>(str); }
template<> mn_tup<u32,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,2,2>>(str, result); }
template<> mn_tup<u32,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,2,2>>(str); }
template<> mn_tup<u32,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,2,3>>(str, result); }
template<> mn_tup<u32,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,2,3>>(str); }
template<> mn_tup<u32,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,2,4>>(str, result); }
template<> mn_tup<u32,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,2,4>>(str); }
template<> mn_tup<u32,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,3,1>>(str, result); }
template<> mn_tup<u32,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,3,1>>(str); }
template<> mn_tup<u32,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,3,2>>(str, result); }
template<> mn_tup<u32,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,3,2>>(str); }
template<> mn_tup<u32,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,3,3>>(str, result); }
template<> mn_tup<u32,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,3,3>>(str); }
template<> mn_tup<u32,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,3,4>>(str, result); }
template<> mn_tup<u32,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,3,4>>(str); }
template<> mn_tup<u32,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,4,1>>(str, result); }
template<> mn_tup<u32,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,4,1>>(str); }
template<> mn_tup<u32,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,4,2>>(str, result); }
template<> mn_tup<u32,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,4,2>>(str); }
template<> mn_tup<u32,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,4,3>>(str, result); }
template<> mn_tup<u32,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,4,3>>(str); }
template<> mn_tup<u32,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u32,4,4>>(str, result); }
template<> mn_tup<u32,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u32,4,4>>(str); }
template<> n_tup<i64,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i64,1>>(str, result); }
template<> n_tup<i64,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i64,1>>(str); }
template<> n_tup<i64,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i64,2>>(str, result); }
template<> n_tup<i64,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i64,2>>(str); }
template<> n_tup<i64,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i64,3>>(str, result); }
template<> n_tup<i64,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i64,3>>(str); }
template<> n_tup<i64,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<i64,4>>(str, result); }
template<> n_tup<i64,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<i64,4>>(str); }
template<> mn_tup<i64,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,1,1>>(str, result); }
template<> mn_tup<i64,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,1,1>>(str); }
template<> mn_tup<i64,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,1,2>>(str, result); }
template<> mn_tup<i64,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,1,2>>(str); }
template<> mn_tup<i64,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,1,3>>(str, result); }
template<> mn_tup<i64,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,1,3>>(str); }
template<> mn_tup<i64,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,1,4>>(str, result); }
template<> mn_tup<i64,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,1,4>>(str); }
template<> mn_tup<i64,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,2,1>>(str, result); }
template<> mn_tup<i64,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,2,1>>(str); }
template<> mn_tup<i64,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,2,2>>(str, result); }
template<> mn_tup<i64,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,2,2>>(str); }
template<> mn_tup<i64,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,2,3>>(str, result); }
template<> mn_tup<i64,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,2,3>>(str); }
template<> mn_tup<i64,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,2,4>>(str, result); }
template<> mn_tup<i64,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,2,4>>(str); }
template<> mn_tup<i64,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,3,1>>(str, result); }
template<> mn_tup<i64,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,3,1>>(str); }
template<> mn_tup<i64,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,3,2>>(str, result); }
template<> mn_tup<i64,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,3,2>>(str); }
template<> mn_tup<i64,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,3,3>>(str, result); }
template<> mn_tup<i64,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,3,3>>(str); }
template<> mn_tup<i64,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,3,4>>(str, result); }
template<> mn_tup<i64,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,3,4>>(str); }
template<> mn_tup<i64,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,4,1>>(str, result); }
template<> mn_tup<i64,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,4,1>>(str); }
template<> mn_tup<i64,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,4,2>>(str, result); }
template<> mn_tup<i64,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,4,2>>(str); }
template<> mn_tup<i64,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,4,3>>(str, result); }
template<> mn_tup<i64,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,4,3>>(str); }
template<> mn_tup<i64,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<i64,4,4>>(str, result); }
template<> mn_tup<i64,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<i64,4,4>>(str); }
template<> n_tup<u64,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u64,1>>(str, result); }
template<> n_tup<u64,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u64,1>>(str); }
template<> n_tup<u64,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u64,2>>(str, result); }
template<> n_tup<u64,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u64,2>>(str); }
template<> n_tup<u64,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u64,3>>(str, result); }
template<> n_tup<u64,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u64,3>>(str); }
template<> n_tup<u64,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<u64,4>>(str, result); }
template<> n_tup<u64,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<u64,4>>(str); }
template<> mn_tup<u64,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,1,1>>(str, result); }
template<> mn_tup<u64,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,1,1>>(str); }
template<> mn_tup<u64,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,1,2>>(str, result); }
template<> mn_tup<u64,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,1,2>>(str); }
template<> mn_tup<u64,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,1,3>>(str, result); }
template<> mn_tup<u64,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,1,3>>(str); }
template<> mn_tup<u64,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,1,4>>(str, result); }
template<> mn_tup<u64,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,1,4>>(str); }
template<> mn_tup<u64,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,2,1>>(str, result); }
template<> mn_tup<u64,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,2,1>>(str); }
template<> mn_tup<u64,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,2,2>>(str, result); }
template<> mn_tup<u64,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,2,2>>(str); }
template<> mn_tup<u64,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,2,3>>(str, result); }
template<> mn_tup<u64,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,2,3>>(str); }
template<> mn_tup<u64,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,2,4>>(str, result); }
template<> mn_tup<u64,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,2,4>>(str); }
template<> mn_tup<u64,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,3,1>>(str, result); }
template<> mn_tup<u64,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,3,1>>(str); }
template<> mn_tup<u64,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,3,2>>(str, result); }
template<> mn_tup<u64,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,3,2>>(str); }
template<> mn_tup<u64,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,3,3>>(str, result); }
template<> mn_tup<u64,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,3,3>>(str); }
template<> mn_tup<u64,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,3,4>>(str, result); }
template<> mn_tup<u64,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,3,4>>(str); }
template<> mn_tup<u64,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,4,1>>(str, result); }
template<> mn_tup<u64,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,4,1>>(str); }
template<> mn_tup<u64,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,4,2>>(str, result); }
template<> mn_tup<u64,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,4,2>>(str); }
template<> mn_tup<u64,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,4,3>>(str, result); }
template<> mn_tup<u64,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,4,3>>(str); }
template<> mn_tup<u64,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<u64,4,4>>(str, result); }
template<> mn_tup<u64,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<u64,4,4>>(str); }
template<> n_tup<f32,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f32,1>>(str, result); }
template<> n_tup<f32,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f32,1>>(str); }
template<> n_tup<f32,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f32,2>>(str, result); }
template<> n_tup<f32,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f32,2>>(str); }
template<> n_tup<f32,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f32,3>>(str, result); }
template<> n_tup<f32,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f32,3>>(str); }
template<> n_tup<f32,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f32,4>>(str, result); }
template<> n_tup<f32,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f32,4>>(str); }
template<> mn_tup<f32,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,1,1>>(str, result); }
template<> mn_tup<f32,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,1,1>>(str); }
template<> mn_tup<f32,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,1,2>>(str, result); }
template<> mn_tup<f32,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,1,2>>(str); }
template<> mn_tup<f32,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,1,3>>(str, result); }
template<> mn_tup<f32,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,1,3>>(str); }
template<> mn_tup<f32,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,1,4>>(str, result); }
template<> mn_tup<f32,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,1,4>>(str); }
template<> mn_tup<f32,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,2,1>>(str, result); }
template<> mn_tup<f32,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,2,1>>(str); }
template<> mn_tup<f32,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,2,2>>(str, result); }
template<> mn_tup<f32,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,2,2>>(str); }
template<> mn_tup<f32,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,2,3>>(str, result); }
template<> mn_tup<f32,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,2,3>>(str); }
template<> mn_tup<f32,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,2,4>>(str, result); }
template<> mn_tup<f32,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,2,4>>(str); }
template<> mn_tup<f32,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,3,1>>(str, result); }
template<> mn_tup<f32,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,3,1>>(str); }
template<> mn_tup<f32,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,3,2>>(str, result); }
template<> mn_tup<f32,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,3,2>>(str); }
template<> mn_tup<f32,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,3,3>>(str, result); }
template<> mn_tup<f32,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,3,3>>(str); }
template<> mn_tup<f32,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,3,4>>(str, result); }
template<> mn_tup<f32,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,3,4>>(str); }
template<> mn_tup<f32,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,4,1>>(str, result); }
template<> mn_tup<f32,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,4,1>>(str); }
template<> mn_tup<f32,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,4,2>>(str, result); }
template<> mn_tup<f32,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,4,2>>(str); }
template<> mn_tup<f32,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,4,3>>(str, result); }
template<> mn_tup<f32,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,4,3>>(str); }
template<> mn_tup<f32,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f32,4,4>>(str, result); }
template<> mn_tup<f32,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f32,4,4>>(str); }
template<> n_tup<f64,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f64,1>>(str, result); }
template<> n_tup<f64,1> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f64,1>>(str); }
template<> n_tup<f64,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f64,2>>(str, result); }
template<> n_tup<f64,2> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f64,2>>(str); }
template<> n_tup<f64,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f64,3>>(str, result); }
template<> n_tup<f64,3> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f64,3>>(str); }
template<> n_tup<f64,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _n_tup_from_string<n_tup<f64,4>>(str, result); }
template<> n_tup<f64,4> from_string( const string_span<char> &str ) { return _n_tup_from_string<n_tup<f64,4>>(str); }
template<> mn_tup<f64,1,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,1,1>>(str, result); }
template<> mn_tup<f64,1,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,1,1>>(str); }
template<> mn_tup<f64,1,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,1,2>>(str, result); }
template<> mn_tup<f64,1,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,1,2>>(str); }
template<> mn_tup<f64,1,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,1,3>>(str, result); }
template<> mn_tup<f64,1,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,1,3>>(str); }
template<> mn_tup<f64,1,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,1,4>>(str, result); }
template<> mn_tup<f64,1,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,1,4>>(str); }
template<> mn_tup<f64,2,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,2,1>>(str, result); }
template<> mn_tup<f64,2,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,2,1>>(str); }
template<> mn_tup<f64,2,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,2,2>>(str, result); }
template<> mn_tup<f64,2,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,2,2>>(str); }
template<> mn_tup<f64,2,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,2,3>>(str, result); }
template<> mn_tup<f64,2,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,2,3>>(str); }
template<> mn_tup<f64,2,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,2,4>>(str, result); }
template<> mn_tup<f64,2,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,2,4>>(str); }
template<> mn_tup<f64,3,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,3,1>>(str, result); }
template<> mn_tup<f64,3,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,3,1>>(str); }
template<> mn_tup<f64,3,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,3,2>>(str, result); }
template<> mn_tup<f64,3,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,3,2>>(str); }
template<> mn_tup<f64,3,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,3,3>>(str, result); }
template<> mn_tup<f64,3,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,3,3>>(str); }
template<> mn_tup<f64,3,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,3,4>>(str, result); }
template<> mn_tup<f64,3,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,3,4>>(str); }
template<> mn_tup<f64,4,1> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,4,1>>(str, result); }
template<> mn_tup<f64,4,1> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,4,1>>(str); }
template<> mn_tup<f64,4,2> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,4,2>>(str, result); }
template<> mn_tup<f64,4,2> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,4,2>>(str); }
template<> mn_tup<f64,4,3> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,4,3>>(str, result); }
template<> mn_tup<f64,4,3> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,4,3>>(str); }
template<> mn_tup<f64,4,4> from_string( const string_span<char> &str, bool &result ) noexcept { return _mn_tup_from_string<mn_tup<f64,4,4>>(str, result); }
template<> mn_tup<f64,4,4> from_string( const string_span<char> &str ) { return _mn_tup_from_string<mn_tup<f64,4,4>>(str); }
#endif//CTLE_IMPLEMENTATION

}
//namespace ctle

#endif//_CTLE_NTUP_H_
