// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_PROP_H_
#define _CTLE_PROP_H_

#include <memory>
#include <atomic>
#include <functional>

#include "status_error.h"
#include "util.h"

namespace ctle
{
	    // The prop_[...] template classes are a convenient way to implement properties in classes, where each property can be accessed as a 
    // normal variable, but can also be made read-only, write-only, read/write, and let the owner class override if a value is returned 
    // from a variable, or evaluated on-the-fly, etc.     
         
	class prop
	{
	public:
		// types of property specializations
		enum class type
		{
			cpy_value,		// return a copy, direct stored value
			cref_value,		// return a const ref, direct stored value
			cpy_atomic,		// return a copy, atomic-access value
			cref_atomic,	// return a const ref, atomic-access value
			cpy_ptr,		// return a copy, value stored as a unique ptr
			cref_ptr,		// return a const ref, value stored as a unique ptr
			cptr_ptr,		// return a const pointer, value stored as a unique ptr
			cpy_custom,		// return a copy, custom value handled by the owner 
			cref_custom,	// return a const ref, custom value handled by the owner 
			cptr_custom,	// return a const pointer, custom value handled by the owner 
		};
		
		// const aliases, to directly use enum value
		static constexpr const type cpy_value = type::cpy_value;		// return a copy, direct stored value
		static constexpr const type cref_value = type::cref_value;		// return a const ref, direct stored value
		static constexpr const type cpy_atomic = type::cpy_atomic;		// return a copy, atomic-access value
		static constexpr const type cref_atomic = type::cref_atomic;	// return a const ref, atomic-access value
		static constexpr const type cpy_ptr = type::cpy_ptr;			// return a copy, value stored as a unique ptr
		static constexpr const type cref_ptr = type::cref_ptr;			// return a const ref, value stored as a unique ptr
		static constexpr const type cptr_ptr = type::cptr_ptr;			// return a const pointer, value stored as a unique ptr
		static constexpr const type cpy_custom = type::cpy_custom;		// return a copy, custom value handled by the owner 
		static constexpr const type cref_custom = type::cref_custom;	// return a const ref, custom value handled by the owner 
		static constexpr const type cptr_custom = type::cptr_custom;	// return a const pointer, custom value handled by the owner 

		// default types for when return type is not defined. defaults to const ref'd, and should be used as parameter by all set-only templates. 
		static constexpr const type value = type::cref_value;
		static constexpr const type atomic = type::cref_atomic;
		static constexpr const type ptr = type::cref_ptr;
		static constexpr const type custom = type::cref_custom;
	};

	// prop_[...] base templates, get, get/set or set types. defaults to const ref'd direct values if no _type parameter is specified
	template<class _Ty, prop::type _type=prop::value> class prop_get;
	template<class _Ty, prop::type _type=prop::value> class prop_get_set;
	template<class _Ty, prop::type _type=prop::value> class prop_set;

	// prop_[...] template specializations follow below

	template<class _Ty>
	class prop_get<_Ty,prop::cpy_value>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( _Ty &_value ) : value(_value)
			{
				// set a default value IF the value type does not automatically set a default value
				identity_assign_if_trivially_default_constructible<_Ty>( _value );
			}

