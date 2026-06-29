# Exercises — Part 0 — Environment & Toolchain

> Practice problems for [[Part 0 — Environment & Toolchain]]. Write your solutions in `.cpp` files or shell scripts. No hints, no answers.

---

## Section 0.1 — Compilers: GCC, Clang, MSVC

**Ex 0.1.1** `[Easy]`

Write the exact `clang++` command to compile `main.cpp` with: C++23 standard, all common warnings enabled, extra warnings enabled, warnings treated as errors, and debug symbols. Name the output binary `app`.

```cpp
// your solution (shell command)
```

---

**Ex 0.1.2** `[Easy]`

What are the four stages of the C++ compilation pipeline? For a file `util.cpp`, write the `g++` commands to:
1. Stop after preprocessing and save output to `util.i`
2. Stop after compilation (assembly) and save to `util.s`
3. Stop after assembling (produce object file) `util.o`
4. Link `main.o` and `util.o` into an executable named `program`

```cpp
// your solution (shell commands)
```

---

**Ex 0.1.3** `[Medium]`

Given this code, which compiler flag(s) would catch each problem at compile time? Name the flag and the compiler (GCC or Clang).

```cpp
int foo() {
    int x;
    return x;           // problem A
}

void bar(int n) {
    int arr[n];         // problem B — not standard C++
    (void)arr;
}

int baz(bool flag) {
    if (flag) return 1;
                        // problem C — missing return
}
```

```cpp
// your solution
```

---

**Ex 0.1.4** `[Medium]`

Explain the difference between `-O0`, `-O2`, and `-O3`. Why should you NOT use `-O2` or `-O3` during development when debugging with GDB? What flag combination should you use during development?

```
// your answer
```

---

## Section 0.2 — Build Systems: Make, CMake

**Ex 0.2.1** `[Easy]`

Write a `Makefile` for a project with these files: `main.cpp`, `parser.cpp`, `lexer.cpp`. The executable should be named `compiler`. Each `.cpp` must compile to its own `.o` separately. Include a `clean` target.

```makefile
# your solution
```

---

**Ex 0.2.2** `[Medium]`

Write a `CMakeLists.txt` for a project that:
- Requires CMake 3.20+
- Is named `MyParser`
- Uses C++20
- Builds an executable `parser` from `main.cpp`, `parser.cpp`, `lexer.cpp`
- Links against the `fmt` library (found via `find_package`)
- Enables all warnings

```cmake
# your solution
```

---

**Ex 0.2.3** `[Medium]`

What is the purpose of the `build/` directory in a CMake workflow? Write the exact sequence of shell commands to configure, build, and run a CMake project from its root directory, placing all build artifacts in `build/`.

```sh
# your solution
```

---

**Ex 0.2.4** `[Hard]`

Write a `CMakeLists.txt` for a project with a static library `libutils` (from `utils.cpp` and `utils.h`) and an executable `main` that links against it. The library's include directory should be automatically propagated to any target that links it using `target_include_directories` with `PUBLIC`.

```cmake
# your solution
```

---

## Section 0.3 — Debuggers: GDB, LLDB

**Ex 0.3.1** `[Easy]`

List the GDB commands needed to:
1. Set a breakpoint at line 42 of `main.cpp`
2. Run the program
3. Print the value of variable `total`
4. Step into the next function call
5. Continue to the next breakpoint
6. Show the current call stack

```
// your solution
```

---

**Ex 0.3.2** `[Medium]`

This program crashes. Without running it, identify the likely cause. Then write the GDB session (commands and expected output) you would use to confirm it.

```cpp
#include <vector>
int main() {
    std::vector<int> v = {1, 2, 3};
    return v[10];
}
```

```
// your solution
```

---

**Ex 0.3.3** `[Medium]`

What does the `-g` flag do and why is it essential for debugging? What happens to binary size and runtime performance when `-g` is used? Should it be combined with `-O2`?

```
// your answer
```

---

## Section 0.4 — Package Managers: vcpkg, Conan

**Ex 0.4.1** `[Easy]`

Write the vcpkg command to install the `catch2` testing library. Then write the `CMakeLists.txt` snippet (just the relevant lines) to find and link `catch2` in a project that already has `add_executable(tests test_main.cpp)`.

```sh
# your solution
```

---

**Ex 0.4.2** `[Medium]`

What is the CMake toolchain file for vcpkg and why must it be passed at configure time (not inside `CMakeLists.txt`)? Write the full `cmake` configure command that uses it, assuming vcpkg is installed at `~/vcpkg`.

```sh
# your solution
```

---

## Section 0.5 — Sanitizers

**Ex 0.5.1** `[Easy]`

Write the `clang++` command to compile `main.cpp` with both AddressSanitizer and UndefinedBehaviorSanitizer enabled. Include debug symbols. Why should you NOT use `-O2` with sanitizers during debugging?

```sh
# your solution
```

---

**Ex 0.5.2** `[Medium]`

Each snippet below contains a bug. Which sanitizer (ASan, UBSan, or TSan) would catch it? Describe what the sanitizer output would say.

```cpp
// Snippet A
int arr[5];
arr[5] = 42;

// Snippet B
int x = INT_MAX;
x = x + 1;

// Snippet C — two threads
int counter = 0;
// thread 1: counter++;
// thread 2: counter++;
```

```
// your solution
```

---

**Ex 0.5.3** `[Hard]`

Write a small C++ program (15–30 lines) that contains a use-after-free bug. It must compile cleanly with no warnings under `-Wall -Wextra` but be caught immediately by ASan. Show the program and describe the expected ASan output.

```cpp
// your solution
```
