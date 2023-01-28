// ctle Copyright (c) 2021 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
#pragma once

#include <mutex>
#include <cstring>
#include <array>

#include "string_funcs.h"

namespace ctle
	{
    // uuid implements a portable, variant 1, version 4 (RNG generated) of uuid
    struct uuid
        {
            union
                {   
                uint64_t _data_q[2] = {};
                uint8_t data[16];
                };
            
            static const uuid nil;

            // generates an uuid using the mt19937 seeded from random_device
            // the call is thread-safe and uses a mutex to avoid collisions
            static uuid generate();
        };

    template <> std::string value_to_hex_string<uuid>( uuid value );
    template <> uuid hex_string_to_value<uuid>( const char *hex_string );
    }

inline bool operator<( const ctle::uuid &Left, const ctle::uuid &Right ) 
	{
	return std::memcmp( &Left, &Right, sizeof( ctle::uuid ) ) < 0;
	};

inline bool operator==( const ctle::uuid &Left, const ctle::uuid &Right ) 
	{
	return (Left._data_q[0] == Right._data_q[0]) 
        && (Left._data_q[1] == Right._data_q[1]);
	};

inline bool operator!=( const ctle::uuid &Left, const ctle::uuid &Right ) 
	{
	return (Left._data_q[0] != Right._data_q[0]) 
        || (Left._data_q[1] != Right._data_q[1]);
	};

std::ostream &operator<<( std::ostream &os, const ctle::uuid &_uuid );

#ifdef CTLE_IMPLEMENTATION

#include <random>
#include <algorithm>
#include <memory>

namespace ctle
    {
    const uuid uuid::nil = {};

    template <> std::string value_to_hex_string<uuid>( uuid value )
        {
        std::string ret;

        // format: 123e4567-e89b-12d3-a456-426614174000 , 36 characters long (32 hex + 4 dash)
        ret += bytes_to_hex_string( &value.data[0] , 4 );
        ret += "-";
        ret += bytes_to_hex_string( &value.data[4] , 2 );
        ret += "-";
        ret += bytes_to_hex_string( &value.data[6] , 2);
        ret += "-";
        ret += bytes_to_hex_string( &value.data[8] , 2 );
        ret += "-";
        ret += bytes_to_hex_string( &value.data[10] , 6 );

        return ret;
        }

    template <> uuid hex_string_to_value<uuid>( const char *hex_string )
        {
        if( hex_string[8] != '-'
         || hex_string[13] != '-'
         || hex_string[18] != '-'
         || hex_string[23] != '-' ) 
           {
           throw std::runtime_error("hex_string_to_value: ill-formated hex_string");
           } 

        uuid value;

        hex_string_to_bytes( &value.data[0] , &hex_string[0] , 4 );
        hex_string_to_bytes( &value.data[4] , &hex_string[9] , 2 );
        hex_string_to_bytes( &value.data[6] , &hex_string[14] , 2);
        hex_string_to_bytes( &value.data[8] , &hex_string[19] , 2 );
        hex_string_to_bytes( &value.data[10] , &hex_string[24] , 6 );

        return value;
        }

    uuid uuid::generate()
        {
        static std::mutex generate_mutex;
        static std::unique_ptr<std::mt19937> generator;

        // make it thread safe
        const std::lock_guard<std::mutex> lock( generate_mutex );

        // in first call, set up generator
        if( !generator )
            {
            std::random_device rd;
            auto seed_data = std::array<int, std::mt19937::state_size> {};
            std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            generator = std::make_unique<std::mt19937>(seq);
            }

        // generate random values
        uuid value;
        value._data_q[0] = ((uint64_t)generator.get()) << 32 | ((uint64_t)generator.get());
        value._data_q[1] = ((uint64_t)generator.get()) << 32 | ((uint64_t)generator.get());

        // set the version and variant values
        // version byte is: 0100xxxx , (version 4)
        value.data[6] &= 0x4F;
        value.data[6] |= 0x40;

        // variant byte is: 10xxxxxx , (variant 1)
        value.data[8] &= 0xBF;
        value.data[8] |= 0x80;

        return value;
        }

    }

std::ostream &operator<<( std::ostream &os, const ctle::uuid &_uuid )
    {
    os << ctle::value_to_hex_string( _uuid );
    return os;
    }

#endif