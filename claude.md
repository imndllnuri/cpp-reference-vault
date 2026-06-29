# Claude Context — CPP Reference Wiki

## What This Project Is

A complete personal C++ reference wiki built in Obsidian, mirrored to GitHub at `git@github.com:imndllnuri/cpp-reference-vault.git`. It covers everything from compiler toolchains to algorithms — written as a self-contained reference so nothing needs to be looked up elsewhere.

## File Naming Conventions

- Reference files: `Part N — Title.md` (em dash —, not hyphen)
- Exercise files: `Exercises — Part N — Title.md`
- Obsidian cross-file links: `[[Part N — Title]]`
- Obsidian same-file anchor links: `[[#Heading Text]]`

## Heading Hierarchy

```
#    — page title
##   — main section (e.g., ## Section 5.3 — Lambdas)
###  — subsection
```

## Reference Files (Parts 0–10)

All 11 reference files are complete.

| Part | Title | Sections |
|------|-------|----------|
| 0 | Environment & Toolchain | Compilers, Make/CMake, GDB, vcpkg, sanitizers |
| 1 | C++ Fundamentals | History, compilation model, types, qualifiers |
| 2 | Object-Oriented Programming | Classes, constructors, inheritance, polymorphism, rule of five |
| 3 | Memory Management | Stack/heap, RAII, smart pointers, allocators |
| 4 | Standard Template Library | Containers, iterators, algorithms, string, span, ranges |
| 5 | Modern C++ (C++11–C++23) | Move semantics, lambdas, constexpr, concepts, modules, coroutines |
| 6 | Templates & Metaprogramming | Function/class templates, SFINAE, type traits, CRTP, policy-based |
| 7 | Concurrency & Multithreading | Threads, mutexes, atomics, futures, thread pools, pitfalls |
| 8 | Systems Programming | POSIX, fork/exec, signals, IPC, sockets, mmap, linking |
| 9 | Design Patterns | SOLID, GoF patterns, PIMPL, type erasure, NVI |
| 10 | Data Structures & Algorithms | Complexity, DS, sorting, graphs, DP, backtracking, bits |

## Exercise Files (Parts 0–10)

All 11 exercise files are complete. One file per part, mirroring the section numbers of the reference.

**Exercise format:**

```markdown
**Ex N.M.K** `[Easy/Medium/Hard]`

Problem statement in plain text.

```cpp
// starter or broken code (when relevant)
```

```cpp
// your solution
```
```

**Rules:** No hints, no answers. The user writes solutions in separate `.cpp` files.

**Exercise types used:**
- Trace — "what does this print / what is UB?"
- Debug — broken code to find and fix
- Implement — write a function or class from scratch
- Explain — short written answer
- Design — larger open-ended problem

## Key Content Details (for future reference)

- Part 5 sections 5.1–5.14 cover every C++ standard from C++11 to C++23
- Part 6 has a full SFINAE→Concepts migration section (6.6 → 6.9)
- Part 7 includes a complete thread pool implementation and memory order table
- Part 8 includes a full TCP server/client with epoll
- Part 9 includes type erasure from scratch (Concept/Model pattern)
- Part 10 has bitmask DP (TSP), O(n log n) LIS, and Floyd's cycle detection

## GitHub Remote

```
git@github.com:imndllnuri/cpp-reference-vault.git
```

The user commits each part individually with descriptive messages.
