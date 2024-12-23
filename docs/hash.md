## hash.h

The `hash.h` file provides the hash<> template structure for hash digests of various sizes (64, 128, 256, and 512 bits). It includes comparison operators for comparing hash values.

### `template<size_t _Size> struct hash`

A hash digest structure defined for 64, 128, 256, and 512 bits. Attempting to use other sizes will result in a static assertion failure.
The hash values are stored in big-endian format, so the most significant byte is at index 0, and the least significant byte is at the last index.
The comparison operators (<, ==, !=) allow for comparing hash values.

### Example Usage

#### Creating and Comparing Hashes

```cpp
#include "hash.h"
#include <iostream>

int main() 
{
    // Create two 256-bit hash values
    ctle::hash<256> hash1;
    ctle::hash<256> hash2;

    // Set some data in the hashes
    hash1.data[0] = 0x01;
    hash2.data[0] = 0x02;

    // Compare the hashes
    if (hash1 < hash2) 
	{
        std::cout << "hash1 is less than hash2" << std::endl;
    } 
	else if (hash1 == hash2) 
	{
        std::cout << "hash1 is equal to hash2" << std::endl;
    } 
	else 
	{
        std::cout << "hash1 is greater than hash2" << std::endl;
    }

    return 0;
}
```

#### Using Different Hash Sizes

```cpp
#include "hash.h"
#include <iostream>

int main() 
{
    // Create a 64-bit hash value
    ctle::hash<64> hash64;
    hash64.data[0] = 0x01;

    // Create a 128-bit hash value
    ctle::hash<128> hash128;
    hash128.data[0] = 0x01;

    // Create a 512-bit hash value
    ctle::hash<512> hash512;
    hash512.data[0] = 0x01;

    // Print the sizes of the hashes
    std::cout << "Size of 64-bit hash: " << sizeof(hash64) << " bytes" << std::endl;
    std::cout << "Size of 128-bit hash: " << sizeof(hash128) << " bytes" << std::endl;
    std::cout << "Size of 512-bit hash: " << sizeof(hash512) << " bytes" << std::endl;

    return 0;
}
```