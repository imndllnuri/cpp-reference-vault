# Exercises — Part 6 — Templates & Metaprogramming

> Practice problems for [[Part 6 — Templates & Metaprogramming]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 6.1 — Function Templates

**Ex 6.1.1** `[Easy]`

Write a function template `clamp(val, lo, hi)` that returns `val` clamped to the range `[lo, hi]`. It must work for any type with `<` and `>`.

```cpp
template<typename T>
T clamp(T val, T lo, T hi) {
    // your solution
}
```

---

**Ex 6.1.2** `[Medium]`

Why does this fail to compile? Fix it without using explicit template arguments.

```cpp
template<typename T>
T max_val(T a, T b) { return a > b ? a : b; }

int main() {
    max_val(1, 2.5);   // error — why?
}
```

```cpp
// your solution
```

---

**Ex 6.1.3** `[Medium]`

Why must templates be defined in header files? What linker error occurs if you put the definition in a `.cpp` file? Describe the exception (explicit instantiation) and why it's rarely used.

```
// your answer
```

---

**Ex 6.1.4** `[Hard]`

Implement a variadic template function `zip_transform(f, range1, range2)` that applies `f` to corresponding elements of two ranges and returns a `std::vector` of results.

```cpp
template<typename F, typename R1, typename R2>
auto zip_transform(F f, const R1& r1, const R2& r2) {
    // your solution
}
// zip_transform(std::plus<>{}, v1, v2)  // element-wise sum
```

---

## Section 6.2 — Class Templates

**Ex 6.2.1** `[Medium]`

Implement a `Pair<T, U>` class template with: constructor, `first()`, `second()`, and `swap()`. Add a non-member `make_pair` function template.

```cpp
template<typename T, typename U>
class Pair {
    // your solution
};
```

---

**Ex 6.2.2** `[Medium]`

Implement a `FixedStack<T, N>` — a stack with a compile-time maximum capacity stored entirely on the stack (no heap allocation). Support `push`, `pop`, `top`, `size`, `empty`, and `full`.

```cpp
template<typename T, std::size_t N>
class FixedStack {
    // your solution
};
```

---

**Ex 6.2.3** `[Hard]`

Implement a `TypedBuffer<T, Alignment>` class that provides correctly-aligned storage for a single `T` object without default-constructing it (like `std::optional`'s storage). Use `alignas` and `std::aligned_storage_t` (or a plain `alignas` array). Support `construct(args...)`, `destroy()`, and `get()`.

```cpp
template<typename T, std::size_t Alignment = alignof(T)>
class TypedBuffer {
    // your solution
};
```

---

## Section 6.3 — Template Specialization: Full & Partial

**Ex 6.3.1** `[Medium]`

Fully specialize `Serializer<T>` for `bool` (output "true"/"false") and `std::string` (output with surrounding quotes). The primary template should use `operator<<`.

```cpp
template<typename T>
struct Serializer {
    static std::string serialize(const T& val) {
        std::ostringstream oss;
        oss << val;
        return oss.str();
    }
};

// your specializations
```

---

**Ex 6.3.2** `[Medium]`

Write a partial specialization of `TypeTraits<T>` for any pointer type `T*` that adds a `is_pointer = true` flag and a `pointed_to` typedef.

```cpp
template<typename T>
struct TypeTraits {
    static constexpr bool is_pointer = false;
};

// your partial specialization for T*
```

---

**Ex 6.3.3** `[Hard]`

Specialize `std::hash<>` for a `Color` struct with `uint8_t r, g, b, a` fields, using a hash-combine approach. Then put `Color` values in an `std::unordered_set`.

```cpp
struct Color { uint8_t r, g, b, a; };

template<>
struct std::hash<Color> {
    // your solution
};
```

---

## Section 6.4 — Variadic Templates & Parameter Packs

**Ex 6.4.1** `[Medium]`

Write a variadic `max_of(args...)` that returns the maximum of all its arguments. It must work for any comparable type and any number of arguments ≥ 1.

```cpp
template<typename T, typename... Rest>
T max_of(T first, Rest... rest) {
    // your solution
}
// max_of(3, 1, 4, 1, 5, 9, 2, 6)  →  9
```

---

**Ex 6.4.2** `[Medium]`

Implement a variadic `tuple_print(const std::tuple<Args...>&)` function that prints each element separated by ", " using `std::index_sequence`.

```cpp
template<typename... Args>
void tuple_print(const std::tuple<Args...>& t) {
    // your solution
}
```

---

**Ex 6.4.3** `[Hard]`

Implement `apply` yourself — it should call a callable `f` with the elements of a `std::tuple` as individual arguments.

```cpp
template<typename F, typename Tuple>
decltype(auto) my_apply(F&& f, Tuple&& t) {
    // your solution — use std::index_sequence
}
```

---

## Section 6.5 — Template Argument Deduction

**Ex 6.5.1** `[Medium]`

For each call, state what `T` is deduced to and explain why:

```cpp
template<typename T> void f(T x);
template<typename T> void g(T& x);
template<typename T> void h(T&& x);

int i = 5;
const int ci = 5;
int& ri = i;

f(i);   // T = ?
f(ci);  // T = ?
g(i);   // T = ?
g(ci);  // T = ?
h(i);   // T = ? (forwarding reference!)
h(5);   // T = ?
```

```
// your answer
```

---

**Ex 6.5.2** `[Medium]`

What is the difference between `auto` and `decltype(auto)` as a return type? Write a function `identity` using each, and show a case where they return different types.

```cpp
template<typename T>
auto         identity_auto(T&& x)         { return std::forward<T>(x); }

template<typename T>
decltype(auto) identity_decltype(T&& x)   { return std::forward<T>(x); }

// Show where they differ
```

---

## Section 6.6 — SFINAE: Substitution Failure Is Not An Error

**Ex 6.6.1** `[Medium]`

Use `std::enable_if` to write two overloads of `process`: one for integral types and one for floating-point types.

```cpp
template<typename T, /* enable_if condition */>
void process(T val) { std::cout << "integral: " << val; }

template<typename T, /* enable_if condition */>
void process(T val) { std::cout << "float: " << val; }
```

---

**Ex 6.6.2** `[Medium]`

Use `std::void_t` to write a type trait `has_size_method<T>` that is true if `T` has a `.size()` member function returning something convertible to `size_t`.

```cpp
template<typename T, typename = void>
struct has_size_method : std::false_type {};

// your specialization
```

---

**Ex 6.6.3** `[Hard]`

Implement a `call_if_has_reserve(container, n)` function that calls `container.reserve(n)` only if the container has a `reserve` method (i.e., it's a vector-like container, not a list or map). Use SFINAE or `if constexpr`.

```cpp
template<typename Container>
void call_if_has_reserve(Container& c, size_t n) {
    // your solution
}
```

---

## Section 6.7 — Type Traits (`<type_traits>`)

**Ex 6.7.1** `[Easy]`

Use type traits to write `static_assert`s verifying:
- `int` is integral
- `double` is floating point
- `std::string` is a class
- `int*` is a pointer
- `const int` is the same as `int` after `remove_const`
- `int&&` decays to `int`

```cpp
// your solution
```

---

**Ex 6.7.2** `[Medium]`

Write a function `safe_cast<To>(from)` that uses `std::is_convertible` and `std::is_arithmetic` to only compile when the conversion is safe (both arithmetic types, no narrowing detected via `sizeof` comparison).

```cpp
template<typename To, typename From>
To safe_cast(From val) {
    // your solution — static_assert to prevent unsafe casts
}
```

---

**Ex 6.7.3** `[Hard]`

Implement a custom type trait `is_iterable<T>` that is true if `T` has `begin()` and `end()` member functions. Then write a generic `print_if_iterable` that prints contents if iterable, or just the value otherwise.

```cpp
template<typename T, typename = void>
struct is_iterable : std::false_type {};

// your solution
```

---

## Section 6.8 — `if constexpr` in Templates

**Ex 6.8.1** `[Medium]`

Write a single `to_string<T>` function template using `if constexpr` that handles: `bool`, integers, floats, `std::string` (identity), and anything else with an `operator<<` fallback.

```cpp
template<typename T>
std::string to_string(const T& val) {
    // your solution
}
```

---

**Ex 6.8.2** `[Hard]`

Write a `serialize_binary` function that uses `if constexpr` and type traits to: write trivially-copyable types as raw bytes, write `std::string` as a length-prefixed byte sequence, and refuse to compile for other types.

```cpp
void serialize_binary(std::ostream& out, const auto& val) {
    // your solution
}
```

---

## Section 6.9 — Concepts (C++20) — Replacing SFINAE

**Ex 6.9.1** `[Medium]`

Rewrite Ex 6.6.1 (integral vs floating-point overloads of `process`) using concepts instead of `enable_if`.

```cpp
// your solution
```

---

**Ex 6.9.2** `[Medium]`

Write a concept `Hashable` that requires a type to be usable as a key in `std::unordered_map` (i.e., `std::hash<T>` exists and returns `size_t`, and `T` is equality comparable).

```cpp
template<typename T>
concept Hashable = /* your solution */;
```

---

**Ex 6.9.3** `[Hard]`

Write a concept `OutputStream` satisfied by any type that supports `operator<<` for `int`, `double`, and `std::string`. Then write a generic `Logger` class template constrained by it.

```cpp
template<typename T>
concept OutputStream = /* your solution */;

template<OutputStream Out>
class Logger {
    // your solution
};
```

---

## Section 6.10 — `consteval` & Compile-Time Computation

**Ex 6.10.1** `[Medium]`

Write a `consteval` function `nth_prime(int n)` that returns the n-th prime number. Use it to compute the first 10 primes as a `constexpr` array at compile time.

```cpp
consteval int nth_prime(int n) {
    // your solution
}

constexpr std::array<int, 10> first_10_primes = {/* your solution */};
```

---

**Ex 6.10.2** `[Hard]`

Write a compile-time `CRC32` function as a `consteval` function that computes the CRC32 checksum of a `std::string_view` literal. Verify a known string's checksum with `static_assert`.

```cpp
consteval uint32_t crc32(std::string_view s) {
    // your solution
}
```

---

## Section 6.11 — CRTP: Curiously Recurring Template Pattern

**Ex 6.11.1** `[Medium]`

Use CRTP to create a `Comparable<Derived>` mixin that provides `!=`, `>`, `<=`, `>=` from just `==` and `<` defined in the derived class.

```cpp
template<typename Derived>
class Comparable {
    // your solution
};

class Weight : public Comparable<Weight> {
    double kg_;
public:
    explicit Weight(double kg) : kg_(kg) {}
    bool operator==(const Weight& o) const { return kg_ == o.kg_; }
    bool operator< (const Weight& o) const { return kg_ <  o.kg_; }
};
```

---

**Ex 6.11.2** `[Hard]`

Implement a CRTP `Singleton<Derived>` base class that ensures only one instance can be created. The instance should be accessible via `Derived::instance()`.

```cpp
template<typename Derived>
class Singleton {
    // your solution
};

class Config : public Singleton<Config> {
    // only one Config can ever exist
};
```

---

## Section 6.12 — Policy-Based Design

**Ex 6.12.1** `[Medium]`

Design a `Cache<T, StoragePolicy, EvictionPolicy>` class template where:
- `StoragePolicy` provides `store(key, value)` and `load(key)` 
- `EvictionPolicy` decides what to remove when the cache is full

Implement `VectorStorage` and `LRUEviction` as concrete policies.

```cpp
template<typename T,
         typename StoragePolicy,
         typename EvictionPolicy>
class Cache {
    // your solution
};
```

---

**Ex 6.12.2** `[Hard]`

Implement a `Logger<FormatPolicy, OutputPolicy>` where `FormatPolicy` determines how to format a message (plain, with timestamp, with severity) and `OutputPolicy` determines where to write it (stdout, file, network). Show at least two concrete policies for each.

```cpp
// your solution
```
