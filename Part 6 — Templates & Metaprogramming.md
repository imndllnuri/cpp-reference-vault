# Part 6 — Templates & Metaprogramming

> Templates are C++'s mechanism for generic programming — write code once, have it work for any type that satisfies the requirements. Metaprogramming extends this to compute at compile time, enforce constraints, and generate specialized code. This part covers everything from basic templates through SFINAE, type traits, CRTP, and C++20 concepts.

## Table of Contents

- [[#6.1 — Function Templates]]
- [[#6.2 — Class Templates]]
- [[#6.3 — Template Specialization: Full & Partial]]
- [[#6.4 — Variadic Templates & Parameter Packs]]
- [[#6.5 — Template Argument Deduction]]
- [[#6.6 — SFINAE: Substitution Failure Is Not An Error]]
- [[#6.7 — Type Traits (<type_traits>)]]
- [[#6.8 — if constexpr in Templates]]
- [[#6.9 — Concepts (C++20) — Replacing SFINAE]]
- [[#6.10 — consteval & Compile-Time Computation]]
- [[#6.11 — CRTP: Curiously Recurring Template Pattern]]
- [[#6.12 — Policy-Based Design]]

---

## 6.1 — Function Templates

### What Templates Are

A template is a blueprint — the compiler uses it to generate actual functions or classes when the template is instantiated with concrete types. No code is generated until the template is actually used.

```cpp
// A regular function — only works for int
int max_int(int a, int b) { return a > b ? a : b; }

// A function template — works for any type that supports >
template<typename T>
T max_val(T a, T b) { return a > b ? a : b; }

max_val(3, 5);        // T = int, compiler generates max_val<int>
max_val(3.14, 2.71);  // T = double, compiler generates max_val<double>
max_val('a', 'z');    // T = char
```

The `typename` keyword introduces a template parameter (interchangeable with `class` here — `class` does not mean the type must be a class):

```cpp
template<typename T>   // idiomatic for general type parameters
template<class T>      // equivalent — a historical artifact
```

### Explicit Template Arguments

When the compiler can't deduce the type, or when you want to override deduction, specify it explicitly:

```cpp
template<typename T>
T zero() { return T{}; }

zero<int>();      // returns 0
zero<double>();   // returns 0.0
zero<std::string>(); // returns ""

// Can't deduce — zero() has no arguments to deduce from
```

Partial explicit specification — specify the ones the compiler can't deduce:

```cpp
template<typename To, typename From>
To convert(From val) { return static_cast<To>(val); }

convert<double>(42);     // To = double (explicit), From = int (deduced)
convert<float, int>(42); // both explicit
```

### Multiple Template Parameters

```cpp
template<typename T, typename U>
auto add(T a, U b) { return a + b; }   // return type deduced — might be int+double = double

add(1, 2.5);   // T=int, U=double, returns double

// Non-type template parameters — values, not types
template<int N>
void print_n_times(const std::string& s) {
    for (int i = 0; i < N; ++i) std::cout << s << "\n";
}

print_n_times<3>("hello");   // prints 3 times — N is a compile-time constant
```

### Default Template Arguments

```cpp
template<typename T, typename Allocator = std::allocator<T>>
class MyVector { ... };

MyVector<int>             // uses std::allocator<int>
MyVector<int, MyAlloc>    // uses custom allocator

template<typename T = int>
T make_default() { return T{}; }

make_default();       // T = int, returns 0
make_default<long>(); // T = long
```

### Where Templates Must Be Defined

Templates must be **defined** in the header file (or in a file `#include`d by the header). They cannot be split across a `.h` declaration and a `.cpp` definition — the compiler needs the full definition at every instantiation point.

```cpp
// WRONG — putting template definition in .cpp
// foo.h
template<typename T> T foo(T x);   // declaration only

// foo.cpp
template<typename T> T foo(T x) { return x * 2; }  // definition

// main.cpp
#include "foo.h"
foo(42);   // linker error: undefined reference to foo<int>
```

```cpp
// CORRECT — full definition in header
// foo.h
template<typename T>
T foo(T x) { return x * 2; }   // definition in header — OK
```

The exception: **explicit instantiation**. If you list every type you'll use:

```cpp
// foo.cpp
template<typename T> T foo(T x) { return x * 2; }
template int foo<int>(int);      // explicit instantiation for int
template double foo<double>(double); // and for double

// foo.h
extern template int foo<int>(int);       // tell other TUs not to instantiate it
extern template double foo<double>(double);
```

This is rarely worth the hassle. Put templates in headers.

---

## 6.2 — Class Templates

### Basic Class Template

```cpp
template<typename T>
class Stack {
public:
    void push(const T& val) { data_.push_back(val); }
    void push(T&& val)      { data_.push_back(std::move(val)); }
    void pop()              { data_.pop_back(); }
    T&   top()              { return data_.back(); }
    bool empty() const      { return data_.empty(); }
    size_t size() const     { return data_.size(); }

private:
    std::vector<T> data_;
};

Stack<int>         int_stack;
Stack<std::string> str_stack;
Stack<Stack<int>>  nested;
```

### Member Function Definitions Outside the Class

```cpp
template<typename T>
class Stack {
public:
    void push(const T& val);
    T& top();
    // ...
};

// Outside definition — must repeat the template header
template<typename T>
void Stack<T>::push(const T& val) {
    data_.push_back(val);
}

template<typename T>
T& Stack<T>::top() {
    return data_.back();
}
```

### Non-Type Template Parameters

Template parameters can be values of integral types, pointers, references, or (since C++20) floating-point values and class types with certain constraints:

```cpp
template<typename T, std::size_t N>
class FixedArray {
public:
    T& operator[](std::size_t i) { return data_[i]; }
    std::size_t size() const { return N; }
private:
    T data_[N];
};

FixedArray<int, 10>    arr10;   // N = 10 — no heap allocation
FixedArray<double, 3>  vec3;

// This is how std::array is implemented:
// template<typename T, std::size_t N>
// struct array { T data_[N]; ... };
```

The array size is a compile-time constant — the compiler generates separate types for `FixedArray<int, 10>` and `FixedArray<int, 20>`.

### Template Template Parameters

Parameters that are themselves templates:

```cpp
template<typename T, template<typename, typename> class Container = std::vector>
class Wrapper {
    Container<T, std::allocator<T>> data_;
};

Wrapper<int>                     // uses std::vector<int>
Wrapper<int, std::list>          // uses std::list<int>
Wrapper<int, std::deque>         // uses std::deque<int>
```

### Nested Types and `typename`

When accessing a type that depends on a template parameter, you must say `typename` — otherwise the compiler doesn't know if the name is a type or a value:

```cpp
template<typename Container>
void print_first(const Container& c) {
    typename Container::value_type first = c.front();  // need typename
    std::cout << first;
}

// Without typename: ambiguous — is Container::value_type a type or a static member?
```

Similarly for member templates:

```cpp
template<typename T>
struct Foo {
    template<typename U>
    void bar(U x);
};

template<typename T>
void use(Foo<T>& f) {
    f.template bar<int>(42);   // need .template to parse bar<int> as a template call
}
```

---

## 6.3 — Template Specialization: Full & Partial

### Full Specialization

Provide a completely different implementation for a specific type:

```cpp
// Primary template — general case
template<typename T>
class TypeInfo {
public:
    static std::string name() { return "unknown"; }
    static bool is_pointer() { return false; }
};

// Full specialization for int
template<>
class TypeInfo<int> {
public:
    static std::string name() { return "int"; }
    static bool is_pointer() { return false; }
};

// Full specialization for const char*
template<>
class TypeInfo<const char*> {
public:
    static std::string name() { return "C-string"; }
    static bool is_pointer() { return true; }
};

TypeInfo<int>::name();        // "int"
TypeInfo<double>::name();     // "unknown"
TypeInfo<const char*>::name(); // "C-string"
```

Function templates can also be fully specialized:

```cpp
template<typename T>
void print(T val) { std::cout << val; }

template<>
void print<bool>(bool val) { std::cout << (val ? "true" : "false"); }

print(42);    // primary template
print(true);  // specialization — prints "true"
```

**Warning:** Overloading is usually better than function template specialization. Specializations don't participate in overload resolution — they're selected after the primary template is chosen, which can lead to surprising behavior. Prefer overloading.

### Partial Specialization

Specialize for a family of types (e.g., all pointer types, all `std::vector` instantiations). Only available for class templates, not function templates (use overloads for functions).

```cpp
// Primary
template<typename T>
class TypeInfo {
public:
    static bool is_pointer() { return false; }
};

// Partial specialization: any pointer type T*
template<typename T>
class TypeInfo<T*> {
public:
    static bool is_pointer() { return true; }
    using pointed_to = T;   // can expose the pointed-to type
};

// Partial specialization: any pair
template<typename A, typename B>
class TypeInfo<std::pair<A, B>> {
public:
    static std::string name() { return "pair<" + TypeInfo<A>::name() + ", " + TypeInfo<B>::name() + ">"; }
};

TypeInfo<int>::is_pointer();    // false — primary
TypeInfo<int*>::is_pointer();   // true — partial spec for T*
TypeInfo<int**>::is_pointer();  // true — T** matches T* where T=int*
```

### Specialization Matching Rules

The compiler picks the **most specialized** partial specialization that matches:

```cpp
template<typename T, typename U>  struct S {};              // primary

template<typename T>              struct S<T, T> {};        // both same type
template<typename T>              struct S<T, int> {};      // second is int
template<typename T>              struct S<T*, int> {};     // pointer + int

S<double, float>{};   // primary
S<int, int>{};        // S<T,T> — both same
S<double, int>{};     // S<T,int> — second is int
S<double*, int>{};    // S<T*,int> — most specific (pointer + int)
```

If two specializations match equally well, it's a compile error (ambiguous).

### `std::hash` Specialization Pattern

The standard pattern for making a custom type hashable:

```cpp
struct Point { int x, y; };

template<>
struct std::hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t h1 = std::hash<int>{}(p.x);
        std::size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 1);   // simple hash combine
    }
};

std::unordered_map<Point, std::string> map;  // now works
```

---

## 6.4 — Variadic Templates & Parameter Packs

### Parameter Packs

A variadic template accepts any number of template arguments:

```cpp
template<typename... Args>
void print_all(Args... args) {
    // args is a parameter pack — a sequence of 0 or more arguments
}
```

The `...` is the **ellipsis operator**. It appears in different positions for different meanings:
- `typename... Args` — declares a type parameter pack
- `Args...` — expands the pack (called a pack expansion)
- `sizeof...(Args)` — count of elements in the pack

### Recursive Variadic Templates (Pre-C++17 Pattern)

Before fold expressions, the standard idiom was recursion:

```cpp
// Base case — empty pack
void print_all() {}

// Recursive case — process one, recurse on the rest
template<typename First, typename... Rest>
void print_all(First first, Rest... rest) {
    std::cout << first << " ";
    print_all(rest...);   // recurse with one fewer argument
}

print_all(1, "hello", 3.14, true);
// prints: 1 hello 3.14 1
```

### Fold Expressions (C++17) — The Modern Way

```cpp
template<typename... Args>
void print_all(Args... args) {
    ((std::cout << args << " "), ...);   // fold over comma operator
}

template<typename... Args>
auto sum(Args... args) {
    return (args + ...);   // fold over +
}

template<typename... Args>
bool all_positive(Args... args) {
    return ((args > 0) && ...);   // fold over &&
}

sum(1, 2, 3, 4);             // 10
all_positive(1, 2, 3);       // true
all_positive(1, -2, 3);      // false
```

Full fold expression reference is in [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.7.

### Pack Expansion Contexts

A pack expansion `pack...` can appear in many contexts:

```cpp
template<typename... Args>
void foo(Args... args) {
    // Function argument list
    bar(args...);                    // passes all args to bar

    // Initializer list
    int arr[] = {args...};           // requires all Args to be int-convertible

    // Template argument list
    std::tuple<Args...> t{args...};  // tuple of all types

    // Base class list
    // struct Derived : Args... {};  // inheriting from all types

    // sizeof
    sizeof...(args);    // number of arguments

    // In a function call, each arg processed individually
    std::make_tuple(std::forward<Args>(args)...);
    // expands to: std::make_tuple(std::forward<A1>(a1), std::forward<A2>(a2), ...)
}
```

### `std::tuple` — The Variadic Data Structure

`std::tuple` stores one value of each type in a parameter pack:

```cpp
std::tuple<int, std::string, double> t{42, "hello", 3.14};

std::get<0>(t);    // 42
std::get<1>(t);    // "hello"
std::get<2>(t);    // 3.14
std::get<double>(t); // 3.14 — get by type (only works if type is unique in tuple)

std::tuple_size_v<decltype(t)>;             // 3
std::tuple_element_t<1, decltype(t)>;      // std::string

auto [a, b, c] = t;   // structured binding — C++17
```

**`std::apply`** — call a function with a tuple's elements as arguments:

```cpp
auto add = [](int a, int b) { return a + b; };
std::tuple<int, int> args{3, 4};
std::apply(add, args);   // 7

// Equivalent to: add(std::get<0>(args), std::get<1>(args))
```

**`std::make_from_tuple`** — construct an object using a tuple as arguments:

```cpp
struct Point { int x, y; };
auto p = std::make_from_tuple<Point>(std::make_tuple(3, 4));
// Equivalent to: Point{3, 4}
```

### Perfect Forwarding with Variadic Templates

The canonical pattern for forwarding constructors and factory functions:

```cpp
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// std::forward<Args>(args)... expands to:
// std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3), ...
// preserving the value category of each argument independently
```

---

## 6.5 — Template Argument Deduction

### How Deduction Works

When you call a function template without specifying types, the compiler deduces them from the arguments. Deduction happens independently for each template parameter by matching the function parameter type to the argument type.

```cpp
template<typename T>
void foo(T x);

foo(42);       // T = int   (exact match)
foo(3.14f);    // T = float (exact match)
foo("hello");  // T = const char* (array decays to pointer)
```

### Deduction Rules — What Gets Dropped

When deducing `T` from `T`, `T&`, `const T&`, etc., certain qualifiers are stripped:

```cpp
template<typename T> void f(T x);
template<typename T> void g(T& x);
template<typename T> void h(const T& x);

int i = 5;
const int ci = 5;
int& ri = i;
const int& cri = i;

f(i);    // T = int
f(ci);   // T = int   (const dropped — passed by value, copy is made)
f(ri);   // T = int   (reference dropped — T is int, not int&)

g(i);    // T = int   (g's parameter is int&)
g(ci);   // T = const int  (g's parameter is const int& — preserves const)
// g(5); // ERROR — 5 is rvalue, can't bind to T& (non-const lvalue ref)

h(i);    // T = int   (const T& binds to anything, T deduced as int)
h(ci);   // T = int   (const already in the parameter type — not duplicated)
h(5);    // T = int   (const T& binds to rvalue)
```

### Forwarding Reference Deduction

When the parameter is `T&&` in a deduced context (forwarding reference), special rules apply:

```cpp
template<typename T>
void forward_to(T&& arg);

int x = 5;
forward_to(x);    // T = int&  → parameter becomes int& && = int&  (lvalue stays lvalue)
forward_to(5);    // T = int   → parameter becomes int&&            (rvalue stays rvalue)
```

This is the reference collapsing mechanism that makes `std::forward` possible.

### Deduction Fails With Conversions

Template deduction does NOT apply implicit conversions:

```cpp
template<typename T>
T max_val(T a, T b) { return a > b ? a : b; }

max_val(1, 2);      // OK — both int, T = int
max_val(1, 2.0);    // ERROR — deduction conflict: T = int from 1, T = double from 2.0
max_val<double>(1, 2.0);  // OK — explicit T = double, 1 converted implicitly
```

### `decltype` and `decltype(auto)` in Return Types

```cpp
// C++11 trailing return type
template<typename A, typename B>
auto add(A a, B b) -> decltype(a + b) { return a + b; }

// C++14 deduced return type
template<typename A, typename B>
auto add(A a, B b) { return a + b; }  // return type deduced from the expression

// decltype(auto) — preserves references and const, unlike plain auto
int x = 5;
int& get_x() { return x; }

auto        v  = get_x();     // int — auto drops reference
decltype(auto) r = get_x();   // int& — decltype(auto) preserves it
```

`decltype(auto)` is critical for transparent wrappers and proxies that need to forward return value categories:

```cpp
template<typename F, typename... Args>
decltype(auto) call_and_forward(F&& f, Args&&... args) {
    return std::forward<F>(f)(std::forward<Args>(args)...);
    // returns the exact type F returns — including references
}
```

### Class Template Argument Deduction (CTAD) — C++17

Covered in [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.7. You can also write **deduction guides** to help the compiler:

```cpp
template<typename T>
class Wrapper {
public:
    Wrapper(T val) : val_(val) {}
private:
    T val_;
};

// Deduction guide — not always needed, compiler generates one for constructors
template<typename T>
Wrapper(T) -> Wrapper<T>;

Wrapper w(42);   // deduced as Wrapper<int>

// Custom deduction guide — convert char* to std::string
Wrapper(const char*) -> Wrapper<std::string>;
Wrapper("hello");  // Wrapper<std::string>, not Wrapper<const char*>
```

---

## 6.6 — SFINAE: Substitution Failure Is Not An Error

### The Rule

When the compiler substitutes a template argument and the result would be an ill-formed expression in the **immediate context** of the template, this is not a hard error — it simply removes that template from the overload set. Other overloads and specializations can still be considered.

```cpp
template<typename T>
typename T::value_type front(const T& container) {  // T must have ::value_type
    return container.front();
}

front(std::vector<int>{1, 2, 3});  // OK: vector<int>::value_type = int
front(42);                          // no error! 42 is int, int has no ::value_type
                                    // substitution fails, template removed from overload set
                                    // if no other overload exists, THEN it's a compile error
```

### `std::enable_if` — Controlling Instantiation

`std::enable_if<Condition, T>` has a member `::type = T` only if `Condition` is true. Used to enable/disable overloads:

```cpp
// Only enabled for integral types
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
increment(T val) { return val + 1; }

// C++14 shorthand
template<typename T>
std::enable_if_t<std::is_integral_v<T>, T>
increment(T val) { return val + 1; }

increment(42);     // works — int is integral
increment(3.14);   // substitution failure — not in overload set
                   // if no other overload, compile error: "no matching function"
```

Common SFINAE patterns:

```cpp
// Enable via return type
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
sqrt_safe(T x) { return x >= 0 ? std::sqrt(x) : T{}; }

// Enable via dummy parameter (default argument)
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void process_int(T x) { ... }

// Enable via non-type parameter
template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
void process_int(T x) { ... }
```

The dummy-parameter and non-type-parameter forms are needed when you want to have two overloads for the same function name — you can't have two functions with the same return type that only differ in an `enable_if`:

```cpp
// WRONG — identical signatures when both enabled
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void foo(T x) { ... }

template<typename T, typename = std::enable_if_t<!std::is_integral_v<T>>>
void foo(T x) { ... }   // error: redefinition (the default arg doesn't distinguish)

// CORRECT — non-type parameter distinguishes them
template<typename T, std::enable_if_t< std::is_integral_v<T>, int> = 0>
void foo(T x) { ... }

template<typename T, std::enable_if_t<!std::is_integral_v<T>, int> = 0>
void foo(T x) { ... }   // OK — different non-type parameter values
```

### `std::void_t` — Detecting Member Existence

`std::void_t<T...>` is always `void` — but SFINAE removes it from the overload set if any `T` is invalid:

```cpp
// Primary — used when T has no begin()
template<typename T, typename = void>
struct HasBegin : std::false_type {};

// Specialization — only exists when T::begin() is valid
template<typename T>
struct HasBegin<T, std::void_t<decltype(std::declval<T>().begin())>>
    : std::true_type {};

HasBegin<std::vector<int>>::value;  // true
HasBegin<int>::value;               // false

// std::declval<T>() creates a "pretend" instance of T for use in decltype
// — lets you query expressions without actually constructing T
```

### Detection Idiom

A general pattern for checking if an expression is valid:

```cpp
// Check if T has a serialize() method
template<typename T, typename = void>
constexpr bool has_serialize = false;

template<typename T>
constexpr bool has_serialize<T, std::void_t<decltype(std::declval<T>().serialize())>> = true;

template<typename T>
void save(const T& obj) {
    if constexpr (has_serialize<T>) {
        obj.serialize();
    } else {
        // fallback
    }
}
```

### SFINAE Limitations and Why Concepts Replace It

SFINAE is powerful but has serious problems:
- **Error messages** — failures produce deeply nested template errors that don't clearly state the constraint
- **Readability** — `enable_if` nesting is hard to write and read
- **Combinability** — AND-ing/OR-ing constraints requires awkward nesting
- **Order sensitivity** — the `dummy parameter` trick is a workaround for a language limitation

C++20 Concepts (section 6.9) replace all of this with clean, readable syntax.

---

## 6.7 — Type Traits (`<type_traits>`)

Type traits are class templates that provide information about types at compile time. They are the foundation of SFINAE and concepts.

### Category Checks

```cpp
#include <type_traits>

// Primary type categories — mutually exclusive
std::is_void_v<void>            // true
std::is_null_pointer_v<std::nullptr_t>  // true
std::is_integral_v<int>         // true
std::is_floating_point_v<double>// true
std::is_array_v<int[5]>         // true
std::is_pointer_v<int*>         // true
std::is_reference_v<int&>       // true
std::is_lvalue_reference_v<int&>  // true
std::is_rvalue_reference_v<int&&> // true
std::is_function_v<void()>      // true
std::is_enum_v<MyEnum>          // true
std::is_class_v<std::string>    // true
std::is_union_v<MyUnion>        // true

// Composite categories
std::is_arithmetic_v<int>       // true (integral || floating_point)
std::is_fundamental_v<double>   // true (arithmetic || void || nullptr_t)
std::is_compound_v<int*>        // true (everything not fundamental)
std::is_scalar_v<int*>          // true (arithmetic || enum || pointer || ptr-to-member || nullptr_t)
std::is_object_v<int>           // true (not function, reference, or void)
```

### Type Property Checks

```cpp
std::is_const_v<const int>           // true
std::is_volatile_v<volatile int>     // true
std::is_trivial_v<int>               // true — trivially constructible/copyable/destructible
std::is_standard_layout_v<int>       // true — predictable memory layout
std::is_pod_v<int>                   // true — both trivial and standard layout (deprecated C++20)
std::is_empty_v<EmptyStruct>         // true — sizeof is 1 (empty base optimization)
std::is_polymorphic_v<T>             // true — has virtual functions
std::is_abstract_v<AbstractBase>     // true — has pure virtual functions
std::is_final_v<FinalClass>          // true — marked final

std::is_default_constructible_v<T>   // T{} is valid
std::is_copy_constructible_v<T>      // T(const T&) exists
std::is_move_constructible_v<T>      // T(T&&) exists
std::is_copy_assignable_v<T>         // T = const T& works
std::is_move_assignable_v<T>         // T = T&& works
std::is_destructible_v<T>            // ~T() is valid

// Nothrow versions — important for noexcept propagation
std::is_nothrow_move_constructible_v<T>  // move ctor is noexcept
std::is_nothrow_move_assignable_v<T>
std::is_nothrow_destructible_v<T>
std::is_nothrow_swappable_v<T>
```

### Type Relationships

```cpp
std::is_same_v<int, int>             // true
std::is_same_v<int, const int>       // false — const matters
std::is_base_of_v<Base, Derived>     // true if Derived inherits from Base
std::is_convertible_v<int, double>   // true — implicit conversion exists
std::is_invocable_v<F, Args...>      // F can be called with Args
std::is_invocable_r_v<R, F, Args...> // F called with Args returns R
```

### Type Transformations

```cpp
// Add/remove qualifiers
std::add_const_t<int>          // const int
std::remove_const_t<const int> // int
std::add_volatile_t<int>       // volatile int
std::remove_volatile_t<volatile int> // int
std::remove_cv_t<const volatile int> // int  (both const and volatile)

// Add/remove pointers and references
std::add_pointer_t<int>        // int*
std::remove_pointer_t<int*>    // int
std::add_lvalue_reference_t<int> // int&
std::add_rvalue_reference_t<int> // int&&
std::remove_reference_t<int&>  // int
std::remove_reference_t<int&&> // int

// Decay — models what happens when passing by value (C-array → pointer, function → pointer, strips cv)
std::decay_t<int[5]>           // int*
std::decay_t<void()>           // void(*)()
std::decay_t<const int&>       // int

// Conditional type selection
std::conditional_t<true,  int, double>  // int
std::conditional_t<false, int, double>  // double

// Common type of multiple types
std::common_type_t<int, double>    // double
std::common_type_t<int, long, double> // double

// Underlying type of enum
enum class Color : uint8_t { Red, Green, Blue };
std::underlying_type_t<Color>  // uint8_t
```

### Writing Custom Type Traits

```cpp
// Pattern: inherit from true_type or false_type
template<typename T>
struct is_smart_pointer : std::false_type {};

template<typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

template<typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

template<typename T>
struct is_smart_pointer<std::weak_ptr<T>> : std::true_type {};

// Convenience variable template
template<typename T>
inline constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

is_smart_pointer_v<std::unique_ptr<int>>;   // true
is_smart_pointer_v<int*>;                   // false
```

---

## 6.8 — `if constexpr` in Templates

`if constexpr` is the clean replacement for the most common SFINAE patterns. Covered conceptually in [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.7. This section shows advanced patterns in template contexts.

### Replacing Recursive Template Specialization

The old way — multiple specializations to handle base and recursive cases:

```cpp
// Pre-C++17: recursive specialization chain
template<std::size_t N>
struct Factorial { static constexpr int value = N * Factorial<N-1>::value; };

template<>
struct Factorial<0> { static constexpr int value = 1; };
```

With `if constexpr` — a single function:

```cpp
// C++17: single constexpr function
constexpr int factorial(int n) {
    if constexpr (n == 0) return 1;    // ERROR — n must be constexpr, not a runtime param
    // actually, this works as a regular function now:
    return n <= 0 ? 1 : n * factorial(n - 1);
}
```

The real value is in template functions where you check the type, not the value:

```cpp
template<typename T>
void serialize(std::ostream& out, const T& val) {
    if constexpr (std::is_arithmetic_v<T>) {
        out.write(reinterpret_cast<const char*>(&val), sizeof(T));
    } else if constexpr (std::is_same_v<T, std::string>) {
        uint32_t len = val.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(val.data(), len);
    } else {
        val.serialize(out);   // assumes T has a serialize method
                              // NOT compiled for arithmetic or string types — no error
    }
}
```

### Visiting Variant with `if constexpr`

```cpp
std::variant<int, double, std::string> v = "hello";

std::visit([](auto&& val) {
    using T = std::decay_t<decltype(val)>;
    if constexpr (std::is_same_v<T, int>) {
        std::cout << "int: " << val;
    } else if constexpr (std::is_same_v<T, double>) {
        std::cout << "double: " << val;
    } else {
        std::cout << "string: " << val;
    }
}, v);
```

### Enabling/Disabling Member Functions

`if constexpr` doesn't work to conditionally compile member functions — the whole class is compiled. For that, use concepts or SFINAE:

```cpp
template<typename T>
class Container {
public:
    // Only available when T is equality comparable
    bool contains(const T& val) const requires std::equality_comparable<T> {
        return std::find(data_.begin(), data_.end(), val) != data_.end();
    }
private:
    std::vector<T> data_;
};
```

---

## 6.9 — Concepts (C++20) — Replacing SFINAE

Concepts are covered in detail in [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.10. This section covers advanced usage patterns specific to metaprogramming — composing concepts, using them in class templates, and replacing SFINAE patterns.

### Composing Concepts

Concepts can be built from other concepts using `&&` and `||`:

```cpp
template<typename T>
concept SignedIntegral = std::integral<T> && std::signed_integral<T>;

template<typename T>
concept StringLike = std::same_as<T, std::string>
                  || std::same_as<T, std::string_view>
                  || std::convertible_to<T, std::string_view>;

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;
```

### Concepts in Class Templates

```cpp
template<std::integral T>
class IntWrapper {
    T val_;
public:
    IntWrapper(T v) : val_(v) {}
    T get() const { return val_; }
};

IntWrapper<int>    a{5};   // OK
IntWrapper<double> b{5.0}; // ERROR — double doesn't satisfy std::integral
```

Constrained member functions:

```cpp
template<typename T>
class Optional {
public:
    // operator== only available when T is equality_comparable
    bool operator==(const Optional& other) const
        requires std::equality_comparable<T>
    {
        if (!has_val_ || !other.has_val_) return has_val_ == other.has_val_;
        return val_ == other.val_;
    }
};
```

### SFINAE Pattern → Concept Replacement

**Before (SFINAE):**

```cpp
template<typename T>
std::enable_if_t<
    std::is_integral_v<T> && (sizeof(T) <= 4),
    void
>
process(T val) { ... }
```

**After (Concepts):**

```cpp
template<typename T>
concept SmallInt = std::integral<T> && (sizeof(T) <= 4);

void process(SmallInt auto val) { ... }
// or: template<SmallInt T> void process(T val) { ... }
```

### Subsumption — Concept Ordering

When multiple concepts apply, the compiler picks the **most constrained** one. This is called subsumption:

```cpp
void foo(std::regular auto x)    { std::cout << "regular"; }
void foo(std::integral auto x)   { std::cout << "integral"; }  // more constrained

foo(42);      // calls foo(std::integral auto) — integral subsumes regular
foo(3.14);    // calls foo(std::regular auto) — double is regular but not integral
```

A concept `A` subsumes concept `B` if `A`'s constraints include all of `B`'s constraints (formally, `A`'s constraint expression implies `B`'s). `std::integral` requires `std::regular` (among other things), so `std::integral` subsumes `std::regular`.

This is the key advantage over SFINAE — ordering between overloads is well-defined.

### `requires` Clauses for Method Constraints

```cpp
template<typename T>
class SmartRef {
    T* ptr_;
public:
    // Only callable when T has an operator->
    auto operator->() requires requires(T* p) { p->something; }
    { return ptr_; }

    // Chain requires
    void save(std::ostream& out) const
        requires std::copyable<T> && requires(T v, std::ostream& s) { s << v; }
    {
        out << *ptr_;
    }
};
```

---

## 6.10 — `consteval` & Compile-Time Computation

`constexpr` and `consteval` are covered in [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.4. This section extends those concepts into metaprogramming territory.

### Compile-Time Data Structures

C++20 allows `std::vector` and `std::string` in `constexpr` context — they can be used during compilation, but the final value stored in a `constexpr` variable must not contain heap-allocated memory (it must have been "freed" before the result is stored):

```cpp
constexpr std::vector<int> get_evens(int n) {
    std::vector<int> result;
    for (int i = 2; i <= n; i += 2) result.push_back(i);
    return result;   // vector is destroyed — but the compiler tracks the values
}

// The compiler evaluates this and stores the result in an array-like structure
constexpr auto evens = get_evens(10);
// evens = {2, 4, 6, 8, 10}
```

### Template Metaprogramming (TMP) — The Pre-`constexpr` Era

Before `constexpr` functions, computation was done via template recursion. Understanding this helps you read older code:

```cpp
// Compile-time factorial via template recursion
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N-1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

Factorial<10>::value;   // 3628800 — computed at compile time

// Modern equivalent — just a constexpr function
constexpr int factorial(int n) {
    return n <= 0 ? 1 : n * factorial(n - 1);
}
constexpr int f = factorial(10);
```

**Prefer `constexpr` functions** over template metaprogramming for computation. TMP syntax is opaque; `constexpr` functions look like ordinary code.

### `std::integral_constant` — Type-Level Values

The foundation of `true_type` and `false_type`:

```cpp
// std::integral_constant<T, v> wraps a compile-time value as a type
using TrueType  = std::integral_constant<bool, true>;   // std::true_type
using FalseType = std::integral_constant<bool, false>;  // std::false_type

std::true_type::value;   // true
std::false_type::value;  // false

// Can use in function overloads to dispatch at compile time
template<typename T>
void serialize_impl(const T& val, std::true_type /* is_trivial */) {
    // Fast path: raw memory copy
}

template<typename T>
void serialize_impl(const T& val, std::false_type /* is_trivial */) {
    // Slow path: element-wise serialization
}

template<typename T>
void serialize(const T& val) {
    serialize_impl(val, std::is_trivially_copyable<T>{});
    // passes true_type or false_type — selects overload at compile time
}
```

This "tag dispatch" pattern is older than `if constexpr` and still seen in standard library implementations.

### `constinit` for Static Initialization Order

```cpp
// In a header
inline constinit int global_count = 0;   // guaranteed constant initialization
                                           // prevents static initialization order fiasco
                                           // for this variable
global_count = 5;   // still mutable — constinit is not const

// In a .cpp
constinit int another = some_constexpr_function();   // must be a constant expression
```

---

## 6.11 — CRTP: Curiously Recurring Template Pattern

### The Pattern

CRTP is when a class derives from a template instantiation of itself:

```cpp
template<typename Derived>
class Base {
public:
    void interface() {
        // Call the derived class's implementation
        static_cast<Derived*>(this)->implementation();
    }
};

class Concrete : public Base<Concrete> {
public:
    void implementation() {
        std::cout << "Concrete::implementation\n";
    }
};

Concrete c;
c.interface();  // calls Concrete::implementation — no virtual dispatch
```

The key: `Base` receives the derived type as a template argument. It can call derived methods directly via `static_cast<Derived*>(this)` — this is resolved at **compile time**, not runtime. No vtable, no virtual function overhead.

### Static Polymorphism (Compile-Time Polymorphism)

```cpp
template<typename Derived>
class Animal {
public:
    void speak() {
        static_cast<Derived*>(this)->speak_impl();
    }
    void move() {
        static_cast<Derived*>(this)->move_impl();
    }
};

class Dog : public Animal<Dog> {
public:
    void speak_impl() { std::cout << "Woof\n"; }
    void move_impl()  { std::cout << "Dog runs\n"; }
};

class Cat : public Animal<Cat> {
public:
    void speak_impl() { std::cout << "Meow\n"; }
    void move_impl()  { std::cout << "Cat slinks\n"; }
};

// Works like polymorphism — but Dog and Cat are different types, no common base*
Dog d; d.speak();  // "Woof" — resolved at compile time
Cat c; c.speak();  // "Meow" — resolved at compile time
```

**CRTP vs virtual dispatch:**
- CRTP: polymorphism only at compile time. All types must be known. Inlinable. No vtable.
- Virtual: runtime polymorphism. Can store `Base*` to different derived objects. Vtable overhead (~2ns per call, inhibits inlining).

### CRTP for Adding Operators

A very common CRTP use: implement `!=`, `>`, `<=`, `>=` from just `==` and `<`:

```cpp
template<typename Derived>
class Comparable {
public:
    bool operator!=(const Derived& other) const {
        return !(static_cast<const Derived&>(*this) == other);
    }
    bool operator>(const Derived& other) const {
        return other < static_cast<const Derived&>(*this);
    }
    bool operator<=(const Derived& other) const {
        return !(other < static_cast<const Derived&>(*this));
    }
    bool operator>=(const Derived& other) const {
        return !(static_cast<const Derived&>(*this) < other);
    }
};

class Temperature : public Comparable<Temperature> {
public:
    Temperature(double deg) : degrees_(deg) {}
    bool operator==(const Temperature& o) const { return degrees_ == o.degrees_; }
    bool operator< (const Temperature& o) const { return degrees_ <  o.degrees_; }
private:
    double degrees_;
};

Temperature a{100}, b{200};
a != b;  // true — from Comparable
a <= b;  // true — from Comparable
```

Note: C++20's spaceship operator (`<=>`) largely supersedes this pattern for comparisons.

### CRTP for Mixin Behavior

Add functionality to a class by inheriting from a CRTP base that provides it:

```cpp
template<typename Derived>
class Printable {
public:
    void print() const {
        static_cast<const Derived*>(this)->print_impl(std::cout);
    }
    std::string to_string() const {
        std::ostringstream oss;
        static_cast<const Derived*>(this)->print_impl(oss);
        return oss.str();
    }
};

template<typename Derived>
class Serializable {
public:
    void save(const std::string& filename) const {
        std::ofstream f(filename);
        static_cast<const Derived*>(this)->serialize(f);
    }
};

class Config : public Printable<Config>, public Serializable<Config> {
public:
    void print_impl(std::ostream& os) const {
        os << "Config{width=" << width_ << ", height=" << height_ << "}";
    }
    void serialize(std::ofstream& f) const {
        f << width_ << " " << height_;
    }
private:
    int width_ = 1920, height_ = 1080;
};

Config cfg;
cfg.print();             // uses Printable
cfg.save("config.txt");  // uses Serializable
```

### CRTP Pitfalls

**Object slicing** — if you store by value as `Base<Derived>`, you lose the Derived part. Always store CRTP objects as `Derived&` or `Derived*`, never as `Base`.

**Access** — the implementation methods (`speak_impl`, `print_impl`) can be `private` if you `friend` the base:

```cpp
class Dog : public Animal<Dog> {
    friend class Animal<Dog>;   // allow base to call private methods
private:
    void speak_impl() { ... }  // hidden from outside
};
```

---

## 6.12 — Policy-Based Design

### The Concept

Policy-based design (popularized by Andrei Alexandrescu in "Modern C++ Design") decomposes a class's behavior into independent, interchangeable **policies** that are passed as template parameters. Each policy handles one specific aspect of behavior.

```cpp
// A class that stores data — storage policy is separate from the container logic
template<
    typename T,
    typename StoragePolicy = std::vector<T>,
    typename SortPolicy    = std::less<T>
>
class SortedContainer {
public:
    void insert(const T& val) {
        storage_.push_back(val);
        std::sort(storage_.begin(), storage_.end(), SortPolicy{});
    }

    const T& min() const { return storage_.front(); }
    const T& max() const { return storage_.back(); }

private:
    StoragePolicy storage_;
};

SortedContainer<int>                          sc1;  // vector + less
SortedContainer<int, std::deque<int>>         sc2;  // deque + less
SortedContainer<int, std::vector<int>, std::greater<int>> sc3; // vector + greater
```

### Policy vs Strategy Pattern

Both solve "vary one aspect of behavior" — but differently:

| | Policy (template) | Strategy (virtual) |
|---|---|---|
| Resolved | Compile time | Runtime |
| Overhead | None — inlined | Virtual call |
| Flexibility | Fixed at instantiation | Swappable at runtime |
| Error detection | Compile time | Runtime |

Use policy when the behavior is fixed for the lifetime of the object. Use virtual strategy when it needs to change at runtime.

### Real Example: Allocator Policy

`std::vector` uses this — the allocator is a policy:

```cpp
std::vector<int>                              // uses std::allocator<int>
std::vector<int, MyAllocator<int>>            // uses custom allocator
std::vector<int, std::pmr::polymorphic_allocator<int>>  // polymorphic allocator
```

The allocator policy controls all memory operations without changing any of `vector`'s logic.

### Empty Base Optimization (EBO)

When a policy has no state (it's a stateless functor or an empty struct), inheriting from it instead of holding it as a member makes the object smaller — an empty base takes zero space, while an empty member takes at least 1 byte (plus alignment padding):

```cpp
template<typename Derived, typename Compare = std::less<>>
class Sorted : private Compare {   // inherit from Compare (empty policy)
    // sizeof(Sorted) = sizeof(data_) — Compare takes no space
    std::vector<Derived> data_;
public:
    bool less_than(const Derived& a, const Derived& b) const {
        return Compare::operator()(a, b);   // call inherited comparator
    }
};
```

C++20 `[[no_unique_address]]` achieves the same for members without inheritance:

```cpp
template<typename T, typename Compare = std::less<T>>
class Sorted {
    std::vector<T> data_;
    [[no_unique_address]] Compare cmp_;   // takes zero space if Compare is empty
};
```

### Policy Traits — Customizing Behavior Through Traits Classes

A traits class bundles multiple related policies for a type:

```cpp
// Default traits for numeric types
template<typename T>
struct NumericTraits {
    static constexpr T zero()    { return T{0}; }
    static constexpr T one()     { return T{1}; }
    static constexpr T epsilon() { return std::numeric_limits<T>::epsilon(); }
    static bool equal(T a, T b)  { return a == b; }
};

// Specialization for floating point — use epsilon comparison
template<>
struct NumericTraits<double> {
    static constexpr double zero()    { return 0.0; }
    static constexpr double one()     { return 1.0; }
    static constexpr double epsilon() { return 1e-9; }
    static bool equal(double a, double b) { return std::abs(a - b) < epsilon(); }
};

template<typename T, typename Traits = NumericTraits<T>>
class Matrix {
public:
    bool is_zero_element(T val) const {
        return Traits::equal(val, Traits::zero());
    }
};

Matrix<double> m;   // uses floating-point-aware equality by default
```

This pattern appears throughout the standard library — `std::char_traits` for string operations, allocators for containers, hash and equality for unordered containers.
