// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <unordered_map>
#include <mutex>

namespace ctle
	{
	// bi-directional unordered_map, so has (on average) O(1) lookup of both key->value and value->key
	// both key and value are stored directly, and needs to have std::hash functors defined
	// (currently implemented using 2 maps, and duplicates the memory used. should change this to a shared memory layout)
	template<class _Kty, class _Vty> class bimap
		{
		private:
			using map_type = std::unordered_map<_Kty, _Vty>;
			using rmap_type = std::unordered_map<_Vty, _Kty>;
			using key_type = _Kty;
			using value_type = _Vty;
			using iterator = typename map_type::iterator;
			using riterator = typename rmap_type::iterator;

			map_type Fwd;
			rmap_type Rev;

		public:
			using const_iterator = typename map_type::const_iterator;

			// just default ctor/dtor and assignments
			bimap() = default;
			bimap( const bimap &other ) = default;
			bimap &operator=( const bimap &other ) = default;
			bimap( bimap &&other ) = default;
			bimap &operator=( bimap &&other ) = default;
			~bimap() = default;

			// only need to compare one of the maps, since the other is just a reverse lookup
			bool operator==( const bimap &other ) const noexcept { return (this->Fwd == other.Fwd); }
			bool operator!=( const bimap &other ) const noexcept { return !operator==( other ); }

			// returns the constant iterator to the beginning of the forward mapping key->value
			const_iterator begin() const noexcept
				{
				return this->Fwd.begin();
				}

			// returns the constant iterator to beyond the end of the forward mapping key->value
			const_iterator end() const noexcept
				{
				return this->Fwd.end();
				}

			// returns true if key exists in map
			bool contains_key( const _Kty &key ) const noexcept
				{
				return this->Fwd.find( key ) != this->Fwd.end();
				}

			// returns true if value exists in map
			bool contains_value( const _Vty &value ) const noexcept
				{
				return this->Rev.find( value ) != this->Rev.end();
				}

			// returs the value mapped at key (will throw std::out_of_bounds if key does not exist)
			const _Vty & at_key( const _Kty &key ) const
				{
				return this->Fwd.at( key );
				}

			// retuns the key that maps to value (will throw std::out_of_bounds if value does not exist)
			const _Kty & at_value( const _Vty &value ) const
				{
				return this->Rev.at( value );
				}

			// gets the value at key, IF the key exists (the returned bool is false otherwise)
			std::pair<_Vty, bool> get_value( const _Kty &key ) const noexcept
				{
				const iterator it = this->Fwd.find( key );
				if( it != this->Fwd.end() )
					{
					return std::make_pair( it->second, true );
					}
				return std::make_pair( _Vty() , false );
				}

			// gets the key that maps to value, IF the value exists (the returned bool is false otherwise)
			std::pair<_Kty, bool> get_key( const _Vty &value ) const noexcept
				{
				const riterator it = this->Rev.find( value );
				if( it != this->Rev.end() )
					{
					return std::make_pair( it->second, true );
					}
				return std::make_pair( _Vty() , false );
				}

			// clear the map
			void clear() noexcept
				{
				this->Fwd.clear();
				this->Rev.clear();
				}

			// inserts or replaces the key/value pair
			// Note: if the key or value already exist in the bimap, they are removed along with the values which they are mapped to.
			void insert( const _Kty &key , const _Vty &value ) noexcept
				{
				// make sure neither value is already in the map
				erase_by_key( key );
				erase_by_value( value );

				// insert into maps
				this->Fwd.emplace( key, value );
				this->Rev.emplace( value, key );
				}

			// remove by key
			size_t erase_by_key( const _Kty &key ) noexcept
				{
				iterator it = this->Fwd.find( key );
				if( it != this->Fwd.end() )
					{
					riterator rit = this->Rev.find( it->second );
					this->Fwd.erase( it );
					if( rit != this->Rev.end() )
						{
						this->Rev.erase( rit );
						}
					return 1;
					}
				return 0;
				}

			// remove by value
			size_t erase_by_value( const _Vty &value ) noexcept
				{
				riterator rit = this->Rev.find( value );
				if( rit != this->Rev.end() )
					{
					iterator it = this->Fwd.find( rit->second );
					this->Rev.erase( rit );
					if( it != this->Fwd.end() )
						{
						this->Fwd.erase( it );
						}
					return 1;
					}
				return 0;
				}

			// returns the number of mappings (key->value pairs)
			size_t size() const noexcept
				{
				return this->Fwd.size();
				}
		};
	};
