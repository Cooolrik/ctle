## multithread_pool.h

The `multithread_pool.h` file provides a template class for creating a pool of objects that can be shared by tasks in multiple threads. This is useful for objects that are expensive to allocate or have allocated, allowing them to be shared among multiple threads/tasks instead of having one pool per thread.

### Ownership and lifecycle

`initialize(objects)` transfers the object list into the pool and leaves the input
vector empty. It builds the available-item index before transferring ownership;
if that allocation fails, the caller's objects and the existing pool are unchanged.
Installing the new list clears previous loan tracking under the same write lock.
Replaced objects are destroyed after releasing that lock.

`deinitialize(objects)` transfers every owned object back to the caller and clears
both the available and borrowed indexes under one write lock. It returns `false`
if loans were outstanding at that point, but still transfers all objects and
empties the pool. Otherwise, it returns `true`.
The output vector's previous contents are destroyed only after releasing the lock,
so their destructors may query the now-empty pool.

After deinitialization, availability and borrowed-state queries are empty and
`borrow_item()` returns `nullptr`. `return_item()` accepts only a currently tracked
loan; it does not put detached objects back into the pool. An empty initialization
also leaves no available objects or tracked loans.

The lock protects pool bookkeeping, not the lifetime of pointers already handed
to callers. Keep transferred objects alive until borrowers stop using them, and
stop old borrowers before replacing or destroying pool objects. Raw pointers do
not distinguish loan generations or address reuse; callers must coordinate reuse.
The object-list vector supplied to a lifecycle operation must not be modified
concurrently by its caller.

### Example Usage: Creating and Using a Multithread Pool

```cpp
#include "multithread_pool.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

class ExpensiveObject 
{
public:
    ExpensiveObject() { std::cout << "ExpensiveObject created" << std::endl; }
    ~ExpensiveObject() { std::cout << "ExpensiveObject destroyed" << std::endl; }
    void do_work() { std::cout << "ExpensiveObject is working" << std::endl; }
};

void worker(ctle::multithread_pool<ExpensiveObject>& pool) 
{
    auto obj = pool.borrow_item();
    if(obj) 
	{
        obj->do_work();
        pool.return_item(obj);
    } 
	else 
	{
        std::cout << "No available objects in the pool" << std::endl;
    }
}

int main() 
{
	// make 5 expensive objects
    std::vector<std::unique_ptr<ExpensiveObject>> objects;
    for (int i = 0; i < 5; ++i) 
	{
        objects.push_back(std::make_unique<ExpensiveObject>());
    }

	// create a pool out of the expensive objects
    ctle::multithread_pool<ExpensiveObject> pool;
    pool.initialize(objects);

	// run a number of workers which need to use an object from the pool
    std::thread t1(worker, std::ref(pool));
    std::thread t2(worker, std::ref(pool));
    std::thread t3(worker, std::ref(pool));

    t1.join();
    t2.join();
    t3.join();

    std::vector<std::unique_ptr<ExpensiveObject>> returned_objects;
    bool all_returned = pool.deinitialize(returned_objects);

    std::cout << "All objects has been returned: " << all_returned << std::endl;

    return 0;
}
```