// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

namespace ctle
	{
	// represents a pair of result/status and return type for 
	// functions which return a type along with a status 
	template<class _StatusType, class _RetType> class status_return;

	// general implementation
	template<class _StatusType, class _RetType> class status_return
		{
		static_assert( std::is_trivially_copyable<_StatusType>() , "_StatusType needs to be trivially copyable, like a plain old value, like bool, int or enum, or a class with a default copy ctor" );

		private:
			_StatusType sstatus = {};
			_RetType svalue = {};

			status_return() = delete;

		public:
			status_return( const status_return &other ) = default;
			status_return( status_return &&other ) = default;

			status_return( _StatusType _status ) : sstatus(_status) {}
			status_return( _StatusType _status, const _RetType &_value ) : sstatus(_status) , svalue(_value) {}
			status_return( _StatusType _status, _RetType &&_value ) noexcept : sstatus(_status) , svalue(std::move(_value)) {}
			status_return( const _RetType &_value ) : svalue(_value) {}
			status_return( _RetType &&_value ) noexcept : svalue(std::move(_value)) {}
			
			// returns the status as a bool, requires that status can be converted to a bool implicitly
			operator bool() const { return this->sstatus; }
			bool operator !() const { return !(this->sstatus); }

			// get the status/result
			_StatusType status() const { return this->sstatus; }
			
			// get a reference to the value returned
			_RetType &value() { return this->svalue; }
			const _RetType &value() const { return this->svalue; }
		};

	// partial implementation when we only have a _StatusType
	template<class _StatusType> class status_return<_StatusType,void>
		{
		static_assert( std::is_trivially_copyable<_StatusType>() , "_StatusType needs to be trivially copyable, like a plain old value, like bool, int or enum, or a class with a default copy ctor" );

		private:
			_StatusType sstatus = {};

			status_return() = delete;

		public:
			status_return( const status_return &other ) = default;
			status_return( status_return &&other ) = default;

			status_return( const _StatusType _status ) : sstatus(_status) {};

			// returns the status as a bool, requires that status can be converted to a bool implicitly
			operator bool() const { return this->sstatus; }
			bool operator !() const { return !(this->sstatus); }

			// get the status/result
			_StatusType status() const { return this->sstatus; }
		};

	}