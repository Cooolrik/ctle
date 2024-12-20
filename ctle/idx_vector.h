// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_IDX_VECTOR_H_
#define _CTLE_IDX_VECTOR_H_

#include <vector>
#include <cstdint>

#include "types.h"

namespace ctle
{

// idx_vector: std::vector of values, with an std::vector of int as index into the values
template <class _Ty, class _IdxTy /* = std::vector<i32>*/, class _VecTy /* = std::vector<_Ty>*/>
class idx_vector
{
public:
	using value_type = _Ty;
	using values_vector_type = _VecTy;
	using index_vector_type = _IdxTy;

	using allocator_type = typename values_vector_type::allocator_type;
	using pointer = typename values_vector_type::pointer;
	using const_pointer = typename values_vector_type::const_pointer;
	using reference = typename values_vector_type::reference;
	using const_reference = typename values_vector_type::const_reference;
	using size_type = typename values_vector_type::size_type;

private:
	values_vector_type values_m;
	index_vector_type index_m;

public:
	idx_vector() = default;
	idx_vector( const idx_vector &_other ) noexcept : values_m( _other.values_m ), index_m( _other.index_m )
	{}
	idx_vector &operator = ( const idx_vector &_other )
	{
		this->values_m = _other.values_m; this->index_m = _other.index_m; return *this;
	}
	idx_vector( idx_vector &&_other ) noexcept : values_m( std::move( _other.values_m ) ), index_m( std::move( _other.index_m ) )
	{}
	idx_vector &operator = ( idx_vector &&_other )
	{
		this->values_m = std::move( _other.values_m ); this->index_m = std::move( _other.index_m ); return *this;
	}

	bool operator==( const idx_vector &_other ) const;
	bool operator!=( const idx_vector &_other ) const;

	void clear()
	{
		this->values_m.clear(); this->index_m.clear();
	}

	index_vector_type &index()
	{
		return this->index_m;
	}
	const index_vector_type &index() const
	{
		return this->index_m;
	}

	values_vector_type &values()
	{
		return this->values_m;
	}
	const values_vector_type &values() const
	{
		return this->values_m;
	}
};

template <class _Ty, class _IdxTy, class _VecTy>
bool idx_vector<_Ty, _IdxTy, _VecTy>::operator==( const idx_vector &_other ) const
{
	return ( this->values_m == _other.values_m )
		&& ( this->index_m == _other.index_m );
}

template <class _Ty, class _IdxTy, class _VecTy>
bool idx_vector<_Ty, _IdxTy, _VecTy>::operator!=( const idx_vector &_other ) const
{
	return ( this->values_m != _other.values_m )
		|| ( this->index_m != _other.index_m );
}

}
//namespace ctle

#endif//_CTLE_IDX_VECTOR_H_
