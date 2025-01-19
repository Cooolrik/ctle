## util.h

The `util.h` header provides utility functions and classes for various purposes, including conditional assignment and nil object handling.

### Functions

#### `identity_assign_if_trivially_default_constructible`

```cpp
template<typename _Ty, typename std::enable_if<std::is_trivially_default_constructible<_Ty>{},bool>::type = true>
void identity_assign_if_trivially_default_constructible(_Ty &val);

template<typename _Ty, typename std::enable_if<!std::is_trivially_default_constructible<_Ty>{},bool>::type = true>
void identity_assign_if_trivially_default_constructible(_Ty &);
```

This function conditionally assigns a value to a variable if the variable is trivially default constructible. For non-trivially default constructible types, it does nothing.

### Classes

#### `nil_object`

The `nil_object` class provides a static allocation for a nil object, which can be used to reference an invalid object when `nullptr` is not applicable or allowed.

### Examples

#### Conditional Assignment

```cpp
#include "util.h"
#include <iostream>

struct NonTrivial
{
    NonTrivial(int x = 0) : value(x) {}
    int value;
};

int main()
{
    int trivial_value;
    NonTrivial non_trivial_value;

    ctle::identity_assign_if_trivially_default_constructible(trivial_value);
    ctle::identity_assign_if_trivially_default_constructible(non_trivial_value);

    std::cout << "Trivial value: " << trivial_value << std::endl;
    std::cout << "Non-trivial value: " << non_trivial_value.value << std::endl;

    return 0;
}
```

#### Using `nil_object`

```cpp
#include "util.h"
#include <iostream>

int main()
{
    int* ptr = ctle::nil_object::ptr<int>();
    if (ctle::nil_object::is_nil(ptr))
    {
        std::cout << "Pointer is nil" << std::endl;
    }

    int& ref = ctle::nil_object::ref<int>();
    if (ctle::nil_object::is_nil(ref))
    {
        std::cout << "Reference is nil" << std::endl;
    }

    return 0;
}
```