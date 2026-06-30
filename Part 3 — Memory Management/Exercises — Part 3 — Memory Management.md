# Exercises — Part 3 — Memory Management

> Practice problems for [[Part 3 — Memory Management]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 3.1 — Storage Durations

**Ex 3.1.1** `[Easy]`

Classify each variable by storage duration (automatic, static, dynamic, thread-local) and state its lifetime:

```cpp
int global = 1;
thread_local int tl = 2;

void foo() {
    static int s = 3;
    int local = 4;
    int* heap = new int(5);
}
```

```
// your answer
```

---

**Ex 3.1.2** `[Medium]`

This function has a bug. What is it? What would a debugger or ASan show?

```cpp
int* get_value() {
    int x = 42;
    return &x;
}
int main() {
    int* p = get_value();
    std::cout << *p << "\n";   // UB
}
```

```
// your answer
```

---

**Ex 3.1.3** `[Medium]`

Explain what causes a stack overflow. Write a program that reliably triggers one. What is the typical default stack size on Linux?

```cpp
// your solution
```

---

## Section 3.2 — `new` and `delete`

**Ex 3.2.1** `[Easy]`

Each snippet mismatches `new`/`delete`. For each: what is the bug, and what is the correct fix?

```cpp
// A
int* p = new int[10];
delete p;

// B
int* p = new int(5);
delete[] p;

// C
int* p = new int(5);
delete p;
delete p;
```

```
// your answer
```

---

**Ex 3.2.2** `[Medium]`

What is `std::bad_alloc`? Write a program that catches it. When does `new` throw vs return null? How do you request null-on-failure instead of an exception?

```cpp
// your solution
```

---

**Ex 3.2.3** `[Hard]`

Implement a custom global `operator new` and `operator delete` that track the total number of live allocations and total bytes currently allocated. Print a report in `main` after all allocations.

```cpp
// your solution
```

---

## Section 3.3 — RAII: Resource Acquisition Is Initialization

**Ex 3.3.1** `[Easy]`

Rewrite this function to be exception-safe using RAII. The current version leaks memory if `process()` throws.

```cpp
void bad_function() {
    int* buf = new int[1024];
    process(buf);          // may throw
    delete[] buf;
}
```

```cpp
// your solution — use std::vector or unique_ptr
```

---

**Ex 3.3.2** `[Medium]`

Implement a `MutexLock` RAII class that takes a `std::mutex&`, locks it on construction, and unlocks it on destruction. It must be non-copyable and non-movable.

```cpp
class MutexLock {
    // your solution
};
```

---

**Ex 3.3.3** `[Medium]`

Implement a `ScopeGuard` class that takes any callable and invokes it in its destructor — used for cleanup actions at scope exit regardless of how the scope exits (normal return or exception).

```cpp
class ScopeGuard {
    // your solution
};

// Usage example:
void foo() {
    auto cleanup = ScopeGuard([]{ std::cout << "cleaning up\n"; });
    // cleanup fires even if an exception is thrown
}
```

---

## Section 3.4 — `std::unique_ptr`

**Ex 3.4.1** `[Easy]`

Rewrite this code using `std::unique_ptr`. Ensure no explicit `delete` is needed.

```cpp
void old_style() {
    Widget* w = new Widget("hello");
    w->process();
    delete w;
}
```

```cpp
// your solution
```

---

**Ex 3.4.2** `[Medium]`

`unique_ptr` is non-copyable. This code doesn't compile. Fix it the correct way — don't make it copyable.

```cpp
std::unique_ptr<int> make_int() { return std::make_unique<int>(42); }

void take_ownership(std::unique_ptr<int> p) {
    std::cout << *p << "\n";
}

int main() {
    auto p = make_int();
    take_ownership(p);    // error
}
```

```cpp
// your solution
```

---

**Ex 3.4.3** `[Medium]`

Write a `unique_ptr` with a custom deleter for a `FILE*` resource — the deleter must call `fclose`. Use `std::unique_ptr<FILE, ...>`.

```cpp
// your solution
```

---

**Ex 3.4.4** `[Hard]`

Implement `make_unique` yourself as a variadic template function. It must forward all arguments to `T`'s constructor and return a `unique_ptr<T>`.

```cpp
template<typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    // your solution
}
```

---

## Section 3.5 — `std::shared_ptr`

**Ex 3.5.1** `[Easy]`

What is the reference count after each line? What triggers the destructor?

```cpp
auto a = std::make_shared<int>(10);    // count?
auto b = a;                            // count?
{
    auto c = b;                        // count?
}                                      // count after scope exit?
a.reset();                             // count?
b.reset();                             // count? destructor fires?
```

```
// your answer
```

---

**Ex 3.5.2** `[Medium]`

Why should you prefer `std::make_shared<T>()` over `std::shared_ptr<T>(new T())`? Give two distinct reasons.

```
// your answer
```

---

**Ex 3.5.3** `[Hard]`

Implement a thread-safe reference-counted smart pointer `SharedPtr<T>` from scratch using `std::atomic<int>` for the reference count. Support copy, move, dereference, and automatic deletion.

```cpp
template<typename T>
class SharedPtr {
    // your solution
};
```

---

## Section 3.6 — `std::weak_ptr` — Breaking Cycles

**Ex 3.6.1** `[Medium]`

This code has a memory leak. Identify the cycle and fix it using `weak_ptr`.

```cpp
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;
    ~Node() { std::cout << "~Node\n"; }
};

int main() {
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->next = b;
    b->prev = a;
    // destructors never called — memory leaked
}
```

```cpp
// your solution
```

---

**Ex 3.6.2** `[Medium]`

Implement an observer pattern where the subject holds `weak_ptr` to observers. When an observer is destroyed, the subject skips it on notification without explicit unregistration.

```cpp
// your solution
```

---

## Section 3.7 — Common Memory Errors

**Ex 3.7.1** `[Medium]`

Identify the memory error type (leak, dangling pointer, double-free, buffer overflow, use-after-move) in each snippet:

```cpp
// A
std::string s = "hello";
std::string t = std::move(s);
std::cout << s.size();  // is this UB?

// B
std::vector<int> v = {1, 2, 3};
auto it = v.begin();
v.push_back(4);
std::cout << *it;

// C
int* p = new int(5);
delete p;
*p = 10;

// D
char buf[4];
strcpy(buf, "hello world");

// E
void* p = malloc(100);
// p is never freed, function returns
```

```
// your answer — name each error type and consequence
```

---

**Ex 3.7.2** `[Hard]`

Write a program containing exactly one of each: a memory leak, a use-after-free, and an out-of-bounds write — all of which ASan would catch. Compile with ASan and document the output for each error.

```cpp
// your solution
```

---

## Section 3.8 — Tools: Valgrind, AddressSanitizer

**Ex 3.8.1** `[Easy]`

What is the key practical difference between Valgrind/memcheck and AddressSanitizer? When would you prefer each?

```
// your answer
```

---

**Ex 3.8.2** `[Medium]`

Given an ASan output that says:

```
ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010
READ of size 4 at 0x602000000010
  #0 main main.cpp:7
```

Write the minimal C++ program that would produce exactly this error (an integer heap allocation, freed, then read on line 7 of main.cpp).

```cpp
// your solution
```

---

## Section 3.9 — Custom Allocators & Placement New

**Ex 3.9.1** `[Medium]`

Use placement new to construct a `Widget` object in a pre-allocated `std::aligned_storage` buffer. Then explicitly call the destructor. Explain why you must call the destructor manually.

```cpp
// your solution
```

---

**Ex 3.9.2** `[Hard]`

Implement a simple `ArenaAllocator` that:
- Takes a fixed-size `char` buffer at construction
- Allocates objects by bumping a pointer (no individual frees)
- Has a `reset()` that makes all memory available again
- Works with `std::pmr::monotonic_buffer_resource` or as a standalone class

```cpp
class ArenaAllocator {
    // your solution
};
```
