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
		static_assert( std::is_trivial<_StatusType>() , "_StatusType needs to be just a plain old value, like bool, int or enum" );

		private:
			_StatusType Status;
			_RetType Value;

			status_return() = delete;

		public:
			status_return( const status_return &other ) noexcept : Status(other.Status) , Value(other.Value) {};
			const status_return & operator = ( const status_return &other )
				{
				this->Status = other.status;
				this->Value = other.value;
				}

			status_return( status_return &&other ) noexcept : Status(other.Status) , Value(std::move(other.Value)) {};
			const status_return & operator = ( status_return &&other )
				{
				this->Status = other.Status;
				this->Value = std::move(other.Value);
				}

			status_return( const _StatusType status, const _RetType &value ) : Status(status) , Value(value) {};
			status_return( const _StatusType status, _RetType &&value ) noexcept : Status(status) , Value(std::move(value)) {};

			// get the status/result
			_StatusType GetStatus() const { return this->Status; }
			
			// get a reference to the value returned
			_RetType &GetValue() { return this->Value; }
			const _RetType &GetValue() const { return this->Value; }
		};

	// partial implementation when we only have a _StatusType
	template<class _StatusType> class status_return<_StatusType,void>
		{
		static_assert( std::is_trivial<_StatusType>() , "_StatusType needs to be just a plain old value, like bool, int or enum" );

		private:
			_StatusType Status;

			status_return() = delete;

		public:
			status_return( const status_return &other ) noexcept : Status(other.Status) {};
			const status_return & operator = ( const status_return &other ) { this->Status = other.status; }

			status_return( status_return &&other ) noexcept : Status(other.Status) {};
			const status_return & operator = ( status_return &&other ) { this->Status = other.Status; }

			status_return( const _StatusType status ) : Status(status) {};

			// get the status/result
			_StatusType GetStatus() const { return this->Status; }
		};

	}