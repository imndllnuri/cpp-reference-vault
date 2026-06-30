# C++ Fundamentals

> This file covers the conceptual and mechanical foundation of C++. Before writing classes, templates, or anything advanced, these are the things you need to truly understand — not just know.

## Table of Contents

- [[#1.1 — History & Philosophy of C++]]
- [[#1.2 — The Compilation Model]]
- [[#1.3 — Basic Types & Literals]]
- [[#1.4 — Type Qualifiers: const, volatile, mutable]]

---

## 1.1 — History & Philosophy of C++

### Origin

C++ was created by **Bjarne Stroustrup** at Bell Labs starting in **1979**. He was writing his PhD thesis in simulation software and ran into a recurring frustration: the languages available forced him to choose between two bad options.

- **C** gave him full control over hardware, memory, and performance — but it was too low-level for complex software. No classes, no type abstraction, manual everything.
- **Simula** had the right ideas — classes, inheritance, object-oriented structure — but was far too slow for serious systems work.

He wanted both. He started with C and added Simula's class model on top of it. The first version was called **"C with Classes"** (1979). By **1983** it had grown enough to earn a new name: **C++** (the `++` is the increment operator — a nod to "one step beyond C").

The first ISO standard was published in **1998** (C++98). Since then, the language has been revised on a roughly 3-year cycle.

### The Zero-Overhead Principle

This is the single most important idea in C++. Stroustrup stated it as two rules:

1. **You don't pay for what you don't use.**
2. **What you do use, you couldn't hand-write cheaper.**

Every major design decision in C++ traces back to this. There is no garbage collector — because GC has overhead even when you don't need it. There is no mandatory runtime — because not all programs need one. Abstractions like classes, templates, and RAII compile down to exactly the machine code you would have written by hand.

This is what separates C++ from Java, Python, Go, or Rust: in those languages, the runtime or safety system always costs something. In C++, you only pay for the features you actually use.

### Multi-Paradigm by Design

C++ does not force you into a single programming style. It supports:

- **Procedural** — write functions, call them in sequence, just like C.
- **Object-Oriented** — classes, inheritance, polymorphism, encapsulation.
- **Generic** — templates let you write code that works for any type.
- **Functional** — lambdas, `std::function`, higher-order functions (partial support).

This is intentional. Different problems are best solved with different paradigms. C++ lets you choose the right tool per problem rather than forcing everything through one lens.

### The Standards Timeline

Understanding which standard a piece of code was written against is critical — C++98 and C++20 look like different languages.

| Standard | Year | What it introduced |
|----------|------|-------------------|
| C++98 | 1998 | First standard. STL, templates, exceptions. |
| C++03 | 2003 | Minor bug-fix revision of C++98. |
| C++11 | 2011 | **The watershed.** `auto`, move semantics, lambdas, `nullptr`, range-for, threading, `constexpr`, smart pointers. |
| C++14 | 2014 | Polish on C++11. Generic lambdas, `std::make_unique`. |
| C++17 | 2017 | Structured bindings, `if constexpr`, `std::optional`, `std::variant`, `std::filesystem`. |
| C++20 | 2020 | Concepts, modules, coroutines, ranges, `std::format`. Biggest update since C++11. |
| C++23 | 2023 | `std::expected`, `std::print`, `std::flat_map`, more. |

When you read code from a codebase, always ask: *what era is this from?* A codebase written in C++03 style will look alien compared to modern C++20 code — raw pointers everywhere, manual `delete`, no lambdas.

**"Modern C++"** refers to C++11 and beyond. This wiki targets C++23 throughout.

### Where C++ Lives Today

C++ dominates in environments where performance is non-negotiable or where you need direct control over hardware:

- **Game engines** — Unreal Engine is C++. Most AAA games are.
- **Browsers** — Chrome (Chromium), Firefox (Gecko), Safari (WebKit) — all C++.
- **Operating systems** — Windows kernel components, parts of Linux drivers, macOS internals.
- **Compilers and toolchains** — LLVM, GCC, and Clang are themselves written in C++.
- **Databases** — MySQL, MongoDB, RocksDB.
- **High-frequency trading** — latency is measured in nanoseconds; GC pauses are unacceptable.
- **Embedded and robotics** — ROS (Robot Operating System) is C++.
- **Graphics and compute** — Vulkan, DirectX, CUDA drivers.

The common thread: situations where you cannot afford a runtime, where memory layout matters, or where the cost of abstraction must be zero.

### The Honest Complexity Trade-Off

C++ is genuinely complex. This is not an accident and not a design failure — it is the direct consequence of two things:

1. **Backward compatibility is nearly sacred.** C++ has almost never removed a feature. Every version adds to the language without breaking code written decades ago. This accumulates.
2. **Low-level control requires more decisions from the programmer.** When the language doesn't make choices for you (no GC, no mandatory bounds checking, no runtime type info by default), you have to make those choices yourself.

Understanding this is freeing. You stop fighting the language and start working with it — using the right feature for the right context, and knowing why each one exists.

---

## 1.2 — The Compilation Model

Understanding how C++ code becomes an executable is not optional. It explains why header files exist, why "undefined reference" errors happen, and why `#include` is nothing like Python's `import`.

C++ compilation happens in **three distinct phases**: preprocessing, compilation, and linking.

### Phase 1 — Preprocessing

The preprocessor runs **before the compiler sees your code**. It is a pure text-manipulation engine. It knows nothing about C++ syntax, types, or semantics — it only processes lines that start with `#`.

**`#include`** is the most important directive. It does exactly one thing: **copy-paste the contents of the named file at that exact line.** That's it. There is no module system, no namespace isolation, no smart importing — just a literal textual substitution.

```cpp
#include <vector>   // pastes the entire <vector> header here
#include "utils.h"  // pastes utils.h here
```

This is why large headers slow down compilation. Every file that includes `<windows.h>` or a heavy Boost header is effectively compiling millions of lines of text.

**`#define`** replaces every occurrence of a name with a value — again, pure text substitution before the compiler runs:

```cpp
#define MAX_SIZE 100
#define SQUARE(x) ((x) * (x))  // dangerous — x is evaluated twice
```

Macros have no type checking, no scope, and can cause subtle bugs. Prefer `const`, `constexpr`, and `inline` functions instead. See [[#1.4 — Type Qualifiers const, volatile, mutable]] for `const`, and [[Part 5 — Modern C++ (C++11 to C++23)]] for `constexpr`.

**Include guards** exist because `#include` is copy-paste. If two files both include `utils.h`, and a third includes both of them, `utils.h` gets pasted twice — causing duplicate definition errors. Guards prevent this:

```cpp
// utils.h
#ifndef UTILS_H
#define UTILS_H

// ... declarations ...

#endif
```

The modern equivalent is `#pragma once` — simpler, and supported by every major compiler:

```cpp
#pragma once

// ... declarations ...
```

**Seeing preprocessor output:** You can inspect what the preprocessor produces before the compiler touches it:

```sh
g++ -E main.cpp -o main.i   # output the preprocessed file
```

Open `main.i` and you will see thousands of lines from included headers. This is educational to do once.

### Phase 2 — Compilation (Translation Units)

After preprocessing, the compiler takes each `.cpp` file — now just one big blob of text with all includes expanded — and compiles it **completely independently** of every other `.cpp` file.

Each `.cpp` file is called a **translation unit**. The compiler knows nothing about other translation units while compiling yours.

The compiler produces an **object file** (`.o` on Linux/Mac, `.obj` on Windows) for each translation unit. An object file contains:
- Machine code for every function **defined** in that translation unit.
- A **symbol table**: a list of names (functions, variables) that this file *provides* (defined here) and names it *needs* (called here but defined elsewhere — "undefined" symbols).

```sh
g++ -c main.cpp    # compile only — produces main.o
g++ -c utils.cpp   # produces utils.o
```

You can inspect the symbol table of an object file:

```sh
nm main.o          # list all symbols (U = undefined, T = defined in text section)
```

**Why declarations matter:** Since translation units are compiled in isolation, if `main.cpp` calls a function `void foo()` defined in `utils.cpp`, the compiler needs to know `foo` exists and what its signature is — but it doesn't need the full definition yet. A **declaration** is enough:

```cpp
// In main.cpp — just the declaration, not the definition
void foo();   // tells the compiler: foo exists, takes no args, returns void

int main() {
    foo();    // compiler trusts the declaration, leaves a placeholder
}
```

The actual definition (`void foo() { ... }`) lives in `utils.cpp` and will be resolved in the next phase.

You can see the assembly output the compiler produces:

```sh
g++ -S main.cpp    # produces main.s (assembly code)
```

### Phase 3 — Linking

The linker's job is **symbol resolution**: it takes all the `.o` files and resolves every placeholder left by the compiler.

```sh
g++ main.o utils.o -o program   # link both object files into one executable
```

When the linker sees that `main.o` calls `foo` but doesn't define it, it searches all the other `.o` files and libraries for a definition. If it finds exactly one, it patches in the address. If it finds zero or more than one, it errors.

**"Undefined reference to `foo`"** — the most common linker error. It means: the compiler accepted the declaration, but the linker couldn't find a definition anywhere. Causes:
- You forgot to compile or link the file that defines `foo`.
- You declared `foo` but never defined it.
- The library providing `foo` wasn't linked (`-lfoo` flag missing).

**The One Definition Rule (ODR):** You can **declare** something as many times as you want. You can **define** it exactly once across the entire program. Violating this gives "multiple definition of" errors.

```cpp
// Declaration — can appear in many files (put this in headers)
void foo();

// Definition — must appear in exactly ONE .cpp file
void foo() {
    // ...
}
```

**Static vs dynamic linking** (preview — elaborated in [[Part 8 — Systems Programming]]):
- **Static libraries** (`.a`): the linker copies the library code directly into your executable.
- **Dynamic libraries** (`.so` / `.dll`): the linker records a reference; the OS loads the library at runtime.

### Why Header Files Exist

Since each `.cpp` is compiled in isolation, there's no automatic way to share declarations between files. **Header files** are the solution: they contain declarations (not definitions) that every `.cpp` which needs them can `#include`.

```
utils.h       ← declarations: void foo(); int bar(int x);
utils.cpp     ← definitions:  void foo() { ... }  int bar(int x) { ... }
main.cpp      ← #include "utils.h" to get the declarations, links against utils.o
```

This is the fundamental pattern of C++ source organization. The `.h` file is the interface; the `.cpp` file is the implementation.

**Include what you use:** Only include headers you actually need. Every `#include` makes your translation unit larger and your build slower. Tools like `clang-tidy` can enforce this automatically.

### The Full Picture

```
main.cpp ──► [Preprocessor] ──► main.i ──► [Compiler] ──► main.o ──┐
utils.cpp ─► [Preprocessor] ──► utils.i ─► [Compiler] ──► utils.o ─┤
                                                                      ├──► [Linker] ──► program
                                                              libc.a ─┘
```

---

## 1.3 — Basic Types & Literals

C++ is a statically typed language — every variable has a type known at compile time, and the compiler enforces it. Understanding the built-in types and how literals work is foundational to everything else.

### The Integer Family

| Type | Minimum size | Typical size | Signed range (typical) |
|------|-------------|--------------|----------------------|
| `short` | 16 bits | 16 bits | −32,768 to 32,767 |
| `int` | 16 bits | **32 bits** | −2,147,483,648 to 2,147,483,647 |
| `long` | 32 bits | 32 or 64 bits | platform-dependent |
| `long long` | 64 bits | 64 bits | −9.2×10¹⁸ to 9.2×10¹⁸ |

**Critical fact:** The C++ standard only guarantees *minimum* sizes, not exact sizes. `int` is at least 16 bits — on every modern desktop/server platform it's 32, but you cannot rely on that in portable code. Always use fixed-width types when the exact size matters.

All of the above have `unsigned` variants (`unsigned int`, `unsigned long long`, etc.) which trade the negative range for double the positive range.

### Fixed-Width Integer Types

From `<cstdint>`. Use these whenever the exact bit width matters — binary file formats, network protocols, hardware interfaces, portable code:

```cpp
#include <cstdint>

int8_t   x = 127;          // exactly 8-bit signed
uint8_t  y = 255;          // exactly 8-bit unsigned
int16_t  a = 32767;
uint16_t b = 65535;
int32_t  c = 2'147'483'647;
uint64_t d = 18'446'744'073'709'551'615ULL;
```

Signed variants: `int8_t`, `int16_t`, `int32_t`, `int64_t`.
Unsigned variants: `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`.

### `size_t` and `ptrdiff_t`

`size_t` is the type returned by `sizeof` and used for container sizes (e.g. `std::vector::size()`). It is:
- Always **unsigned**
- Exactly large enough to represent any size on the current platform (32-bit on 32-bit systems, 64-bit on 64-bit systems)
- Defined in `<cstddef>`

```cpp
#include <cstddef>

size_t n = sizeof(int);   // 4 on most platforms
```

Mixing `size_t` with signed `int` is a very common source of bugs and warnings. When you compare `int i` with `vec.size()` (which is `size_t`), the `int` is converted to unsigned — if `i` is negative, it wraps to a huge number.

`ptrdiff_t` is the signed counterpart — the type of the difference between two pointers.

### The Floating-Point Family

| Type | Size | Precision | Typical use |
|------|------|-----------|-------------|
| `float` | 32 bits | ~7 decimal digits | Graphics, embedded (when memory matters) |
| `double` | 64 bits | ~15 decimal digits | General purpose — the default |
| `long double` | 80 or 128 bits | ~18–34 digits | High-precision scientific computation |

Floating-point literals default to `double`. Append a suffix to change:

```cpp
3.14        // double
3.14f       // float
3.14L       // long double
1e6         // double: 1,000,000.0
1.5e-3f     // float: 0.0015
```

### Character Types

| Type | Width | Use |
|------|-------|-----|
| `char` | 8 bits | Basic character — may be signed or unsigned (implementation-defined) |
| `unsigned char` | 8 bits | Raw byte — the standard "byte" type |
| `signed char` | 8 bits | Explicitly signed 8-bit integer |
| `wchar_t` | 16 or 32 bits | Wide chars — platform-dependent, avoid in new code |
| `char8_t` | 8 bits | UTF-8 code unit (C++20) |
| `char16_t` | 16 bits | UTF-16 code unit |
| `char32_t` | 32 bits | UTF-32 code unit / Unicode code point |

**Prefer `char` for text, `unsigned char` for raw bytes.** For Unicode in modern code, use `char8_t` with C++20 or just `char` with UTF-8 encoded strings.

### `bool`

Two values: `true` and `false`. Stored as at least one byte (not one bit). Implicitly converts to/from integers: `0` is `false`, anything non-zero is `true`.

```cpp
bool flag = true;
bool result = (x > 0);   // comparison produces bool
if (flag) { ... }
```

### `void`

Not a type you can instantiate — it means "no type" or "no value." Used for:
- Functions that return nothing: `void foo();`
- Generic pointers: `void*` (a pointer to anything — loses type information)

### Integer Literals

```cpp
42          // decimal
052         // octal (prefix 0) — equals 42
0x2A        // hexadecimal (prefix 0x) — equals 42
0b101010    // binary (prefix 0b, C++14) — equals 42
1'000'000   // digit separator (C++14) — just for readability, equals 1000000
```

Integer suffix modifiers:

```cpp
42u         // unsigned int
42l         // long
42ul        // unsigned long
42ll        // long long
42ull       // unsigned long long
```

### Floating-Point Literals

```cpp
3.14        // double
3.14f       // float
3.14L       // long double
.5          // double: 0.5
1.          // double: 1.0
1e6         // double: 1000000.0
1.5e-3f     // float: 0.0015
```

### Character Literals

```cpp
'a'         // char: lowercase a
'Z'         // char: uppercase Z
'\n'        // newline
'\t'        // tab
'\r'        // carriage return
'\0'        // null character (value 0)
'\\'        // backslash
'\''        // single quote
'\"'        // double quote
'\xFF'      // hex escape: character with value 255
'\101'      // octal escape: character with value 65 (= 'A')
```

### String Literals

```cpp
"hello"             // const char[6] — includes null terminator '\0'
"hello " "world"    // adjacent string literals are concatenated: "hello world"
R"(no \n escaping)" // raw string — backslashes are literal, C++11
R"delimiter(
  multi-line
  raw string
)delimiter"         // raw string with custom delimiter
u8"utf-8 text"      // UTF-8 string (char8_t[], C++20)
L"wide string"      // wchar_t[]
```

**String literals are null-terminated arrays**, not `std::string`. They are stored in read-only memory. `"hello"` has type `const char[6]` (5 chars + `'\0'`). For anything serious, use `std::string` (Part 4).

### Boolean Literals

```cpp
true
false
```

### The Null Pointer Literal

```cpp
nullptr     // C++11 — always use this
```

Never use `NULL` (which expands to `0`, an integer) or bare `0` for pointer contexts. `nullptr` has type `std::nullptr_t` and is unambiguous.

### Implicit Conversions — The Danger Zone

C++ performs implicit conversions silently. This is convenient but hides bugs.

**Integer promotion:** `char`, `short`, and their unsigned variants are promoted to `int` (or `unsigned int`) before any arithmetic. You almost never see `char` arithmetic actually operate on chars at the machine level.

```cpp
char a = 100, b = 100;
char c = a + b;   // a and b promoted to int, sum is 200, then truncated back to char
                  // char is 8-bit signed: 200 overflows → undefined behavior
```

**Usual arithmetic conversions:** When two different types meet in an expression, both are converted to a common type. The rules rank types by "width" and prefer the wider one.

**Signed + unsigned mixing — the most dangerous implicit conversion:**

```cpp
int  i = -1;
unsigned int u = 1;
if (i < u)   // i is converted to unsigned: -1 becomes 4294967295
    // this branch is NOT taken — 4294967295 is not less than 1
```

The compiler usually warns about this with `-Wall`. Always heed those warnings.

### `sizeof`

Returns the size in bytes of a type or expression. The result has type `size_t`. Evaluated entirely at compile time (with the exception of variable-length arrays, which are not in the standard).

```cpp
sizeof(int)         // 4 on most platforms
sizeof(double)      // 8
sizeof(char)        // always 1 (by definition)
sizeof("hello")     // 6 (5 chars + null terminator)

int arr[10];
sizeof(arr)         // 40 (10 * sizeof(int))
sizeof(arr) / sizeof(arr[0])  // 10 — classic array-size idiom
                              // replaced by std::size(arr) in C++17
```

---

## 1.4 — Type Qualifiers: const, volatile, mutable

Type qualifiers modify how a variable can be accessed or when it can be modified. `const` is used constantly. `volatile` is rare. `mutable` is a class-level tool.

### `const`

`const` makes a promise to the compiler: *"I will not modify this value through this name."* It does not necessarily mean the value is stored in read-only memory — it means this particular name cannot be used to change it.

#### Basic `const`

```cpp
const int x = 5;
x = 10;           // error: assignment of read-only variable
```

`const int x` and `int const x` are identical. The position relative to the type doesn't matter when there's no pointer involved.

#### `const` with Pointers — Read Right to Left

Pointer declarations with `const` are a common source of confusion. The rule: **read the declaration from right to left.**

```cpp
const int* p;       // pointer to const int — you can change p, but not *p
int* const p;       // const pointer to int — you cannot change p, but can change *p
const int* const p; // const pointer to const int — neither p nor *p can change
```

Breaking it down:
- `const int* p` — `*p` is `const int`: read-only. `p` itself can be reassigned to point elsewhere.
- `int* const p` — `p` is `const`: the pointer is fixed. `*p` can be modified.
- `const int* const p` — both are const.

```cpp
int a = 1, b = 2;

const int* p = &a;
*p = 10;     // error: *p is const
p = &b;      // OK: p itself is not const

int* const q = &a;
*q = 10;     // OK: *q is not const
q = &b;      // error: q is const

const int* const r = &a;
*r = 10;     // error
r = &b;      // error
```

#### `const` References

The most commonly used form of `const` in day-to-day code:

```cpp
void print(const std::string& s);   // takes s by const reference
```

Two benefits:
1. **No copy** — the reference binds directly to the caller's object.
2. **Binds to temporaries** — a `const T&` can bind to a temporary (rvalue), extending its lifetime to the reference's scope. A non-const reference cannot.

```cpp
const int& r = 42;    // OK — temporary 42 stays alive as long as r lives
int& r2 = 42;         // error — non-const reference cannot bind to a temporary
```

Every function parameter that you won't modify and that isn't trivially cheap to copy (i.e., anything larger than a register) should be `const T&`.

#### `const` Member Functions

A `const` member function promises not to modify the object it's called on:

```cpp
class Counter {
    int value = 0;
public:
    int get() const { return value; }   // const: won't modify *this
    void increment() { ++value; }       // non-const: modifies *this
};
```

Internally, the hidden `this` pointer changes type:
- In a non-const method: `this` is `Counter*`
- In a const method: `this` is `const Counter*`

You cannot call a non-const method on a const object:

```cpp
const Counter c;
c.get();        // OK: get() is const
c.increment();  // error: increment() is non-const, c is const
```

#### `const` Correctness

**Const correctness** means designing your APIs so that:
- Every method that doesn't modify the object is marked `const`
- Every parameter that won't be modified is taken as `const T&`

This matters because:
- It self-documents intent — callers know their data is safe.
- It enables passing const objects and temporaries.
- The compiler catches accidental mutations at compile time.
- It enables optimizations the compiler can make when it knows a value won't change.

Once you break const correctness in a type, the breakage spreads — callers that hold `const` references can no longer call your method, and they have to cast it away. Casting away const (`const_cast`) is almost always wrong and is a red flag in code review.

#### `constexpr` Preview

`constexpr` is a stronger version of `const` — it means "must be evaluated at compile time." It belongs to Part 5, but you will see it early and often:

```cpp
constexpr int size = 1024;           // compile-time constant
constexpr int square(int x) { return x * x; }  // compile-time function
```

---

### `volatile`

`volatile` tells the compiler: **do not optimize accesses to this variable — read it from memory every time, and write it to memory every time.**

Normally, the compiler is allowed to cache a variable in a register, reorder accesses, or eliminate what it thinks are redundant reads. `volatile` disables all of that for that variable.

```cpp
volatile int* reg = reinterpret_cast<volatile int*>(0x40021000);  // hardware register
*reg = 1;   // compiler MUST emit a write to that address
int x = *reg;   // compiler MUST emit a read from that address
```

**Use cases:**
- **Memory-mapped hardware registers** — in embedded/systems programming, writing to an address has a physical side effect (toggling a GPIO pin, sending data to a peripheral). The compiler must not eliminate or reorder these.
- **Signal handler communication** — a variable modified by a signal handler should be `volatile sig_atomic_t`.
- **Shared memory with non-C++ code** — in some platform-specific contexts.

**Critical misconception — `volatile` is NOT a threading primitive:**

This is probably the most common misuse of `volatile`. Many developers assume that because `volatile` forces memory reads/writes, it provides thread safety. It does not. `volatile` provides no memory ordering guarantees between threads. Two threads can still observe inconsistent values, and the compiler and CPU are still free to reorder `volatile` accesses relative to non-volatile ones.

For thread communication, use `std::atomic` (see [[Part 7 — Concurrency & Multithreading]]).

If you are writing application code and reach for `volatile`, stop and reconsider. It is almost never the right tool outside of embedded/systems code.

---

### `mutable`

`mutable` is the exception to `const` inside classes. It allows a specific data member to be modified even inside a `const` member function.

```cpp
class ExpensiveQuery {
    mutable std::string cached_result;
    mutable bool cache_valid = false;
    
public:
    std::string get() const {
        if (!cache_valid) {
            cached_result = compute();   // modifying mutable member — OK in const method
            cache_valid = true;
        }
        return cached_result;
    }
};
```

`get()` is `const` — from the outside, calling it doesn't change the observable state of the object. But internally it updates a cache. `mutable` allows this.

**Common use cases:**
- **Lazy / cached computation** — compute a value the first time it's needed, cache it for subsequent calls.
- **Mutex members** — a `const` getter still needs to lock a mutex before reading. The mutex itself must be `mutable`.
- **Access counters and loggers** — recording how many times a `const` method was called.

```cpp
class ThreadSafeCounter {
    mutable std::mutex mtx;
    int value = 0;
public:
    int get() const {
        std::lock_guard<std::mutex> lock(mtx);  // mtx is mutable — OK
        return value;
    }
};
```

#### Logical Const vs Bitwise Const

This is the concept `mutable` formalizes:

- **Bitwise const**: not a single byte of the object changes. The compiler's default view of `const`.
- **Logical const**: the object's *observable, meaningful state* doesn't change, even if some internal implementation detail (a cache, a mutex, a counter) does.

`mutable` lets you implement logical const while allowing the implementation to update internal bookkeeping. Use it only for implementation details that callers should not know or care about — never for fields that are part of the object's logical state.
