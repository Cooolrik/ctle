// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <unordered_map>
#include <mutex>

namespace ctle
	{
	// thread safe map, does not allow access to 
	// items, only insert, erase and find with value returned as copy 
	// all public methods are thread safe 
	template<class _Kty, class _Ty> class thread_safe_map
		{
		private:
			using map_type = std::unordered_map<_Kty, _Ty>;
			using key_type = _Kty;
			using mapped_type = _Ty;
			using iterator = typename map_type::iterator;
			using value_type = std::pair<const _Kty, _Ty>;

			map_type Data;
			std::mutex AccessMutex;

		public:
			std::pair<_Ty, bool> find( const _Kty &key )
				{
				std::lock_guard<std::mutex> guard( this->AccessMutex );
				iterator it = this->Data.find( key );
				if( it != this->Data.end() )
					{
					return std::make_pair( it->second, true );
					}
				return std::make_pair( _Ty(), false );
				}

			bool insert( const value_type &value )
				{
				std::lock_guard<std::mutex> guard( this->AccessMutex );
				return this->Data.insert( value ).second;
				}

			size_t erase( const _Kty &key )
				{
				std::lock_guard<std::mutex> guard( this->AccessMutex );
				return this->Data.erase( key );
				}

			size_t size() 
				{
				std::lock_guard<std::mutex> guard( this->AccessMutex );
				return this->Data.size();
				}
		};
	};
