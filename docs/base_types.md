## base_types.h

This header provides implementations for base integer and floating point types using ctle:
 - ctle::to_string()
 - ctle::from_string()

### Parsing contracts

Initialize the `success` argument to `true`. An error clears it; successful parsing
never erases an earlier failure. Scalar numeric failures return a zero value.
The throwing overload reports invalid syntax with `std::invalid_argument` and an
unrepresentable value with `std::out_of_range`. Floating-point allocation failure
clears the flag or throws `std::bad_alloc`, depending on the overload.

Numeric parsing accepts leading whitespace, one optional sign, and the numeric
prefix of a string, as before. Trailing nonnumeric characters are not rejected.
Unsigned negation retains `std::stoull` semantics: negate in `uint64_t` first, then
check whether the result fits the destination. Floating-point syntax, locale, and
rounding follow the standard library.

Integer parsing reads the supplied span directly, without allocating a temporary
string. C++17 and later use `std::from_chars` for the unsigned magnitude; C++14 uses
a bounded decimal loop. Both detect overflow during conversion. Floating-point
parsing retains a bounded, null-terminated copy for `std::strtof`/`std::strtod`
and translates their error results without throwing on ordinary parse failures.

### Using String Conversion

```cpp
#include "fwd.h"
#include "string_funcs.h"
#include <iostream>

int main()
{
    // Converting values to strings
    ctle::i32 int_val = 42;
    ctle::f32 float_val = 3.14f;
    
    std::string int_str = ctle::to_string(int_val);
    std::string float_str = ctle::to_string(float_val);
    
    std::cout << "Int as string: " << int_str << std::endl;
    std::cout << "Float as string: " << float_str << std::endl;
    
    // Converting strings to values
    bool success = true;
    ctle::i32 parsed_int = ctle::from_string<ctle::i32>( std::string("42"), success );
    
    if (success)
    {
        std::cout << "Parsed int: " << parsed_int << std::endl;
    }
    
    return 0;
}
```
