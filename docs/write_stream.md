## write_stream.h

The `write_stream.h` file provides a `write_stream` class template for writing data sequentially to a data destination while also calculating a hash on the input stream.

### Progress and retries

The buffer remains fixed at 2 MiB. Small writes are buffered; large writes bypass
the buffer. `get_position()` counts bytes accepted by the stream, including buffered
bytes, rather than bytes durably stored by the destination.

The destination's `write()` returns a status and the number of bytes accepted.
Successful positive short writes are continued immediately. Each confirmed prefix
is hashed exactly once, after the destination accepts it.

If a successful destination call reports zero bytes, the stream returns
`status::cant_write` and remains open:

- Buffered bytes remain queued. Retrying `end()` resumes at the unwritten suffix,
  without copying or replaying the already accepted prefix.
- A partially completed direct write advances `get_position()` by its confirmed
  prefix. The caller retains the remaining input and must retry that suffix after
  the destination is ready. Do not resubmit the entire original input.
- If draining an older buffer fails before a new input is accepted, the position
  does not advance for that input.

For a direct-write retry, subtract the position before the call from the position
after it to find the accepted byte count. This count is in bytes even for typed
`write()` calls. Complete the remaining input before finalizing; `end()` cannot
recover direct input that the stream never accepted.

### Finalization and terminal failures

After successful `end()`, the digest is cached. Repeated `end()` calls return
`status::ok` without writing or finalizing again, and further writes return
`status::not_ready`. `get_digest()` returns `not_ready` until successful finalization.
An empty stream can also be finalized.

A destination error status, an impossible reported byte count, or a hash
update/finalization failure permanently fails the stream. A destination error does
not guarantee a reliable byte count, so the stream cannot safely retry it.
Subsequent writes, `end()`, and `get_digest()` return the cached error without
touching the destination or hasher. Impossible counts use `status::corrupted`.

Destination or hasher exceptions propagate to the caller after marking the stream
failed with `status::undefined_error`. Confirmed earlier direct-write progress is
retained, but unreported side effects cannot be reconstructed or rolled back.

Destruction makes one best-effort `end()` attempt and suppresses exceptions.
It does not replay terminal failures or finalize an already ended stream.
**Call and check `end()` explicitly** when failures matter. End drains the stream's
buffer; it does not guarantee an operating-system flush or durable storage.

### Examples

#### Basic Usage

```cpp
#include "write_stream.h"
#include "data_destination.h"
#include <iostream>

int main()
{
    try
    {
        ctle::file_data_destination dest("output.bin");
        ctle::write_stream<ctle::file_data_destination> stream(dest);

        int data = 42;
        if (!stream.write(data))
            return 1;

        std::vector<int> data_vector = {1, 2, 3, 4, 5};
        if (!stream.write(data_vector.data(), data_vector.size()))
            return 1;

        if (!stream.end())
            return 1;

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
#include "data_destination.h"
#include <iostream>

int main()
{
    try
    {
        ctle::file_data_destination dest("output.bin");
        ctle::write_stream<ctle::file_data_destination> stream(dest);

        const char* text = "Hello, World!";
        if (!stream.write_bytes(reinterpret_cast<const ctle::u8*>(text), strlen(text)))
            return 1;

        if (!stream.end())
            return 1;

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