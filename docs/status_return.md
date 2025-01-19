## status_return.h

The `status_return` class template represents a pair of result/status and return type for functions that return a type along with a status.

### Template Parameters

- `_StatusType`: The type representing the status (must be trivially copyable)
- `_ValueType`: The type representing the return value, can be void


### Examples

#### Using `status_return` with a Value Type

```cpp
#include "status_return.h"
#include <iostream>

enum class MyStatus
{
    Ok,
    Error
};

status_return<MyStatus, int> compute_value(bool success)
{
    if (success)
    {
        return {MyStatus::Ok, 42};
    }
    else
    {
        return MyStatus::Error;
    }
}

int main()
{
    auto result = compute_value(true);
    if (result)
    {
        std::cout << "Value: " << result.value() << std::endl;
    }
    else
    {
        std::cerr << "Error occurred" << std::endl;
    }

    return 0;
}
```

#### Using `status_return` without a Value Type

```cpp
#include "status_return.h"
#include <iostream>

enum class MyStatus
{
    Ok,
    Error
};

status_return<MyStatus, void> perform_action(bool success)
{
    if (success)
    {
        return MyStatus::Ok;
    }
    else
    {
        return MyStatus::Error;
    }
}

int main()
{
    auto result = perform_action(true);
    if (result)
    {
        std::cout << "Action succeeded" << std::endl;
    }
    else
    {
        std::cerr << "Action failed" << std::endl;
    }

    return 0;
}
```