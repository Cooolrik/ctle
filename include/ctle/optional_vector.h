// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_OPTIONAL_VECTOR_H_
#define _CTLE_OPTIONAL_VECTOR_H_

/// @file optional_idx_vector.h
/// @brief Contains the optional_vector class template, a version of std::vector which adds a flag to indicate if it the vector is allocated or not. 

#include <vector>
#include "fwd.h"
#include "optional_value.h"

namespace ctle
{

/// @brief optional_vector: a vector of values, which can be optionally allocated or not
/// @tparam _Ty The value type.
/// @tparam _VecTy The vector type to use 
template <
	class _Ty, 
	class _VecTy /*= std::vector<_Ty>*/
> class optional_vector 
{
public:
	using value_type = _Ty;
	using values_vector_type = _VecTy;

	using allocator_type = typename values_vector_type::allocator_type;
	using pointer = typename values_vector_type::pointer;
	using const_pointer = typename values_vector_type::const_pointer;
	using reference = typename values_vector_type::reference;
	using const_reference = typename values_vector_type::const_reference;
	using size_type = typename values_vector_type::size_type;

private:
	values_vector_type vector_m;
	bool has_value_m = false;

public:
	optional_vector() = default;
	optional_vector( const optional_vector &_other ) : vector_m( _other.vector_m ), has_value_m(_other.has_value_m) {}
	optional_vector &operator = ( const optional_vector &_other ) { this->has_value_m = _other.has_value_m; this->vector_m = _other.vector_m; return *this; }
	optional_vector( optional_vector &&_other ) : vector_m( std::move(_other.vector_m) ), has_value_m(_other.has_value_m) { _other.has_value_m = false; }
	optional_vector &operator = ( optional_vector &&_other ) { this->has_value_m = _other.has_value_m; this->vector_m = std::move( _other.vector_m ); _other.has_value_m = false; return *this; }

	bool operator==( const optional_vector &_other ) const;
	bool operator!=( const optional_vector &_other ) const;

	/// @brief Reset the optional_vector. has_value() will return false.
	void reset() { this->has_value_m = false; this->vector_m.clear(); }

	/// @brief Set the value of the optional_vector to an empty vector. has_value() will return true.
	void set() { this->has_value_m = true; this->vector_m.clear(); }

	/// @brief Set the value of the optional_vector. has_value() will return true.
	void set(const values_vector_type& _values) { this->has_value_m = true; this->vector_m = _values; }

	/// @brief Check if the optional_vector has a value.
	bool has_value() const { return this->has_value_m; }

	/// @brief Get the vector of the optional_vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	values_vector_type& vector() { if (!this->has_value_m) { throw ctle::bad_optional_value_access("optional_vector has no value"); } return this->vector_m; }

	/// @brief Get the vector of the optional_vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	const values_vector_type& vector() const { if (!this->has_value_m) { throw ctle::bad_optional_value_access("optional_vector has no value"); } return this->vector_m; }

	/// @brief Get a reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	values_vector_type& values() { return vector(); }

	/// @brief Get a const reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	const values_vector_type& values() const { return vector(); }

	/// @brief Get a reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	operator values_vector_type &() { return vector(); }

	/// @brief Get a const reference to the vector.
	/// @throw ctle::bad_optional_value_access if the optional_vector has no value.
	operator const values_vector_type& () const { return vector(); }
};

template <class _Ty, class _Alloc>
bool optional_vector<_Ty, _Alloc>::operator==( const optional_vector &_other ) const
{
	if( this->has_value_m )
	{
		if( _other.has_value_m )
			return this->vector_m == _other.vector_m;
		return false;
	}
	else
	{
		if( _other.has_value_m )
			return false;
		return true;
	}
}

template <class _Ty, class _Alloc>
bool optional_vector<_Ty, _Alloc>::operator!=( const optional_vector &_other ) const
{
	if( this->has_value_m )
	{
		if( _other.has_value_m )
			return this->vector_m != _other.vector_m;
		return true;
	}
	else
	{
		if( _other.has_value_m )
			return true;
		return false;
	}
}

}
//namespace ctle

#endif//_CTLE_OPTIONAL_VECTOR_H_
