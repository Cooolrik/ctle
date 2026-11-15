## util.h

The `util.h` header provides a utility for conditional value initialization.

### Functions

#### `identity_assign_if_trivially_default_constructible`

```cpp
template<typename _Ty, typename std::enable_if<std::is_trivially_default_constructible<_Ty>{},bool>::type = true>
void identity_assign_if_trivially_default_constructible(_Ty &val);

template<typename _Ty, typename std::enable_if<!std::is_trivially_default_constructible<_Ty>{},bool>::type = true>
void identity_assign_if_trivially_default_constructible(_Ty &);
```

This function conditionally assigns a value to a variable if the variable is trivially default constructible. For non-trivially default constructible types, it does nothing.

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
