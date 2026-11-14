## status

The `status` class provides a unified error handling system that maps various error codes from different sources (STL, Vulkan, etc.) into a single enumeration.

### Features

- Unified error codes from multiple sources
- Automatic conversion from STL error codes (`std::errc`)
- Automatic conversion from Vulkan error codes (`VkResult`)
- String descriptions for all error codes
- Boolean operator for quick success/failure checks
- Stream output operator support

### Optional conversion adapters

`status.h` does not require Vulkan headers. Include `status_vulkan.h` to enable
`VkResult` conversions, or `status_stl.h` for `std::errc` conversions. These adapters
include their native type headers and work even if `status.h` was included earlier.

If Vulkan was included before `status.h`, the Vulkan adapter is included
automatically. STL auto-detection supports the MSVC, libstdc++, and libc++ standard
libraries when `<system_error>` was included first. Explicitly including the adapter
does not depend on auto-detection.

Adapters must be visible before conversion in each translation unit that uses them.
Their declarations are always available, but their conversion definitions and
mapping tables are compiled only when `CTLE_IMPLEMENTATION` is defined. Include
each used adapter in exactly one implementation translation unit. The core `status`
class has the same definition with or without adapters.

Implicit construction, assignment, and `status::to_status_code()` are supported.
`std::errc{}` and nonnegative `VkResult` values map to `status::ok`; unmapped errors
map to the corresponding `*_unrecognized_error_code`. Native error aliases use the
first matching entry in the generated mapping.

#### Implementing the adapters

Define `CTLE_IMPLEMENTATION` before the first ctle include in the implementation
source file. Native headers can be included before `status.h`, which automatically
includes and implements their adapters:

```cpp
#define CTLE_IMPLEMENTATION
#include <system_error>
#include <vulkan/vulkan.h>
#include <ctle/status.h>
```

Alternatively, include `status.h` first, then explicitly include the adapters:

```cpp
#define CTLE_IMPLEMENTATION
#include <ctle/status.h>
#include <system_error>
#include <vulkan/vulkan.h>
#include <ctle/status_stl.h>
#include <ctle/status_vulkan.h>
```

The explicit adapter headers include their native headers themselves, so the
separate native includes in the second example are optional. Omit the Vulkan
headers entirely if no Vulkan conversion is needed.

Other source files include the adapters without defining `CTLE_IMPLEMENTATION`.
Missing an adapter implementation produces a linker error. Re-including `status.h`
after a native header does not repeat auto-detection; include the adapter explicitly.

### Examples

#### Basic Usage

```cpp
#include "status.h"
#include <iostream>

void function_that_might_fail()
{
    // Return error status
    return ctle::status::cant_allocate;
}

int main()
{
    ctle::status result = function_that_might_fail();
    
    if (!result)
    {
        std::cout << "Error occurred: " << result << std::endl;
        // Prints: Error occurred: cant_allocate ("cant allocate memory")
    }
    
    return 0;
}
```

#### Converting from STL Errors

```cpp
#include <ctle/status_stl.h>
#include <iostream>

int main()
{
    // Convert from std::errc
    ctle::status result = std::errc::no_such_file_or_directory;
    
    if (!result)
    {
        std::cout << "STL error: " << result << std::endl;
        // Prints: STL error: stl_no_such_file_or_directory ("stl error code: errc::no_such_file_or_directory")
    }
    
    return 0;
}
```

#### Using with Vulkan

```cpp
#include <ctle/status_vulkan.h>
#include <iostream>

VkResult vulkan_function_which_fails()
{
	return VK_ERROR_OUT_OF_HOST_MEMORY;
}

int main()
{
    ctle::status result = vulkan_function_which_fails();
    
    if (!result)
    {
        std::cout << "Vulkan error: " << result << std::endl;
        // Prints: Vulkan error: vulkan_out_of_host_memory ("Vulkan error code VkResult::VK_ERROR_OUT_OF_HOST_MEMORY")
    }
    
    return 0;
}
```

#### Error Handling Pattern

```cpp
#include "status.h"
#include <iostream>

ctle::status operation1()
{
    // Simulated failure
    return ctle::status::cant_open;
}

ctle::status operation2()
{
    // Check result of operation1
    ctle::status result = operation1();
    if (!result)
    {
        return result;  // Propagate error
    }
    
    return ctle::status::ok;
}

int main()
{
    ctle::status result = operation2();
    
    if (!result)
    {
        std::cerr << "Operation failed: " << result << std::endl;
        return -1;
    }
    
    std::cout << "Operation succeeded" << std::endl;
    return 0;
}
```