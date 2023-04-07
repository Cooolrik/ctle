// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <vector>

#include "optional_value.h"
#include "idx_vector.h"

namespace ctle
	{
	// optional_idx_vector: optional idx_vector 
	template <class _Ty, class _IdxTy = std::vector<std::int32_t>, class _VecTy = std::vector<_Ty>>
	class optional_idx_vector 
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
			idx_vector<_Ty,_IdxTy,_VecTy> vector_m;
			bool has_value_m = false;

		public:
			optional_idx_vector() = default;
			optional_idx_vector( const optional_idx_vector &_other ) noexcept : vector_m( _other.vector_m ), has_value_m(_other.has_value_m) {}
			optional_idx_vector &operator = ( const optional_idx_vector &_other ) { this->vector_m = _other.vector_m; this->has_value_m = _other.has_value_m; return *this; }
			optional_idx_vector( optional_idx_vector &&_other ) noexcept : vector_m( std::move(_other.vector_m) ), has_value_m(_other.has_value_m) { _other.has_value_m = false; }
			optional_idx_vector &operator = ( optional_idx_vector &&_other ) { this->vector_m = std::move(_other.vector_m); this->has_value_m = _other.has_value_m; _other.has_value_m = false; return *this; }

			bool operator==( const optional_idx_vector &_other ) const;
			bool operator!=( const optional_idx_vector &_other ) const;

			void reset() { this->has_value_m = false; this->vector_m.clear(); }
			void set() { this->has_value_m = true; this->vector_m.clear(); }
			void set( const idx_vector<_Ty,_IdxTy,_VecTy> &_vector ) { this->has_value_m = true; this->vector_m = _vector; }
			void set( const values_vector_type &_values , index_vector_type &_index ) { this->has_value_m = true; this->vector_m.values() = _values; this->vector_m.index() = _index; }
			bool has_value() const { return this->has_value_m; }

			idx_vector<_Ty, _IdxTy, _VecTy> &vector() { if( !this->has_value_m ) { throw ctle::bad_optional_value_access("optional_idx_vector has no value"); } return this->vector_m; }
			const idx_vector<_Ty,_IdxTy,_VecTy> &vector() const { if( !this->has_value_m ) { throw ctle::bad_optional_value_access("optional_idx_vector has no value"); } return this->vector_m; }

			values_vector_type &values() { return this->vector().values(); }
			const values_vector_type &values() const { return this->vector().values(); }

			index_vector_type &index() { return this->vector().index(); }
			const index_vector_type &index() const { return this->vector().index(); }
		};

	template <class _Ty, class _Alloc, class _IdxAlloc>
	bool optional_idx_vector<_Ty,_Alloc,_IdxAlloc>::operator==( const optional_idx_vector &_other ) const
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

	template <class _Ty, class _Alloc, class _IdxAlloc>
	bool optional_idx_vector<_Ty,_Alloc,_IdxAlloc>::operator!=( const optional_idx_vector &_other ) const
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

	};