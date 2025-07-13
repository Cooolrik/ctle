// ctle Copyright (c) 2024 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_OPTIONAL_VALUE_H_
#define _CTLE_OPTIONAL_VALUE_H_

/// @file optional_value.h
/// @brief Contains the optional_value class template, kind of like std::optional, but available for all C++ versions since C++11.

#include <memory>
#include <stdexcept>

#include "fwd.h"

namespace ctle
{
class bad_optional_value_access : public std::runtime_error
{
public:
	explicit bad_optional_value_access( char const *const _Message ) noexcept : std::runtime_error( _Message ) {}
};

/// @brief optional_value: an optional value, which may be allocated or not. Uses global new for allocatimg the object.
/// @tparam _Ty The value type.
/// @tparam _PtrTy The pointer type to use, defaults to unique_ptr
template<
	class _Ty, 
	class _PtrTy /*= std::unique_ptr<_Ty>*/ 
> class optional_value
{
protected:
	_PtrTy value_m;

public:
	optional_value() = default;
	optional_value( const _Ty &_value ) : value_m( new _Ty( _value ) ) {}
	optional_value( const optional_value &other ) noexcept : value_m( other.value_m ? _PtrTy( new _Ty( *other.value_m ) ) : nullptr )	{}
	optional_value &operator = ( const optional_value &_other )	{ this->value_m = _other.value_m ? _PtrTy( new _Ty( *_other.value_m ) ) : nullptr; return *this; }
	optional_value( optional_value &&other ) noexcept : value_m( std::move( other.value_m ) ) {}
	optional_value &operator = ( optional_value &&_other ) { this->value_m = std::move( _other.value_m ); return *this;	}

	bool operator==( const _Ty &_other ) const;
	bool operator!=( const _Ty &_other ) const;
	bool operator==( const optional_value &_other ) const;
	bool operator!=( const optional_value &_other ) const;

	/// @brief Reset the optional_value. Removes the value. has_value() will return false.
	void reset() { this->value_m.reset(); }

	/// @brief Set the value of the optional_value. has_value() will return true.
	/// @param _value the value to set the optional_value to.
	void set(const _Ty& _value = {}) { this->value_m = _PtrTy(new _Ty(_value)); }

	/// @brief Check if the optional_value has a value.
	bool has_value() const noexcept { return bool(this->value_m); }

	/// @brief Get the value of the optional_value.
	_Ty& value()
	{
		if( !this->has_value() )
		{
			throw ctle::bad_optional_value_access( "optional_value has no value" );
		} return *( this->value_m );
	}

	/// @brief Get the value of the optional_value.
	const _Ty& value() const
	{
		if( !this->has_value() )
		{
			throw ctle::bad_optional_value_access( "optional_value has no value" );
		} return *( this->value_m );
	}

	/// @brief Get the value of the optional_value.
	operator _Ty& ()
	{
		return value();
	}

	/// @brief Get the value of the optional_value.
	operator const _Ty& () const
	{
		return value();
	}
};

template<class _Ty, class _PtrTy>
inline bool optional_value<_Ty,_PtrTy>::operator==( const _Ty &_other ) const
{
	if( !this->value_m )
		return false;
	return *( this->value_m ) == _other;
}

template<class _Ty, class _PtrTy>
inline bool optional_value<_Ty,_PtrTy>::operator!=( const _Ty &_other ) const
{
	if( !this->value_m )
		return true;
	return *( this->value_m ) != _other;
}

template<class _Ty, class _PtrTy>
inline bool optional_value<_Ty,_PtrTy>::operator==( const optional_value &_other ) const
{
	if( this->value_m )
	{
		if( _other.value_m )
			return *( this->value_m ) == *( _other.value_m );
		return false;
	}
	else
	{
		if( _other.value_m )
			return false;
		return true;
	}
}

template<class _Ty, class _PtrTy>
inline bool optional_value<_Ty,_PtrTy>::operator!=( const optional_value &_other ) const
{
	if( this->value_m )
	{
		if( _other.value_m )
			return *( this->value_m ) != *( _other.value_m );
		return true;
	}
	else
	{
		if( _other.value_m )
			return true;
		return false;
	}
}

}
//namespace ctle

#endif//_CTLE_OPTIONAL_VALUE_H_
