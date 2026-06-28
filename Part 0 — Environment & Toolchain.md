# Environment and Toolchain

> Before writing any meaningful C++ code, you need to know how to compile it, build it, debug it, manage its dependencies, and verify its correctness. This file covers all of those tools.

## Table of Contents

- [[#0.1 — Compilers GCC, Clang, MSVC]]
- [[#0.2 — Build Systems Make, CMake]]
- [[#0.3 — Debuggers GDB, LLDB]]
- [[#0.4 — Package Managers vcpkg, Conan]]
- [[#0.7 — Sanitizers]]

---

## 0.1 — Compilers: GCC, Clang, MSVC

A compiler translates your `.cpp` source code into machine code the CPU can execute. This happens in several stages: **preprocessing** (expanding macros and `#include` directives) → **compilation** (source to assembly) → **assembly** (assembly to object files `.o`) → **linking** (combining object files into a final executable).

There are three major C++ compilers:

### GCC (GNU Compiler Collection)

- The standard compiler on Linux. Command: `g++`
- Very mature, excellent optimization, best support for the latest C++ standards on Linux.
- Default on most distros — install with: `sudo apt install g++`

### Clang

- Built by Apple/LLVM. Command: `clang++`
- Produces much better error messages than GCC — this matters a lot when learning.
- Also powers static analysis tools: `clang-tidy` and `clang-format`
- Install: `sudo apt install clang`

### MSVC (Microsoft Visual C++)

- Windows only — ships with Visual Studio.
- The standard for Windows development. Not relevant on Linux.

### Why does the choice of compiler matter?

They all implement the same C++ standard, but they differ in:

- **Error message quality** — Clang's errors are far more readable when starting out.
- **Warning coverage** — Some warnings only surface in certain compilers.
- **Standard feature support** — Not all compilers support all C++20/23 features at the same time; GCC and Clang tend to be ahead of MSVC.
- **Platform tooling** — MSVC integrates tightly with the Windows SDK and COM.

### Basic Usage and Common Flags

```sh
g++     -std=c++23 -Wall -Wextra -o output main.cpp
clang++ -std=c++23 -Wall -Wextra -o output main.cpp
```

| Flag | Meaning |
|------|---------|
| `-std=c++23` | Use the C++23 standard. Also valid: `c++17`, `c++20` |
| `-Wall` | Enable all common warnings |
| `-Wextra` | Enable extra warnings beyond `-Wall` |
| `-o output` | Name the output binary `output` |
| `-g` | Embed debug symbols — needed for GDB/LLDB (see [[#0.3 — Debuggers GDB, LLDB]]) |
| `-O2` | Optimize for speed — use in release builds, not during development |
| `-c` | Compile only, do not link — produces a `.o` object file |

> **Always use `-Wall -Wextra` while developing.** Warnings are the compiler catching bugs for you at zero cost. Treating them as errors (`-Werror`) is common in production codebases.

---

## 0.2 — Build Systems: Make, CMake

When your project is a single file, you can compile directly with `g++`. When it grows to 10, 50, or 500 files, you need a build system to track which files have changed and only recompile what is necessary.

### Make

The oldest Unix build tool. You write a `Makefile` that describes **targets** (what to produce), **dependencies** (what each target needs), and **recipes** (the commands to build it). Make compares file modification timestamps and only rebuilds targets whose dependencies have changed.

```makefile
main: main.o utils.o
	g++ -o main main.o utils.o

main.o: main.cpp
	g++ -c main.cpp

utils.o: utils.cpp
	g++ -c utils.cpp
```

Run it: just type `make` — Make reads the `Makefile` in the current directory and builds the first target.

Low-level and verbose, but Make is still used everywhere — and you will encounter it in legacy codebases and generated build files regularly.

### CMake

The modern standard for cross-platform C++ projects. You don't write build commands directly — you write a description of your project in `CMakeLists.txt`, and CMake **generates** the actual build files for you (Makefiles, Ninja files, Visual Studio projects, etc.) depending on your platform.

**Why CMake?**
- The same `CMakeLists.txt` works on Linux, Mac, and Windows.
- Integrates directly with package managers like vcpkg and Conan (see [[#0.4 — Package Managers vcpkg, Conan]]).
- Used by virtually every serious open-source C++ project.

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyProject)
set(CMAKE_CXX_STANDARD 23)
add_executable(main main.cpp utils.cpp)
```

**Workflow:**

```sh
mkdir build && cd build
cmake ..    # read CMakeLists.txt and generate build files
make        # actually compile the project
```

The `build/` directory keeps all generated files out of your source tree. Never commit it to version control.

> **Rule of thumb:** Use CMake for any project with more than one source file. Understanding Make is still valuable because CMake generates it, and you will read Makefiles in the wild constantly.

---

## 0.3 — Debuggers: GDB, LLDB

A debugger lets you pause program execution, inspect variables, step through code line by line, and find exactly where and why something goes wrong. **`printf` debugging is a last resort** — a debugger gives you the full picture in seconds.

> Always compile with `-g` when debugging. Without debug symbols, the debugger cannot map machine instructions back to your source lines. See the flag table in [[#0.1 — Compilers GCC, Clang, MSVC]].

### GDB (GNU Debugger)

Pairs with GCC. Works on any binary compiled with debug symbols (`-g`). Command-line tool — powerful, but with a noticeable learning curve.

```sh
g++ -g -o main main.cpp   # compile with debug symbols
gdb ./main                # launch the debugger
```

**Key GDB commands:**

| Command | What it does |
|---------|-------------|
| `break main` | Set a breakpoint at the start of `main` |
| `break file.cpp:42` | Set a breakpoint at line 42 of `file.cpp` |
| `run` | Start the program |
| `next` | Step over — execute the next line without entering function calls |
| `step` | Step into — enter the called function |
| `print x` | Print the current value of variable `x` |
| `backtrace` | Show the full call stack at the current point |
| `continue` | Resume execution until the next breakpoint |
| `quit` | Exit GDB |

### LLDB

Pairs with Clang. Same concept, slightly different syntax. Generally preferred on Mac; on Linux it is comparable to GDB.

```sh
clang++ -g -o main main.cpp
lldb ./main
```

**Key LLDB commands:**

| Command | GDB equivalent |
|---------|---------------|
| `breakpoint set --name main` | `break main` |
| `process launch` | `run` |
| `thread step-over` | `next` |
| `thread step-in` | `step` |
| `frame variable x` | `print x` |
| `thread backtrace` | `backtrace` |

> **For release builds:** strip debug symbols and enable optimization — `clang++ -std=c++23 -O2 -o main main.cpp`. Debug symbols significantly increase binary size and slightly affect performance.

---

## 0.4 — Package Managers: vcpkg, Conan

C++ has no built-in package manager (unlike Python's `pip` or Node's `npm`). These tools fill that gap — they download, build, and make third-party libraries available to your project automatically.

### vcpkg

Made by Microsoft, now fully cross-platform. Integrates directly with CMake via a toolchain file.

```sh
vcpkg install fmt        # rich string formatting
vcpkg install catch2     # unit testing framework
vcpkg install spdlog     # fast logging library
vcpkg install boost      # massive general-purpose library collection
```

After installation, CMake can find the library automatically when you pass the vcpkg toolchain file at configure time:

```sh
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Simple, well-documented, and the right choice to start with.

### Conan

More powerful — designed for professional and enterprise use. Supports strict version pinning, multiple build configurations (debug/release/asan), and complex dependency graphs. Steeper learning curve. Come back to it when vcpkg is not enough.

> **For now:** vcpkg is sufficient. When we reach later sections on the STL and real projects, we will pull in libraries like `{fmt}`, `Catch2`, `spdlog`, and `Boost`. That is when this section becomes essential. Sanitizers (see [[#0.7 — Sanitizers]]) integrate naturally into the build configurations managed by these tools.

---

## 0.5 — Sanitizers

Sanitizers are compiler instruments that inject runtime checks into your binary to catch bugs that would otherwise be invisible — or silently trigger undefined behavior that only manifests as a crash in production.

They work at compile time: the compiler adds extra instrumentation around every memory access, pointer dereference, integer operation, etc. The resulting binary checks itself as it runs and immediately reports violations with precise diagnostics, including the exact source line.

### AddressSanitizer (ASan)

Catches memory safety errors:
- Buffer overflows (reading or writing past array bounds)
- Use-after-free (accessing memory after `delete`)
- Use-after-return (returning a pointer to a local variable)
- Memory leaks

```sh
clang++ -std=c++23 -g -fsanitize=address -Wall -Wextra -o main main.cpp
```

### UndefinedBehaviorSanitizer (UBSan)

Catches undefined behavior — operations the C++ standard explicitly leaves undefined, which compilers are allowed to handle in any way (and optimizers often exploit to produce surprising results):
- Signed integer overflow
- Null pointer dereference
- Invalid type casts (`static_cast` to the wrong type)
- Out-of-bounds array access

```sh
clang++ -std=c++23 -g -fsanitize=undefined -Wall -Wextra -o main main.cpp
```

### ThreadSanitizer (TSan)

Catches data races — two threads accessing the same memory concurrently without proper synchronization. These bugs are notoriously hard to reproduce and diagnose without a tool like this.

```sh
clang++ -std=c++23 -g -fsanitize=thread -Wall -Wextra -o main main.cpp
```

> **Cannot be combined with ASan.** Use one or the other depending on what you are investigating.

### Combining Sanitizers

ASan and UBSan can be combined safely and should be your default development build:

```sh
# Development build — use every time
clang++ -std=c++23 -g -fsanitize=address,undefined -Wall -Wextra -o main main.cpp

# Release build — strip sanitizers, enable optimization
clang++ -std=c++23 -O2 -o main main.cpp
```

> **Key point:** Sanitizers add roughly 2x runtime overhead and 2x memory usage. Use them constantly while writing and testing code — disable them only for release. They catch bugs that would otherwise appear silently in production, and catching them early is orders of magnitude cheaper than debugging a live failure. See also the compiler flags section in [[#0.1 — Compilers GCC, Clang, MSVC]].
