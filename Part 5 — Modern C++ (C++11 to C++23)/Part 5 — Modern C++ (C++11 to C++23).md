# Part 5 — Modern C++ (C++11 to C++23)

> C++11 was a watershed moment — the committee effectively redesigned the language from the inside. Everything since has refined and extended that foundation. This part covers every major feature from C++11 through C++23, what problem each one solves, and how to use it correctly.

## Table of Contents

- [[#5.1 — C++11: auto, range-for, nullptr, uniform initialization]]
- [[#5.2 — C++11: Rvalue References & Move Semantics]]
- [[#5.3 — C++11: Lambdas]]
- [[#5.4 — C++11: constexpr, static_assert, noexcept]]
- [[#5.5 — C++11: Threading primitives (std::thread, std::mutex)]]
- [[#5.6 — C++14: Generic Lambdas, Variable Templates, std::make_unique]]
- [[#5.7 — C++17: Structured Bindings, if constexpr, fold expressions]]
- [[#5.8 — C++17: std::optional, std::variant, std::any]]
- [[#5.9 — C++17: std::filesystem, std::string_view]]
- [[#5.10 — C++20: Concepts & Constraints]]
- [[#5.11 — C++20: Modules]]
- [[#5.12 — C++20: Coroutines]]
- [[#5.13 — C++20: std::format, std::span, Ranges]]
- [[#5.14 — C++23: std::expected, std::print, flat_map, and more]]

---

## 5.1 — C++11: `auto`, range-for, `nullptr`, uniform initialization

### `auto` — Type Deduction

`auto` tells the compiler to deduce the variable's type from its initializer. It removes the need to write out long type names and keeps declarations in sync with their initializers automatically.

```cpp
auto x = 42;                          // int
auto d = 3.14;                        // double
auto s = std::string("hello");        // std::string
auto v = std::vector<int>{1, 2, 3};  // std::vector<int>

// The real value: complex iterator types
auto it = some_map.find("key");       // std::map<K,V>::iterator — without auto you'd write this
for (auto it = v.begin(); it != v.end(); ++it) { ... }
```

**`auto` drops references and const by default.** If you want to bind without copying:

```cpp
const std::vector<int>& get_data();

auto  data  = get_data();    // COPY — auto drops the reference
auto& data  = get_data();    // reference — no copy
const auto& data = get_data(); // const reference — no copy, no modify
auto&& data = get_data();    // forwarding reference — binds to anything
```

**When to use `auto`:**
- Iterator types, template return types, lambda types (which have no writable type)
- When the type is obvious from the right-hand side: `auto p = std::make_unique<Foo>()`
- Range-for loops

**When not to use `auto`:**
- When you want to be explicit about a type conversion: `auto x = static_cast<float>(some_int)` is fine, but `auto x = some_int` being float would be surprising
- In function signatures (don't use `auto` as a parameter type — use templates or concepts for that)
- When the type is not obvious: `auto result = foo()` where `foo`'s return type is not clear from context makes code harder to read

### Range-for — Simplified Iteration

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

// Old way
for (size_t i = 0; i < v.size(); ++i) { std::cout << v[i]; }
for (auto it = v.begin(); it != v.end(); ++it) { std::cout << *it; }

// Range-for
for (int x : v) { std::cout << x; }            // copy each element
for (const int& x : v) { std::cout << x; }     // read without copy
for (int& x : v) { x *= 2; }                   // modify in place
for (auto& x : v) { x *= 2; }                  // same, idiomatic
```

**How it works:** The compiler expands `for (T x : range)` into:

```cpp
{
    auto&& __range = range;
    auto __begin = __range.begin();
    auto __end   = __range.end();
    for (; __begin != __end; ++__begin) {
        T x = *__begin;
        // body
    }
}
```

Any type with `begin()` and `end()` (member or free functions) works — STL containers, `std::array`, raw arrays, and your own custom types.

**Always use `const auto&` for elements you don't modify** — it avoids copies for non-trivial types and works for any element type:

```cpp
for (const auto& elem : container) { ... }  // idiomatic read-only range-for
```

### `nullptr` — The Null Pointer Literal

`nullptr` replaces `NULL` and `0` for null pointers. It has type `std::nullptr_t`, which is distinct from all integer types.

```cpp
int* p = nullptr;          // correct
int* p = NULL;             // works but NULL is #define'd as 0 (integer) — avoid
int* p = 0;                // works but confusing — avoid

void foo(int x);
void foo(int* p);

foo(NULL);    // ambiguous — is it foo(int) or foo(int*)? NULL is 0, which is int
foo(nullptr); // unambiguous — calls foo(int*)
```

`nullptr` also works in boolean context:

```cpp
if (ptr)        // true if ptr is not null
if (!ptr)       // true if ptr is null
if (ptr != nullptr)  // explicit — equally valid
```

**Always use `nullptr`.** It's unambiguous, type-safe, and explicit.

### Uniform Initialization — Brace Initialization

C++11 introduced `{}` as a universal initialization syntax that works everywhere:

```cpp
// All valid in C++11
int x{5};
double d{3.14};
std::string s{"hello"};
std::vector<int> v{1, 2, 3, 4, 5};

int arr[]{1, 2, 3};    // arrays
struct Point { int x, y; };
Point p{3, 4};         // aggregate initialization

// Function return
Point make_point() { return {3, 4}; }

// Can be used everywhere, including new
auto* obj = new Foo{arg1, arg2};
```

**Solving the Most Vexing Parse:**

```cpp
Foo f();    // This is a FUNCTION DECLARATION, not a variable!
            // "Most vexing parse" — C++ interprets any ambiguity as a declaration

Foo f{};    // This is definitely a variable — default-constructed
Foo f = Foo{};  // Also works
```

**Narrowing conversion prevention** — `{}` prohibits implicit narrowing:

```cpp
int x = 3.7;    // OK — silently truncates to 3
int x{3.7};     // ERROR — narrowing conversion from double to int not allowed
int x{3};       // OK

int big = 1'000'000;
short s{big};   // ERROR — big doesn't fit in short
short s = big;  // silently truncates — no warning by default
```

**`std::initializer_list` — the `{}` trap with constructors:**

When a class has a constructor that takes `std::initializer_list<T>`, brace initialization prefers it over other constructors, even if other constructors seem like a better match:

```cpp
std::vector<int> v1(10, 0);   // 10 elements, all 0
std::vector<int> v2{10, 0};   // 2 elements: 10 and 0 — initializer_list constructor wins!
```

This is one of the few places where `()` and `{}` give different results. Be aware of it for `std::vector` especially.

---

## 5.2 — C++11: Rvalue References & Move Semantics

Move semantics were introduced in C++11 and are covered in depth in [[Part 2 — Object-Oriented Programming]] (section 2.12) and [[Part 3 — Memory Management]] (section 3.4 on `unique_ptr`). This section covers the deeper mechanics and the full value category system.

### Value Categories

Every expression in C++ has a type and a **value category**. The category determines whether the expression can be moved from and whether it has an identity (address in memory).

The full taxonomy:

```
        expression
       /          \
    glvalue      rvalue
    /     \      /    \
lvalue  xvalue  prvalue
```

- **lvalue** — has identity, cannot be moved from. Named variables, dereferenced pointers, function calls returning lvalue references. You can take its address.
- **prvalue** ("pure rvalue") — no identity, can be moved from. Temporaries, literals, function calls returning by value.
- **xvalue** ("expiring value") — has identity, can be moved from. The result of `std::move()`, functions returning `T&&`.
- **glvalue** ("generalized lvalue") — lvalue or xvalue. Has identity.
- **rvalue** — xvalue or prvalue. Can be moved from.

In practice, you work with two categories:
- **lvalue**: has a name, has an address — `x`, `arr[0]`, `*ptr`
- **rvalue**: temporary or explicitly moved — `42`, `foo()`, `std::move(x)`

### Rvalue References (`T&&`)

An rvalue reference binds to rvalues (temporaries and moved-from objects). It enables move operations:

```cpp
int x = 5;
int& lref  = x;      // lvalue reference — binds to x
int&& rref = 5;      // rvalue reference — binds to temporary 5
int&& rref = x;      // ERROR — x is an lvalue
int&& rref = std::move(x);  // OK — std::move casts x to rvalue reference type
```

### Forwarding References (Universal References)

When `T&&` appears in a **deduced context** (template parameter or `auto&&`), it is a **forwarding reference** — it can bind to either lvalues or rvalues:

```cpp
template<typename T>
void foo(T&& arg) { ... }   // T&& here is a forwarding reference, NOT necessarily rvalue ref

int x = 5;
foo(x);    // T deduced as int& — arg is int& (lvalue reference)
foo(5);    // T deduced as int  — arg is int&& (rvalue reference)
```

**Reference collapsing rules:**
- `T& &` → `T&`
- `T& &&` → `T&`
- `T&& &` → `T&`
- `T&& &&` → `T&&`

This is what makes forwarding references work — when called with an lvalue, `T` deduces as `T&`, and `T& &&` collapses to `T&`.

### `std::forward` — Perfect Forwarding

In a template that takes a forwarding reference, you want to forward the argument with its original value category preserved — if it came in as an lvalue, forward it as an lvalue; if rvalue, forward as rvalue:

```cpp
template<typename T>
void wrapper(T&& arg) {
    inner(std::forward<T>(arg));   // forward arg preserving its value category
}
```

Without `std::forward`, `arg` inside the function is always an lvalue (it has a name). `std::forward<T>(arg)` conditionally casts to `T&&` — if `T` is `T&` (lvalue case), the cast is `T& &&` = `T&`; if `T` is `T` (rvalue case), the cast is `T&&`.

This is the basis of `std::make_unique`, `std::emplace_back`, and every factory/forwarding function in the standard library:

```cpp
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

### Return Value Optimization (RVO / NRVO)

The compiler is allowed (and required in some cases) to elide copy/move construction when returning a local object:

```cpp
std::vector<int> make_vector() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    return v;   // NRVO — compiler constructs v directly in the caller's memory
}

auto vec = make_vector();   // zero copies, zero moves — v is constructed in place
```

**Named Return Value Optimization (NRVO)** — the local variable is constructed directly in the return slot. **RVO** applies to unnamed temporaries (`return T{args}`). Both are guaranteed or near-guaranteed in modern compilers.

**Rule:** Just return local variables by value. Don't `std::move` them in return statements — that actually disables NRVO:

```cpp
return v;              // CORRECT — enables NRVO
return std::move(v);   // WRONG — disables NRVO, forces a move instead of elision
```

---

## 5.3 — C++11: Lambdas

### Syntax

A lambda is an anonymous function object defined inline:

```cpp
[capture](parameters) -> return_type { body }
```

All parts except the capture and body are optional:

```cpp
auto greet = []{ std::cout << "hello\n"; };                    // no capture, no params
auto add   = [](int a, int b) { return a + b; };               // params, return deduced
auto mult  = [](int a, int b) -> int { return a * b; };        // explicit return type
greet();       // call like a function
add(3, 4);     // 7
```

### Capture List

The capture list specifies what variables from the enclosing scope the lambda can access:

```cpp
int x = 10, y = 20;

auto f1 = []{};              // capture nothing
auto f2 = [x]{ return x; }; // capture x by value — copy of x at lambda creation
auto f3 = [&x]{ return x; };// capture x by reference — reads x's current value
auto f4 = [=]{ return x+y; };// capture ALL referenced variables by value
auto f5 = [&]{ return x+y; };// capture ALL referenced variables by reference
auto f6 = [=, &y]{ return x+y; }; // all by value, y by reference
auto f7 = [&, x]{ return x+y; };  // all by reference, x by value
```

**By-value vs by-reference:**
- By value (`[x]`): the lambda holds its own copy. Safe if the lambda outlives the enclosing scope.
- By reference (`[&x]`): the lambda holds a reference. Dangling if the lambda outlives the variable. Never use `[&]` capture when storing a lambda that might outlive the current scope.

### `mutable` Lambdas

Captures by value are `const` by default — you cannot modify them inside the lambda. `mutable` removes this restriction:

```cpp
int counter = 0;
auto inc = [counter]() mutable { return ++counter; };  // modifies the copy
inc();   // 1
inc();   // 2
counter; // still 0 — the lambda modified its own copy
```

### Init Captures (C++14)

C++14 allows capturing expressions, not just existing variables — useful for moving into a lambda:

```cpp
auto p = std::make_unique<Foo>();

// Can't capture unique_ptr by value (not copyable)
// Can't capture by reference if lambda outlives p

auto f = [p = std::move(p)]{ p->do_something(); };  // move p into the lambda
// p is now nullptr outside the lambda
```

Init captures also allow creating new names:

```cpp
auto f = [x = expensive_computation()]{ return x * 2; };  // compute once, use many
```

### Lambda Type and Storage

Each lambda has a unique, unnamed type. Two identical-looking lambdas have different types. To store a lambda with a specific signature, use `std::function<>` (with overhead) or a template:

```cpp
auto f = [](int x){ return x * 2; };   // type: some unnamed class

// Store in std::function — type-erased, slight overhead
std::function<int(int)> g = f;

// Store as auto — no overhead
auto h = f;

// In a vector — must use std::function
std::vector<std::function<void()>> callbacks;
callbacks.push_back([]{ std::cout << "hello"; });
```

### Immediately Invoked Lambda Expression (IILE)

A lambda called immediately at its definition point — useful for complex initialization of `const` variables:

```cpp
const int value = [&]() {
    if (condition_a) return compute_a();
    if (condition_b) return compute_b();
    return default_value();
}();   // <-- called immediately

// Without IILE, value can't be const because it requires conditional initialization
```

### Generic Lambdas (C++14)

`auto` parameters make lambdas templates:

```cpp
auto identity = [](auto x) { return x; };
identity(42);       // int
identity(3.14);     // double
identity("hello");  // const char*

auto add = [](auto a, auto b) { return a + b; };
add(1, 2);          // 3 (int)
add(1.0, 2.0);      // 3.0 (double)
```

### Recursive Lambdas

Lambdas can't refer to themselves by name directly. The cleanest C++23 approach uses `this` (deducing `this`):

```cpp
// C++23 — deducing this
auto fib = [](this auto self, int n) -> int {
    return n <= 1 ? n : self(n-1) + self(n-2);
};

// C++14/17 — std::function approach (overhead)
std::function<int(int)> fib = [&fib](int n) -> int {
    return n <= 1 ? n : fib(n-1) + fib(n-2);
};
```

---

## 5.4 — C++11: `constexpr`, `static_assert`, `noexcept`

### `constexpr` — Compile-Time Evaluation

`constexpr` marks a variable or function as potentially evaluable at compile time. If all inputs are compile-time constants, the evaluation happens during compilation — producing a constant with zero runtime cost.

**`constexpr` variables:**

```cpp
constexpr int size = 1024;               // must be a constant expression
constexpr double pi = 3.14159265358979;
constexpr int arr_size = size * 2;       // can use other constexpr values

int runtime_val = 5;
constexpr int bad = runtime_val;         // ERROR — runtime_val is not constexpr
```

**`constexpr` functions:**

```cpp
constexpr int square(int x) { return x * x; }

constexpr int s = square(5);   // evaluated at compile time — s is a compile-time constant
int n = square(runtime_val);   // called at runtime — works as a normal function too
```

A `constexpr` function can be called at runtime if any argument is a runtime value. The `constexpr` only guarantees it CAN be evaluated at compile time.

**Evolution of `constexpr` across standards:**

| Standard | What's allowed in `constexpr` functions |
|----------|----------------------------------------|
| C++11 | Single return statement only |
| C++14 | Loops, multiple statements, local variables |
| C++17 | `if constexpr`, lambdas |
| C++20 | `try`/`catch`, `new`/`delete` (in constexpr context), virtual calls, `std::vector`, `std::string` in constexpr |

```cpp
// C++14+ — constexpr with loops
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

constexpr int f10 = factorial(10);   // 3628800 — computed at compile time
```

**`consteval` (C++20)** — must be evaluated at compile time. Fails if called with a runtime value:

```cpp
consteval int must_be_compiletime(int x) { return x * 2; }
must_be_compiletime(5);             // OK — compile-time
int n = 5;
must_be_compiletime(n);            // ERROR — n is not a constant expression
```

**`constinit` (C++20)** — ensures a variable is initialized at compile time (static initialization), but allows runtime modification afterward:

```cpp
constinit int global = factorial(10);  // must be constant-initialized
global = 99;                           // OK — constinit doesn't mean const
```

### `static_assert` — Compile-Time Assertions

`static_assert` checks a condition at compile time. If false, it's a compile error:

```cpp
static_assert(sizeof(int) == 4, "This code assumes 32-bit int");
static_assert(sizeof(void*) == 8, "This code requires 64-bit pointers");

template<typename T>
void process(T value) {
    static_assert(std::is_integral_v<T>, "process() requires an integer type");
    // ...
}

process(42);      // OK
process(3.14);    // compile error: "process() requires an integer type"
```

The message (second argument) is optional in C++17. Static assertions are invaluable for:
- Documenting assumptions about platform sizes
- Enforcing constraints in template code (before concepts)
- Checking struct layout for serialization/ABI compatibility

```cpp
struct NetworkPacket {
    uint8_t  type;
    uint8_t  flags;
    uint16_t length;
    uint32_t checksum;
};
static_assert(sizeof(NetworkPacket) == 8, "NetworkPacket must be exactly 8 bytes (check padding)");
```

### `noexcept` — Exception Specification

`noexcept` declares that a function will not throw exceptions. The compiler can use this to generate better code — callers don't need to install exception handlers.

```cpp
void safe_op() noexcept { ... }          // will not throw
void risky_op() { ... }                   // may throw (default)
void risky_op() noexcept(false) { ... }  // explicitly may throw
```

**Conditional `noexcept`:**

```cpp
template<typename T>
void swap(T& a, T& b) noexcept(noexcept(T(std::move(a)))) {
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}
// noexcept if and only if T's move constructor is noexcept
```

**`noexcept` as an operator** — returns `true` at compile time if an expression is non-throwing:

```cpp
bool will_throw = noexcept(some_function());   // true if noexcept, false otherwise
```

**Why `noexcept` matters on move operations:**

`std::vector` and other containers may reallocate when they grow. During reallocation, they must move (or copy) their elements to the new buffer. If the move constructor is `noexcept`, the vector uses the fast move. If not, it falls back to copying (for strong exception guarantee):

```cpp
class MyType {
public:
    MyType(MyType&&) noexcept { ... }   // vector will move — fast
};

class SlowType {
public:
    SlowType(SlowType&&) { ... }        // might throw — vector will copy instead
};
```

**Always mark move constructors and move assignment `noexcept`** unless they genuinely might throw (extremely rare).

If a `noexcept` function does throw, `std::terminate()` is called — the program aborts. The promise is enforced at runtime.

---

## 5.5 — C++11: Threading primitives (`std::thread`, `std::mutex`)

C++11 introduced the first standard threading library, ending the era of platform-specific threading APIs. Full coverage is in [[Part 7 — Concurrency & Multithreading]]. This section introduces the primitives so you recognize them.

### `std::thread`

```cpp
#include <thread>

void worker(int id) {
    std::cout << "Thread " << id << " running\n";
}

std::thread t(worker, 42);   // create and start a thread
t.join();                     // wait for it to finish
// t.detach();               // or detach — let it run independently
```

A `std::thread` object that is joinable (started and not yet joined/detached) **must** be joined or detached before its destructor runs — otherwise `std::terminate()` is called.

### `std::mutex` and `std::lock_guard`

```cpp
#include <mutex>

std::mutex mtx;
int shared_counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII — locks on construction
    ++shared_counter;                        // safe — only one thread at a time
}                                            // lock_guard destructs — mutex unlocked
```

`std::lock_guard` is the simplest lock — it acquires the mutex on construction and releases it on destruction. There's no way to unlock early.

`std::unique_lock` is more flexible — supports deferred locking, try-locking, and manual unlock:

```cpp
std::unique_lock<std::mutex> lock(mtx);   // acquired immediately by default
lock.unlock();                             // can release early
lock.lock();                               // and re-acquire
```

### `std::atomic<T>`

For simple shared variables (counters, flags), `std::atomic` provides lock-free thread safety without a mutex:

```cpp
#include <atomic>

std::atomic<int> counter{0};
std::atomic<bool> done{false};

void worker() {
    ++counter;      // atomic increment — thread-safe, no mutex needed
    done = true;    // atomic store
}

while (!done) { ... }   // atomic load — thread-safe
```

See [[Part 7 — Concurrency & Multithreading]] for the full threading model including `std::condition_variable`, `std::future`, `std::promise`, memory ordering, and common pitfalls.

---

## 5.6 — C++14: Generic Lambdas, Variable Templates, `std::make_unique`

C++14 was a focused polish release — it fixed gaps and inconsistencies in C++11 rather than introducing large new features.

### Generic Lambdas

`auto` parameters in lambdas create templated `operator()`:

```cpp
auto add = [](auto a, auto b) { return a + b; };
add(1, 2);       // int + int = 3
add(1.5, 2.5);   // double + double = 4.0
add(std::string("hi"), std::string(" there"));  // string concatenation
```

This was not possible in C++11 where lambda parameters required explicit types.

### Variable Templates

Templates for variables, not just functions and classes:

```cpp
template<typename T>
constexpr T pi = T(3.14159265358979323846);

pi<float>;    // 3.14159f
pi<double>;   // 3.141592653589793
pi<long double>; // higher precision

template<typename T>
constexpr bool is_pointer_v = std::is_pointer<T>::value;
// std::is_pointer_v<int*> — true (this pattern is how _v helpers are implemented)
```

The `_v` suffix type trait helpers (`std::is_integral_v<T>`, `std::is_same_v<T, U>`, etc.) are all variable templates defined this way in the standard library.

### `std::make_unique`

Mysteriously absent from C++11 (an oversight), `std::make_unique` was added in C++14:

```cpp
// C++11 — only make_shared existed
std::unique_ptr<Foo> p(new Foo(arg1, arg2));

// C++14 — make_unique
auto p = std::make_unique<Foo>(arg1, arg2);
```

Always prefer `make_unique` and `make_shared` over `new` — see [[Part 3 — Memory Management]] section 3.4.

### Relaxed `constexpr`

C++14 lifted C++11's restriction that `constexpr` functions could only have a single return statement. Full loops, conditionals, and local variables are now allowed:

```cpp
// C++11 — only single expression
constexpr int factorial(int n) { return n <= 1 ? 1 : n * factorial(n-1); }

// C++14 — loops and variables allowed
constexpr int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}
```

### `[[deprecated]]` Attribute

Mark functions or types as deprecated — the compiler emits a warning when they're used:

```cpp
[[deprecated("Use new_function() instead")]]
void old_function() { ... }

[[deprecated]]
struct OldStruct { ... };

old_function();   // warning: 'old_function' is deprecated: Use new_function() instead
```

### `std::integer_sequence` and `std::index_sequence`

Utilities for working with packs of integers at compile time — primarily used to implement `std::apply`, `std::make_from_tuple`, and similar:

```cpp
template<std::size_t... Is>
void print_tuple_impl(const auto& t, std::index_sequence<Is...>) {
    ((std::cout << std::get<Is>(t) << " "), ...);
}

template<typename Tuple>
void print_tuple(const Tuple& t) {
    print_tuple_impl(t, std::make_index_sequence<std::tuple_size_v<Tuple>>{});
}
```

---

## 5.7 — C++17: Structured Bindings, `if constexpr`, fold expressions

### Structured Bindings

Decompose a tuple, pair, array, or aggregate struct into named variables:

```cpp
// std::pair
auto [key, value] = std::make_pair("hello", 42);
// key = "hello", value = 42

// std::tuple
auto [a, b, c] = std::make_tuple(1, 2.5, "three");

// std::map iteration — before C++17 this was verbose
std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
for (const auto& [name, score] : scores) {
    std::cout << name << ": " << score << "\n";
}

// Struct (aggregate)
struct Point { int x, y, z; };
Point p{1, 2, 3};
auto [x, y, z] = p;

// Array
int arr[3] = {10, 20, 30};
auto [first, second, third] = arr;

// Return multiple values from a function
std::pair<bool, int> find_value(const std::vector<int>& v, int target) {
    for (int i = 0; i < v.size(); ++i)
        if (v[i] == target) return {true, i};
    return {false, -1};
}

auto [found, index] = find_value(v, 42);
```

Structured bindings bind to the elements by the same rules as `auto` — use `const auto&` or `auto&` for references:

```cpp
for (auto& [key, val] : map) val *= 2;   // modify values in-place
```

### `if constexpr` — Compile-Time Branching

`if constexpr` evaluates the condition at compile time and discards the untaken branch entirely — including its syntax check. This makes template specialization dramatically simpler:

```cpp
template<typename T>
std::string stringify(T val) {
    if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (std::is_integral_v<T>) {
        return std::to_string(val);
    } else if constexpr (std::is_floating_point_v<T>) {
        return std::to_string(val);
    } else {
        return std::string(val);   // assumes string-constructible
    }
}

stringify(true);     // "true"
stringify(42);       // "42"
stringify("hello");  // "hello"
```

The discarded branch is not instantiated — so `std::to_string(val)` for the `bool` branch doesn't need to compile when `T = std::string`. This replaces most of the pre-C++17 template specialization boilerplate.

**`if constexpr` vs `if`:** Regular `if` evaluates at runtime — all branches must compile. `if constexpr` evaluates at compile time — only the taken branch needs to compile.

### Fold Expressions

Fold expressions apply a binary operator across all elements of a parameter pack:

```cpp
// Sum of all arguments
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);   // unary right fold: a + (b + (c + d))
}
sum(1, 2, 3, 4);   // 10

// With initial value
template<typename... Args>
auto sum_with_init(Args... args) {
    return (0 + ... + args);   // binary left fold with 0 as init
}
sum_with_init();   // 0 — works with empty pack

// Print all arguments
template<typename... Args>
void print_all(Args&&... args) {
    ((std::cout << args << " "), ...);   // comma fold
}
print_all(1, "hello", 3.14);   // "1 hello 3.14 "
```

**Fold syntax:**

| Form | Expansion |
|------|-----------|
| `(pack op ...)` | Unary right fold: `a op (b op c)` |
| `(... op pack)` | Unary left fold: `(a op b) op c` |
| `(pack op ... op init)` | Binary right fold with init |
| `(init op ... op pack)` | Binary left fold with init |

Binary folds with init handle empty packs correctly — the init value is returned when the pack is empty.

### Class Template Argument Deduction (CTAD)

You no longer need to specify template arguments when they can be deduced from the constructor:

```cpp
// Before C++17
std::pair<int, double> p(1, 2.5);
std::vector<int> v = {1, 2, 3};   // this already worked via initializer list

// C++17 CTAD
std::pair p(1, 2.5);           // deduced: pair<int, double>
std::tuple t(1, 2.5, "hello"); // deduced: tuple<int, double, const char*>
std::optional opt(42);         // deduced: optional<int>
std::lock_guard lock(mtx);     // deduced: lock_guard<std::mutex>
```

### `if`/`switch` with Initializer

An initializer statement can appear inside `if` and `switch`, scoping a variable to the conditional:

```cpp
if (auto it = map.find(key); it != map.end()) {
    use(it->second);   // it is in scope here
} else {
    handle_missing();  // and here
}
// it is NOT in scope here — doesn't pollute the enclosing scope

switch (auto result = try_operation(); result.error_code()) {
    case 0: handle_success(); break;
    case 1: handle_error(); break;
}
```

### Inline Variables

`inline` can now be applied to variables, allowing a variable to be defined in a header without violating the ODR:

```cpp
// header.h
inline int global_count = 0;      // can be included in multiple .cpp files
inline constexpr double pi = 3.14159265358979;
```

Before C++17, defining a variable in a header required `extern` declarations and a single definition in one `.cpp` file.

---

## 5.8 — C++17: `std::optional`, `std::variant`, `std::any`

These three types solve the "what do I return when there might be nothing / one of several things / anything?" problems.

### `std::optional<T>` — A Value That May Not Exist

Represents either a value of type `T` or nothing (`std::nullopt`). A type-safe alternative to returning sentinel values (`-1`, `nullptr`, `""`) or out-parameters.

```cpp
#include <optional>

std::optional<int> find_index(const std::vector<int>& v, int target) {
    for (int i = 0; i < (int)v.size(); ++i)
        if (v[i] == target) return i;   // return the value
    return std::nullopt;                 // return nothing
}

auto idx = find_index(v, 42);

if (idx)              { use(*idx); }   // dereference like a pointer
if (idx.has_value())  { use(*idx); }   // explicit check
idx.value();          // *idx, but throws std::bad_optional_access if empty
idx.value_or(-1);     // return the value or -1 if empty — no exception

// Reset
idx.reset();          // make it empty
idx = std::nullopt;   // same
idx = 42;             // assign a value
```

**Constructing optional:**

```cpp
std::optional<std::string> a;           // empty
std::optional<std::string> b = "hello"; // contains "hello"
std::optional<std::string> c = std::nullopt; // explicitly empty
auto d = std::make_optional<std::string>(5, 'x'); // construct in-place: "xxxxx"
```

**When to use:** Return type for operations that might fail or produce no result. Function parameters that are genuinely optional. Member variables that might not be set. Prefer over: `bool + out_param`, sentinel values, `nullptr` for non-pointers.

### `std::variant<T1, T2, ...>` — A Type-Safe Union

Holds exactly one value, which can be any of the listed types. The compiler knows which type is currently active.

```cpp
#include <variant>

std::variant<int, double, std::string> v;

v = 42;          // holds int
v = 3.14;        // holds double
v = "hello";     // holds std::string

// Access
std::get<double>(v);           // 3.14 — throws std::bad_variant_access if wrong type
std::get<1>(v);                // index-based access
std::holds_alternative<double>(v);  // true — check type
v.index();                     // 1 — index of currently held type
```

**`std::visit` — the idiomatic way to handle all cases:**

```cpp
std::visit([](auto&& val) {
    std::cout << val << "\n";
}, v);  // prints whatever type is currently held

// With overload set — handle each type differently
std::visit(overloaded{
    [](int i)               { std::cout << "int: " << i; },
    [](double d)            { std::cout << "double: " << d; },
    [](const std::string& s){ std::cout << "string: " << s; },
}, v);

// Helper: overloaded (combine lambdas into a single callable)
template<typename... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// C++17 deduction guide:
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;
```

**`std::monostate`** — for variants that need a default-constructible empty state:

```cpp
std::variant<std::monostate, NotDefaultConstructible, int> v;
// v starts holding monostate — the "empty" state
std::holds_alternative<std::monostate>(v);   // true
```

**When to use:** When a value can be one of several unrelated types and you need type-safe dispatch. Replaces C-style `union` with type tag. Classic use case: AST nodes, parser results, message types, error-or-value patterns (though `std::expected` is better for error handling in C++23).

### `std::any` — A Container for Any Copyable Type

Holds a single value of any type that is copyable, without compile-time knowledge of the type:

```cpp
#include <any>

std::any a;
a = 42;          // now holds int
a = 3.14;        // now holds double — previous int is destroyed
a = std::string("hello");  // now holds string

std::any_cast<std::string>(a);    // "hello" — copy
std::any_cast<std::string&>(a);   // reference — no copy
std::any_cast<int>(a);            // throws std::bad_any_cast — wrong type

a.has_value();    // true
a.type();         // std::type_info for the currently held type
a.reset();        // empty it
```

**When to use:** Heterogeneous containers where types are not known at compile time, plugin systems, configuration/property maps. The escape hatch — use `variant` when you know the set of possible types; use `any` only when you truly don't.

**The cost:** `any` uses type erasure with heap allocation for types that don't fit in a small buffer (typically 1-3 pointers in size). It has the overhead of `std::function` — unavoidable for a fully generic container.

---

## 5.9 — C++17: `std::filesystem`, `std::string_view`

`std::string_view` is covered in depth in [[Part 4 — Standard Template Library (STL)]] section 4.9. This section focuses on `std::filesystem`.

### `std::filesystem` — Portable File System Operations

`std::filesystem` (from `<filesystem>`) provides a portable, C++ native interface to the file system. Before C++17, this required platform-specific APIs (`POSIX`, `Win32`) or third-party libraries (Boost.Filesystem, which the standard library was based on).

```cpp
#include <filesystem>
namespace fs = std::filesystem;
```

### `fs::path` — Representing Paths

`path` is the central type. It handles platform-specific separators automatically:

```cpp
fs::path p = "/home/user/docs/file.txt";
fs::path p2 = "relative/path/file.cpp";

p.filename();       // "file.txt"
p.stem();           // "file" (filename without extension)
p.extension();      // ".txt"
p.parent_path();    // "/home/user/docs"
p.root_path();      // "/"
p.root_name();      // "" (empty on POSIX, drive letter on Windows: "C:")

p / "subdir";       // append with /: "/home/user/docs/file.txt/subdir" (use parent_path first)
p.parent_path() / "other.txt";  // "/home/user/docs/other.txt"

p.string();         // convert to std::string
p.generic_string(); // forward-slash separators on all platforms
```

### Querying the File System

```cpp
fs::path p = "some/file.txt";

fs::exists(p);              // does this path exist?
fs::is_regular_file(p);     // is it a regular file?
fs::is_directory(p);        // is it a directory?
fs::is_symlink(p);          // is it a symlink?
fs::file_size(p);           // size in bytes (file must exist and be a regular file)
fs::last_write_time(p);     // last modification time — returns file_time_type

fs::status(p).permissions(); // file permissions (bitmask)
```

### File and Directory Operations

```cpp
fs::create_directory("new_dir");           // create one directory
fs::create_directories("a/b/c/d");        // create all missing intermediate dirs

fs::copy("src.txt", "dst.txt");           // copy file
fs::copy("src_dir", "dst_dir", fs::copy_options::recursive);  // copy directory tree

fs::rename("old.txt", "new.txt");         // rename or move
fs::remove("file.txt");                   // delete file
fs::remove_all("directory");              // delete directory and all contents

fs::current_path();                        // current working directory
fs::current_path("new/cwd");              // change working directory
fs::absolute("relative/path");            // absolute path
fs::canonical("path/with/../dots");       // resolve all . and .. (path must exist)
fs::temp_directory_path();                // system temp directory
```

### Directory Iteration

```cpp
// Iterate one level
for (const auto& entry : fs::directory_iterator("some/dir")) {
    std::cout << entry.path() << "\n";
    entry.is_regular_file();
    entry.is_directory();
    entry.file_size();
}

// Iterate recursively
for (const auto& entry : fs::recursive_directory_iterator("some/dir")) {
    if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
        std::cout << entry.path() << "\n";
    }
}
```

### Error Handling

All operations have two overloads: one that throws `fs::filesystem_error` on failure, and one that takes an `std::error_code` out-parameter:

```cpp
// Throwing version
try {
    fs::remove("nonexistent.txt");
} catch (const fs::filesystem_error& e) {
    std::cerr << e.what() << "\n";  // includes path and OS error
}

// Non-throwing version
std::error_code ec;
fs::remove("nonexistent.txt", ec);
if (ec) std::cerr << ec.message() << "\n";
```

Use the `error_code` overload in performance-critical code or when failures are expected — exception throwing is expensive.

---

## 5.10 — C++20: Concepts & Constraints

### The Problem With Templates Before C++20

Template error messages were (and still are, without concepts) notoriously unreadable. A simple mistake deep in a template instantiation chain produces pages of internal error output rather than a clear message about the actual constraint that was violated.

Additionally, documentation of what a template requires was purely informal — comments like `// T must be LessThanComparable`. Nothing was enforced by the compiler.

### Concepts — Named Constraints

A **concept** is a named boolean predicate on template parameters, evaluated at compile time:

```cpp
#include <concepts>

template<typename T>
concept Integral = std::is_integral_v<T>;

template<typename T>
concept Addable = requires(T a, T b) { a + b; };  // T must support +

template<typename T>
concept Sortable = requires(T a) {
    { a.begin() } -> std::same_as<typename T::iterator>;
    { a.end() }   -> std::same_as<typename T::iterator>;
    requires std::totally_ordered<typename T::value_type>;
};
```

### Constraining Templates

Four equivalent ways to apply a concept to a template:

```cpp
// 1. requires clause after template parameter list
template<typename T>
    requires Integral<T>
T double_it(T x) { return x * 2; }

// 2. requires clause after function declaration
template<typename T>
T double_it(T x) requires Integral<T> { return x * 2; }

// 3. Concept as type constraint directly
template<Integral T>
T double_it(T x) { return x * 2; }

// 4. Abbreviated function template with auto
Integral auto double_it(Integral auto x) { return x * 2; }
```

All four are equivalent. The abbreviated form (4) is concise for simple cases.

### `requires` Expressions

A `requires` expression tests whether a set of operations is valid for a type:

```cpp
template<typename T>
concept Printable = requires(T x) {
    std::cout << x;   // must be streamable
};

template<typename T>
concept Container = requires(T c) {
    c.begin();            // must have begin()
    c.end();              // must have end()
    c.size();             // must have size()
    typename T::value_type; // must have value_type member
};

template<typename T>
concept Hashable = requires(T x) {
    { std::hash<T>{}(x) } -> std::convertible_to<std::size_t>;
    // the hash expression must return something convertible to size_t
};
```

### Standard Concepts (`<concepts>`)

```cpp
// Type relationships
std::same_as<T, U>           // T and U are the same type
std::convertible_to<T, U>    // T is implicitly convertible to U
std::derived_from<T, Base>   // T derives from Base

// Type categories
std::integral<T>             // T is an integer type
std::floating_point<T>       // T is a floating-point type
std::signed_integral<T>
std::unsigned_integral<T>
std::arithmetic<T>           // integral or floating_point

// Object concepts
std::default_initializable<T>
std::copy_constructible<T>
std::move_constructible<T>
std::copyable<T>
std::movable<T>
std::regular<T>              // default constructible, copyable, equality comparable

// Callable concepts
std::invocable<F, Args...>           // F can be called with Args
std::predicate<F, Args...>           // F returns bool-like with Args
std::relation<R, T, U>               // R is a binary relation on T and U

// Comparison concepts
std::equality_comparable<T>
std::totally_ordered<T>

// Range concepts (in <iterator> and <ranges>)
std::ranges::range<T>
std::ranges::sized_range<T>
std::ranges::forward_range<T>
std::ranges::bidirectional_range<T>
std::ranges::random_access_range<T>
std::ranges::contiguous_range<T>
```

### How Concepts Improve Error Messages

```cpp
template<Integral T>
T double_it(T x) { return x * 2; }

double_it(3.14);
// Error: "constraints not satisfied for call to 'double_it'
//         T = double, which does not satisfy 'Integral'"
// — clear, precise, actionable

// Without concepts:
template<typename T>
T double_it(T x) { return x * 2; }
// No error at the call site — might fail deep inside with a confusing message
```

---

## 5.11 — C++20: Modules

### The Problem With Headers

The `#include` model has fundamental issues:
- **Slow compilation** — each translation unit re-parses every included header. A project with 100 files all including `<vector>` parses `<vector>` 100 times.
- **Macro leakage** — macros defined in one header bleed into everything that includes it, in order.
- **Include-order sensitivity** — some headers must be included before others.
- **No isolation** — internal implementation details of a library become visible to all includers.

### Modules — The Solution

A **module** is a self-contained unit of code that is compiled once and then imported by other modules. Its internal symbols are invisible to importers unless explicitly exported.

```cpp
// math.cppm — module interface unit
export module math;

export int add(int a, int b) { return a + b; }   // exported — visible to importers
int helper(int x) { return x * 2; }               // NOT exported — private to module
```

```cpp
// main.cpp — importing the module
import math;

int main() {
    add(3, 4);     // 7 — works
    helper(5);     // ERROR — not exported
}
```

### Module Structure

**Module interface unit** — defines what the module exports. Has `export module name;` at the top.

**Module implementation unit** — provides implementations without re-exporting. Has `module name;` (no `export`).

**Module partitions** — split a large module across files:

```cpp
// math-add.cppm
export module math:add;
export int add(int a, int b) { return a + b; }

// math-multiply.cppm
export module math:multiply;
export int multiply(int a, int b) { return a * b; }

// math.cppm — primary interface, re-exports partitions
export module math;
export import :add;
export import :multiply;
```

### Exporting

```cpp
export module mylib;

export int global_var = 42;          // export a variable
export void free_function();         // export a function declaration
export class MyClass { ... };        // export a class
export namespace ns { ... }          // export an entire namespace
export { int a; void b(); }         // export a group
```

Private module fragment — declarations after `module :private;` are completely invisible to importers, even declarations (stronger than just non-exporting):

```cpp
export module foo;
export void public_api();

module :private;  // everything below is truly private
void impl_detail() { ... }
```

### Header Units — Migration Path

For existing headers that aren't modules yet, C++20 provides header units — compile the header once and import it:

```cpp
import <vector>;    // import header unit instead of #include <vector>
import "mylib.h";   // import a local header as a header unit
```

Header units are not full modules — they still export macros — but they compile much faster because the header is processed once.

### Build System Support

Modules require build system cooperation (the compiler must tell the build system about dependencies between modules). Support as of 2024–2025:
- **GCC 11+** — partial support
- **Clang 16+** — partial support
- **MSVC** — most complete implementation
- **CMake 3.28+** — experimental module support with `target_sources(... FILE_SET CXX_MODULES ...)`

---

## 5.12 — C++20: Coroutines

### What a Coroutine Is

A coroutine is a function that can **suspend its execution** and later **resume** from where it left off. Unlike a thread, a coroutine suspends cooperatively (it chooses when to suspend) and runs on a single thread.

```
Normal function:  call → runs to completion → returns
Coroutine:        call → runs → suspends → (resumed later) → runs → suspends → ... → completes
```

C++20 adds three new keywords:
- **`co_return`** — return a value and finalize the coroutine
- **`co_yield`** — suspend and produce a value (like Python's `yield`)
- **`co_await`** — suspend until an awaitable completes

Any function containing one of these keywords is a coroutine.

### The Generator Pattern (`co_yield`)

The canonical example — an infinite sequence generator:

```cpp
#include <coroutine>

// Minimal generator type (C++20 requires you to write the machinery;
// std::generator is standard in C++23)
template<typename T>
struct Generator {
    struct promise_type {
        T current_value;
        auto get_return_object() { return Generator{this}; }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        auto yield_value(T val) {
            current_value = val;
            return std::suspend_always{};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    using handle_t = std::coroutine_handle<promise_type>;
    handle_t handle;

    Generator(promise_type* p) : handle(handle_t::from_promise(*p)) {}
    ~Generator() { if (handle) handle.destroy(); }

    bool next() { handle.resume(); return !handle.done(); }
    T value() { return handle.promise().current_value; }
};

// The coroutine — looks like a simple function
Generator<int> iota(int start = 0) {
    while (true) co_yield start++;
}

// Usage
auto gen = iota(1);
for (int i = 0; i < 5; ++i) {
    gen.next();
    std::cout << gen.value() << " ";  // 1 2 3 4 5
}
```

In C++23, `std::generator<T>` is provided by the standard — you don't need to write the boilerplate above.

### `co_await` — Asynchronous Operations

`co_await` suspends the coroutine until the awaited operation completes. This is the foundation for async I/O frameworks:

```cpp
// Pseudo-code — requires an async runtime (like ASIO, cppcoro, etc.)
Task<std::string> fetch_data(std::string url) {
    auto response = co_await http_get(url);   // suspend until HTTP response arrives
    auto body     = co_await response.body(); // suspend until body is read
    co_return body;
}

Task<void> main_task() {
    std::string data = co_await fetch_data("http://example.com");
    std::cout << data;
}
```

The coroutine suspends at each `co_await`, freeing the thread to do other work. When the awaited operation completes (possibly from a network callback), the coroutine is resumed.

### How Coroutines Work Internally

When you call a coroutine:
1. The compiler allocates a **coroutine frame** on the heap (unlike function frames, which are on the stack) — it holds local variables and the current suspension point.
2. A **promise object** (type specified by the user) controls the coroutine's lifecycle.
3. A **coroutine handle** (`std::coroutine_handle<Promise>`) is a lightweight object (just a pointer to the frame) that can `resume()` or `destroy()` the coroutine.

The C++20 coroutine machinery is deliberately low-level — it's a framework for library writers to build higher-level async primitives on top of. You're expected to use a library (ASIO, cppcoro, `std::generator`) rather than writing `promise_type` yourself.

### When to Use Coroutines

- **Generators** — lazy sequences without extra threads (`std::generator` in C++23)
- **Async I/O** — network code, file I/O, without callback hell or threads
- **State machines** — the coroutine's suspension points are the states
- **Cooperative multitasking** — multiple coroutines taking turns on one thread

Coroutines are not threads — they don't run in parallel. They're about structuring code that has natural "wait" points without blocking a thread.

---

## 5.13 — C++20: `std::format`, `std::span`, Ranges

`std::span` is covered in [[Part 4 — Standard Template Library (STL)]] section 4.10. Ranges are covered in section 4.11. This section focuses on `std::format`.

### `std::format` — Type-Safe Python-Style Formatting

C++20 finally gave C++ a modern string formatting facility. Based on Python's `str.format()` / fmtlib. Replaces `printf` (unsafe, no type checking) and `std::ostringstream` (verbose, slow).

```cpp
#include <format>

std::string s = std::format("Hello, {}!", "world");      // "Hello, world!"
std::string s = std::format("{} + {} = {}", 1, 2, 3);   // "1 + 2 = 3"
std::string s = std::format("Pi is {:.4f}", 3.14159);   // "Pi is 3.1416"
```

### Format Specifiers

```cpp
// Width and alignment
std::format("{:10}",  "left");      // "left      " — right-padded to 10
std::format("{:>10}", "right");     // "     right" — right-aligned
std::format("{:^10}", "center");    // "  center  " — centered
std::format("{:*<10}", "fill");     // "fill******" — left-aligned with * fill

// Integer formatting
std::format("{:d}", 42);    // "42"   — decimal (default)
std::format("{:b}", 42);    // "101010" — binary
std::format("{:o}", 42);    // "52"   — octal
std::format("{:x}", 42);    // "2a"   — hex lowercase
std::format("{:X}", 42);    // "2A"   — hex uppercase
std::format("{:#x}", 42);   // "0x2a" — hex with prefix
std::format("{:08d}", 42);  // "00000042" — zero-padded to 8

// Floating point
std::format("{:.2f}", 3.14159);   // "3.14" — fixed, 2 decimal places
std::format("{:.4e}", 12345.0);   // "1.2345e+04" — scientific
std::format("{:.3g}", 0.001234);  // "0.00123" — general (shorter of f or e)

// Positional arguments
std::format("{0} {1} {0}", "hello", "world");  // "hello world hello"
```

### Output Without Creating a String

```cpp
// Write directly to an output iterator
std::format_to(std::back_inserter(vec), "value: {}", x);
std::format_to(std::ostreambuf_iterator<char>(std::cout), "{}\n", x);

// With size limit
char buf[64];
auto result = std::format_to_n(buf, sizeof(buf) - 1, "value: {}", x);
// result.out = one past last written; result.size = total chars (may exceed limit)

// C++23: std::print — format and print in one step
std::print("Hello, {}!\n", "world");   // to stdout
std::println("Hello, {}!", "world");   // with automatic newline
std::print(stderr, "Error: {}\n", msg);
```

### Custom Formatters

Specialize `std::formatter<T>` for your own types:

```cpp
struct Point { int x, y; };

template<>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

Point p{3, 4};
std::format("{}", p);         // "(3, 4)"
std::format("Point: {}", p);  // "Point: (3, 4)"
```

---

## 5.14 — C++23: `std::expected`, `std::print`, `flat_map`, and more

### `std::expected<T, E>` — Typed Error Handling

`std::expected<T, E>` holds either a success value of type `T` or an error value of type `E`. It's Rust's `Result<T, E>` — a type-safe alternative to throwing exceptions or returning error codes.

```cpp
#include <expected>

std::expected<int, std::string> parse_int(std::string_view s) {
    try {
        return std::stoi(std::string(s));   // success: return the int
    } catch (...) {
        return std::unexpected("not a valid integer: " + std::string(s));
    }
}

auto result = parse_int("42");

if (result)                 { use(*result); }       // has_value() — bool conversion
if (result.has_value())     { use(result.value()); }
result.value_or(0);         // returns value or 0 if error
result.error();             // the error value (only if !has_value())

// Propagating errors — monadic operations
auto doubled = parse_int("21")
    .and_then([](int x) -> std::expected<int, std::string> {
        if (x > 100) return std::unexpected("too large");
        return x * 2;
    })
    .transform([](int x) { return x + 1; })   // map over success value
    .or_else([](const std::string& e) -> std::expected<int, std::string> {
        std::cerr << "Error: " << e << "\n";
        return 0;   // recover with default
    });
```

**`std::expected` vs exceptions:**
- Exceptions: zero-cost when not thrown, but force callers to handle all failure paths explicitly (or implicitly propagate). Good for truly exceptional conditions.
- `expected`: the error path is part of the function's type. Callers see that failure is possible. Good for expected failures (file not found, parse error, validation failure).

**`std::expected` vs `std::optional`:** `optional` just says "might have nothing". `expected` says "either a value OR this specific error type" — the error carries information.

### `std::print` and `std::println`

Built on `std::format` — formats and prints to stdout (or any FILE*) in one call:

```cpp
#include <print>

std::print("Hello, {}!\n", "world");       // to stdout
std::println("Hello, {}!", "world");       // same + automatic newline
std::println(stderr, "Error: {}", msg);   // to stderr
std::print(file_ptr, "Writing: {}", x);   // to FILE*
```

Faster than `std::cout <<` because it writes directly without going through stream buffers. Also avoids the `"\n" vs std::endl` confusion — `std::print` just uses `"\n"`.

### `std::flat_map` and `std::flat_set`

Vector-backed sorted associative containers. The same interface as `std::map`/`std::set` but with dramatically better cache performance for lookup-heavy workloads:

```cpp
#include <flat_map>

std::flat_map<std::string, int> scores;
scores["Alice"] = 95;
scores["Bob"]   = 87;

scores["Alice"];        // same interface as map
scores.find("Bob");
scores.lower_bound("B");
```

**How they differ from `map`/`set`:**
- Elements stored in a sorted `std::vector` — contiguous, cache-friendly
- Lookup: O(log n) binary search, but much better cache behavior than tree traversal
- Insert/erase: O(n) — elements must be shifted
- **Better than `map` for:** Read-heavy workloads, small to medium sizes, cache-sensitive code
- **Worse than `map` for:** Frequent insert/erase in the middle, very large datasets

Also added: `std::flat_multimap`, `std::flat_set`, `std::flat_multiset`.

### `std::mdspan` — Multi-Dimensional Span

A non-owning view of a multi-dimensional array, with customizable layout:

```cpp
#include <mdspan>

double data[6] = {1, 2, 3, 4, 5, 6};

// View as a 2×3 matrix
std::mdspan<double, std::extents<size_t, 2, 3>> m(data);
m[0, 0];  // 1
m[0, 1];  // 2
m[1, 2];  // 6

// Dynamic extents
std::mdspan<double, std::dextents<size_t, 2>> dm(data, 2, 3);
dm[1, 0];  // 4

// Custom layout (row-major, column-major, strided)
using layout = std::layout_left;   // column-major (Fortran-style)
std::mdspan<double, std::extents<size_t, 2, 3>, layout> col_major(data);
```

Avoids the old pattern of `data[i * cols + j]` — index calculation is encapsulated and can be optimized by the library.

### `std::generator<T>` — Standard Coroutine Generator

The boilerplate from [[#5.12 — C++20: Coroutines]] is now in the standard:

```cpp
#include <generator>

std::generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        std::tie(a, b) = std::pair{b, a + b};
    }
}

for (int x : fibonacci() | std::views::take(10)) {
    std::cout << x << " ";  // 0 1 1 2 3 5 8 13 21 34
}
```

Works directly with ranges and views — a `std::generator` is a range.

### `std::ranges::to<Container>()` — Collect Range into Container

Covered in [[Part 4 — Standard Template Library (STL)]] section 4.11, but added in C++23:

```cpp
auto v = std::views::iota(1, 11)
       | std::views::filter([](int x){ return x % 2 == 0; })
       | std::ranges::to<std::vector>();
// v = {2, 4, 6, 8, 10}
```

### `std::unreachable()` — Optimizer Hint

Marks a code point as unreachable — tells the optimizer it can assume this path never executes:

```cpp
#include <utility>

int classify(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    if (x == 0) return 0;
    std::unreachable();  // compiler: the above cases are exhaustive
}

// In a switch with all cases covered:
switch (val) {
    case 0: return "zero";
    case 1: return "one";
    case 2: return "two";
    default: std::unreachable();
}
```

`std::unreachable()` in debug builds causes undefined behavior if reached — effectively a crash. Use it only when you can truly guarantee the path is unreachable.

### `if consteval` — Branching on Compile-Time Context

Allows a function to behave differently when called in a constant-evaluated context vs runtime:

```cpp
constexpr double compute(double x) {
    if consteval {
        // This branch runs at compile time
        return compiletime_algorithm(x);
    } else {
        // This branch runs at runtime — can use non-constexpr operations
        return runtime_optimized_algorithm(x);
    }
}
```

Useful when the best compile-time algorithm differs from the best runtime algorithm (e.g., using SIMD intrinsics at runtime, pure C++ at compile time).

### `std::stacktrace` — Stack Trace Access

Capture and print the current call stack:

```cpp
#include <stacktrace>

void inner() {
    auto trace = std::stacktrace::current();
    std::cout << trace;   // prints full stack trace
}

void middle() { inner(); }
void outer() { middle(); }
// Output: inner -> middle -> outer -> main -> ...
```

Useful for logging, crash reporting, and diagnostics.
