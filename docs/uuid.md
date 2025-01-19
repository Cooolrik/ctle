## uuid.h

The `uuid` struct implements a portable, variant 1, version 4 (RNG generated) uuid. It provides functionalities for generating, comparing, and converting uuids to and from strings.

### Static Functions

- `static uuid generate()`: Generates a new uuid using the `mt19937` seeded from `random_device`.

### Examples

#### Generating a uuid

```cpp
#include "uuid.h"
#include <iostream>

int main()
{
    ctle::uuid new_uuid = ctle::uuid::generate();
    std::cout << "Generated uuid: " << new_uuid << std::endl;
    return 0;
}
```

#### Comparing uuids

```cpp
#include "uuid.h"
#include <iostream>

int main()
{
    ctle::uuid uuid1 = ctle::uuid::generate();
    ctle::uuid uuid2 = ctle::uuid::generate();

    if (uuid1 == uuid2)
    {
        std::cout << "uuids are equal" << std::endl;
    }
    else
    {
        std::cout << "uuids are not equal" << std::endl;
    }

    return 0;
}
```

#### Converting uuid to string

```cpp
#include "uuid.h"
#include "string_funcs.h"
#include <iostream>

int main()
{
    ctle::uuid new_uuid = ctle::uuid::generate();
    std::string uuid_str = ctle::to_string(new_uuid);
    std::cout << "uuid as string: " << uuid_str << std::endl;
    return 0;
}
```

#### Converting String to uuid

```cpp
#include "uuid.h"
#include "string_funcs.h"
#include <iostream>

int main()
{
    std::string uuid_str = "123e4567-e89b-12d3-a456-426614174000";
    bool success = true;
    ctle::uuid parsed_uuid = ctle::from_string<ctle::uuid>(uuid_str, success);

    if (success)
    {
        std::cout << "Parsed uuid: " << parsed_uuid << std::endl;
    }
    else
    {
        std::cerr << "Failed to parse uuid" << std::endl;
    }

    return 0;
}
```