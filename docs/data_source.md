## data_source.h

The `data_source.h` file provides data source objects that implement a read method and can be used for streaming data classes, such as `read_stream`.

### Classes

#### `file_data_source`

The `file_data_source` class is used for reading data from a file. It can be used as a source for streaming data classes, such as `read_stream`.

### Member Functions

#### `status_return<status, u64> read(u8* dest_buffer, u64 read_count)`

Reads data from the source into the destination buffer. Returns the number of bytes actually read.

### Examples

#### Basic Usage

```cpp
#include "data_source.h"
#include <iostream>
#include <vector>

int main()
{
    try
    {
        ctle::file_data_source source("example.txt");
        std::vector<u8> buffer(1024);
        auto result = source.read(buffer.data(), buffer.size());

        if (result.status() == ctle::status::ok)
        {
            std::cout << "Read " << result.value() << " bytes from file." << std::endl;
        }
        else
        {
            std::cerr << "Failed to read from file." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
```

#### Handling End of File

```cpp
#include "data_source.h"
#include <iostream>
#include <vector>

int main()
{
    try
    {
        ctle::file_data_source source("example.txt");
        std::vector<u8> buffer(1024);
        u64 total_bytes_read = 0;

        while (true)
        {
            auto result = source.read(buffer.data(), buffer.size());
            if (result.status() != ctle::status::ok || result.value() == 0)
            {
                break;
            }
            total_bytes_read += result.value();
        }

        std::cout << "Total bytes read: " << total_bytes_read << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
```