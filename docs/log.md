## log.h

The `log.h` file provides a simple logging system with log levels and a global log function.

### Functions

#### Global Log Level

- `void set_global_log_level(log_level level)`: Set the global log level.
- `log_level get_global_log_level()`: Get the current global log level.
- `const char* get_log_level_as_string(log_level level)`: Get the log level as a string.

#### Global Log Function

- `void set_global_log_function(log_function func)`: Set the global log function.
- `log_function get_global_log_function()`: Get the current global log function.

### Classes

#### `log_msg`

The `log_msg` class logs a message when it goes out of scope. It can be used with a stream to construct the message.

### Examples

#### Basic Logging

```cpp
#include "log.h"
#include <iostream>

void custom_log_function(ctle::log_level level, const char *function_name, const char *message)
{
    std::cout << ctle::get_log_level_as_string(level) << ": " << function_name << ": " << message << std::endl;
}

int main()
{
    ctle::set_global_log_function(custom_log_function);
    ctle::set_global_log_level(ctle::log_level::debug);

    ctle::log_msg(ctle::log_level::info).message() << "This is an info message";
    ctle::log_msg(ctle::log_level::debug).message() << "This is a debug message";

    return 0;
}
```

#### Using Macros for Logging

```cpp
#include "log.h"
#include <iostream>

#define LOG_FUNCTION_NAME __PRETTY_FUNCTION__

#define ctle_log(msg_level) \
    if (ctle::log_level::msg_level <= ctle::get_global_log_level()) { \
        ctle::log_msg _ctle_log_entry(ctle::log_level::msg_level, __FILE__, __LINE__, LOG_FUNCTION_NAME); \
        _ctle_log_entry.message()

#define ctle_log_error ctle_log(error)
#define ctle_log_warning ctle_log(warning)
#define ctle_log_info ctle_log(info)
#define ctle_log_debug ctle_log(debug)
#define ctle_log_verbose ctle_log(verbose)

#define ctle_log_end ""; }

void custom_log_function(ctle::log_level level, const char *function_name, const char *message)
{
    std::cout << ctle::get_log_level_as_string(level) << ": " << function_name << ": " << message << std::endl;
}

int main()
{
    ctle::set_global_log_function(custom_log_function);
    ctle::set_global_log_level(ctle::log_level::debug);

    ctle_log_info << "This is an info message" << ctle_log_end;
    ctle_log_debug << "This is a debug message" << ctle_log_end;

    return 0;
}
```