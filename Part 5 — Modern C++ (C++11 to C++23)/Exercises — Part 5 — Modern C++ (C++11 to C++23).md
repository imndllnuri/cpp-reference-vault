# Exercises — Part 5 — Modern C++ (C++11 to C++23)

> Practice problems for [[Part 5 — Modern C++ (C++11 to C++23)]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 5.1 — C++11: `auto`, range-for, `nullptr`, uniform initialization

**Ex 5.1.1** `[Easy]`

What type does `auto` deduce for each variable? Does any of them produce a copy when a reference was intended?

```cpp
std::vector<int> v = {1, 2, 3};

auto a = v;              // type?
auto b = v[0];           // type?
auto c = v.begin();      // type?

const std::vector<int>& get_v() { return v; }
auto d = get_v();        // type? copy or reference?
auto& e = get_v();       // type?
```

```
// your answer
```

---

**Ex 5.1.2** `[Medium]`

Explain the "Most Vexing Parse". Why does `Foo f()` not declare a variable? Show how uniform initialization fixes it. Then explain the `std::initializer_list` trap with `std::vector`.

```cpp
// Demo the most vexing parse:
struct Foo { Foo() {} };

Foo f();        // what is this?
Foo g{};        // what is this?

std::vector<int> v1(10, 0);   // vs
std::vector<int> v2{10, 0};   // what's the difference?
```

```
// your answer
```

---

**Ex 5.1.3** `[Medium]`

This code has a narrowing conversion bug that `{}` would catch but `()` does not. Demonstrate it and explain the rule.

```cpp
double pi = 3.14159;
int a = pi;           // silent truncation — show the value
int b{pi};            // what happens?
int c(pi);            // what happens?

long big = 1'000'000'000'000LL;
short s1 = big;       // silent truncation
short s2{big};        // what happens?
```

```cpp
// your answer with demonstration
```

---

## Section 5.2 — C++11: Rvalue References & Move Semantics

**Ex 5.2.1** `[Easy]`

Classify each expression as lvalue or rvalue:

```cpp
int x = 5;
int* p = &x;

x           // ?
5           // ?
x + 1       // ?
*p          // ?
std::move(x) // ?
std::string("hello") // ?
```

```
// your answer
```

---

**Ex 5.2.2** `[Medium]`

Explain why returning `std::move(v)` is wrong in this function and what the compiler does instead:

```cpp
std::vector<int> make_vector() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    return std::move(v);  // why is this wrong?
}
```

```
// your answer
```

---

**Ex 5.2.3** `[Hard]`

Implement a `forward` function yourself that preserves the value category of its argument (perfect forwarding). Explain the reference collapsing rules used.

```cpp
template<typename T>
T&& my_forward(std::remove_reference_t<T>& t) noexcept {
    // your solution
}

template<typename T>
T&& my_forward(std::remove_reference_t<T>&& t) noexcept {
    // your solution
}
```

---

## Section 5.3 — C++11: Lambdas

**Ex 5.3.1** `[Easy]`

Write a lambda that:
1. Takes two `int`s and returns their sum
2. Captures `int multiplier` by value and returns `x * multiplier`
3. Captures `std::vector<int>& v` by reference and pushes `x` onto it
4. Has no captures and no parameters but prints "hello"

```cpp
// your solution
```

---

**Ex 5.3.2** `[Medium]`

This lambda has a dangling reference bug. Explain when and why it crashes.

```cpp
std::function<int()> make_adder() {
    int base = 100;
    return [&base]() { return base + 1; };  // bug?
}

int main() {
    auto adder = make_adder();
    std::cout << adder();   // what happens?
}
```

```
// your answer and fix
```

---

**Ex 5.3.3** `[Medium]`

Use a lambda as a comparator to sort a `vector<pair<string,int>>` by the integer value descending. Then sort it by string length ascending as a secondary key.

```cpp
// your solution
```

---

**Ex 5.3.4** `[Hard]`

Write a recursive lambda to compute Fibonacci numbers. Show two approaches: one using `std::function` (with the overhead that implies) and one using the C++23 `this auto` deducing-this approach.

```cpp
// your solution — both approaches
```

---

## Section 5.4 — C++11: `constexpr`, `static_assert`, `noexcept`

**Ex 5.4.1** `[Easy]`

Write a `constexpr` function `is_prime(int n)` that returns `true` if `n` is prime. Use it in a `static_assert` to verify that 17 is prime and 15 is not at compile time.

```cpp
constexpr bool is_prime(int n) {
    // your solution
}

static_assert(/* your assertions */);
```

---

**Ex 5.4.2** `[Medium]`

Explain: why does `noexcept` on a move constructor matter for `std::vector` performance? Write a class with and without `noexcept` on its move constructor, and explain which one vector will copy vs move during reallocation.

```
// your answer with code example
```

---

**Ex 5.4.3** `[Medium]`

Use `static_assert` to enforce that a `NetworkPacket` struct is exactly 8 bytes and has standard layout (for binary serialization over a socket).

```cpp
struct NetworkPacket {
    uint8_t  type;
    uint8_t  flags;
    uint16_t length;
    uint32_t checksum;
};

// your static_assert(s)
```

---

**Ex 5.4.4** `[Hard]`

Implement a compile-time lookup table (array) mapping digit characters `'0'`–`'9'` to their integer values using `constexpr`. Then write a `constexpr` function `parse_int(std::string_view)` that parses a decimal integer at compile time.

```cpp
constexpr int parse_int(std::string_view s) {
    // your solution
}

static_assert(parse_int("12345") == 12345);
```

---

## Section 5.5 — C++11: Threading Primitives

**Ex 5.5.1** `[Medium]`

Write a program that launches 10 threads, each incrementing a shared counter 100,000 times. First show the buggy version (no synchronization), then fix it with `std::mutex`.

```cpp
// your solution — both versions
```

---

**Ex 5.5.2** `[Medium]`

What is a data race? This code has one. Identify it and fix it using `std::atomic<int>` instead of a mutex.

```cpp
int total = 0;
void worker() {
    for (int i = 0; i < 1000; ++i) ++total;
}
```

```cpp
// your solution
```

---

## Section 5.6 — C++14: Generic Lambdas, Variable Templates, `std::make_unique`

**Ex 5.6.1** `[Easy]`

Write a generic lambda `transform_pair` that takes a `std::pair<A, B>` and a function `f`, and returns a new pair with `f` applied to both elements.

```cpp
auto transform_pair = [](auto pair, auto f) {
    // your solution
};
```

---

**Ex 5.6.2** `[Medium]`

Define variable templates for `pi<T>`, `e<T>`, and `sqrt2<T>` that give the mathematical constants to the precision of type `T`.

```cpp
template<typename T>
constexpr T pi = /* your solution */;

// Verify:
static_assert(pi<float>  != pi<double>);  // different precision
```

---

## Section 5.7 — C++17: Structured Bindings, `if constexpr`, fold expressions

**Ex 5.7.1** `[Easy]`

Rewrite this code using structured bindings:

```cpp
std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
for (auto it = scores.begin(); it != scores.end(); ++it) {
    std::cout << it->first << ": " << it->second << "\n";
}
```

```cpp
// your solution
```

---

**Ex 5.7.2** `[Medium]`

Write a variadic function `print_all` using a fold expression that prints all arguments separated by spaces, followed by a newline.

```cpp
template<typename... Args>
void print_all(Args&&... args) {
    // your solution — fold expression
}
// print_all(1, "hello", 3.14, true);  →  1 hello 3.14 1
```

---

**Ex 5.7.3** `[Medium]`

Use `if constexpr` to write a single `stringify` function that handles `bool` (→ "true"/"false"), integers (→ decimal string), floating point (→ fixed 2dp), and anything else (→ uses `operator<<`).

```cpp
template<typename T>
std::string stringify(T val) {
    // your solution
}
```

---

## Section 5.8 — C++17: `std::optional`, `std::variant`, `std::any`

**Ex 5.8.1** `[Medium]`

Rewrite this function to return `std::optional<double>` instead of using a sentinel value of -1.0:

```cpp
double safe_sqrt(double x) {
    if (x < 0) return -1.0;
    return std::sqrt(x);
}
```

```cpp
// your solution — and show how the caller handles it
```

---

**Ex 5.8.2** `[Medium]`

Implement a simple expression evaluator using `std::variant<int, double, std::string>`. Write a `print_value` visitor that handles each type differently.

```cpp
using Value = std::variant<int, double, std::string>;

void print_value(const Value& v) {
    // your solution — use std::visit
}
```

---

**Ex 5.8.3** `[Hard]`

Implement a `Result<T, E>` type using `std::variant<T, E>` that works like `std::expected`. Support `.value()`, `.error()`, `.has_value()`, and a monadic `.and_then(f)`.

```cpp
template<typename T, typename E>
class Result {
    // your solution
};
```

---

## Section 5.9 — C++17: `std::filesystem`, `std::string_view`

**Ex 5.9.1** `[Medium]`

Write a function that recursively counts all `.cpp` files in a directory tree and returns their total combined file size in bytes.

```cpp
#include <filesystem>

struct CppStats { size_t count; uintmax_t total_bytes; };

CppStats scan_cpp_files(const std::filesystem::path& root) {
    // your solution
}
```

---

**Ex 5.9.2** `[Medium]`

Using `std::filesystem`, implement a function `copy_filtered(src, dst, ext)` that copies all files with the given extension from `src` directory to `dst` directory (flat, no recursion). Create `dst` if it doesn't exist.

```cpp
void copy_filtered(const std::filesystem::path& src,
                   const std::filesystem::path& dst,
                   const std::string& ext) {
    // your solution
}
```

---

## Section 5.10 — C++20: Concepts & Constraints

**Ex 5.10.1** `[Medium]`

Write a concept `Numeric` that is satisfied by any type that is either integral or floating-point. Use it to constrain a `sum` function template.

```cpp
template<typename T>
concept Numeric = /* your solution */;

Numeric auto sum(std::ranges::range auto&& r) {
    // your solution
}
```

---

**Ex 5.10.2** `[Medium]`

Write a concept `Printable` that is satisfied by any type that can be output with `operator<<`. Use it to constrain a `print_vector` function.

```cpp
template<typename T>
concept Printable = /* your solution */;

void print_vector(const std::vector<Printable auto>& v) {
    // your solution
}
```

---

**Ex 5.10.3** `[Hard]`

Write a concept `Container` that requires: `begin()`, `end()`, `size()`, `empty()`, and a nested `value_type`. Then write a function `contains(container, value)` constrained by it.

```cpp
template<typename T>
concept Container = /* your solution */;

bool contains(const Container auto& c, const auto& val) {
    // your solution
}
```

---

## Section 5.11 — C++20: Modules

**Ex 5.11.1** `[Easy]`

What problem do modules solve that `#include` cannot? Name two specific issues with the `#include` model that modules eliminate.

```
// your answer
```

---

**Ex 5.11.2** `[Medium]`

Write a module interface unit `math.cppm` that exports: a constant `pi`, a function `circle_area(double r)`, and a class `Vector2D`. Then write `main.cpp` that imports it.

```cpp
// math.cppm
// your solution

// main.cpp
// your solution
```

---

## Section 5.12 — C++20: Coroutines

**Ex 5.12.1** `[Easy]`

What is the difference between a coroutine and a thread? Fill in the comparison table:

```
              Coroutines    Threads
Scheduling:   ?             ?
Parallelism:  ?             ?
Switch cost:  ?             ?
Stack:        ?             ?
```

---

**Ex 5.12.2** `[Hard]`

Using `std::generator<T>` (C++23) or write a minimal generator type, implement a `range(int start, int stop, int step)` coroutine that yields values like Python's `range`.

```cpp
// your solution
// for (int x : range(0, 10, 2)) std::cout << x; // 0 2 4 6 8
```

---

## Section 5.13 — C++20: `std::format`, `std::span`, Ranges

**Ex 5.13.1** `[Medium]`

Rewrite this `printf`-style function using `std::format`. Then add a custom formatter for `Point` so `std::format("{}", Point{3,4})` produces `"(3, 4)"`.

```cpp
void print_info(const std::string& name, int age, double score) {
    printf("Name: %-20s Age: %3d Score: %.2f%%\n", name.c_str(), age, score);
}

struct Point { int x, y; };
// your formatter
```

```cpp
// your solution
```

---

**Ex 5.13.2** `[Medium]`

Write a function `process(std::span<const int> data)` that returns the sum of squares of all elements. Call it with a `std::vector<int>`, a `std::array<int, 5>`, and a raw `int[]` array — without any changes to `process`.

```cpp
int process(std::span<const int> data) {
    // your solution
}
```

---

## Section 5.14 — C++23: `std::expected`, `std::print`, `flat_map`, and more

**Ex 5.14.1** `[Medium]`

Rewrite this error-handling code using `std::expected<int, std::string>`:

```cpp
// Old style — returns -1 on error, sets a global error string
int parse_port(const std::string& s);
```

```cpp
std::expected<int, std::string> parse_port(std::string_view s) {
    // your solution — return the port or an error message
}
```

---

**Ex 5.14.2** `[Medium]`

Chain three operations using `std::expected` and `.and_then()`: parse a port number from a string, validate it's in range 1-65535, then connect (simulated — just return a success string). If any step fails, propagate the error.

```cpp
// your solution — show the monadic chain
```

---

**Ex 5.14.3** `[Hard]`

Implement a simplified `expected<T, E>` from scratch (without using the standard one) that supports: construction from value or error, `has_value()`, `value()`, `error()`, and `and_then(f)`.

```cpp
template<typename T, typename E>
class expected {
    // your solution
};
```
