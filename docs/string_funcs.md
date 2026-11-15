## string_funcs.h

The `string_funcs.h` header provides utilities for string manipulation, parsing, and conversion.

### Functions

#### String Parsing and Conversion

- `template<class _Ty> _Ty from_string(...)`: Convert string to value
- `template<class _Ty> _Ty from_hex_string(...)`: Convert string hex value to value
- `template<class _Ty> _Ty from_hex_string_unchecked(const char*, bool&)`: Decode a fixed-width unsigned integer from a buffer whose size is already known
- `template<class _Ty> std::string to_string(const _Ty &val)`: Convert value to string
- `template<class _Ty> std::string to_hex_string(const _Ty &val)`: Convert value to string hex value

Integer hex parsing has two entry points over the same decoder:

- `from_hex_string<T>(span, success)` checks for exactly `2 * sizeof(T)` characters.
- `from_hex_string_unchecked<T>(text, success)` skips that length check. The caller
  must guarantee at least `2 * sizeof(T)` readable characters.

Both support `uint8_t`, `uint16_t`, `uint32_t`, and `uint64_t`, accept upper- and
lowercase hex digits, and check characters while decoding. Neither requires a
null terminator when using spans or the unchecked pointer entry point.
The unchecked version does not skip character validation and must never receive
a null, dangling, or undersized input pointer.

Initialize `success` to `true`; these functions only clear it on failure. The
throwing integer hex overload uses `std::invalid_argument` for invalid digits or
incorrect width. Short forms, extra characters, whitespace, and `0x` prefixes are
not accepted by the checked fixed-width integer parser.

UUID and digest parsing validate their own layouts once and then use the shared
byte decoder without redundant per-group bounds checks. Their existing exception
types are unchanged. Numeric decimal parsing retains prefix semantics; see
[base_types.md](base_types.md) for syntax and error handling.

#### String Scanning

- `template<class _Ty> size_t strspn_t(...)`: Get span of character set in prefix
- `template<class _Ty> size_t strcspn_t(...)`: Get span until character in string
- `template<class _Ty> size_t strcrspn_t(...)`: Get position of last occurring character
- `template<class _Ty> size_t strchr_t(...)`: Locate first occurrence of character
- `template<class _Ty> size_t strrchr_t(...)`: Locate last occurrence of character

#### String Tokenization

- `template<class _Ty> string_span<_Ty> strtok_t(...)`: Get first token from string
- `template<class _Ty> bool lex_t(...)`: Lex string into tokens

### Examples

#### String Span Usage

```cpp
#include "string_funcs.h"
#include <iostream>

int main()
{
    const char* text = "Hello, World!";
    ctle::string_span<char> span(text, text + strlen(text));
    
    std::cout << "Length: " << span.length() << std::endl;
    std::cout << "As string: " << std::string(span) << std::endl;
    
    return 0;
}
```

#### String Parsing

```cpp
#include "string_funcs.h"
#include <iostream>

int main()
{
    // Parse numeric values
    bool success = true;
    int value = ctle::from_string<int>(ctle::string_span<char>("123", "123" + 3), success);
    
    if (success)
    {
        std::cout << "Parsed value: " << value << std::endl;
        std::cout << "Back to string: " << ctle::to_string(value) << std::endl;
    }
    
    return 0;
}
```

#### String Tokenization

```cpp
#include "string_funcs.h"
#include <iostream>

int main()
{
    const char* input = "one,two,\"three with spaces\",four";
    std::vector<ctle::string_span<char>> tokens;
    
    if (ctle::lex_t(&tokens, input, input + strlen(input)))
    {
        for (const auto& token : tokens)
        {
            std::cout << "Token: " << std::string(token) << std::endl;
        }
    }
    
    return 0;
}
```

#### Hex Conversion

```cpp
#include "string_funcs.h"
#include <iostream>

int main()
{
    // Convert values to hex
    uint32_t value = 0x12345678;
    std::string hex = ctle::to_hex_string(value);
    std::cout << "Hex: " << hex << std::endl;
    
    // Convert hex back to value
    bool success = true;
    uint32_t parsed = ctle::from_hex_string<uint32_t>(hex, success);
    
    if (success)
    {
        std::cout << "Parsed value: 0x" << std::hex << parsed << std::endl;
    }
    
    // Convert raw bytes to hex
    uint8_t bytes[] = {0x12, 0x34, 0x56, 0x78};
    std::string hex_bytes = ctle::_bytes_to_hex_string(bytes, sizeof(bytes));
    std::cout << "Bytes as hex: " << hex_bytes << std::endl;
    
    return 0;
}
```

#### String Scanning

```cpp
#include "string_funcs.h"
#include <iostream>

int main()
{
    const char* text = "Hello, World!";
    const size_t len = strlen(text);
    
    // Find first comma
    size_t pos = ctle::strchr_t(text, text + len, ',');
    if (pos < len)
    {
        std::cout << "Found comma at position: " << pos << std::endl;
    }
    
    // Get span of letters
    size_t letters = ctle::strspn_t(text, text + len, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    std::cout << "Initial word length: " << letters << std::endl;
    
    return 0;
}
```