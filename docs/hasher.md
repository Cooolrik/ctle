## hasher.h

The `hasher.h` file provides various hasher classes for generating hash values. It includes a no-operation hasher, a SHA-256 hasher, and XXH3 XXH64/128 hashers.

The classes provide declarations of the different hashers, but the user needs to add an implementation of the hashing code. See [ctle.h](#ctle.h) for details.

### Ownership

The SHA-256, XXH64, XXH128, and salted XXH128 hashers own their backend state and
cannot be copied. Use `std::move` to transfer a partially computed hash. Move
assignment releases the destination's previous state before taking ownership.

After a move, `update()` and `finish()` on the source return
`status::not_initialized`. The source may be destroyed or assigned a new hasher:

```cpp
ctle::hasher_xxh64 source;
source.update(data, size);
ctle::hasher_xxh64 destination = std::move(source);
source = ctle::hasher_xxh64();
```

Backend types remain hidden behind implementation-only state definitions, so
ordinary consumers do not need backend headers. Backend allocation failure throws
`std::bad_alloc`. The resource-free `hasher_noop` remains copyable.

### Example Usage

```cpp
#include "hasher.h"
#include <iostream>

int main() 
{
	// Uncomment a line to select that hasher and hash size
    // SHA-256:
	ctle::hasher_sha256 hasher;
    
	// XXH 64 or 128
	//ctle::hasher_xxh64 hasher;
    //ctle::hasher_xxh128 hasher;

	// two XXH 128 hashes are concatenated into one 256 bit hash with a fixed salt of 'dcb7be9cd0fcf505' on the second hash 
	// ctle::hasher_2x_xxh128_dcb7be9cd0fcf505 hasher;

	// no-operation hashers (just 0 hashes) 64-512 bits sizes:
    //ctle::hasher_noop<64> hasher;
    //ctle::hasher_noop<128> hasher;
    //ctle::hasher_noop<256> hasher;
    //ctle::hasher_noop<512> hasher;

	// update the hash with 3 bytes
    uint8_t data[] = {0x01, 0x02, 0x03};
    hasher.update(data, sizeof(data));

	// retrieve the resulting hash value 
    auto result = hasher.finish();
    if (result.status == ctle::status::ok) 
	{
        std::cout << "Hash generated successfully." << std::endl;
        auto hash_value = result.value;
        std::cout << "Hash value: ";
        for (size_t i = 0; i < sizeof(hash_value.data); ++i) 
		{
            std::cout << std::hex << static_cast<int>(hash_value.data[i]);
        }
        std::cout << std::endl;
    } 
	else 
	{
        std::cerr << "Failed to generate hash value." << std::endl;
    }

    return 0;
}
```