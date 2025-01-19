## write_stream.h

The `write_stream.h` file provides a `write_stream` class template for writing data sequentially to a data destination while also calculating a hash on the input stream.

### Examples

#### Basic Usage

```cpp
#include "write_stream.h"
#include "file_data_source.h"
#include <iostream>

int main()
{
    try
    {
        ctle::file_data_source dest("output.bin");
        ctle::write_stream<ctle::file_data_source> stream(dest);

        int data = 42;
        stream.write(data);

        std::vector<int> data_vector = {1, 2, 3, 4, 5};
        stream.write(data_vector.data(), data_vector.size());

        stream.end();

        auto digest = stream.get_digest();
        if (digest.status() == ctle::status::ok)
        {
            std::cout << "Hash: " << digest.value() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
```

#### Writing Raw Bytes

```cpp
#include "write_stream.h"
#include "file_data_source.h"
#include <iostream>

int main()
{
    try
    {
        ctle::file_data_source dest("output.bin");
        ctle::write_stream<ctle::file_data_source> stream(dest);

        const char* text = "Hello, World!";
        stream.write_bytes(reinterpret_cast<const u8*>(text), strlen(text));

        stream.end();

        auto digest = stream.get_digest();
        if (digest.status() == ctle::status::ok)
        {
            std::cout << "Hash: " << digest.value() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
```