			// property standard ctor with specific assignment of default value
			prop_get( _Ty &_value, const _Ty &_initial_value ) : value(_value)
			{
				// set an initial value to the object
				this->value = _initial_value;
			}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value;
			}

			// property get method which returns _Ty
			// - note: does not raise any exceptions
			_Ty get() const noexcept
			{
				return this->value;
			}

			// property cast to _Ty operator
			// - note: does not raise any exceptions
			operator _Ty() const noexcept
			{
				return this->value;
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			_Ty &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cpy_value>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( _Ty &_value ) : value(_value)
			{
				// set a default value IF the value type does not automatically set a default value
				identity_assign_if_trivially_default_constructible<_Ty>( _value );
			}

			// property standard ctor with specific assignment of default value
			prop_get_set( _Ty &_value, const _Ty &_initial_value ) : value(_value)
			{
				// set an initial value to the object
				this->value = _initial_value;
			}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value;
			}

			// property get method which returns _Ty
			// - note: does not raise any exceptions
			_Ty get() const noexcept
			{
				return this->value;
			}

			// property cast to _Ty operator
			// - note: does not raise any exceptions
			operator _Ty() const noexcept
			{
				return this->value;
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				this->value = _value;
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				this->value = _value;
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			_Ty &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cref_value>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( _Ty &_value ) : value(_value)
			{
				// set a default value IF the value type does not automatically set a default value
				identity_assign_if_trivially_default_constructible<_Ty>( _value );
			}

			// property standard ctor with specific assignment of default value
			prop_get( _Ty &_value, const _Ty &_initial_value ) : value(_value)
			{
				// set an initial value to the object
				this->value = _initial_value;
			}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value;
			}

			// property get method which returns const _Ty &
			// - note: does not raise any exceptions
			const _Ty & get() const noexcept
			{
				return this->value;
			}

			// property cast to const _Ty & operator
			// - note: does not raise any exceptions
			operator const _Ty &() const noexcept
			{
				return this->value;
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			_Ty &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cref_value>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( _Ty &_value ) : value(_value)
			{
				// set a default value IF the value type does not automatically set a default value
				identity_assign_if_trivially_default_constructible<_Ty>( _value );
			}

			// property standard ctor with specific assignment of default value
			prop_get_set( _Ty &_value, const _Ty &_initial_value ) : value(_value)
			{
				// set an initial value to the object
				this->value = _initial_value;
			}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value;
			}

			// property get method which returns const _Ty &
			// - note: does not raise any exceptions
			const _Ty & get() const noexcept
			{
				return this->value;
			}

			// property cast to const _Ty & operator
			// - note: does not raise any exceptions
			operator const _Ty &() const noexcept
			{
				return this->value;
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				this->value = _value;
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				this->value = _value;
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			_Ty &value;
	};

	template<class _Ty>
	class prop_set<_Ty,prop::value>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_set( _Ty &_value ) : value(_value)
			{
				// set a default value IF the value type does not automatically set a default value
				identity_assign_if_trivially_default_constructible<_Ty>( _value );
			}

			// property standard ctor with specific assignment of default value
			prop_set( _Ty &_value, const _Ty &_initial_value ) : value(_value)
			{
				// set an initial value to the object
				this->value = _initial_value;
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				this->value = _value;
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_set & operator = ( const _Ty & _value ) noexcept
			{
				this->value = _value;
				return *this;
			}

		private:
			prop_set() = delete;
			prop_set( const prop_set & ) = delete;
			const prop_set & operator= ( const prop_set & ) = delete;
			prop_set( prop_set && ) = delete;
			const prop_set & operator= ( prop_set && ) = delete;

			_Ty &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cpy_atomic>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( std::atomic<_Ty> &_value ) : value(_value) {}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value.load();
			}

			// property get method which returns _Ty
			// - note: does not raise any exceptions
			_Ty get() const noexcept
			{
				return this->value.load();
			}

			// property cast to _Ty operator
			// - note: does not raise any exceptions
			operator _Ty() const noexcept
			{
				return this->value.load();
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			std::atomic<_Ty> &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cpy_atomic>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( std::atomic<_Ty> &_value ) : value(_value) {}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value.load();
			}

			// property get method which returns _Ty
			// - note: does not raise any exceptions
			_Ty get() const noexcept
			{
				return this->value.load();
			}

			// property cast to _Ty operator
			// - note: does not raise any exceptions
			operator _Ty() const noexcept
			{
				return this->value.load();
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				this->value.store( _value );
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				this->value.store( _value );
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			std::atomic<_Ty> &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cref_atomic>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( std::atomic<_Ty> &_value ) : value(_value) {}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value.load();
			}

			// property get method which returns const _Ty &
			// - note: does not raise any exceptions
			const _Ty & get() const noexcept
			{
				return this->value.load();
			}

			// property cast to const _Ty & operator
			// - note: does not raise any exceptions
			operator const _Ty &() const noexcept
			{
				return this->value.load();
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			std::atomic<_Ty> &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cref_atomic>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( std::atomic<_Ty> &_value ) : value(_value) {}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				result = status::ok;
				return this->value.load();
			}

			// property get method which returns const _Ty &
			// - note: does not raise any exceptions
			const _Ty & get() const noexcept
			{
				return this->value.load();
			}

			// property cast to const _Ty & operator
			// - note: does not raise any exceptions
			operator const _Ty &() const noexcept
			{
				return this->value.load();
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				this->value.store( _value );
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				this->value.store( _value );
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			std::atomic<_Ty> &value;
	};

	template<class _Ty>
	class prop_set<_Ty,prop::atomic>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_set( std::atomic<_Ty> &_value ) : value(_value) {}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				this->value.store( _value );
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_set & operator = ( const _Ty & _value ) noexcept
			{
				this->value.store( _value );
				return *this;
			}

		private:
			prop_set() = delete;
			prop_set( const prop_set & ) = delete;
			const prop_set & operator= ( const prop_set & ) = delete;
			prop_set( prop_set && ) = delete;
			const prop_set & operator= ( prop_set && ) = delete;

			std::atomic<_Ty> &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cpy_ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				if( !(this->value) )
				{
					result = status::invalid;
					return nil_object::ref<_Ty>();
				}
				result = status::ok;
				return *(this->value.get());
			}

			// property get method which returns _Ty
			// - note: raises a status_exception if the call fails
			_Ty get() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

			// property cast to _Ty operator
			// - note: raises a status_exception if the call fails
			operator _Ty() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cpy_ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				if( !(this->value) )
				{
					result = status::invalid;
					return nil_object::ref<_Ty>();
				}
				result = status::ok;
				return *(this->value.get());
			}

			// property get method which returns _Ty
			// - note: raises a status_exception if the call fails
			_Ty get() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

			// property cast to _Ty operator
			// - note: raises a status_exception if the call fails
			operator _Ty() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return status::ok;
			}

			// property set method which sets the value using a const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return *this;
			}

			// pointer operator which assigns the property to a const _Ty *
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cref_ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				if( !(this->value) )
				{
					result = status::invalid;
					return nil_object::ref<const _Ty &>();
				}
				result = status::ok;
				return *(this->value.get());
			}

			// property get method which returns const _Ty &
			// - note: raises a status_exception if the call fails
			const _Ty & get() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

			// property cast to const _Ty & operator
			// - note: raises a status_exception if the call fails
			operator const _Ty &() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cref_ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				if( !(this->value) )
				{
					result = status::invalid;
					return nil_object::ref<const _Ty &>();
				}
				result = status::ok;
				return *(this->value.get());
			}

			// property get method which returns const _Ty &
			// - note: raises a status_exception if the call fails
			const _Ty & get() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

			// property cast to const _Ty & operator
			// - note: raises a status_exception if the call fails
			operator const _Ty &() const
			{
				if( !(this->value) )
				{
					throw ctle::status_error( status::invalid );
				}
				return *(this->value.get());
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return status::ok;
			}

			// property set method which sets the value using a const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return *this;
			}

			// pointer operator which assigns the property to a const _Ty *
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cptr_ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property get method which returns const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty * get( status &result ) const noexcept
			{
				if( !(this->value) )
				{
					result = status::invalid;
					return nil_object::ref<const _Ty *>();
				}
				result = status::ok;
				return this->value.get();
			}

			// property get method which returns const _Ty *
			// - note: does not raise any exceptions
			const _Ty * get() const noexcept
			{
				return this->value.get();
			}

			// property cast to const _Ty * operator
			// - note: does not raise any exceptions
			operator const _Ty *() const noexcept
			{
				return this->value.get();
			}

			// pointer operator which returns const _Ty *
			// - note: does not raise any exceptions
			const _Ty * operator -> () const noexcept
			{
				return this->value.get();
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cptr_ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_get_set( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property get method which returns const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty * get( status &result ) const noexcept
			{
				if( !(this->value) )
				{
					result = status::invalid;
					return nil_object::ref<const _Ty *>();
				}
				result = status::ok;
				return this->value.get();
			}

			// property get method which returns const _Ty *
			// - note: does not raise any exceptions
			const _Ty * get() const noexcept
			{
				return this->value.get();
			}

			// property cast to const _Ty * operator
			// - note: does not raise any exceptions
			operator const _Ty *() const noexcept
			{
				return this->value.get();
			}

			// pointer operator which returns const _Ty *
			// - note: does not raise any exceptions
			const _Ty * operator -> () const noexcept
			{
				return this->value.get();
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return status::ok;
			}

			// property set method which sets the value using a const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return *this;
			}

			// pointer operator which assigns the property to a const _Ty *
			// - note: does not raise any exceptions
			const prop_get_set & operator = ( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_set<_Ty,prop::ptr>
	{
		public:
			using value_type = _Ty;

			// property standard ctor
			prop_set( std::unique_ptr<_Ty> &_value ) : value(_value) {}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return status::ok;
			}

			// property set method which sets the value using a const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return status::ok;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: does not raise any exceptions
			const prop_set & operator = ( const _Ty & _value ) noexcept
			{
				if( this->value )
				{
					*(this->value.get()) = _value; // in-place replace value
				}
				else
				{
					this->value = std::unique_ptr<_Ty>( new _Ty(_value) ); // create new object, copy ctor
				}
				return *this;
			}

			// pointer operator which assigns the property to a const _Ty *
			// - note: does not raise any exceptions
			const prop_set & operator = ( const _Ty * _value ) noexcept
			{
				if( _value )
				{
					if( this->value )
					{
						*(this->value.get()) = *_value; // in-place replace value
					}
					else
					{
						this->value = std::unique_ptr<_Ty>( new _Ty(*_value) ); // create new object, copy ctor
					}
				}
				else
				{
					this->value.reset(); // remove value
				}
				return *this;
			}

		private:
			prop_set() = delete;
			prop_set( const prop_set & ) = delete;
			const prop_set & operator= ( const prop_set & ) = delete;
			prop_set( prop_set && ) = delete;
			const prop_set & operator= ( prop_set && ) = delete;

			std::unique_ptr<_Ty> &value;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cpy_custom>
	{
		public:
			using value_type = _Ty;
			using _get_type = const std::function<_Ty (const prop_get *, status & )>;

			// property standard ctor
			prop_get( _get_type _get ) : get_method(_get) {}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				result = status::ok; // default to ok, let method update
				_Ty ret_val = this->get_method(this, result);
				return ret_val;
			}

			// property get method which returns _Ty
			// - note: raises a status_exception if the call fails
			_Ty get() const
			{
				status result = status::ok; // default to ok, let method update
				_Ty ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property cast to _Ty operator
			// - note: raises a status_exception if the call fails
			operator _Ty() const
			{
				status result = status::ok; // default to ok, let method update
				_Ty ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			_get_type get_method;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cpy_custom>
	{
		public:
			using value_type = _Ty;
			using _get_type = const std::function<_Ty (const prop_get_set *, status & )>;
			using _set_type = const std::function<status (prop_get_set *, const _Ty &)>;

			// property standard ctor
			prop_get_set( _get_type _get, _set_type _set ) : get_method(_get), set_method(_set) {}

			// property get method which returns _Ty
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			_Ty get( status &result ) const noexcept
			{
				result = status::ok; // default to ok, let method update
				_Ty ret_val = this->get_method(this, result);
				return ret_val;
			}

			// property get method which returns _Ty
			// - note: raises a status_exception if the call fails
			_Ty get() const
			{
				status result = status::ok; // default to ok, let method update
				_Ty ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property cast to _Ty operator
			// - note: raises a status_exception if the call fails
			operator _Ty() const
			{
				status result = status::ok; // default to ok, let method update
				_Ty ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				status result = this->set_method(this, _value);
				return result;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: raises a status_exception if the call fails
			const prop_get_set & operator = ( const _Ty & _value )
			{
				status result = this->set_method(this, _value);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			_get_type get_method;
			_set_type set_method;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cref_custom>
	{
		public:
			using value_type = _Ty;
			using _get_type = const std::function<const _Ty & (const prop_get *, status & )>;

			// property standard ctor
			prop_get( _get_type _get ) : get_method(_get) {}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				result = status::ok; // default to ok, let method update
				const _Ty & ret_val = this->get_method(this, result);
				return ret_val;
			}

			// property get method which returns const _Ty &
			// - note: raises a status_exception if the call fails
			const _Ty & get() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty & ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property cast to const _Ty & operator
			// - note: raises a status_exception if the call fails
			operator const _Ty &() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty & ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			_get_type get_method;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cref_custom>
	{
		public:
			using value_type = _Ty;
			using _get_type = const std::function<const _Ty & (const prop_get_set *, status & )>;
			using _set_type = const std::function<status (prop_get_set *, const _Ty &)>;

			// property standard ctor
			prop_get_set( _get_type _get, _set_type _set ) : get_method(_get), set_method(_set) {}

			// property get method which returns const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty & get( status &result ) const noexcept
			{
				result = status::ok; // default to ok, let method update
				const _Ty & ret_val = this->get_method(this, result);
				return ret_val;
			}

			// property get method which returns const _Ty &
			// - note: raises a status_exception if the call fails
			const _Ty & get() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty & ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property cast to const _Ty & operator
			// - note: raises a status_exception if the call fails
			operator const _Ty &() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty & ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				status result = this->set_method(this, _value);
				return result;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: raises a status_exception if the call fails
			const prop_get_set & operator = ( const _Ty & _value )
			{
				status result = this->set_method(this, _value);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			_get_type get_method;
			_set_type set_method;
	};

	template<class _Ty>
	class prop_get<_Ty,prop::cptr_custom>
	{
		public:
			using value_type = _Ty;
			using _get_type = const std::function<const _Ty * (const prop_get *, status & )>;

			// property standard ctor
			prop_get( _get_type _get ) : get_method(_get) {}

			// property get method which returns const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty * get( status &result ) const noexcept
			{
				result = status::ok; // default to ok, let method update
				const _Ty * ret_val = this->get_method(this, result);
				return ret_val;
			}

			// property get method which returns const _Ty *
			// - note: raises a status_exception if the call fails
			const _Ty * get() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty * ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property cast to const _Ty * operator
			// - note: raises a status_exception if the call fails
			operator const _Ty *() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty * ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

		private:
			prop_get() = delete;
			prop_get( const prop_get & ) = delete;
			const prop_get & operator= ( const prop_get & ) = delete;
			prop_get( prop_get && ) = delete;
			const prop_get & operator= ( prop_get && ) = delete;

			_get_type get_method;
	};

	template<class _Ty>
	class prop_get_set<_Ty,prop::cptr_custom>
	{
		public:
			using value_type = _Ty;
			using _get_type = const std::function<const _Ty * (const prop_get_set *, status & )>;
			using _set_type = const std::function<status (prop_get_set *, const _Ty &)>;

			// property standard ctor
			prop_get_set( _get_type _get, _set_type _set ) : get_method(_get), set_method(_set) {}

			// property get method which returns const _Ty *
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			const _Ty * get( status &result ) const noexcept
			{
				result = status::ok; // default to ok, let method update
				const _Ty * ret_val = this->get_method(this, result);
				return ret_val;
			}

			// property get method which returns const _Ty *
			// - note: raises a status_exception if the call fails
			const _Ty * get() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty * ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property cast to const _Ty * operator
			// - note: raises a status_exception if the call fails
			operator const _Ty *() const
			{
				status result = status::ok; // default to ok, let method update
				const _Ty * ret_val = this->get_method(this, result);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return ret_val;
			}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				status result = this->set_method(this, _value);
				return result;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: raises a status_exception if the call fails
			const prop_get_set & operator = ( const _Ty & _value )
			{
				status result = this->set_method(this, _value);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return *this;
			}

		private:
			prop_get_set() = delete;
			prop_get_set( const prop_get_set & ) = delete;
			const prop_get_set & operator= ( const prop_get_set & ) = delete;
			prop_get_set( prop_get_set && ) = delete;
			const prop_get_set & operator= ( prop_get_set && ) = delete;

			_get_type get_method;
			_set_type set_method;
	};

	template<class _Ty>
	class prop_set<_Ty,prop::custom>
	{
		public:
			using value_type = _Ty;
			using _set_type = const std::function<status (prop_set *, const _Ty &)>;

			// property standard ctor
			prop_set( _set_type _set ) : set_method(_set) {}

			// property set method which sets the value using a const _Ty &
			// - note: sets the result of the call in the result status parameter
			// - note: does not raise any exceptions
			status set( const _Ty & _value ) noexcept
			{
				status result = this->set_method(this, _value);
				return result;
			}

			// pointer operator which assigns the property to a const _Ty &
			// - note: raises a status_exception if the call fails
			const prop_set & operator = ( const _Ty & _value )
			{
				status result = this->set_method(this, _value);
				if( result != status::ok )
				{
					throw ctle::status_error( result );
				}
				return *this;
			}

		private:
			prop_set() = delete;
			prop_set( const prop_set & ) = delete;
			const prop_set & operator= ( const prop_set & ) = delete;
			prop_set( prop_set && ) = delete;
			const prop_set & operator= ( prop_set && ) = delete;

			_set_type set_method;
	};

}

#endif//_CTLE_PROP_H_
