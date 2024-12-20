// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once
#ifndef _CTLE_BIMAP_H_
#define _CTLE_BIMAP_H_

#include <unordered_map>
#include <mutex>

namespace ctle
{

/**
 * @brief Bi-directional unordered_map, providing O(1) average lookup for both key->value and value->key.
 * 
 * This class stores both key and value directly and requires std::hash functors to be defined for both.
 * Currently implemented using two maps, which duplicates the memory used. This should be changed to a shared memory layout.
 * 
 * @tparam _Kty Type of the key.
 * @tparam _Vty Type of the value.
 */
template<class _Kty, class _Vty> class bimap
{
private:
    using map_type = std::unordered_map<_Kty, _Vty>;
    using rmap_type = std::unordered_map<_Vty, _Kty>;
    using key_type = _Kty;
    using value_type = _Vty;
    using iterator = typename map_type::iterator;
    using riterator = typename rmap_type::iterator;

    map_type Fwd; ///< Forward map from key to value.
    rmap_type Rev; ///< Reverse map from value to key.

public:
    using const_iterator = typename map_type::const_iterator;

    /**
     * @brief Default constructor.
     */
    bimap() = default;

    /**
     * @brief Copy constructor.
     * 
     * @param other Another bimap to copy from.
     */
    bimap( const bimap &other ) = default;

    /**
     * @brief Copy assignment operator.
     * 
     * @param other Another bimap to copy from.
     * @return Reference to this bimap.
     */
    bimap &operator=( const bimap &other ) = default;

    /**
     * @brief Move constructor.
     * 
     * @param other Another bimap to move from.
     */
    bimap( bimap &&other ) = default;

    /**
     * @brief Move assignment operator.
     * 
     * @param other Another bimap to move from.
     * @return Reference to this bimap.
     */
    bimap &operator=( bimap &&other ) = default;

    /**
     * @brief Destructor.
     */
    ~bimap() = default;

    /**
     * @brief Equality operator.
     * 
     * Compares this bimap with another for equality. Only the forward map is compared, as the reverse map is just a reverse lookup.
     * 
     * @param other Another bimap to compare with.
     * @return True if the bimaps are equal, false otherwise.
     */
    bool operator==( const bimap &other ) const noexcept
    {
        return ( this->Fwd == other.Fwd );
    }

    /**
     * @brief Inequality operator.
     * 
     * Compares this bimap with another for inequality.
     * 
     * @param other Another bimap to compare with.
     * @return True if the bimaps are not equal, false otherwise.
     */
    bool operator!=( const bimap &other ) const noexcept
    {
        return !operator==( other );
    }

    /**
     * @brief Returns a constant iterator to the beginning of the forward mapping key->value.
     * 
     * @return Constant iterator to the beginning of the forward map.
     */
    const_iterator begin() const noexcept
    {
        return this->Fwd.begin();
    }

    /**
     * @brief Returns a constant iterator to beyond the end of the forward mapping key->value.
     * 
     * @return Constant iterator to beyond the end of the forward map.
     */
    const_iterator end() const noexcept
    {
        return this->Fwd.end();
    }

    /**
     * @brief Checks if a key exists in the map.
     * 
     * @param key The key to check.
     * @return True if the key exists, false otherwise.
     */
    bool contains_key( const _Kty &key ) const noexcept
    {
        return this->Fwd.find( key ) != this->Fwd.end();
    }

    /**
     * @brief Checks if a value exists in the map.
     * 
     * @param value The value to check.
     * @return True if the value exists, false otherwise.
     */
    bool contains_value( const _Vty &value ) const noexcept
    {
        return this->Rev.find( value ) != this->Rev.end();
    }

    /**
     * @brief Returns the value mapped to a key.
     * 
     * @param key The key to look up.
     * @return The value mapped to the key.
     * @throws std::out_of_range if the key does not exist.
     */
    const _Vty &at_key( const _Kty &key ) const
    {
        return this->Fwd.at( key );
    }

    /**
     * @brief Returns the key that maps to a value.
     * 
     * @param value The value to look up.
     * @return The key that maps to the value.
     * @throws std::out_of_range if the value does not exist.
     */
    const _Kty &at_value( const _Vty &value ) const
    {
        return this->Rev.at( value );
    }

    /**
     * @brief Gets the value mapped to a key, if the key exists.
     * 
     * @param key The key to look up.
     * @return A pair containing the value and a boolean indicating if the key exists.
     */
    std::pair<_Vty, bool> get_value( const _Kty &key ) const noexcept
    {
        auto it = this->Fwd.find( key );
        if( it != this->Fwd.end() )
        {
            return std::make_pair( it->second, true );
        }
        return std::make_pair( _Vty(), false );
    }

    /**
     * @brief Gets the key that maps to a value, if the value exists.
     * 
     * @param value The value to look up.
     * @return A pair containing the key and a boolean indicating if the value exists.
     */
    std::pair<_Kty, bool> get_key( const _Vty &value ) const noexcept
    {
        auto it = this->Rev.find( value );
        if( it != this->Rev.end() )
        {
            return std::make_pair( it->second, true );
        }
        return std::make_pair( _Kty(), false );
    }

    /**
     * @brief Clears the map.
     */
    void clear() noexcept
    {
        this->Fwd.clear();
        this->Rev.clear();
    }

    /**
     * @brief Inserts or replaces a key/value pair.
     * 
     * If the key or value already exist in the bimap, they are removed along with the values they are mapped to.
     * 
     * @param key The key to insert.
     * @param value The value to insert.
     */
    void insert( const _Kty &key, const _Vty &value ) noexcept
    {
        // Make sure neither value is already in the map.
        erase_by_key( key );
        erase_by_value( value );

        // Insert into maps.
        this->Fwd.emplace( key, value );
        this->Rev.emplace( value, key );
    }

    /**
     * @brief Removes a key from the map.
     * 
     * @param key The key to remove.
     * @return The number of elements removed.
     */
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

    /**
     * @brief Removes a value from the map.
     * 
     * @param value The value to remove.
     * @return The number of elements removed.
     */
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

    /**
     * @brief Returns the number of mappings (key->value pairs).
     * 
     * @return The number of mappings.
     */
    size_t size() const noexcept
    {
        return this->Fwd.size();
    }
};

}
//namespace ctle

#endif//_CTLE_BIMAP_H_
