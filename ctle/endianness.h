// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE

#pragma once

#include <stdint.h>
#include <stddef.h>

#include <utility>

namespace ctle
	{
    // functions which converts endianness

	// creates values from big-endian raw 2, 4 or 8 byte data (template implemented for uint16_t, uint32_t and uint64_t)
	template <class T> T value_from_bigendian( const uint8_t *src );
	template <> inline uint16_t value_from_bigendian<uint16_t>( const uint8_t *src ) 
        { 
        return (uint16_t) (uint16_t(uint16_t(src[0]) << 8) | uint16_t(src[1])); 
        }

	template <> inline uint32_t value_from_bigendian<uint32_t>( const uint8_t *src ) 
        { 
        return (uint32_t(value_from_bigendian<uint16_t>( &src[0] )) << 16) | uint32_t(value_from_bigendian<uint16_t>( &src[2] )); 
        }
	template <> inline uint64_t value_from_bigendian<uint64_t>( const uint8_t *src ) 
        { 
        return (uint64_t(value_from_bigendian<uint32_t>( &src[0] )) << 32) | uint64_t(value_from_bigendian<uint32_t>( &src[4] )); 
        }

	// creates big-endian raw 2, 4 or 8 byte data from values (template implemented for uint16_t, uint32_t and uint64_t)
	template <class T> void bigendian_from_value( uint8_t *dst, T value );
	template <> inline void bigendian_from_value<uint16_t>( uint8_t *dst , uint16_t value ) 
        { 
        dst[0] = uint8_t((value >> 8) & 0xff); 
        dst[1] = uint8_t(value & 0xff); 
        }

	template <> inline void bigendian_from_value<uint32_t>( uint8_t *dst , uint32_t value ) 
        { 
        bigendian_from_value<uint16_t>( &dst[0] , uint16_t((value >> 16) & 0xffff) ); 
        bigendian_from_value<uint16_t>( &dst[2] , uint16_t(value & 0xffff) ); 
        }

	template <> inline void bigendian_from_value<uint64_t>( uint8_t *dst , uint64_t value ) 
        { 
        bigendian_from_value<uint32_t>( &dst[0] , uint32_t((value >> 32) & 0xffffffff) ); 
        bigendian_from_value<uint32_t>( &dst[4] , uint32_t(value & 0xffffffff) ); 
        }

	// swap byte order on one or multiple words
	inline void swap_bytes( uint8_t *pA, uint8_t *pB )
		{
		uint8_t tmp = *pA;
		*pA = *pB;
		*pB = tmp;
		}

	template <class T> void swap_byte_order( T *dest );
	template<> inline void swap_byte_order<uint16_t>( uint16_t *dest )
		{
		swap_bytes( &((uint8_t *)dest)[0], &((uint8_t *)dest)[1] );
		}
	template<> inline void swap_byte_order<uint32_t>( uint32_t *dest )
		{
		swap_bytes( &((uint8_t *)dest)[0], &((uint8_t *)dest)[3] );
		swap_bytes( &((uint8_t *)dest)[1], &((uint8_t *)dest)[2] );
		}
	template<> inline void swap_byte_order<uint64_t>( uint64_t *dest )
		{
		swap_bytes( &((uint8_t *)dest)[0], &((uint8_t *)dest)[7] );
		swap_bytes( &((uint8_t *)dest)[1], &((uint8_t *)dest)[6] );
		swap_bytes( &((uint8_t *)dest)[2], &((uint8_t *)dest)[5] );
		swap_bytes( &((uint8_t *)dest)[3], &((uint8_t *)dest)[4] );
		}
	template <class T> void swap_byte_order( T *dest , size_t count );
	template<> inline void swap_byte_order<uint16_t>( uint16_t *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((uint8_t *)dest)[0], &((uint8_t *)dest)[1] );
			++dest;
			}
		}
	template<> inline void swap_byte_order<uint32_t>( uint32_t *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((uint8_t *)dest)[0], &((uint8_t *)dest)[3] );
			swap_bytes( &((uint8_t *)dest)[1], &((uint8_t *)dest)[2] );
			++dest;
			}
		}
	template<> inline void swap_byte_order<uint64_t>( uint64_t *dest , size_t count )
		{
		for( size_t i = 0; i < count; ++i )
			{
			swap_bytes( &((uint8_t *)dest)[0], &((uint8_t *)dest)[7] );
			swap_bytes( &((uint8_t *)dest)[1], &((uint8_t *)dest)[6] );
			swap_bytes( &((uint8_t *)dest)[2], &((uint8_t *)dest)[5] );
			swap_bytes( &((uint8_t *)dest)[3], &((uint8_t *)dest)[4] );
			++dest;
			}
		}



    }