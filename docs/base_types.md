## base_types.h

This header provides implementations for base integer and floating point types using ctle:
 - ctle::to_string()
 - ctle::from_string()

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
    bool success = false;
    ctle::i32 parsed_int = ctle::from_string<ctle::i32>( std::string("42"), success );
    
    if (success)
    {
        std::cout << "Parsed int: " << parsed_int << std::endl;
    }
    
    return 0;
}
```
