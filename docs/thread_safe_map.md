## thread_safe_map.h

The `thread_safe_map` class template provides a thread-safe wrapper around `std::unordered_map`, ensuring single access to the map with performance similar to single-threaded access.

### Template Parameters

- `_Kty`: The type of the keys in the map
- `_Ty`: The type of the values in the map

### Examples

#### Basic Usage

```cpp
#include "thread_safe_map.h"
#include <iostream>

int main()
{
    ctle::thread_safe_map<int, std::string> map;

    // Insert elements
    map.insert({1, "one"});
    map.insert({2, "two"});

    // Check if key exists
    if (map.has(1))
    {
        std::cout << "Key 1 exists" << std::endl;
    }

    // Get value
    auto result = map.get(1);
    if (result.second)
    {
        std::cout << "Value for key 1: " << result.first << std::endl;
    }

    // Erase element
    map.erase(1);

    // Check size
    std::cout << "Map size: " << map.size() << std::endl;

    return 0;
}
```

#### Thread-Safe Access

```cpp
#include "thread_safe_map.h"
#include <iostream>
#include <thread>

void insert_values(ctle::thread_safe_map<int, std::string>& map)
{
    for (int i = 0; i < 10; ++i)
    {
        map.insert({i, "value" + std::to_string(i)});
    }
}

void read_values(ctle::thread_safe_map<int, std::string>& map)
{
    for (int i = 0; i < 10; ++i)
    {
        auto result = map.get(i);
        if (result.second)
        {
            std::cout << "Key " << i << ": " << result.first << std::endl;
        }
    }
}

int main()
{
    ctle::thread_safe_map<int, std::string> map;

    std::thread writer(insert_values, std::ref(map));
    std::thread reader(read_values, std::ref(map));

    writer.join();
    reader.join();

    return 0;
}
```