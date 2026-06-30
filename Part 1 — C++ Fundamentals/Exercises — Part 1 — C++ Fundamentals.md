# Exercises — Part 1 — C++ Fundamentals

> Practice problems for [[Part 1 — C++ Fundamentals]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 1.1 — History & Design Philosophy

**Ex 1.1.1** `[Easy]`

Name the two core design principles Bjarne Stroustrup built C++ around. For each, give one concrete example of how the language enforces or reflects it.

```
// your answer
```

---

**Ex 1.1.2** `[Easy]`

Place these C++ standards in chronological order and name one major feature introduced in each: C++17, C++11, C++03, C++20, C++14.

```
// your answer
```

---

**Ex 1.1.3** `[Medium]`

C++ is often described as a "multi-paradigm" language. Name the three paradigms it supports and give one example of a standard library feature that exemplifies each.

```
// your answer
```

---

## Section 1.2 — The Compilation Pipeline

**Ex 1.2.1** `[Easy]`

Trace what happens step-by-step when this file is compiled. Specifically: what does the preprocessor do, what does the compiler produce, and what does the linker need from `math.cpp`?

```cpp
// main.cpp
#include "math.h"
#define SCALE 2

int main() {
    int result = add(3, 4) * SCALE;
    return result;
}
```

```
// your answer
```

---

**Ex 1.2.2** `[Medium]`

What is the One Definition Rule (ODR)? This program has two translation units. Will it compile and link? If not, why not? If yes, is the behavior well-defined?

```cpp
// a.cpp
int value = 42;
int get() { return value; }

// b.cpp
int value = 99;
int main() { return 0; }
```

```
// your answer
```

---

**Ex 1.2.3** `[Medium]`

What is the difference between a **declaration** and a **definition**? For each of the following, state whether it is a declaration, a definition, or both:

```cpp
extern int x;
int x;
int x = 5;
void foo();
void foo() {}
class Bar;
class Bar { int val; };
```

```
// your answer
```

---

**Ex 1.2.4** `[Hard]`

Explain what a **translation unit** is. A project has `a.cpp`, `b.cpp`, and a shared header `shared.h` that defines an `inline` function `helper()`. How many translation units are there? Is `helper()` defined once or multiple times? Why doesn't this violate the ODR?

```
// your answer
```

---

## Section 1.3 — Types & Type System

**Ex 1.3.1** `[Easy]`

What are the guaranteed minimum sizes (in bits) of `char`, `short`, `int`, `long`, and `long long` according to the C++ standard? Which fixed-width types from `<cstdint>` would you use when you need exactly 8, 16, 32, and 64 bits?

```
// your answer
```

---

**Ex 1.3.2** `[Easy]`

Write the C++ literal for each of the following. Use the appropriate suffix where needed:
- Integer 255 in hexadecimal
- Integer 255 in binary (C++14)
- `long long` value one million
- `unsigned int` 42
- `float` 3.14
- `double` 3.14
- A raw string literal containing `C:\Users\name`

```cpp
// your solution
```

---

**Ex 1.3.3** `[Medium]`

What does this program print? Explain each line.

```cpp
#include <iostream>
int main() {
    std::cout << sizeof(char)   << "\n";
    std::cout << sizeof(true)   << "\n";
    std::cout << sizeof(3.14f)  << "\n";
    std::cout << sizeof(3.14)   << "\n";
    std::cout << sizeof(nullptr)<< "\n";

    unsigned int a = 0;
    a -= 1;
    std::cout << a << "\n";

    int b = INT_MIN;
    std::cout << (b < 0) << "\n";
    b = -b;
    std::cout << b << "\n";   // what happens here?
}
```

```
// your answer
```

---

**Ex 1.3.4** `[Medium]`

Implement a function `to_hex_string(uint32_t n)` that returns the hex representation of `n` as a `std::string` (e.g., `255` → `"0xff"`). Do not use `printf`, `std::format`, or any formatting library — only arithmetic and string operations.

```cpp
#include <string>

std::string to_hex_string(uint32_t n) {
    // your solution
}
```

---

**Ex 1.3.5** `[Hard]`

Explain why comparing floating-point numbers with `==` is almost always wrong. Write a function `approx_equal(double a, double b, double epsilon)` that correctly handles the general case, including values near zero and very large values. What is the difference between absolute epsilon and relative epsilon comparisons?

```cpp
bool approx_equal(double a, double b, double epsilon) {
    // your solution
}
```

---

## Section 1.4 — Type Qualifiers: `const`, `volatile`, `mutable`

**Ex 1.4.1** `[Easy]`

Declare the following (read right-to-left):
1. A pointer to a constant int
2. A constant pointer to an int
3. A constant pointer to a constant int
4. A function taking a const reference to string and returning void

```cpp
// your solution
```

---

**Ex 1.4.2** `[Medium]`

Which of these lines compile? For those that don't, explain why.

```cpp
int x = 5;
const int cx = 5;

int* p1 = &x;
int* p2 = &cx;
const int* p3 = &x;
const int* p4 = &cx;
int* const p5 = &x;
int* const p6 = &cx;

*p1 = 10;
*p3 = 10;
p3  = &x;
p5  = &x;
```

```
// your answer — which lines fail and why
```

---

**Ex 1.4.3** `[Medium]`

This class has a bug. Find it, explain why it's a bug, and fix it using `mutable`.

```cpp
class TextProcessor {
    std::string text_;
    mutable int word_count_ = -1;  // -1 means not yet computed
public:
    explicit TextProcessor(std::string t) : text_(std::move(t)) {}

    int word_count() {              // <-- bug is here
        if (word_count_ == -1) {
            word_count_ = count_words(text_);
        }
        return word_count_;
    }
private:
    static int count_words(const std::string& s);
};

void print_stats(const TextProcessor& tp) {
    std::cout << tp.word_count();   // fails to compile — why?
}
```

```cpp
// your fixed version
```

---

**Ex 1.4.4** `[Medium]`

Explain: is `volatile` useful for thread synchronization in modern C++? Write a code example showing what problem `volatile` was historically misused to solve, then show the correct modern solution using `std::atomic`.

```cpp
// your solution — show the wrong way and the right way
```

---

**Ex 1.4.5** `[Hard]`

Implement a `const`-correct `Stack<T>` class with: `push`, `pop`, `top` (non-const and const overloads), `size`, and `empty`. Ensure that `const Stack<T>` objects can call `top`, `size`, and `empty` but not `push` or `pop`.

```cpp
template<typename T>
class Stack {
    // your solution
};
```
