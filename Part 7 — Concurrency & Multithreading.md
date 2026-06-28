# Part 7 — Concurrency & Multithreading

> Writing correct concurrent code in C++ — threads, synchronization, atomics, futures, and the pitfalls that cause data races and deadlocks. Concurrency is one of the hardest parts of systems programming: bugs are non-deterministic, timing-dependent, and often invisible under a debugger.

## Table of Contents

- [[#7.1 — Why Concurrency? Problems it Solves]]
- [[#7.2 — std::thread — Creating & Joining]]
- [[#7.3 — Data Races & Undefined Behavior]]
- [[#7.4 — std::mutex, std::lock_guard, std::unique_lock]]
- [[#7.5 — Condition Variables]]
- [[#7.6 — std::atomic & Memory Order]]
- [[#7.7 — std::future, std::promise, std::async]]
- [[#7.8 — Thread Pools]]
- [[#7.9 — Coroutines (C++20): co_await, co_yield, co_return]]
- [[#7.10 — Common Pitfalls: Deadlock, Livelock, Starvation]]

---

## 7.1 — Why Concurrency? Problems it Solves

### Two Reasons to Use Concurrency

**1. True parallelism — more work done per unit of time.**

Modern CPUs have 4, 8, 16, or more cores, each capable of executing instructions independently. A single-threaded program uses one core and leaves the rest idle. Distributing work across threads allows the computation to use all available hardware.

```
Single-threaded: [== task A ==][== task B ==][== task C ==]   → 3 seconds
Multi-threaded:  [== task A ==]
                 [== task B ==]                                → 1 second
                 [== task C ==]
```

This matters for CPU-bound work: image processing, simulation, compression, scientific computation.

**2. Concurrency — progress on multiple things at once.**

Even on a single core, concurrency is useful when tasks spend time waiting: waiting for a network response, disk I/O, user input. While one task waits, another runs. Without concurrency, the program blocks — an unresponsive UI or a server that can only handle one request at a time.

### The Cost

Concurrency introduces complexity:
- **Data races** — two threads touching shared data without synchronization → undefined behavior
- **Deadlocks** — two threads waiting for each other forever
- **Livelocks** — both threads keep reacting to each other but make no progress
- **Starvation** — one thread never gets CPU time
- **Order dependencies** — operations in different threads execute in unpredictable order
- **Cache coherence overhead** — cores sharing data must coordinate via the memory subsystem

The fundamental rule: if two threads access the same memory location and at least one access is a write, you have a data race — unless synchronization is in place.

### Hardware Model

Each CPU core has its own L1/L2 cache. Writes by one core are not immediately visible to other cores — they must propagate through the cache coherence protocol (MESI or similar). This propagation takes time and is the reason memory ordering matters.

```
Core 0       Core 1       Core 2       Core 3
[L1 cache]  [L1 cache]  [L1 cache]  [L1 cache]
      \           |           |           /
       ====[  L2/L3 shared cache  ]====
                    |
               [Main Memory]
```

A write by Core 0 to `x` may sit in Core 0's cache for some time before Core 1 sees it. Without memory fences or atomic operations, Core 1 may read a stale value.

---

## 7.2 — `std::thread` — Creating & Joining

### Creating a Thread

`std::thread` launches a new OS thread immediately on construction:

```cpp
#include <thread>

// From a function
void worker(int id, const std::string& msg) {
    std::cout << "Thread " << id << ": " << msg << "\n";
}
std::thread t(worker, 42, "hello");   // arguments forwarded to worker

// From a lambda
std::thread t2([](int x) { std::cout << x * 2; }, 21);

// From a member function
struct Task {
    void run() { std::cout << "running\n"; }
};
Task task;
std::thread t3(&Task::run, &task);   // &task is the implicit 'this'
```

Arguments are **copied** into the thread by default. To pass by reference, wrap with `std::ref`:

```cpp
int result = 0;
std::thread t([&result]{ result = 42; });  // capture by reference — careful with lifetime
// or:
std::thread t(worker_fn, std::ref(result)); // std::ref wraps a reference
```

### Join and Detach

Every `std::thread` must be either **joined** or **detached** before its destructor runs. If neither happens, the destructor calls `std::terminate()`.

```cpp
std::thread t(worker);

t.join();     // block until the thread finishes — synchronize with its completion
              // after join(), t is no longer joinable
// or:
t.detach();   // let the thread run independently — we no longer manage it
              // the thread outlives t — be careful about lifetime of shared data
```

`t.joinable()` returns `true` if `t` represents a running thread (not default-constructed, not joined, not detached).

### RAII Thread Wrapper

A utility to ensure join happens automatically:

```cpp
class JoinThread {
    std::thread t_;
public:
    explicit JoinThread(std::thread t) : t_(std::move(t)) {}
    ~JoinThread() { if (t_.joinable()) t_.join(); }
    // Non-copyable, movable
    JoinThread(const JoinThread&) = delete;
    JoinThread& operator=(const JoinThread&) = delete;
};

{
    JoinThread jt(std::thread(worker, 42));
}   // destructor joins automatically — no risk of std::terminate
```

C++20 provides `std::jthread` which joins automatically and supports cooperative cancellation:

```cpp
#include <thread>

std::jthread jt([](std::stop_token stop) {
    while (!stop.stop_requested()) {
        do_work();
    }
});

jt.request_stop();  // signal the thread to stop
// destructor joins automatically
```

### Thread Identity

```cpp
std::thread::id main_id = std::this_thread::get_id();   // ID of current thread
std::thread t(worker);
t.get_id();    // ID of the thread t manages

std::this_thread::sleep_for(std::chrono::milliseconds(100));   // sleep
std::this_thread::yield();   // hint to yield the CPU time slice

unsigned int n = std::thread::hardware_concurrency();   // number of hardware threads (cores)
// Returns 0 if not computable — use as a hint, not a guarantee
```

### Exception Safety in Threads

If the thread function throws an uncaught exception, `std::terminate()` is called. Always catch exceptions inside thread functions:

```cpp
std::thread t([]() {
    try {
        risky_operation();
    } catch (const std::exception& e) {
        // log or store the error
    }
});
```

To propagate exceptions from a thread back to the launcher, use `std::exception_ptr`:

```cpp
std::exception_ptr ex_ptr;

std::thread t([&ex_ptr]() {
    try {
        risky_operation();
    } catch (...) {
        ex_ptr = std::current_exception();   // capture any exception
    }
});

t.join();

if (ex_ptr) std::rethrow_exception(ex_ptr);  // rethrow in the calling thread
```

---

## 7.3 — Data Races & Undefined Behavior

### What a Data Race Is

A **data race** occurs when:
1. Two or more threads access the same memory location **concurrently**
2. At least one access is a **write**
3. The accesses are **not synchronized**

A data race is **undefined behavior** — not just wrong output, but literally anything can happen. The compiler assumes data races don't exist and optimizes accordingly. A variable that one thread writes and another reads without synchronization may never be "seen" as updated — or may be seen partially updated.

```cpp
int counter = 0;

// Thread A
counter++;   // READ counter, ADD 1, WRITE result

// Thread B (running at the same time)
counter++;   // READ counter, ADD 1, WRITE result

// Expected: counter == 2
// Possible result: counter == 1 (both read 0, both write 1)
// Possible result: counter is in an inconsistent state
// Possible result: UB — the compiler may reorder, cache, or eliminate reads/writes
```

`counter++` is **not atomic** — it's three separate operations: read, modify, write. Another thread can interleave anywhere between them.

### What the Compiler Does Without Synchronization

The C++ memory model gives the compiler and CPU significant freedom to reorder operations. Without synchronization, a write in one thread may never become visible to another:

```cpp
bool ready = false;
int data = 0;

// Thread A
data = 42;
ready = true;   // compiler may reorder this BEFORE data = 42

// Thread B
while (!ready) {}
std::cout << data;  // may print 0 — data write may not be visible yet
```

Without proper synchronization (a memory fence, atomic with acquire/release, or mutex), Thread B has no guarantee about when or whether it sees Thread A's writes.

### ThreadSanitizer (TSan)

TSan detects data races at runtime — compile with `-fsanitize=thread`:

```sh
clang++ -std=c++23 -g -fsanitize=thread main.cpp
```

Output when a race is detected:
```
WARNING: ThreadSanitizer: data race (pid=12345)
  Write of size 4 at 0x... by thread T2:
    #0 increment() main.cpp:8
  Previous read of size 4 at 0x... by thread T1:
    #0 increment() main.cpp:8
```

Run your test suite under TSan regularly — it catches races that would otherwise only appear under load.

---

## 7.4 — `std::mutex`, `std::lock_guard`, `std::unique_lock`

### `std::mutex` — Mutual Exclusion

A mutex (mutual exclusion lock) ensures that only one thread at a time executes a critical section. All other threads attempting to lock the mutex block until it's released.

```cpp
#include <mutex>

std::mutex mtx;
int counter = 0;

void increment() {
    mtx.lock();         // acquire the mutex — blocks if another thread holds it
    ++counter;          // critical section — only one thread here at a time
    mtx.unlock();       // release the mutex
}
```

**Never call `lock()`/`unlock()` directly.** If an exception is thrown between `lock` and `unlock`, the mutex is never released and every other thread deadlocks. Always use RAII wrappers.

### `std::lock_guard` — Simple RAII Lock

Acquires the mutex on construction, releases on destruction. No way to unlock early:

```cpp
void increment() {
    std::lock_guard<std::mutex> lock(mtx);  // acquire — blocks until available
    ++counter;
}   // lock's destructor releases the mutex — even if exception is thrown
```

C++17: the template argument can be deduced:

```cpp
std::lock_guard lock(mtx);   // CTAD — deduces std::lock_guard<std::mutex>
```

### `std::unique_lock` — Flexible RAII Lock

More powerful than `lock_guard` — supports deferred locking, try-locking, timed locking, and manual unlock:

```cpp
// Deferred locking — construct without acquiring
std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
// ... do something without the lock ...
lock.lock();              // acquire later

// Try-locking — non-blocking
std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
if (lock.owns_lock()) {
    // got the lock
}

// Timed locking
std::unique_lock<std::mutex> lock(mtx, std::chrono::milliseconds(100));
if (lock.owns_lock()) {
    // got the lock within 100ms
}

// Manual unlock and re-lock
lock.unlock();
// do non-protected work
lock.lock();
```

`unique_lock` is required by `std::condition_variable` (see section 7.5).

### Mutex Variants

```cpp
std::mutex              // basic non-recursive mutex
std::recursive_mutex    // same thread can lock multiple times (counts locks)
std::timed_mutex        // try_lock_for, try_lock_until
std::recursive_timed_mutex

// C++17
std::shared_mutex       // readers-writer lock: many concurrent readers OR one writer
std::shared_timed_mutex
```

**`std::shared_mutex` — Reader-Writer Lock:**

```cpp
std::shared_mutex rw_mtx;
std::unordered_map<int, std::string> cache;

// Multiple readers can hold shared lock simultaneously
std::string read(int key) {
    std::shared_lock lock(rw_mtx);   // shared (read) lock
    return cache.at(key);
}

// Only one writer can hold exclusive lock
void write(int key, std::string value) {
    std::unique_lock lock(rw_mtx);   // exclusive (write) lock
    cache[key] = std::move(value);
}
```

### Locking Multiple Mutexes Without Deadlock

When you need to lock two mutexes at once, always lock them in the same order — or use `std::lock` which handles it:

```cpp
std::mutex mtx_a, mtx_b;

// WRONG — potential deadlock
// Thread 1: lock(mtx_a), then lock(mtx_b)
// Thread 2: lock(mtx_b), then lock(mtx_a)
// → Circular wait → deadlock

// CORRECT — std::lock acquires all-or-nothing, then RAII manages them
std::unique_lock<std::mutex> lock_a(mtx_a, std::defer_lock);
std::unique_lock<std::mutex> lock_b(mtx_b, std::defer_lock);
std::lock(lock_a, lock_b);   // deadlock-free acquisition of both

// C++17: std::scoped_lock — even simpler
std::scoped_lock lock(mtx_a, mtx_b);  // acquires both atomically, releases both on destruct
```

### `std::call_once` and `std::once_flag`

Execute a function exactly once across all threads — used for one-time initialization:

```cpp
std::once_flag init_flag;
std::unique_ptr<Resource> resource;

void init_resource() {
    resource = std::make_unique<Resource>();
}

void use_resource() {
    std::call_once(init_flag, init_resource);  // init_resource called exactly once
    resource->do_work();
}
```

This is the correct way to implement a thread-safe singleton (or use a `static` local variable — see section 7.10).

---

## 7.5 — Condition Variables

### The Problem

A mutex alone only prevents simultaneous access. It can't efficiently wait for a condition to become true — polling a mutex wastes CPU:

```cpp
// Busy-wait — CPU spins, wastes power and time
while (!data_ready) {
    // spin — terrible for anything more than microseconds
}
```

A **condition variable** blocks a thread until another thread signals that a condition has changed.

### `std::condition_variable`

```cpp
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool data_ready = false;
int data = 0;

// Producer thread
void produce() {
    {
        std::lock_guard lock(mtx);
        data = 42;
        data_ready = true;
    }                         // release lock before notify — avoids waking then immediately blocking
    cv.notify_one();          // wake one waiting thread
}

// Consumer thread
void consume() {
    std::unique_lock lock(mtx);              // unique_lock required by condition_variable
    cv.wait(lock, []{ return data_ready; }); // atomically: release lock + sleep until notified
                                             // re-acquires lock before returning
                                             // the predicate guards against spurious wakeups
    std::cout << "Got data: " << data << "\n";
}
```

**Why the predicate is essential:** `cv.wait()` can wake spuriously (without a `notify` call) — the OS is allowed to do this. The predicate lambda is rechecked after every wakeup. Without it, spurious wakes cause the consumer to proceed with stale data. `cv.wait(lock, pred)` is equivalent to:

```cpp
while (!pred()) cv.wait(lock);
```

### `notify_one` vs `notify_all`

```cpp
cv.notify_one();    // wake one waiting thread — which one is unspecified
cv.notify_all();    // wake all waiting threads — each will try to acquire the lock
```

Use `notify_all` when multiple threads are waiting and any of them can proceed. Use `notify_one` when exactly one thread should proceed per notification.

### Producer-Consumer Queue

A classic pattern — bounded thread-safe queue:

```cpp
template<typename T>
class BlockingQueue {
    std::queue<T> queue_;
    std::mutex mtx_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    const size_t max_size_;

public:
    explicit BlockingQueue(size_t max_size) : max_size_(max_size) {}

    void push(T val) {
        std::unique_lock lock(mtx_);
        not_full_.wait(lock, [this]{ return queue_.size() < max_size_; });
        queue_.push(std::move(val));
        not_empty_.notify_one();
    }

    T pop() {
        std::unique_lock lock(mtx_);
        not_empty_.wait(lock, [this]{ return !queue_.empty(); });
        T val = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return val;
    }
};
```

### Timed Wait

```cpp
auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);

std::unique_lock lock(mtx);
bool signaled = cv.wait_until(lock, deadline, []{ return data_ready; });
if (!signaled) {
    // timed out — data_ready is still false
}
```

---

## 7.6 — `std::atomic` & Memory Order

### `std::atomic<T>` — Lock-Free Operations

`std::atomic<T>` provides indivisible (atomic) read-modify-write operations on a variable without a mutex. Operations complete as an uninterruptible unit — no thread can observe a partial state.

```cpp
#include <atomic>

std::atomic<int>  counter{0};
std::atomic<bool> done{false};
std::atomic<int*> ptr{nullptr};

// Load and store
int val = counter.load();      // atomic read
counter.store(42);             // atomic write

// Shorthand — implicitly load/store
int val = counter;             // load
counter = 42;                  // store

// Read-modify-write operations
counter++;              // atomic increment
counter--;              // atomic decrement
counter += 5;           // atomic add
counter.fetch_add(5);   // returns old value, then adds 5
counter.fetch_sub(3);   // returns old value, then subtracts 3

int old = counter.exchange(100);   // swap: sets to 100, returns old value
```

### `compare_exchange` — CAS (Compare-And-Swap)

The fundamental building block of lock-free algorithms:

```cpp
// If counter == expected, set counter = desired and return true
// If counter != expected, update expected = counter and return false
int expected = 0;
bool success = counter.compare_exchange_strong(expected, 1);
// success == true: counter was 0, now 1
// success == false: counter was something else, expected is now that value

// Retry loop — the lock-free increment pattern
int old_val = counter.load();
while (!counter.compare_exchange_weak(old_val, old_val + 1)) {
    // CAS failed — old_val was updated with the current value — retry
}
// _weak is allowed to fail spuriously (optimization for loops on some architectures)
// _strong never fails spuriously — use when not in a loop
```

### Memory Order — The Hard Part

Every atomic operation takes a memory order argument that controls how the operation is ordered relative to other memory accesses. The default (`memory_order_seq_cst`) is safest and most expensive.

| Memory Order | What it means |
|---|---|
| `memory_order_relaxed` | No ordering constraint — only atomicity is guaranteed |
| `memory_order_acquire` | No reads or writes in this thread can move before this operation |
| `memory_order_release` | No reads or writes in this thread can move after this operation |
| `memory_order_acq_rel` | Both acquire and release semantics (for RMW operations) |
| `memory_order_seq_cst` | Total sequential consistency — default, strongest, most expensive |

**The acquire-release pattern** — establishing happens-before relationships:

```cpp
std::atomic<int> data{0};
std::atomic<bool> ready{false};

// Thread A — producer
data.store(42, std::memory_order_relaxed);    // write data first
ready.store(true, std::memory_order_release); // RELEASE — all writes before this
                                               // become visible to anyone who acquires

// Thread B — consumer
while (!ready.load(std::memory_order_acquire)) {}  // ACQUIRE — all writes from
                                                    // the release side are now visible
int val = data.load(std::memory_order_relaxed);     // guaranteed to see 42
```

The `release` on Thread A and `acquire` on Thread B form a **synchronization point** — everything written before the release is visible after the acquire.

**`memory_order_relaxed`** — use when you only need atomicity, not ordering. Example: a counter that multiple threads increment but nobody reads until all threads are done:

```cpp
std::atomic<int> total_ops{0};

void worker() {
    total_ops.fetch_add(1, std::memory_order_relaxed);  // no ordering needed
}
// After all threads join: total_ops is correct, but reads during execution may be stale
```

**`memory_order_seq_cst`** — the default. All `seq_cst` operations appear in a single total order visible to all threads. Correct but prevents hardware optimizations:

```cpp
counter.fetch_add(1);   // same as fetch_add(1, memory_order_seq_cst)
```

**Rule of thumb:** Use `seq_cst` until you've profiled and proved a bottleneck. Then carefully switch to `acquire`/`release`. Never use `relaxed` for synchronization — only for pure counters where only the final value matters.

### `std::atomic_flag` — The Simplest Atomic

A boolean flag guaranteed to be lock-free on every platform. Used to implement spinlocks:

```cpp
std::atomic_flag flag = ATOMIC_FLAG_INIT;

// Spinlock using atomic_flag
void lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {}  // spin until cleared
}
void unlock() {
    flag.clear(std::memory_order_release);
}
```

C++20 adds `test()` (read without setting), `wait()`, and `notify_one/all()` to `atomic_flag`.

### `std::atomic` for Pointers and Shared Structures

```cpp
std::atomic<Node*> head{nullptr};

// Lock-free prepend to linked list
void push(Node* new_node) {
    Node* current_head = head.load(std::memory_order_relaxed);
    do {
        new_node->next = current_head;
    } while (!head.compare_exchange_weak(
        current_head, new_node,
        std::memory_order_release,  // success ordering
        std::memory_order_relaxed   // failure ordering
    ));
}
```

---

## 7.7 — `std::future`, `std::promise`, `std::async`

### `std::async` — The Easy Path to a Background Task

`std::async` launches a callable asynchronously and returns a `std::future` that eventually holds the result:

```cpp
#include <future>

int compute(int x) { return x * x; }

std::future<int> f = std::async(std::launch::async, compute, 10);
// compute(10) runs in a background thread

// Do other work here...

int result = f.get();   // block until the result is ready, then return it
                        // if compute threw, get() rethrows the exception
std::cout << result;    // 100
```

**Launch policies:**

```cpp
std::async(std::launch::async,  task);  // always a new thread
std::async(std::launch::deferred, task);// runs lazily — only when get() or wait() is called
std::async(std::launch::async | std::launch::deferred, task);  // implementation chooses
std::async(task);   // same as above — implementation choice
```

**The `future` destructor behavior:** A `future` returned by `std::launch::async` blocks in its destructor if `get()` was never called. This means an unassigned async result can still block:

```cpp
// TRAP: this blocks until done — the temporary future's destructor joins the thread
std::async(std::launch::async, long_running_task);

// To truly fire-and-forget (with a thread), store the future somewhere
auto f = std::async(std::launch::async, long_running_task);
```

### `std::promise` and `std::future` — Manual Control

`std::promise` is the write end; `std::future` is the read end. Together they form a single-use channel:

```cpp
std::promise<int> prom;
std::future<int>  fut = prom.get_future();

// In another thread — set the value
std::thread t([&prom]() {
    int result = expensive_computation();
    prom.set_value(result);         // unblocks any thread waiting on fut.get()
});

// Main thread — wait for the value
int val = fut.get();   // blocks until promise is satisfied
t.join();
```

You can also set an exception:

```cpp
try {
    int result = risky_computation();
    prom.set_value(result);
} catch (...) {
    prom.set_exception(std::current_exception());  // fut.get() will rethrow
}
```

### `std::shared_future` — Multiple Readers

A `std::future` can only be `.get()`-ed once. If multiple threads need the result, use `std::shared_future`:

```cpp
std::promise<int> prom;
std::shared_future<int> sfut = prom.get_future().share();

// Multiple threads can all call sfut.get()
std::thread t1([sfut]{ std::cout << sfut.get(); });
std::thread t2([sfut]{ std::cout << sfut.get(); });

prom.set_value(42);
t1.join(); t2.join();
```

### `std::packaged_task` — Wrapping a Callable

Wraps a callable and connects it to a future — useful for thread pools:

```cpp
std::packaged_task<int(int)> task([](int x) { return x * x; });
std::future<int> fut = task.get_future();

std::thread t(std::move(task), 10);   // run the task in a thread
t.join();

std::cout << fut.get();   // 100
```

`packaged_task` separates the "create a task with a future" concern from the "choose when/how to run it" concern — key for thread pool implementations.

### Waiting and Polling

```cpp
std::future<int> f = std::async(std::launch::async, compute, 5);

// Poll without blocking
auto status = f.wait_for(std::chrono::milliseconds(10));
switch (status) {
    case std::future_status::ready:    /* done */ break;
    case std::future_status::timeout:  /* still running */ break;
    case std::future_status::deferred: /* not started yet */ break;
}

// Wait with deadline
f.wait_until(std::chrono::steady_clock::now() + std::chrono::seconds(5));

// Wait forever (then call get)
f.wait();
int result = f.get();
```

---

## 7.8 — Thread Pools

### Why Thread Pools

Creating and destroying threads is expensive — thread creation on Linux typically costs ~10–50μs. If you have thousands of small tasks (like handling HTTP requests or processing events), spinning up a thread per task is wasteful. A thread pool creates a fixed set of threads once and reuses them for all tasks.

### A Basic Thread Pool

```cpp
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <future>

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lock(mtx_);
                        cv_.wait(lock, [this] {
                            return stop_ || !tasks_.empty();
                        });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using ReturnType = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            [f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
                return std::invoke(f, args...);
            }
        );

        std::future<ReturnType> fut = task->get_future();
        {
            std::lock_guard lock(mtx_);
            if (stop_) throw std::runtime_error("ThreadPool is stopped");
            tasks_.emplace([task]{ (*task)(); });
        }
        cv_.notify_one();
        return fut;
    }

    ~ThreadPool() {
        {
            std::lock_guard lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        for (auto& w : workers_) w.join();
    }

private:
    std::vector<std::thread>          workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex                        mtx_;
    std::condition_variable           cv_;
    bool                              stop_{false};
};
```

**Usage:**

```cpp
ThreadPool pool(std::thread::hardware_concurrency());

std::vector<std::future<int>> results;
for (int i = 0; i < 100; ++i) {
    results.push_back(pool.submit([i] { return i * i; }));
}

for (auto& f : results) {
    std::cout << f.get() << " ";
}
```

### Work Stealing

High-performance thread pools (like Intel TBB, or `std::execution` backends) use **work stealing**: each thread has its own deque of tasks. When a thread's deque is empty, it "steals" tasks from the back of another thread's deque. This reduces lock contention and improves cache locality.

### C++17 Parallel Algorithms

The standard library provides a simpler way to parallelize common algorithms — just pass an execution policy:

```cpp
#include <execution>
#include <algorithm>

std::vector<int> v(1'000'000);
std::iota(v.begin(), v.end(), 0);

// Sequential — normal
std::sort(v.begin(), v.end());

// Parallel — uses multiple threads automatically
std::sort(std::execution::par, v.begin(), v.end());

// Parallel + SIMD vectorization
std::sort(std::execution::par_unseq, v.begin(), v.end());

// SIMD only (no threads)
std::sort(std::execution::unseq, v.begin(), v.end());  // C++20

// Works with all major algorithms
std::for_each(std::execution::par, v.begin(), v.end(), [](int& x){ x *= 2; });
std::transform(std::execution::par_unseq, v.begin(), v.end(), out.begin(), f);
std::reduce(std::execution::par, v.begin(), v.end(), 0);  // parallel sum
```

**Constraints on parallel algorithms:** The callable must not have data races, must not throw (or exceptions are swallowed), and must not modify iterators. `std::reduce` (parallel version) requires the operation to be associative and commutative — `std::accumulate` is always sequential.

---

## 7.9 — Coroutines (C++20): `co_await`, `co_yield`, `co_return`

Coroutines are covered fully in [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.12. This section focuses on how coroutines relate to concurrency — they are distinct from threads but often used alongside them.

### Coroutines Are Not Threads

A coroutine suspends **cooperatively** — it explicitly yields control at `co_await` or `co_yield` points. A thread is preemptively scheduled by the OS — it can be paused at any instruction.

| | Coroutines | Threads |
|---|---|---|
| Scheduling | Cooperative (explicit suspend) | Preemptive (OS scheduler) |
| Parallelism | No — single thread at a time | Yes — true parallelism |
| Stack | Heap-allocated frame (small) | OS stack (~1MB default) |
| Switch cost | ~nanoseconds | ~microseconds |
| Synchronization needed | No (single thread) | Yes — data races possible |
| Count per process | Millions (cheap) | Thousands (expensive) |

### Coroutines + Thread Pool = Async Framework

The common pattern: coroutines provide async, readable code structure; a thread pool provides the actual parallelism:

```cpp
// Pseudocode — as implemented by async frameworks like ASIO, cppcoro, etc.

Task<int> fetch_and_process() {
    // Suspends this coroutine, thread returns to pool to do other work
    auto data = co_await async_http_get("http://api.example.com/data");

    // Resumed by the pool thread when HTTP response arrives
    auto result = co_await async_parse(data);

    co_return result;
}

// Coroutine itself runs on whatever thread the scheduler assigns
// Other coroutines can run on other threads while this one is awaiting
```

The framework's scheduler decides which thread resumes a coroutine after it's been awaited. This enables thousands of concurrent operations (coroutines) on a small thread pool.

### `std::generator<T>` (C++23) for Lazy Ranges

```cpp
#include <generator>

std::generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i)
        co_yield i;
}

// Lazy — elements generated on demand
for (int x : range(0, 1'000'000)) {
    if (x > 100) break;  // stops generation here, no memory for the full range
}
```

---

## 7.10 — Common Pitfalls: Deadlock, Livelock, Starvation

### Deadlock

**Definition:** Two or more threads are permanently blocked, each waiting for a resource held by another.

**Classic example — circular lock order:**

```cpp
std::mutex mtx_a, mtx_b;

// Thread 1
std::lock_guard la(mtx_a);   // acquires A
std::lock_guard lb(mtx_b);   // waits for B → but Thread 2 holds B

// Thread 2 (running simultaneously)
std::lock_guard lb(mtx_b);   // acquires B
std::lock_guard la(mtx_a);   // waits for A → but Thread 1 holds A
// → Both threads wait forever
```

**Prevention:**
1. **Consistent lock ordering** — always acquire locks in the same global order (e.g., by address or by an agreed-upon numbering)
2. **`std::lock` / `std::scoped_lock`** — acquire multiple locks atomically
3. **Lock hierarchy** — assign each mutex a level; only acquire higher-level locks while holding lower-level ones
4. **Timeout** — `try_lock_for` with a timeout and retry/abort

**Detection:** Deadlock is reproducible under certain timing. ThreadSanitizer can detect lock ordering violations. Helgrind (Valgrind tool) builds a lock-order graph and warns about cycles.

### Deadlock With Condition Variables

Forgetting to hold the mutex when calling `notify`:

```cpp
// WRONG — data and ready may not be visible to consumer
data = 42;
data_ready = true;
cv.notify_one();   // notifying without the lock — consumer may miss it

// CORRECT
{
    std::lock_guard lock(mtx);
    data = 42;
    data_ready = true;
}                   // release lock first
cv.notify_one();    // then notify — prevents spurious missed wakeup
```

Missing the predicate in `cv.wait` means spurious wakeups cause incorrect execution:

```cpp
// WRONG — wakes on spurious wakeup even if data_ready is false
cv.wait(lock);
use(data);   // data may not be ready

// CORRECT
cv.wait(lock, []{ return data_ready; });
```

### Livelock

**Definition:** Two threads continuously change their state in response to each other but never make progress.

```cpp
// Two threads yielding to each other forever — neither proceeds
while (true) {
    if (other_thread_trying()) {
        this_thread_yield();   // be polite — let the other go first
        continue;
    }
    do_work();
    break;
}
// If both threads see the other trying and both yield simultaneously,
// they both retry, see each other again, yield again...
```

Livelocks are rarer than deadlocks. Solutions: randomized backoff (wait a random duration before retrying), priority assignment, or a centralized coordinator.

### Starvation

**Definition:** One thread is perpetually denied access to a resource because other threads always get priority.

Common causes:
- A high-priority thread always preempts a low-priority thread
- Many readers with `std::shared_mutex` preventing a writer from ever acquiring the exclusive lock
- An unfair scheduling policy

Solutions: use fair locks, limit thread priority spread, or use queue-based work distribution.

### Priority Inversion

A high-priority thread is blocked waiting for a resource held by a low-priority thread. If a medium-priority thread then preempts the low-priority thread, the high-priority thread is effectively blocked by a medium-priority thread — its priority has been "inverted."

Common in real-time systems. Solution: **priority inheritance** — when a low-priority thread holds a lock wanted by a high-priority thread, temporarily boost the low-priority thread's priority.

### The Thread-Safe Singleton Pattern

Two correct ways:

```cpp
// Method 1: Static local variable — guaranteed thread-safe since C++11
Singleton& instance() {
    static Singleton s;   // initialized exactly once, even with concurrent calls
    return s;
}

// Method 2: std::call_once
std::once_flag flag;
std::unique_ptr<Singleton> singleton_ptr;

Singleton& instance() {
    std::call_once(flag, []{ singleton_ptr = std::make_unique<Singleton>(); });
    return *singleton_ptr;
}
```

**The Broken Double-Checked Locking Pattern (DCLP):**

```cpp
// WRONG — was common pre-C++11, has a race on the check
Singleton* instance() {
    if (ptr == nullptr) {            // (1) read ptr without lock
        std::lock_guard lock(mtx);
        if (ptr == nullptr) {        // (2) read ptr with lock
            ptr = new Singleton();   // (3) write ptr — reorder: memory written, ptr set,
        }                            //     but construction may not be complete when
    }                                //     (1) sees non-null ptr on another thread
    return ptr;
}
// Fix: make ptr std::atomic<Singleton*> with proper memory_order
```

Use the static local variable approach — it's simpler, correct, and zero-overhead.

### Common Concurrency Mistakes Summary

| Mistake | Consequence | Fix |
|---|---|---|
| Shared data without mutex | Data race — UB | `std::mutex` + `lock_guard` |
| Inconsistent lock order | Deadlock | `std::scoped_lock` or consistent ordering |
| Locking too coarsely | Serialization, no parallelism | Narrow critical sections |
| Locking too finely | Overhead, harder to reason about | Identify true sharing boundaries |
| Missing predicate in `cv.wait` | Spurious wakeup bugs | Always use the predicate overload |
| Capturing reference in thread lambda | Dangling reference | Capture by value or ensure lifetime |
| `std::async` result ignored | Thread blocks at ~destructor | Store the future |
| Using `volatile` for threading | No effect on modern compilers | Use `std::atomic` |
| Double-checked locking without atomics | Data race | Static local variable or `call_once` |
| Detached thread accessing local data | Use-after-free | Ensure lifetimes; prefer `join` over `detach` |

### `volatile` Is NOT for Threading

`volatile` tells the compiler not to cache a variable in a register — it was historically misused for threading. It does not:
- Prevent data races
- Provide memory ordering guarantees
- Make operations atomic

```cpp
volatile int flag = 0;   // WRONG for threading
std::atomic<int> flag{0}; // CORRECT
```

`volatile` is only for hardware-mapped registers, memory-mapped I/O, and `setjmp`/`longjmp` interoperability. For all thread communication: use `std::atomic` or synchronize with a mutex.
