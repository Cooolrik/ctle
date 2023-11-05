// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <memory>
#include <string>
#include <stdexcept>

namespace ctle
	{
	class bad_optional_value_access : public std::runtime_error
		{
		public:
			explicit bad_optional_value_access(char const* const _Message) noexcept : std::runtime_error(_Message) {}
		};

	template<class T> class optional_value
		{
		protected:
			std::unique_ptr<T> value_m;

		public:
			optional_value() = default;
			optional_value( const T &_value ) : value_m( new T( _value ) ) {}
			optional_value( const optional_value &other ) noexcept : value_m( other.value_m ? std::unique_ptr<T>( new T(*other.value_m) ) : nullptr ) {}
			optional_value &operator = ( const optional_value &_other ) { this->value_m = _other.value_m ? std::unique_ptr<T>( new T(*_other.value_m) ) : nullptr; return *this; }
			optional_value( optional_value &&other ) noexcept : value_m( std::move( other.value_m ) ) {}
			optional_value &operator = ( optional_value &&_other ) { this->value_m = std::move( _other.value_m ); return *this; }

			bool operator==( const T &_other ) const;
			bool operator!=( const T &_other ) const;
			bool operator==( const optional_value &_other ) const;
			bool operator!=( const optional_value &_other ) const;

			void reset() { this->value_m.reset(); }
			void set( const T &_value = {} ) { this->value_m = std::unique_ptr<T>( new T(_value) ); }
			bool has_value() const noexcept { return bool( this->value_m ); }

			T &value() { if( !this->has_value() ) { throw ctle::bad_optional_value_access("optional_value has no value"); } return *(this->value_m); }
			const T &value() const { if( !this->has_value() ) { throw ctle::bad_optional_value_access("optional_value has no value"); } return *(this->value_m); }

			operator T &() { return value(); }
			operator const T &() const { return value(); }
		};

	template<class T> 
	bool optional_value<T>::operator==( const T &_other ) const
		{
		if( !this->value_m )
			return false;
		return *(this->value_m) == _other;
		}

	template<class T> 
	bool optional_value<T>::operator!=( const T &_other ) const
		{
		if( !this->value_m )
			return true;
		return *(this->value_m) != _other;
		}

	template<class T> 
	bool optional_value<T>::operator==( const optional_value &_other ) const
		{
		if( this->value_m )
			{
			if( _other.value_m )
				return *(this->value_m) == *(_other.value_m);
			return false;
			}
		else
			{
			if( _other.value_m )
				return false;
			return true;
			}
		}

	template<class T> 
	bool optional_value<T>::operator!=( const optional_value &_other ) const
		{
		if( this->value_m )
			{
			if( _other.value_m )
				return *(this->value_m) != *(_other.value_m);
			return true;
			}
		else
			{
			if( _other.value_m )
				return true;
			return false;
			}
		}


	};