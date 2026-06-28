 ## =====================================
 ## C++ MASTERY REFERENCE
 ## From Fundamentals to Production
 ## =====================================

PART 0 — ENVIRONMENT & TOOLCHAIN
  0.1  Compilers: GCC, Clang, MSVC — differences and flags
  0.2  Build Systems: Make, CMake
  0.3  Debuggers: GDB, LLDB
  0.4  Package Managers: vcpkg, Conan
  0.7  Sanitizers: ASan, UBSan, ThreadSanitizer

PART 1 — C++ FUNDAMENTALS
  1.1  History & Philosophy of C++
  1.2  The Compilation Model: preprocessing → compilation → linking
  1.3  Basic Types & Literals (int, float, char, bool, size_t, etc.)
  1.4  Type Qualifiers: const, volatile, mutable
  1.5  Variables, Scope & Storage Duration
  1.6  Operators & Expressions
  1.7  Control Flow: if, switch, for, while, do-while
  1.8  Functions: declaration, definition, overloading, inline
  1.9  Arrays & C-style Strings
  1.10 Pointers & References
  1.11 Namespaces


PART 2 — OBJECT-ORIENTED PROGRAMMING
  2.1  Classes vs Structs
  2.2  Constructors & Destructors
  2.3  Access Control: public, protected, private
  2.4  The `this` Pointer
  2.5  Static Members
  2.6  Operator Overloading
  2.7  Inheritance & the is-a Relationship
  2.8  Polymorphism & Virtual Functions
  2.9  Abstract Classes & Pure Virtual Functions
  2.10 Multiple Inheritance & Diamond Problem
  2.11 Copy Constructor & Copy Assignment
  2.12 Move Constructor & Move Assignment (Rule of 5)


PART 3 — MEMORY MANAGEMENT
  3.1  Stack vs Heap — How They Work
  3.2  new and delete — Raw Allocation
  3.3  RAII: Resource Acquisition Is Initialization
  3.4  std::unique_ptr
  3.5  std::shared_ptr & Reference Counting
  3.6  std::weak_ptr — Breaking Cycles
  3.7  Memory Leaks, Dangling Pointers, Double Free
  3.8  Tools: Valgrind, AddressSanitizer
  3.9  Placement new & Custom Allocators


PART 4 — STANDARD TEMPLATE LIBRARY (STL)
  4.1  Design Philosophy of the STL
  4.2  Sequence Containers: vector, array, deque, list, forward_list
  4.3  Associative Containers: set, map, multiset, multimap
  4.4  Unordered Containers: unordered_map, unordered_set
  4.5  Container Adaptors: stack, queue, priority_queue
  4.6  Iterators: categories, traits, custom iterators
  4.7  Algorithms: sorting, searching, transforming, partitioning
  4.8  Functors & std::function
  4.9  std::string & std::string_view
  4.10 std::span (C++20)
  4.11 Ranges (C++20): views, adaptors, lazy evaluation


PART 5 — MODERN C++ (C++11 TO C++23)
  5.1  C++11: auto, range-for, nullptr, uniform initialization
  5.2  C++11: Rvalue References & Move Semantics
  5.3  C++11: Lambdas
  5.4  C++11: constexpr, static_assert, noexcept
  5.5  C++11: Threading primitives (std::thread, std::mutex)
  5.6  C++14: Generic Lambdas, Variable Templates, std::make_unique
  5.7  C++17: Structured Bindings, if constexpr, fold expressions
  5.8  C++17: std::optional, std::variant, std::any
  5.9  C++17: std::filesystem, std::string_view
  5.10 C++20: Concepts & Constraints
  5.11 C++20: Modules
  5.12 C++20: Coroutines
  5.13 C++20: std::format, std::span, Ranges
  5.14 C++23: std::expected, std::print, flat_map, and more


PART 6 — TEMPLATES & METAPROGRAMMING
  6.1  Function Templates
  6.2  Class Templates
  6.3  Template Specialization: Full & Partial
  6.4  Variadic Templates & Parameter Packs
  6.5  Template Argument Deduction
  6.6  SFINAE: Substitution Failure Is Not An Error
  6.7  Type Traits (<type_traits>)
  6.8  if constexpr in Templates
  6.9  Concepts (C++20) — Replacing SFINAE
  6.10 consteval & Compile-Time Computation
  6.11 CRTP: Curiously Recurring Template Pattern
  6.12 Policy-Based Design


PART 7 — CONCURRENCY & MULTITHREADING
  7.1  Why Concurrency? Problems it Solves
  7.2  std::thread — Creating & Joining
  7.3  Data Races & Undefined Behavior
  7.4  std::mutex, std::lock_guard, std::unique_lock
  7.5  Condition Variables
  7.6  std::atomic & Memory Order
  7.7  std::future, std::promise, std::async
  7.8  Thread Pools
  7.9  Coroutines (C++20): co_await, co_yield, co_return
  7.10 Common Pitfalls: Deadlock, Livelock, Starvation


PART 8 — SYSTEMS PROGRAMMING
  8.1  File I/O: C-style and C++ streams
  8.2  POSIX API Overview
  8.3  Processes: fork, exec, wait
  8.4  Signals
  8.5  IPC: Pipes, Shared Memory, Message Queues
  8.6  Sockets: TCP/UDP networking basics
  8.7  Memory-Mapped Files
  8.8  Linking: Static vs Dynamic Libraries


PART 9 — DESIGN PATTERNS
  9.1  SOLID Principles
  9.2  Creational Patterns: Singleton, Factory, Builder, Prototype
  9.3  Structural Patterns: Adapter, Decorator, Proxy, Facade, Composite
  9.4  Behavioral Patterns: Observer, Strategy, Command, Iterator, State
  9.5  C++ Idioms: PIMPL, CRTP, Type Erasure, NVI


PART 10 — DATA STRUCTURES & ALGORITHMS
  10.1  Complexity Analysis: Big-O, Big-Theta, Big-Omega
  10.2  Arrays & Strings
  10.3  Linked Lists: Singly, Doubly, Circular
  10.4  Stacks & Queues
  10.5  Hash Tables — Internals
  10.6  Trees: BST, AVL, Red-Black, Trie
  10.7  Heaps & Priority Queues
  10.8  Graphs: Representation, BFS, DFS
  10.9  Graph Algorithms: Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim
  10.10 Sorting: Bubble, Merge, Quick, Heap, Radix, Tim
  10.11 Searching: Binary Search & variants
  10.12 Dynamic Programming: Memoization vs Tabulation
  10.13 Greedy Algorithms
  10.14 Backtracking
  10.15 Bit Manipulation
