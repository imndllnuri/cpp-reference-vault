# Part 3 — Memory Management

> Memory management is where C++ earns its reputation for both power and danger. Understanding how memory works at the machine level — and how to manage it correctly with RAII and smart pointers — is non-negotiable for writing correct C++ code.

## Table of Contents

- [[#3.1 — Stack vs Heap]]
- [[#3.2 — new and delete]]
- [[#3.3 — RAII: Resource Acquisition Is Initialization]]
- [[#3.4 — std::unique_ptr]]
- [[#3.5 — std::shared_ptr & Reference Counting]]
- [[#3.6 — std::weak_ptr — Breaking Cycles]]
- [[#3.7 — Memory Leaks, Dangling Pointers, Double Free]]
- [[#3.8 — Tools: Valgrind, AddressSanitizer]]
- [[#3.9 — Placement new & Custom Allocators]]

---

## 3.1 — Stack vs Heap

### Two Regions of Memory

Every running process has (at minimum) two regions where objects can live: the **stack** and the **heap**. They behave completely differently.

### The Stack

The stack is a contiguous region of memory managed automatically by the CPU and compiler. It operates as a LIFO (last in, first out) structure.

When a function is called, a **stack frame** is pushed: it contains the function's local variables, parameters, the return address, and saved registers. When the function returns, the frame is popped — all local variables are instantly destroyed.

```cpp
void foo() {
    int x = 10;           // on the stack — allocated when foo() is called
    double arr[100];      // also on the stack — 800 bytes
}                         // stack frame popped — x and arr instantly gone
```

**Characteristics of the stack:**
- **Extremely fast** — allocation is just decrementing the stack pointer (one instruction)
- **Automatic lifetime** — variables are destroyed when they go out of scope, guaranteed
- **Fixed and small** — typically 1–8 MB per thread. Exceeding it is a **stack overflow** (usually a crash)
- **No fragmentation** — it's purely sequential

**Stack overflow** happens when you recurse too deeply or allocate a huge array locally:

```cpp
void infinite() { infinite(); }   // stack overflow — each call adds a frame

void bad() {
    int huge[10'000'000];   // 40 MB on the stack — immediate stack overflow
}
```

### The Heap

The heap (also called the free store in C++) is a large pool of memory managed by the allocator. You explicitly request memory with `new` (or `malloc`) and release it with `delete` (or `free`).

```cpp
void foo() {
    int* p = new int(10);   // on the heap — survives beyond foo()
    // ...
    delete p;               // must be done manually
}
```

**Characteristics of the heap:**
- **Large** — limited by available RAM and virtual address space (gigabytes)
- **Manual lifetime** — you control when memory is allocated and freed
- **Slower** — allocation requires the allocator to find a free block, update bookkeeping
- **Fragmentation** — over time, free blocks become scattered; the allocator may struggle to find large contiguous blocks
- **Survives function returns** — you can allocate on the heap and return a pointer to it safely

### Object Lifetimes in C++

C++ formalizes four storage durations:

| Duration | Where | When created | When destroyed |
|----------|--------|-------------|----------------|
| **Automatic** | Stack | At point of declaration | When scope exits |
| **Dynamic** | Heap | On `new` | On `delete` |
| **Static** | Static region | Before `main` | After `main` returns |
| **Thread-local** | Per-thread stack/static | Thread start | Thread exit |

```cpp
int global_count = 0;           // static storage duration

void foo() {
    int local = 5;              // automatic storage duration
    static int call_count = 0;  // static storage duration, but scoped to foo
    int* heap_int = new int(7); // dynamic storage duration
    ++call_count;
    delete heap_int;
}

thread_local int per_thread;    // thread-local storage duration
```

### When to Use Each

**Use the stack (automatic storage) when:**
- The object is small (fits comfortably within the stack budget)
- The lifetime is tied to the current scope or function
- The size is known at compile time

**Use the heap (dynamic storage) when:**
- The size is not known at compile time (user input, file size)
- The object needs to outlive the function that created it
- The object is large (a few hundred KB or more)
- You need a variable number of objects (an array of unknown length)
- You need polymorphism (base class pointer to derived object)

In modern C++, you almost never call `new` directly — you use `std::vector`, `std::string`, `std::unique_ptr`, etc., which manage the heap internally.

---

## 3.2 — `new` and `delete`

### `new` — Allocating on the Heap

`new T` does two things: allocates memory for a `T` on the heap, then calls `T`'s constructor. It returns a `T*`.

```cpp
int* p = new int(42);           // allocate an int, initialize to 42
double* d = new double;         // allocate a double, default-initialized (indeterminate)
double* z = new double();       // allocate a double, value-initialized to 0.0

std::string* s = new std::string("hello");   // allocate and construct
```

### `new[]` — Allocating Arrays

```cpp
int* arr = new int[10];         // allocate array of 10 ints, default-initialized
int* arr2 = new int[10]();      // allocate and zero-initialize all 10
int* arr3 = new int[n];         // n can be a runtime value — unlike stack arrays
```

### `delete` and `delete[]`

`delete` calls the destructor and then frees the memory. **You must use `delete[]` for arrays allocated with `new[]` — mixing them is undefined behavior.**

```cpp
delete p;       // for objects allocated with new T
delete[] arr;   // for arrays allocated with new T[n]

delete[] p;     // UNDEFINED BEHAVIOR — allocated with new T, not new T[]
delete arr;     // UNDEFINED BEHAVIOR — allocated with new T[], not new T
```

### What Happens When `new` Fails

By default, if the allocator cannot satisfy the request, `new` throws `std::bad_alloc`:

```cpp
#include <new>

try {
    int* p = new int[1'000'000'000'000LL];   // probably fails
} catch (const std::bad_alloc& e) {
    std::cerr << "Allocation failed: " << e.what() << "\n";
}
```

To get a `nullptr` instead of an exception, use `std::nothrow`:

```cpp
int* p = new (std::nothrow) int[large_size];
if (!p) {
    // handle failure
}
```

### Ownership — The Fundamental Problem

The hardest part of raw `new`/`delete` is **ownership**: which piece of code is responsible for calling `delete`? This is implicit and unenforced.

```cpp
int* create() {
    return new int(42);   // caller must delete — but this is not in the type system
}

void use() {
    int* p = create();
    // ... what if an exception is thrown here? delete is never called.
    delete p;
}
```

Raw owning pointers make ownership invisible to the compiler and fragile in the presence of exceptions. This is why smart pointers exist.

### `delete` on `nullptr` Is Safe

Calling `delete nullptr` or `delete[] nullptr` does nothing. It's defined behavior and safe:

```cpp
int* p = nullptr;
delete p;    // safe — no-op
```

This means you don't need null checks before deleting.

### Why Raw `new`/`delete` Is Dangerous

```cpp
void dangerous() {
    Resource* r = new Resource();
    might_throw();   // if this throws, r is never deleted — memory leak
    delete r;
}
```

Even without exceptions, manually tracking every `new` with a matching `delete` across large codebases is error-prone. The solution is RAII, covered next.

**Rule:** In modern C++, `new` should appear only inside constructors of RAII types (like `std::unique_ptr`) or in very specific low-level code. Never write `new` in application logic.

---

## 3.3 — RAII: Resource Acquisition Is Initialization

### The Concept

RAII is the most important idiom in C++. The rule:

> **Acquire a resource in a constructor. Release it in the destructor.**

Because destructors are called automatically when an object goes out of scope — even when exceptions are thrown — resource cleanup becomes guaranteed and implicit.

```cpp
class FileHandle {
    FILE* file;
public:
    explicit FileHandle(const char* path, const char* mode)
        : file(fopen(path, mode)) {
        if (!file) throw std::runtime_error("cannot open file");
    }

    ~FileHandle() {
        if (file) fclose(file);   // guaranteed to run
    }

    FILE* get() const { return file; }
};

void process() {
    FileHandle f("data.txt", "r");   // file opened
    // ... read file ...
    // no explicit fclose needed — ~FileHandle() calls it when f goes out of scope
}   // even if an exception is thrown above, f is destroyed here, file is closed
```

### Why RAII Is Essential: The Exception Problem

Without RAII, every resource cleanup requires manual code at every possible exit point:

```cpp
// Without RAII — fragile
void process() {
    FILE* f = fopen("data.txt", "r");
    if (!f) return;

    if (some_error()) {
        fclose(f);   // must remember to close here...
        return;
    }

    might_throw();   // if this throws, fclose is never called — leak!

    fclose(f);       // ...and here
}
```

With RAII, there is one cleanup point — the destructor — and it runs unconditionally:

```cpp
// With RAII — safe
void process() {
    FileHandle f("data.txt", "r");
    if (some_error()) return;   // destructor called — file closed
    might_throw();              // if thrown — destructor called — file closed
}                               // destructor called — file closed
```

### Resources RAII Manages

Any resource that must be released after use is a candidate for RAII:

| Resource | Acquire | Release | RAII wrapper |
|----------|---------|---------|--------------|
| Heap memory | `new` | `delete` | `std::unique_ptr`, `std::shared_ptr` |
| File | `fopen` / open | `fclose` / close | `std::ifstream`, `std::ofstream` |
| Mutex | `lock()` | `unlock()` | `std::lock_guard`, `std::unique_lock` |
| Network socket | `connect()` / `accept()` | `close()` | custom wrapper |
| Database connection | `open()` | `close()` | custom wrapper |
| GPU memory | `cudaMalloc` | `cudaFree` | custom wrapper |

### Writing Your Own RAII Wrapper

When a resource doesn't have a standard wrapper, write one. It's always the same pattern:

```cpp
class MutexLock {
    std::mutex& mtx;
public:
    explicit MutexLock(std::mutex& m) : mtx(m) { mtx.lock(); }
    ~MutexLock() { mtx.unlock(); }

    // Non-copyable, non-movable — owning a lock is not transferable
    MutexLock(const MutexLock&) = delete;
    MutexLock& operator=(const MutexLock&) = delete;
};

void critical_section(std::mutex& m) {
    MutexLock lock(m);   // mutex locked
    // ... do work ...
}   // mutex unlocked — even on exception
```

(The standard already provides `std::lock_guard` for exactly this — but this shows the pattern.)

### RAII and the Rule of Three/Five/Zero

RAII types typically manage a unique resource and should not be accidentally copied (copying a file handle would mean two closes for one file). They either:
- Delete copy operations and define move operations (like `std::unique_ptr`)
- Define deep copy (like `std::string`, `std::vector`)
- Delete both copy and move (like `std::mutex`, `std::lock_guard`)

See [[Part 2 — Object-Oriented Programming]] section 2.11 and 2.12 for the Rule of Three/Five/Zero.

---

## 3.4 — `std::unique_ptr`

### What It Is

`std::unique_ptr<T>` is an RAII wrapper for a heap-allocated object that enforces **sole ownership**: exactly one `unique_ptr` owns the resource at any time. When the `unique_ptr` is destroyed or reset, it automatically deletes the managed object.

```cpp
#include <memory>

std::unique_ptr<int> p = std::make_unique<int>(42);
// *p == 42
// when p goes out of scope, delete is called automatically
```

### Always Use `make_unique` — Never `new`

```cpp
// WRONG — don't do this
std::unique_ptr<Foo> p(new Foo(args));

// CORRECT
auto p = std::make_unique<Foo>(args);
```

`std::make_unique` (C++14) is safer because it prevents a subtle memory leak in function arguments (the order of evaluation of `new` and other arguments was unspecified before C++17). It also reads more cleanly.

### Sole Ownership — No Copying

`unique_ptr` cannot be copied — copying would mean two owners, violating the invariant:

```cpp
auto p1 = std::make_unique<int>(10);
auto p2 = p1;             // ERROR — copy is deleted
auto p2 = std::move(p1);  // OK — p1 gives up ownership, p2 now owns it
// p1 is now nullptr — do not use it
```

After a move, the source `unique_ptr` is `nullptr`. Dereferencing it is undefined behavior.

### Key Operations

```cpp
auto p = std::make_unique<std::string>("hello");

*p;               // dereference — "hello"
p->size();        // member access — 5
p.get();          // raw pointer — does NOT transfer ownership
p.reset();        // destroy the object, p becomes nullptr
p.reset(new std::string("world"));  // destroy old, own new
p.release();      // give up ownership, return raw pointer — YOU must delete it
bool b = (bool)p; // true if p owns an object, false if nullptr
```

### Passing `unique_ptr` to Functions

The right way depends on what the function needs:

```cpp
// Function uses the object but doesn't take ownership — pass by reference
void process(const std::string& s);
process(*p);   // dereference to get the object

// Function takes ownership — pass by value (move)
void store(std::unique_ptr<std::string> s);
store(std::move(p));   // p is now nullptr

// Function might take ownership — pass raw pointer
void maybe_store(std::string* s);
maybe_store(p.get());  // does not transfer ownership
```

The general rule: **pass `*ptr` or `ptr.get()` when the function doesn't take ownership. Pass `std::move(ptr)` when it does.**

### Returning `unique_ptr` from Functions — The Factory Pattern

```cpp
std::unique_ptr<Shape> make_shape(ShapeType type) {
    switch (type) {
        case ShapeType::Circle:    return std::make_unique<Circle>(5.0);
        case ShapeType::Rectangle: return std::make_unique<Rectangle>(3.0, 4.0);
        default: return nullptr;
    }
}

auto shape = make_shape(ShapeType::Circle);
shape->area();   // calls Circle::area via virtual dispatch
```

The return is automatically moved (NRVO or implicit move). This is the idiomatic factory pattern in modern C++.

### `unique_ptr` for Arrays

```cpp
auto arr = std::make_unique<int[]>(100);   // allocates int[100]
arr[0] = 1;
arr[99] = 99;
// deleted with delete[] automatically
```

Prefer `std::vector<int>` over `unique_ptr<int[]>` — vectors give you size tracking, resize, and iterators.

### Custom Deleters

When the cleanup isn't `delete` (e.g., closing a FILE*, freeing with a C API):

```cpp
auto file_deleter = [](FILE* f) { fclose(f); };
std::unique_ptr<FILE, decltype(file_deleter)> f(fopen("data.txt", "r"), file_deleter);
```

---

## 3.5 — `std::shared_ptr` & Reference Counting

### What It Is

`std::shared_ptr<T>` implements **shared ownership**: multiple `shared_ptr` instances can own the same object. The object is destroyed when the **last** `shared_ptr` owning it is destroyed or reset.

```cpp
#include <memory>

auto p1 = std::make_shared<std::string>("hello");
auto p2 = p1;   // both p1 and p2 own the same string — reference count is 2

p1.reset();     // p1 releases — reference count drops to 1, string lives
p2.reset();     // p2 releases — reference count drops to 0, string is destroyed
```

### How Reference Counting Works

Every `shared_ptr` points to two things: the managed object and a **control block**. The control block holds:
- The **strong reference count** (number of `shared_ptr` instances owning the object)
- The **weak reference count** (number of `weak_ptr` instances observing it — see next section)
- The deleter

When you copy a `shared_ptr`, the strong count increments. When a `shared_ptr` is destroyed, the strong count decrements. When the strong count hits zero, the managed object is destroyed. When the weak count also hits zero, the control block is freed.

```cpp
auto p1 = std::make_shared<int>(42);
std::cout << p1.use_count();   // 1

auto p2 = p1;
std::cout << p1.use_count();   // 2
std::cout << p2.use_count();   // 2 — they share the same count

p2.reset();
std::cout << p1.use_count();   // 1
```

### Always Use `make_shared`

```cpp
// WRONG — two separate heap allocations: one for the int, one for the control block
std::shared_ptr<int> p(new int(42));

// CORRECT — single allocation for both object and control block
auto p = std::make_shared<int>(42);
```

`std::make_shared` is more efficient (one allocation vs two) and safer. Use it always.

### Thread Safety of `shared_ptr`

The **reference count** is modified atomically — it is safe to copy/destroy `shared_ptr` instances from multiple threads. However, the **managed object itself** is not thread-safe. Accessing `*p` from multiple threads without synchronization is a data race.

```cpp
auto p = std::make_shared<int>(0);

// Safe: copying the shared_ptr from multiple threads
auto copy1 = p;   // thread 1
auto copy2 = p;   // thread 2

// Not safe: accessing the int from multiple threads without a mutex
*p = 1;   // thread 1  ← data race if thread 2 also reads/writes *p simultaneously
```

### The Cost of `shared_ptr`

`shared_ptr` is not free:
- **Space:** each `shared_ptr` stores two pointers (object + control block). The control block is a separate allocation.
- **Time:** each copy increments an atomic counter; each destruction decrements it. Atomic operations have a cost, especially under contention.
- **Cache:** the control block and object may be in different cache lines.

`unique_ptr` has zero overhead over a raw pointer. `shared_ptr` has a measurable cost. **Don't use `shared_ptr` unless ownership is genuinely shared.** Most cases that seem to need `shared_ptr` actually need `unique_ptr` with careful ownership design.

### When to Use `shared_ptr`

- Caches that hand out the same object to multiple callers
- Event systems where multiple subscribers hold a reference to an event source
- Data structures where nodes are referenced from multiple places (e.g., graph nodes)
- Any situation where you genuinely cannot determine which owner will outlive the others

If you can determine a clear owner, use `unique_ptr` and pass raw pointers or references to non-owners.

---

## 3.6 — `std::weak_ptr` — Breaking Cycles

### The Problem: Reference Cycles

`shared_ptr` uses reference counting. If two objects hold `shared_ptr`s to each other, their reference counts never reach zero — neither is ever destroyed. This is a memory leak:

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;   // cycle!
};

auto a = std::make_shared<Node>();
auto b = std::make_shared<Node>();

a->next = b;   // a's count: 1, b's count: 2
b->prev = a;   // a's count: 2, b's count: 2

// When a and b go out of scope:
// a's count drops to 1 (b->prev still holds it) — not destroyed
// b's count drops to 1 (a->next still holds it) — not destroyed
// Both leak forever
```

### `weak_ptr` — A Non-Owning Observer

`std::weak_ptr<T>` observes a `shared_ptr`-managed object **without participating in ownership**. It does not increment the strong reference count.

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;   // non-owning — breaks the cycle
};

auto a = std::make_shared<Node>();
auto b = std::make_shared<Node>();

a->next = b;       // b's strong count: 2
b->prev = a;       // a's strong count: 1 — weak_ptr doesn't increment

// When a and b go out of scope:
// a's strong count: 0 — a is destroyed, a->next (= b) releases b's strong ref
// b's strong count: 0 — b is destroyed
// No leak!
```

### Using `weak_ptr` — Always Lock Before Use

A `weak_ptr` cannot be dereferenced directly. You must convert it to a `shared_ptr` first using `lock()`. If the object has already been destroyed, `lock()` returns an empty `shared_ptr`:

```cpp
std::weak_ptr<int> weak;
{
    auto shared = std::make_shared<int>(42);
    weak = shared;

    if (auto p = weak.lock()) {   // lock() returns shared_ptr<int>
        std::cout << *p;          // prints 42 — object still alive
    }
}
// shared goes out of scope — object destroyed

if (auto p = weak.lock()) {
    // not entered — lock() returns empty shared_ptr
} else {
    std::cout << "object expired\n";
}

weak.expired();   // true — convenient check without locking
```

### Common Use Cases

**Parent-child back-pointers** — a child holds a `weak_ptr` to its parent to avoid a cycle:

```cpp
struct TreeNode {
    std::vector<std::shared_ptr<TreeNode>> children;
    std::weak_ptr<TreeNode> parent;   // back-pointer — non-owning
};
```

**Observer pattern** — observers hold `weak_ptr` to the subject; when the subject is destroyed, observers detect it via `expired()`:

```cpp
class EventSource {
    std::vector<std::weak_ptr<Listener>> listeners;
public:
    void notify() {
        listeners.erase(
            std::remove_if(listeners.begin(), listeners.end(),
                [](const auto& w) { return w.expired(); }),
            listeners.end());
        for (auto& w : listeners) {
            if (auto l = w.lock()) l->on_event();
        }
    }
};
```

**Caches** — the cache holds `weak_ptr`; if the object is still alive (because someone else holds a `shared_ptr`), the cache returns it; otherwise it recreates it:

```cpp
std::map<int, std::weak_ptr<Resource>> cache;

std::shared_ptr<Resource> get(int id) {
    auto it = cache.find(id);
    if (it != cache.end()) {
        if (auto p = it->second.lock()) return p;   // still alive
    }
    auto p = std::make_shared<Resource>(id);
    cache[id] = p;
    return p;
}
```

---

## 3.7 — Memory Leaks, Dangling Pointers, Double Free

These are the three classic memory errors in C++. All are **undefined behavior** (except leaks, which are merely wasteful). Understanding them precisely is essential for debugging.

### Memory Leaks

A memory leak occurs when allocated memory is never freed. The process's memory usage grows without bound over time.

```cpp
void leak() {
    int* p = new int(42);
    // function returns — p goes out of scope
    // delete is never called — 4 bytes leaked
}

void leak_on_exception() {
    int* p = new int(42);
    might_throw();   // if this throws, p is never deleted
    delete p;
}
```

Leaks don't crash immediately — they degrade performance and eventually exhaust memory. In short-lived programs they're often harmless; in servers that run for days, they're fatal.

**Detection:** LeakSanitizer (built into ASan on Linux), Valgrind's memcheck.
**Prevention:** RAII, smart pointers, never raw `new` in application code.

### Dangling Pointers (Use-After-Free)

A dangling pointer points to memory that has been freed. Using it is undefined behavior — it may appear to work, crash immediately, corrupt data silently, or behave differently on different runs.

```cpp
int* p = new int(42);
delete p;
*p = 10;   // UNDEFINED BEHAVIOR — p is dangling
std::cout << *p;   // UNDEFINED BEHAVIOR
```

```cpp
int* get_local() {
    int x = 42;
    return &x;   // DANGER: returning pointer to local variable
}               // x is destroyed here

int* p = get_local();
*p;   // UNDEFINED BEHAVIOR — x's stack memory may be reused
```

Common causes:
- Using a pointer after `delete`
- Returning a pointer/reference to a local variable
- Keeping a raw pointer to an element in a container that was resized (vector reallocation invalidates all pointers to its elements)
- `shared_ptr` with a raw pointer alias: holding a raw pointer and separately holding a `shared_ptr` that can destroy the object

**Detection:** AddressSanitizer (`-fsanitize=address`) catches use-after-free with a precise error and stack trace.

### Double Free

Calling `delete` on the same pointer twice is undefined behavior. The allocator's internal bookkeeping is corrupted, which often leads to a crash or, worse, an exploitable security vulnerability.

```cpp
int* p = new int(42);
delete p;
delete p;   // UNDEFINED BEHAVIOR — double free
```

```cpp
int* p = new int(42);
int* q = p;   // q and p point to the same object
delete p;
delete q;   // UNDEFINED BEHAVIOR — same object deleted twice
```

**Prevention:** Set pointers to `nullptr` after deleting (so a second delete is a no-op). Better: use smart pointers, which cannot double-free.

### Wild Pointers (Uninitialized Pointers)

An uninitialized pointer holds a garbage address. Dereferencing it is undefined behavior:

```cpp
int* p;   // uninitialized — holds garbage
*p = 10;  // UNDEFINED BEHAVIOR — writing to random memory
```

Always initialize pointers: to `nullptr` if you don't have a value yet, or to a valid address.

### Stack Use After Return

Returning a pointer or reference to a local variable:

```cpp
int& danger() {
    int x = 42;
    return x;   // WARNING: reference to local variable returned
}               // x is destroyed

int& r = danger();
r = 10;   // UNDEFINED BEHAVIOR — modifying destroyed stack memory
```

Compilers warn about this (`-Wall`). Never ignore this warning.

### Use After Move

After `std::move`, the source object is in a "valid but unspecified state". It is not undefined behavior to use it (unlike the others above), but the value is unpredictable:

```cpp
std::string s = "hello";
std::string t = std::move(s);   // s's resources stolen

std::cout << s;   // valid but prints "" or something empty — not "hello"
s = "world";      // OK — reassigning a moved-from object is always safe
```

---

## 3.8 — Tools: Valgrind, AddressSanitizer

### AddressSanitizer (ASan)

ASan is a compiler-based memory error detector built into GCC and Clang. Covered in [[Part 0 — Environment & Toolchain]] section 0.7, but worth elaborating here on how to read its output.

**Enable:**
```sh
clang++ -std=c++23 -g -fsanitize=address,leak -Wall -Wextra -o main main.cpp
```

**What ASan catches:**
- Use-after-free (dangling pointers)
- Heap buffer overflows (reading/writing past allocated bounds)
- Stack buffer overflows
- Use-after-return
- Use-after-scope
- Memory leaks (via LeakSanitizer, included in `-fsanitize=address` on Linux)

**Reading ASan output:**

```
==12345==ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010
READ of size 4 at 0x602000000010 thread T0
    #0 0x401234 in main main.cpp:8
    #1 0x7f... in __libc_start_main

0x602000000010 was freed at:
    #0 0x401200 in main main.cpp:6

previously allocated at:
    #0 0x401190 in main main.cpp:5
```

Read it bottom-up: "previously allocated at" tells you where the memory came from. "was freed at" tells you where it was deleted. The top stack trace tells you where you used it after freeing.

### LeakSanitizer (LSan)

On Linux, LeakSanitizer is bundled with ASan. It reports leaks at program exit:

```
==12345==ERROR: LeakSanitizer: detected memory leaks
Direct leak of 4 byte(s) in 1 object(s) allocated from:
    #0 0x... in operator new(unsigned long)
    #1 0x401234 in leak() main.cpp:3
```

### Valgrind

Valgrind is a runtime instrumentation framework. The primary tool is **Memcheck**.

```sh
g++ -g -o main main.cpp   # compile with debug symbols, no special flags needed
valgrind --leak-check=full --track-origins=yes ./main
```

**What Valgrind Memcheck catches:**
- Use of uninitialized memory
- Use-after-free
- Memory leaks (direct and indirect)
- Double free
- Invalid heap reads/writes (overflows)
- Mismatched `new`/`delete` vs `new[]`/`delete[]`

**`--track-origins=yes`** tells Valgrind to track where uninitialized memory came from — very useful for diagnosing the root cause.

**Valgrind vs ASan — comparison:**

| | Valgrind | ASan |
|--|---------|------|
| Requires recompilation | No | Yes (`-fsanitize=address`) |
| Slowdown | ~10–50x | ~2x |
| Detects uninitialized reads | Yes | Partially (with MSan separately) |
| Detects stack errors | Limited | Yes |
| Works with any binary | Yes | No — must be compiled in |
| Output detail | Good | Excellent |

**Workflow:** Use ASan during development (compiled in, fast). Use Valgrind when you need to debug a binary you can't recompile, or when you specifically need uninitialized-read detection (use `-fsanitize=memory` for that with Clang).

### MemorySanitizer (MSan)

Catches reads of uninitialized memory — something ASan doesn't fully cover:

```sh
clang++ -std=c++23 -g -fsanitize=memory -o main main.cpp
```

Clang only. Cannot be combined with ASan. More expensive than ASan (~3x overhead).

---

## 3.9 — Placement `new` & Custom Allocators

### Placement `new`

Placement `new` constructs an object at a **specific, already-allocated memory address**. It does not allocate memory — it only calls the constructor.

```cpp
#include <new>

char buffer[sizeof(int)];         // raw storage — can be stack, pool, shared memory
int* p = new (buffer) int(42);    // construct int at buffer's address

// No delete! The memory isn't heap-allocated.
// You must call the destructor manually:
p->~int();   // explicit destructor call — only needed with placement new
```

**Why explicit destructor?** Because `delete p` would try to free `buffer` as if it were heap memory, which it isn't. Placement `new` separates "allocate memory" from "construct object" — so you must also separate "destruct object" from "free memory."

### Use Cases for Placement `new`

**Memory pools:** Pre-allocate a large block, then construct objects inside it without going through the allocator repeatedly:

```cpp
class Pool {
    alignas(std::max_align_t) char storage[1024 * 1024];  // 1 MB pool
    size_t offset = 0;
public:
    template<typename T, typename... Args>
    T* allocate(Args&&... args) {
        size_t align = alignof(T);
        offset = (offset + align - 1) & ~(align - 1);  // align up
        T* ptr = new (storage + offset) T(std::forward<Args>(args)...);
        offset += sizeof(T);
        return ptr;
    }
};
```

Objects in the pool are destroyed by calling their destructors individually; the pool itself is freed at once when the pool object is destroyed.

**Shared memory:** OS shared memory is a raw byte region. To put a C++ object in it:

```cpp
void* shm = mmap(...);   // OS shared memory region
MyStruct* obj = new (shm) MyStruct();   // construct C++ object in shared memory
```

**Embedded systems:** On microcontrollers, you may have specific memory regions (SRAM, flash, DMA buffers) where objects must live.

### `alignas` and Alignment

Objects have alignment requirements — they must be placed at addresses that are multiples of their alignment. Misaligned access is undefined behavior on most platforms (and a crash on architectures like ARM that enforce it):

```cpp
alignas(16) char buffer[128];   // buffer is 16-byte aligned — suitable for SIMD types
```

When writing a memory pool, always respect alignment. `std::aligned_storage` (deprecated in C++23) or `alignas` + `char[]` are the tools.

### `std::allocator` — The Default STL Allocator

All STL containers have a template parameter for a custom allocator, defaulting to `std::allocator<T>`:

```cpp
std::vector<int, std::allocator<int>> v;   // equivalent to std::vector<int>
```

`std::allocator<T>` calls `operator new` / `operator delete`. You can replace it with a custom allocator that serves memory from a pool, a specific region, or with custom tracking.

### Writing a Custom Allocator (Minimal Example)

```cpp
template<typename T>
struct PoolAllocator {
    using value_type = T;

    PoolAllocator() = default;
    template<typename U> PoolAllocator(const PoolAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        return static_cast<T*>(pool.allocate(n * sizeof(T)));
    }
    void deallocate(T* p, std::size_t n) noexcept {
        pool.deallocate(p, n * sizeof(T));
    }
};

std::vector<int, PoolAllocator<int>> v;   // vector uses your pool
```

Custom allocators are complex. They're used in game engines, real-time systems, and high-frequency trading where allocator performance is critical.

### Polymorphic Memory Resources (C++17) — `std::pmr`

C++17 introduced `std::pmr` (polymorphic memory resources) — a cleaner alternative to custom allocators that doesn't require template parameters:

```cpp
#include <memory_resource>

char buffer[1024];
std::pmr::monotonic_buffer_resource pool(buffer, sizeof(buffer));

std::pmr::vector<int> v(&pool);   // vector allocates from our stack buffer
v.push_back(1);
v.push_back(2);
// no heap allocation happened — everything came from buffer
```

**Key `std::pmr` resources:**

| Resource | Behavior |
|----------|---------|
| `monotonic_buffer_resource` | Allocates linearly from a buffer, never frees individually — free all at once |
| `unsynchronized_pool_resource` | Pool allocator, single-threaded |
| `synchronized_pool_resource` | Pool allocator, thread-safe |
| `new_delete_resource()` | Falls back to global `new`/`delete` |

`monotonic_buffer_resource` is the fastest possible allocator for scratch/temporary work — allocation is just a pointer bump, and deallocation is a no-op. Free everything at the end by destroying the resource.

`std::pmr` lets you write container code once and swap out the allocator at runtime — a major improvement over the C++11 allocator model where allocators were baked into the type.
