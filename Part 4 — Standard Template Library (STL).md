# Part 4 — Standard Template Library (STL)

> The STL is the backbone of everyday C++ code. Understanding its design — and the contracts between containers, iterators, and algorithms — lets you compose powerful solutions from simple, well-tested building blocks without reinventing anything.

## Table of Contents

- [[#4.1 — Design Philosophy of the STL]]
- [[#4.2 — Sequence Containers: vector, array, deque, list, forward_list]]
- [[#4.3 — Associative Containers: set, map, multiset, multimap]]
- [[#4.4 — Unordered Containers: unordered_map, unordered_set]]
- [[#4.5 — Container Adaptors: stack, queue, priority_queue]]
- [[#4.6 — Iterators: categories, traits, custom iterators]]
- [[#4.7 — Algorithms: sorting, searching, transforming, partitioning]]
- [[#4.8 — Functors & std::function]]
- [[#4.9 — std::string & std::string_view]]
- [[#4.10 — std::span (C++20)]]
- [[#4.11 — Ranges (C++20): views, adaptors, lazy evaluation]]

---

## 4.1 — Design Philosophy of the STL

### Origin

The STL was designed by **Alexander Stepanov** at HP in the early 1990s and standardized in C++98. Stepanov's core insight was that algorithms and data structures should be **decoupled** — an algorithm like sort shouldn't need to know what container it's sorting, and a container shouldn't need to know which algorithm will be applied to it.

This decoupling is achieved through **iterators**: a common interface that containers expose and algorithms consume.

### The Three Pillars

```
Containers ──► Iterators ──► Algorithms
```

- **Containers** store and own elements. They expose iterators.
- **Iterators** provide a uniform traversal interface. They abstract away the container's internal structure.
- **Algorithms** operate on iterator ranges. They know nothing about the container.

Because of this design, `std::sort` works on a `vector`, a `deque`, a raw array, or any other contiguous range — without a single line of `std::sort` changing. Adding a new container that exposes correct iterators gives it access to the entire algorithm library instantly.

### Generic Programming — No Virtual Functions

The STL achieves zero-overhead abstraction through **templates**, not inheritance. There are no virtual function calls in the STL's hot paths. An `std::sort` on a `vector<int>` compiles to code that is as fast as a hand-written sort on an array of ints — the type information is erased at compile time, not runtime.

This is the STL's application of the zero-overhead principle from [[Part 1 — C++ Fundamentals]].

### Value Semantics

STL containers **own their elements by value**. When you insert into a `vector<std::string>`, the string is copied (or moved) in. When the vector is destroyed, all its elements are destroyed. This is different from Java, where collections hold references to heap-allocated objects.

Value semantics mean containers work naturally with RAII types. A `std::vector<std::unique_ptr<Foo>>` correctly destroys all its `Foo` objects when the vector is destroyed.

### Half-Open Ranges

All STL algorithms operate on **half-open ranges** `[first, last)` — the range includes `first` but excludes `last`. `end()` points one past the last element.

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
//                     ^              ^
//                   begin()        end() — one past last

std::sort(v.begin(), v.end());   // sort [begin, end)
```

This convention makes empty ranges natural (`begin == end`), enables simple loop termination, and allows subrange expressions like `[begin, begin + 3)` for the first three elements.

---

## 4.2 — Sequence Containers: `vector`, `array`, `deque`, `list`, `forward_list`

Sequence containers maintain elements in a specific order that you control.

### `std::vector<T>` — The Default Choice

A dynamic array. Elements are stored **contiguously in memory** — just like a C array, but with automatic resizing.

```cpp
#include <vector>

std::vector<int> v;               // empty
std::vector<int> v(10);          // 10 default-initialized ints (0)
std::vector<int> v(10, 42);      // 10 ints, all 42
std::vector<int> v = {1, 2, 3};  // initializer list
```

**Key operations:**

```cpp
v.push_back(4);       // append — amortized O(1)
v.pop_back();         // remove last — O(1)
v.emplace_back(4);    // construct in-place — avoids a copy (prefer this)
v[2];                 // random access — O(1), no bounds check
v.at(2);              // random access with bounds check — throws std::out_of_range
v.front();            // first element
v.back();             // last element
v.size();             // number of elements
v.empty();            // true if size() == 0
v.clear();            // remove all elements, size = 0 (capacity unchanged)
v.insert(it, val);    // insert before iterator — O(n)
v.erase(it);          // remove at iterator — O(n)
v.resize(n);          // change size
```

**Capacity vs size:**

```cpp
v.capacity();         // current allocated storage (>= size)
v.reserve(100);       // pre-allocate storage for 100 elements — no size change
v.shrink_to_fit();    // request capacity reduction to match size
```

When `push_back` exceeds capacity, the vector **reallocates**: it allocates a new, larger buffer (typically 2x), moves all elements there, and frees the old one. This is amortized O(1) per push, but it **invalidates all iterators, pointers, and references** to the vector's elements. If you need stable pointers, `reserve()` in advance or use a different container.

**Complexity:**

| Operation | Complexity |
|-----------|-----------|
| `push_back` | Amortized O(1) |
| `pop_back` | O(1) |
| `operator[]` | O(1) |
| `insert` / `erase` at middle | O(n) |
| `find` (unsorted) | O(n) |

**When to use:** Almost always. `vector` is cache-friendly (contiguous memory), has the best access performance, and works well with algorithms. Default to `vector` and only switch when profiling shows a problem.

### `std::array<T, N>` — Fixed-Size Stack Array

A fixed-size array whose size is a compile-time constant. Stored on the stack (or wherever the object lives). Zero overhead vs a C array, but with a proper interface.

```cpp
#include <array>

std::array<int, 5> a = {1, 2, 3, 4, 5};
a[2];           // O(1) access
a.size();       // 5 — compile-time constant
a.front();      // 1
a.back();       // 5
a.fill(0);      // set all elements to 0

// Works with algorithms
std::sort(a.begin(), a.end());
```

Unlike C arrays, `std::array` knows its own size, doesn't decay to a pointer silently, and supports `begin()`/`end()` iterators. Prefer it over `int arr[N]` in all new code.

**When to use:** When the size is known at compile time and you want stack allocation — buffers, fixed-size tables, small fixed collections.

### `std::deque<T>` — Double-Ended Queue

A segmented buffer that supports O(1) push and pop at **both** ends. Internally it's a sequence of fixed-size chunks, not a single contiguous block.

```cpp
#include <deque>

std::deque<int> d = {2, 3, 4};
d.push_front(1);   // O(1) — prepend
d.push_back(5);    // O(1) — append
d.pop_front();     // O(1)
d.pop_back();      // O(1)
d[2];              // O(1) random access — but slower than vector (extra indirection)
```

**When to use:** When you need efficient insert/remove at both ends. `std::queue` and `std::stack` (the adaptors) use `deque` by default. If you only need one end, `vector` is faster.

### `std::list<T>` — Doubly Linked List

Each element is a separately heap-allocated node containing the value and pointers to the previous and next nodes.

```cpp
#include <list>

std::list<int> l = {1, 2, 3};
l.push_front(0);      // O(1)
l.push_back(4);       // O(1)
l.pop_front();        // O(1)
l.insert(it, val);    // O(1) — if you already have the iterator
l.erase(it);          // O(1) — iterator not invalidated for other elements
l.splice(pos, other); // O(1) — move elements from another list (no copy)
// No random access — no operator[]
```

**When to use:** Rarely. Despite the O(1) insert/erase promise, linked lists have terrible cache performance — every node is a separate allocation scattered in memory, causing cache misses on every access. Benchmarks consistently show that `std::vector` with its O(n) insert is faster than `std::list` for most real workloads because of cache locality.

Use `list` only when: you need O(1) insert/erase with a stable iterator that you already hold, and you're doing this so frequently (millions of times) that the cache miss penalty is accepted. `splice` is its main advantage — moving elements between lists in O(1) with no copies.

### `std::forward_list<T>` — Singly Linked List

Like `list` but with only a forward pointer. Even lower overhead per node (one pointer instead of two), but you can only iterate forward and you cannot efficiently access the previous element.

```cpp
#include <forward_list>

std::forward_list<int> fl = {1, 2, 3};
fl.push_front(0);              // O(1) — only front operations, no push_back
fl.insert_after(it, val);      // O(1) — insert after a given iterator
fl.erase_after(it);            // O(1)
// No size() method — tracking size has a cost they chose to avoid
```

**When to use:** Very specific memory-critical scenarios where the overhead of a second pointer per node matters (embedded systems). Otherwise use `vector` or `list`.

### Container Comparison

| Container | Memory | Access | Insert front | Insert back | Insert middle | Use when |
|-----------|--------|--------|-------------|-------------|---------------|----------|
| `vector` | Contiguous | O(1) | O(n) | O(1) amort | O(n) | Almost always |
| `array` | Contiguous | O(1) | — | — | — | Fixed compile-time size |
| `deque` | Segmented | O(1) | O(1) | O(1) | O(n) | Double-ended queues |
| `list` | Scattered | O(n) | O(1) | O(1) | O(1)* | Splice operations |
| `forward_list` | Scattered | O(n) | O(1) | O(n) | O(1)* | Minimal memory, forward-only |

*O(1) only if you already hold the iterator to that position.

---

## 4.3 — Associative Containers: `set`, `map`, `multiset`, `multimap`

All four are backed by a **balanced binary search tree** (red-black tree). Elements are always **sorted**. Operations are O(log n).

### `std::set<T>` — Sorted Unique Keys

Stores unique elements in sorted order. No duplicates.

```cpp
#include <set>

std::set<int> s = {5, 1, 3, 2, 4};
// Internally stored as: {1, 2, 3, 4, 5}

s.insert(3);        // no-op — 3 already exists
s.insert(6);        // inserts 6
s.erase(3);         // removes 3
s.count(4);         // 0 or 1 — whether 4 is in the set
s.find(4);          // returns iterator to 4, or s.end() if not found
s.contains(4);      // C++20 — cleaner than count() > 0

for (int x : s) std::cout << x << " ";   // prints in sorted order
```

**Range queries:**

```cpp
s.lower_bound(3);    // iterator to first element >= 3
s.upper_bound(3);    // iterator to first element > 3
s.equal_range(3);    // pair of (lower_bound, upper_bound)
```

These enable efficient "find all elements between 3 and 7" without scanning the whole set.

### `std::map<K, V>` — Sorted Key-Value Pairs

Associates unique keys with values, sorted by key.

```cpp
#include <map>

std::map<std::string, int> scores;
scores["Alice"] = 95;
scores["Bob"] = 87;
scores.insert({"Charlie", 91});
scores.emplace("Dave", 88);   // construct in-place — more efficient

scores["Alice"];      // 95 — if key doesn't exist, default-inserts it (!)
scores.at("Alice");   // 95 — throws std::out_of_range if key missing (safer)
scores.find("Eve");   // returns end() — Eve not found
scores.count("Bob");  // 1

// Iteration is always in sorted key order
for (const auto& [name, score] : scores) {   // C++17 structured binding
    std::cout << name << ": " << score << "\n";
}

scores.erase("Bob");
```

**Important:** `operator[]` on a map **inserts a default-initialized value** if the key doesn't exist. This is a common bug — use `find()` or `at()` when you want to check existence without inserting.

### `std::multiset<T>` and `std::multimap<K, V>`

Identical to `set` and `map` but allow **duplicate keys**:

```cpp
std::multiset<int> ms = {1, 2, 2, 3, 3, 3};
ms.count(3);        // 3 — counts all occurrences
ms.erase(2);        // removes ALL 2s
ms.erase(ms.find(3)); // removes only ONE 3 (erase by iterator)
```

### Custom Comparators

By default, `set` and `map` use `std::less<T>` (ascending order). You can supply any comparator:

```cpp
// Descending order set
std::set<int, std::greater<int>> desc = {3, 1, 4, 1, 5};
// Stored as: {5, 4, 3, 1}

// Case-insensitive string map
struct CaseInsensitiveLess {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::lexicographical_compare(
            a.begin(), a.end(), b.begin(), b.end(),
            [](char c1, char c2) { return std::tolower(c1) < std::tolower(c2); });
    }
};
std::map<std::string, int, CaseInsensitiveLess> m;
m["Hello"] = 1;
m["hello"];   // finds "Hello" — same key under case-insensitive comparison
```

### When to Use Associative Containers

- When you need **sorted iteration** over keys
- When you need **range queries** (give me all keys between X and Y)
- When you need **stable performance** (O(log n) guaranteed, unlike hash tables which have O(n) worst case)
- For smaller datasets (a few thousand elements) where the O(log n) constant factor doesn't matter compared to hash table overhead

For pure lookup-by-key, **unordered containers are faster** (next section). Use `map`/`set` when ordering matters.

---

## 4.4 — Unordered Containers: `unordered_map`, `unordered_set`

Backed by a **hash table**. No ordering. O(1) average for insert, erase, and find — O(n) worst case (all keys hash to the same bucket).

### `std::unordered_set<T>`

```cpp
#include <unordered_set>

std::unordered_set<int> us = {3, 1, 4, 1, 5, 9};
// No order — {3, 1, 4, 5, 9} in some implementation-defined order
// 1 appears once — duplicates are rejected

us.insert(6);
us.erase(3);
us.count(5);       // 0 or 1
us.find(5);        // iterator, or us.end() if not found
us.contains(5);    // C++20

us.bucket_count();   // number of buckets in the hash table
us.load_factor();    // size / bucket_count — high = more collisions
us.max_load_factor(0.5);  // trigger rehash sooner
us.rehash(100);      // set minimum number of buckets
```

### `std::unordered_map<K, V>`

```cpp
#include <unordered_map>

std::unordered_map<std::string, int> scores;
scores["Alice"] = 95;
scores.emplace("Bob", 87);

scores["Alice"];         // 95
scores.at("Alice");      // 95 — throws if missing (same as map)
scores.find("Charlie");  // end() if not found

for (auto& [key, val] : scores) { ... }  // order not guaranteed
```

### Hash Functions

For the key type to work in an unordered container, `std::hash<K>` must be specialized. The standard provides specializations for all primitive types, `std::string`, pointers, and several others.

For custom types, you must provide a hash function:

```cpp
struct Point { int x, y; };

struct PointHash {
    size_t operator()(const Point& p) const {
        size_t h1 = std::hash<int>{}(p.x);
        size_t h2 = std::hash<int>{}(p.y);
        return h1 ^ (h2 << 32);   // combine hashes — simple but adequate
    }
};

struct PointEqual {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

std::unordered_set<Point, PointHash, PointEqual> points;
```

A good hash function distributes keys uniformly across buckets. A poor one causes many collisions, degrading to O(n).

### Unordered vs Ordered — When to Choose Which

| Criterion | Use `map` / `set` | Use `unordered_map` / `unordered_set` |
|-----------|-------------------|--------------------------------------|
| Need sorted iteration | ✅ | ❌ |
| Need range queries | ✅ | ❌ |
| Fastest lookup | ❌ | ✅ (average) |
| Guaranteed worst-case | O(log n) | O(n) worst |
| Key is hashable | Either | Required |
| Small dataset | Either | Overhead may negate benefit |

**Default choice:** prefer `unordered_map`/`unordered_set` for pure key lookup. Switch to the ordered variants only when you need sorted order or range queries.

---

## 4.5 — Container Adaptors: `stack`, `queue`, `priority_queue`

Adaptors are not containers — they are **wrappers** that restrict the interface of an underlying container to enforce a specific access pattern.

### `std::stack<T>` — LIFO

Last in, first out. Backed by `std::deque` by default (configurable).

```cpp
#include <stack>

std::stack<int> s;
s.push(1);    // add to top
s.push(2);
s.push(3);
s.top();      // 3 — access top without removing
s.pop();      // remove top — no return value (!)
s.size();     // 2
s.empty();    // false

// To read and remove:
int val = s.top();
s.pop();
```

Note: `pop()` does not return the value. This is a deliberate design choice — returning by value would be wasteful if the caller doesn't need it, and would be exception-unsafe in some cases. Always call `top()` first, then `pop()`.

Use a vector as the underlying container for better cache performance:

```cpp
std::stack<int, std::vector<int>> s;
```

### `std::queue<T>` — FIFO

First in, first out. Backed by `std::deque` by default.

```cpp
#include <queue>

std::queue<int> q;
q.push(1);     // enqueue at back
q.push(2);
q.push(3);
q.front();     // 1 — access front without removing
q.back();      // 3 — access back without removing
q.pop();       // remove from front — no return value
q.size();      // 2
```

### `std::priority_queue<T>` — Heap

Elements are retrieved in priority order — the largest element is always at the top (max-heap by default).

```cpp
#include <queue>

std::priority_queue<int> pq;
pq.push(3);
pq.push(1);
pq.push(4);
pq.push(1);
pq.push(5);

pq.top();   // 5 — largest element
pq.pop();   // removes 5
pq.top();   // 4

while (!pq.empty()) {
    std::cout << pq.top() << " ";
    pq.pop();
}
// prints: 4 3 1 1 — descending order
```

**Min-heap** — use `std::greater<T>` as the comparator:

```cpp
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
// top() returns the smallest element
```

**Custom priority:**

```cpp
struct Task {
    int priority;
    std::string name;
    bool operator<(const Task& other) const {
        return priority < other.priority;   // lower priority value = lower in queue
    }
};

std::priority_queue<Task> tasks;
tasks.push({3, "low priority"});
tasks.push({10, "high priority"});
tasks.top().name;   // "high priority"
```

`priority_queue` is backed by `std::vector` and uses `std::make_heap`/`std::push_heap`/`std::pop_heap` internally. Insert and removal are O(log n); access to top is O(1).

---

## 4.6 — Iterators: categories, traits, custom iterators

### What an Iterator Is

An iterator is an object that behaves like a pointer — you can dereference it to get a value (`*it`), advance it (`++it`), and compare it to another iterator (`it == end`). The STL defines iterator categories based on what operations are supported.

### Iterator Categories

From weakest (fewest capabilities) to strongest:

**Input iterator** — single-pass, read-only. Can only be incremented forward, and each position can only be read once. Used for streams:

```cpp
std::istream_iterator<int> it(std::cin), end;
// reading from stdin — can't go back
```

**Output iterator** — single-pass, write-only. Used for writing to streams or inserting into containers:

```cpp
std::ostream_iterator<int> out(std::cout, " ");
std::back_insert_iterator<std::vector<int>> back(v);  // or std::back_inserter(v)
```

**Forward iterator** — multi-pass, read/write. Can traverse a range multiple times. `std::forward_list` provides these.

**Bidirectional iterator** — forward iterator that can also go backward (`--it`). `std::list`, `std::set`, `std::map` provide these.

**Random access iterator** — bidirectional iterator that supports arbitrary jumps in O(1): `it + n`, `it - n`, `it[n]`, `it1 - it2` (distance), `it1 < it2`. `std::vector`, `std::deque`, raw pointers provide these.

**Contiguous iterator** (C++17) — random access iterator where elements are guaranteed to be adjacent in memory. `std::vector`, `std::array`, raw arrays. Enables optimizations like `memcpy`.

### Common Iterator Operations

```cpp
std::vector<int> v = {1, 2, 3, 4, 5};

auto it = v.begin();    // iterator to first element
*it;                    // 1 — dereference
++it;                   // advance to next
it++;                   // post-increment (prefer pre-increment — avoids a copy)
--it;                   // go back (bidirectional+)
it + 3;                 // jump forward 3 (random access+)
it - v.begin();         // distance from begin (random access+)
it[2];                  // same as *(it + 2) (random access+)

v.end();                // one past last — never dereference this
v.cbegin(), v.cend();   // const iterators — cannot modify through them
v.rbegin(), v.rend();   // reverse iterators — rbegin() points to last element
v.crbegin(), v.crend(); // const reverse iterators
```

### Iterator Helper Functions

```cpp
#include <iterator>

std::advance(it, 3);         // advance it by 3 — works for any iterator category
std::distance(first, last);  // count elements between — O(1) for random access, O(n) otherwise
std::next(it);               // returns it + 1 without modifying it
std::next(it, 3);            // returns it + 3
std::prev(it);               // returns it - 1
```

### Iterator Invalidation — Critical

Some container operations invalidate existing iterators, pointers, and references. Using an invalidated iterator is undefined behavior.

| Container | Operation that invalidates |
|-----------|--------------------------|
| `vector` | Any reallocation (`push_back` beyond capacity, `insert`, `resize`, `reserve`) invalidates ALL iterators |
| `vector` | `insert`/`erase` invalidates iterators at or after the point of modification |
| `deque` | `push_front`/`push_back` invalidates ALL iterators (but not pointers/references to elements) |
| `list`, `set`, `map` | Only the erased element's iterator is invalidated |
| `unordered_map`/`set` | Any rehash invalidates ALL iterators |

```cpp
std::vector<int> v = {1, 2, 3};
auto it = v.begin();
v.push_back(4);   // may reallocate — it is now INVALID
*it;              // UNDEFINED BEHAVIOR
```

Safe pattern — re-obtain iterators after potentially invalidating operations, or use indices:

```cpp
size_t idx = it - v.begin();
v.push_back(4);
it = v.begin() + idx;   // re-derive iterator
```

### `std::iterator_traits`

Algorithms need to query iterator properties at compile time — what type does dereferencing return? Is the iterator bidirectional? `iterator_traits` provides this:

```cpp
template<typename It>
void my_algorithm(It first, It last) {
    using value_type = typename std::iterator_traits<It>::value_type;
    using difference_type = typename std::iterator_traits<It>::difference_type;
    // iterator_category, pointer, reference also available
}
```

Works for raw pointers too — `std::iterator_traits<int*>::value_type` is `int`.

### Writing a Custom Iterator

A minimal forward iterator for a custom container:

```cpp
class IntRange {
    int current, end_val;
public:
    IntRange(int start, int end) : current(start), end_val(end) {}

    struct iterator {
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;
        using pointer = const int*;
        using reference = const int&;

        int value;
        iterator(int v) : value(v) {}

        int operator*() const { return value; }
        iterator& operator++() { ++value; return *this; }
        iterator operator++(int) { auto tmp = *this; ++value; return tmp; }
        bool operator==(const iterator& o) const { return value == o.value; }
        bool operator!=(const iterator& o) const { return !(*this == o); }
    };

    iterator begin() const { return iterator(current); }
    iterator end()   const { return iterator(end_val); }
};

IntRange r(1, 6);
for (int x : r) std::cout << x << " ";   // 1 2 3 4 5
std::vector<int> v(r.begin(), r.end());   // works with any algorithm
```

---

## 4.7 — Algorithms: sorting, searching, transforming, partitioning

All algorithms live in `<algorithm>` (and `<numeric>` for math algorithms). They operate on iterator ranges `[first, last)`.

### Non-Modifying Algorithms

```cpp
#include <algorithm>

std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

// Search
std::find(v.begin(), v.end(), 5);                // iterator to first 5, or end()
std::find_if(v.begin(), v.end(), [](int x) { return x > 5; }); // first > 5
std::count(v.begin(), v.end(), 1);               // 2 — count occurrences of 1
std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }); // count evens

// Predicates
std::all_of(v.begin(), v.end(), [](int x) { return x > 0; });  // true
std::any_of(v.begin(), v.end(), [](int x) { return x > 8; });  // true (9 > 8)
std::none_of(v.begin(), v.end(), [](int x) { return x < 0; }); // true

// Traverse
std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << " "; });

// Min/Max
std::min_element(v.begin(), v.end());  // iterator to smallest
std::max_element(v.begin(), v.end());  // iterator to largest
auto [mn, mx] = std::minmax_element(v.begin(), v.end());  // both at once
```

### Sorting

```cpp
std::sort(v.begin(), v.end());               // ascending — O(n log n)
std::sort(v.begin(), v.end(), std::greater<int>());  // descending
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; }); // custom

std::stable_sort(v.begin(), v.end());  // preserves relative order of equal elements
std::partial_sort(v.begin(), v.begin() + 3, v.end());  // only sort first 3 (smallest 3 in place)
std::nth_element(v.begin(), v.begin() + 4, v.end());   // element at position 4 is correct; rest are partitioned
std::is_sorted(v.begin(), v.end());    // check without sorting
```

`nth_element` is O(n) — faster than full sort when you only need the k-th element or a partition around it (e.g., median).

### Binary Search (on sorted ranges)

```cpp
// Requires sorted range
std::vector<int> s = {1, 2, 3, 4, 5, 6};

std::binary_search(s.begin(), s.end(), 4);         // true — found
std::lower_bound(s.begin(), s.end(), 4);           // iterator to first element >= 4
std::upper_bound(s.begin(), s.end(), 4);           // iterator to first element > 4
auto [lo, hi] = std::equal_range(s.begin(), s.end(), 4); // [lower_bound, upper_bound)
```

`lower_bound`/`upper_bound` are O(log n) and the primary way to do fast lookups in a sorted vector. A sorted `vector` + binary search is often faster than `std::set` for read-heavy workloads.

### Modifying Algorithms

```cpp
std::vector<int> src = {1, 2, 3, 4, 5};
std::vector<int> dst(5);

std::copy(src.begin(), src.end(), dst.begin());   // copy range to destination
std::copy_if(src.begin(), src.end(), back_inserter(dst),
             [](int x) { return x % 2 == 0; });  // copy even elements

std::transform(src.begin(), src.end(), dst.begin(),
               [](int x) { return x * 2; });      // apply function, store result

std::fill(dst.begin(), dst.end(), 0);             // set all to 0
std::fill_n(dst.begin(), 3, -1);                  // set first 3 to -1

std::generate(v.begin(), v.end(), [n=0]() mutable { return n++; }); // 0,1,2,...

std::replace(v.begin(), v.end(), 1, 99);          // replace all 1s with 99
std::replace_if(v.begin(), v.end(),
                [](int x) { return x < 0; }, 0); // replace negatives with 0
```

### The Erase-Remove Idiom — Critical Pattern

`std::remove` and `std::remove_if` do not erase elements — they move non-removed elements to the front and return an iterator to the new "end". You must then call `erase()`:

```cpp
std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

// Remove all 2s
auto new_end = std::remove(v.begin(), v.end(), 2);
// v is now: {1, 3, 4, 5, ?, ?, ?} — ? are valid but unspecified
v.erase(new_end, v.end());
// v is now: {1, 3, 4, 5}

// Idiomatic one-liner
v.erase(std::remove(v.begin(), v.end(), 2), v.end());
v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
```

In C++20, the cleaner `std::erase` and `std::erase_if` free functions do this in one step:

```cpp
std::erase(v, 2);                                          // remove all 2s
std::erase_if(v, [](int x) { return x % 2 == 0; });       // remove evens
```

### Partitioning

```cpp
// Partition: elements satisfying predicate come first
std::partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
// evens first, odds after — relative order not preserved

std::stable_partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
// evens first, odds after — relative order preserved within each group

std::partition_point(v.begin(), v.end(), pred);  // binary search for partition boundary (range must already be partitioned)
```

### Set Operations (on sorted ranges)

```cpp
std::vector<int> a = {1, 2, 3, 4};
std::vector<int> b = {3, 4, 5, 6};

std::vector<int> result;
std::set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// {1, 2, 3, 4, 5, 6}

std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// {3, 4}

std::set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
// {1, 2} — in a but not in b
```

### Numeric Algorithms (`<numeric>`)

```cpp
#include <numeric>

std::vector<int> v = {1, 2, 3, 4, 5};

std::accumulate(v.begin(), v.end(), 0);         // 15 — sum (initial value 0)
std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>{}); // 120 — product

std::reduce(v.begin(), v.end());                // 15 — like accumulate but parallelizable
std::transform_reduce(v.begin(), v.end(), 0, std::plus<>{},
                      [](int x) { return x * x; });  // sum of squares: 55

std::iota(v.begin(), v.end(), 1);    // fill with 1, 2, 3, 4, 5
std::partial_sum(v.begin(), v.end(), result.begin());  // prefix sums
std::inclusive_scan(v.begin(), v.end(), result.begin());  // C++17, like partial_sum
std::exclusive_scan(v.begin(), v.end(), result.begin(), 0); // prefix sums excluding current
```

---

## 4.8 — Functors & `std::function`

### What a Functor Is

A **functor** (function object) is any object with `operator()` defined. It's callable like a function but can carry state:

```cpp
class Adder {
    int amount;
public:
    Adder(int n) : amount(n) {}
    int operator()(int x) const { return x + amount; }
};

Adder add5(5);
add5(10);   // 15
add5(20);   // 25

// Used directly with algorithms
std::transform(v.begin(), v.end(), v.begin(), Adder(10));
```

A function pointer could do the same, but it can't carry the `amount` state.

### Lambdas Are Functors

Every lambda is syntactic sugar for a compiler-generated class with `operator()`. This means lambdas are fully inlineable — the compiler knows the exact type at compile time:

```cpp
auto add5 = [amount=5](int x) { return x + amount; };
// Equivalent to the Adder class above — generated by the compiler
```

Lambdas have unique unnamed types. Two identical-looking lambdas have different types.

### Standard Functors (`<functional>`)

The standard library provides functor wrappers for common operations:

```cpp
#include <functional>

std::plus<int>{}(3, 4);       // 7
std::minus<int>{}(10, 3);     // 7
std::multiplies<int>{}(3, 4); // 12
std::divides<int>{}(10, 2);   // 5
std::modulus<int>{}(10, 3);   // 1
std::negate<int>{}(5);        // -5

std::equal_to<int>{}(3, 3);   // true
std::not_equal_to<int>{}(3, 4); // true
std::less<int>{}(3, 4);       // true
std::greater<int>{}(4, 3);    // true
std::less_equal<int>{}(3, 3); // true
std::greater_equal<int>{}(4, 3); // true

// Transparent versions (C++14) — deduce type from arguments
std::less<>{}(3, 4);   // works with mixed types
```

These are used with algorithms and containers that take comparators:

```cpp
std::sort(v.begin(), v.end(), std::greater<int>{});   // sort descending
std::set<int, std::greater<int>> descending_set;
```

### `std::function<R(Args...)>` — Type-Erased Callable

`std::function` can wrap any callable — a lambda, a functor, a function pointer, a member function pointer — and store it uniformly:

```cpp
#include <functional>

std::function<int(int, int)> op;

op = [](int a, int b) { return a + b; };    // lambda
op = std::plus<int>{};                       // functor
op = [](int a, int b) { return a * b; };    // different lambda

int result = op(3, 4);   // 12 — calls whichever lambda was last assigned
```

**Use cases:** callbacks, event handlers, storing heterogeneous callables in a container, plugin systems:

```cpp
std::vector<std::function<void()>> callbacks;
callbacks.push_back([]{ std::cout << "one\n"; });
callbacks.push_back([]{ std::cout << "two\n"; });
for (auto& cb : callbacks) cb();
```

**The cost of `std::function`:** It uses type erasure internally — similar overhead to a virtual call, plus a potential heap allocation for the stored callable if it's large. For performance-critical code (tight loops), prefer templates with concrete callable types over `std::function`:

```cpp
// Fast — compiler knows the exact type, can inline
template<typename F>
void apply(F&& f, std::vector<int>& v) {
    for (auto& x : v) x = f(x);
}

// Slower — type erased, virtual-call overhead, possible heap allocation
void apply(std::function<int(int)> f, std::vector<int>& v) {
    for (auto& x : v) x = f(x);
}
```

### `std::invoke` — Uniform Callable Invocation

`std::invoke` calls any callable uniformly — including member function pointers, which have unusual call syntax:

```cpp
#include <functional>

struct Foo { int bar(int x) { return x * 2; } };
Foo f;

std::invoke(&Foo::bar, f, 5);    // 10 — calls f.bar(5) via member pointer
std::invoke(some_lambda, 5);     // calls some_lambda(5)
std::invoke(some_function, 5);   // calls some_function(5)
```

Useful in generic code that accepts any callable including member functions.

### `std::bind` — Mostly Replaced by Lambdas

`std::bind` creates a function object with some arguments pre-bound:

```cpp
auto add10 = std::bind(std::plus<int>{}, std::placeholders::_1, 10);
add10(5);   // 15
```

In C++11 and beyond, lambdas are almost always clearer and more efficient:

```cpp
auto add10 = [](int x) { return x + 10; };   // prefer this
```

Use `std::bind` only for member function binding in pre-C++14 code or when interfacing with older APIs.

---

## 4.9 — `std::string` & `std::string_view`

### `std::string`

`std::string` is a dynamic, owning, heap-allocated (usually) string. It manages its own memory — you never worry about null terminators, buffer sizes, or allocation.

```cpp
#include <string>

std::string s = "hello";
std::string s2("world");
std::string s3(5, 'x');    // "xxxxx"
std::string s4 = s + " " + s2;  // "hello world" — concatenation

s.size();           // 5 — number of characters
s.length();         // same as size()
s.empty();          // false
s.capacity();       // allocated capacity (>= size)
s[2];               // 'l' — no bounds check
s.at(2);            // 'l' — with bounds check
s.front();          // 'h'
s.back();           // 'o'
s.c_str();          // const char* — null-terminated C string
s.data();           // const char* (C++11) — may or may not be null-terminated prior to C++11

// Modifying
s.append(" world"); // s += " world" — more efficient than + for repeated appends
s += '!';
s.push_back('!');
s.pop_back();
s.insert(5, " beautiful");  // insert at position 5
s.erase(5, 10);             // erase 10 chars starting at position 5
s.replace(0, 5, "goodbye"); // replace positions 0-4 with "goodbye"
s.clear();                  // empty the string

// Searching
s.find("world");           // index of first occurrence, or string::npos if not found
s.rfind("l");              // last occurrence
s.find_first_of("aeiou");  // first vowel
s.find_last_of("aeiou");   // last vowel

// Substrings
s.substr(6);       // from position 6 to end
s.substr(6, 5);    // 5 chars starting at position 6

// Comparison
s == "hello";     // true/false
s < "world";      // lexicographic comparison
s.compare("hello"); // 0 if equal, <0 if less, >0 if greater

// Conversion
std::stoi("42");    // string to int
std::stod("3.14");  // string to double
std::to_string(42); // int to string
```

### Small String Optimization (SSO)

Most `std::string` implementations use **SSO**: short strings (typically up to 15-22 characters) are stored directly inside the `std::string` object itself — no heap allocation. Only strings longer than the SSO threshold go to the heap. This makes short strings essentially free.

```cpp
std::string short_s = "hello";   // SSO — on stack, no heap allocation
std::string long_s = "this string is definitely longer than the SSO buffer threshold";
// ↑ heap allocated
```

### `std::string_view` — Non-Owning Reference

`std::string_view` (C++17) is a lightweight, non-owning view of a character sequence. It's just a pointer + length — no allocation, no copy.

```cpp
#include <string_view>

std::string_view sv = "hello";          // view of string literal
std::string s = "hello world";
std::string_view sv2 = s;              // view of s
std::string_view sv3 = s.substr(6);    // ERROR — substr returns string, sv3 dangling!
std::string_view sv4(s.data() + 6, 5); // OK — manual view of s's data

sv.size();          // 5
sv[2];              // 'l'
sv.substr(1, 3);    // string_view — no allocation, just adjusts pointer and length
sv.find("ll");      // 2
sv.starts_with("he");   // C++20
sv.ends_with("lo");     // C++20
sv.remove_prefix(2);    // advance pointer by 2 — modifies the view, not the data
sv.remove_suffix(1);    // shorten length by 1
```

### Using `string_view` for Function Parameters

The key use case: function parameters that just read a string, without needing to own it or modify it:

```cpp
// Before C++17 — had to choose: copy (expensive) or reference to std::string (can't accept literals)
void process(const std::string& s);  // can't pass "literal" efficiently without constructing a std::string

// With string_view — accepts anything: literal, std::string, std::string_view, char array
void process(std::string_view sv);

process("literal");             // OK — no allocation
process(some_std_string);       // OK — no copy
process(some_string_view);      // OK
process(char_array, length);    // OK
```

### Dangling `string_view` — The One Danger

`string_view` is non-owning. If the underlying string is destroyed or reallocated, the view becomes dangling:

```cpp
std::string_view dangerous() {
    std::string s = "hello";
    return s;       // DANGER — s is destroyed, string_view dangling
}

std::string_view sv = std::string("temporary");  // DANGER — temporary destroyed immediately
```

**Rule:** Never store a `string_view` that outlives the string it references. Use it for function parameters and local variables; avoid returning or storing them long-term unless you're certain the underlying data lives long enough.

---

## 4.10 — `std::span` (C++20)

### What It Is

`std::span<T>` is a non-owning view over a **contiguous sequence** of objects — a raw array, a `std::vector`, a `std::array`, or any other contiguous memory. It's `string_view` generalized to any type.

```cpp
#include <span>

void print_all(std::span<const int> data) {
    for (int x : data) std::cout << x << " ";
}

int arr[] = {1, 2, 3, 4, 5};
std::vector<int> vec = {1, 2, 3};
std::array<int, 3> arr2 = {1, 2, 3};

print_all(arr);    // OK — works with C array
print_all(vec);    // OK — works with vector
print_all(arr2);   // OK — works with std::array
```

Before `span`, a function that needed to accept any contiguous container had to either template the container type (verbose) or take a pointer + size (C-style, error-prone):

```cpp
// Old way — C-style
void process(const int* data, size_t count);

// Old way — verbose template
template<typename Container>
void process(const Container& c);

// New way — span captures both at once
void process(std::span<const int> data);
```

### Fixed vs Dynamic Extent

```cpp
std::span<int, 5> fixed = arr;   // compile-time size — more optimization potential
std::span<int>    dyn   = arr;   // runtime size (std::dynamic_extent)
```

Fixed-extent spans encode the size in the type — `span<int, 5>` vs `span<int, 3>` are different types. Dynamic-extent spans are more flexible.

### Key Operations

```cpp
std::span<int> s(vec.data(), vec.size());  // or just: std::span<int> s(vec)

s.size();           // number of elements
s[2];               // access (no bounds check by default)
s.front();          // first element
s.back();           // last element
s.data();           // raw pointer to the first element
s.begin(), s.end(); // iterators

// Subspans
s.subspan(1, 3);    // elements [1, 4)
s.first(3);         // first 3 elements
s.last(2);          // last 2 elements
```

### When to Use `span`

Use `std::span<const T>` as a function parameter whenever you need read-only access to a contiguous sequence of any origin. Use `std::span<T>` when you need to modify elements in place. Avoid using `span` as a return type or member (same dangling risk as `string_view`).

---

## 4.11 — Ranges (C++20): views, adaptors, lazy evaluation

### What Ranges Are

A **range** is any type with `begin()` and `end()`. Every STL container is a range. C++20 introduced the `<ranges>` library which builds on this to provide:

1. **Range-based algorithms** — take a range directly instead of two iterators
2. **Views** — lazy, composable transformations on ranges
3. **Pipe syntax** — chain transformations with `|`

### Range Algorithms

```cpp
#include <algorithm>
#include <ranges>

std::vector<int> v = {3, 1, 4, 1, 5, 9};

// Old way — two iterators
std::sort(v.begin(), v.end());
auto it = std::find(v.begin(), v.end(), 5);

// New way — pass the range directly
std::ranges::sort(v);
auto it = std::ranges::find(v, 5);
std::ranges::sort(v, std::greater<>{});   // descending — clean
```

Range algorithms are strictly safer — no mismatched begin/end pairs possible.

### Views — Lazy Transformations

A **view** is a lightweight, non-owning range adaptor. It doesn't compute anything until you iterate it — this is **lazy evaluation**.

```cpp
#include <ranges>

std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// filter: only even numbers
auto evens = v | std::views::filter([](int x) { return x % 2 == 0; });

// transform: double each
auto doubled = v | std::views::transform([](int x) { return x * 2; });

// Compose them with |
auto result = v
    | std::views::filter([](int x) { return x % 2 == 0; })
    | std::views::transform([](int x) { return x * 2; });

for (int x : result) std::cout << x << " ";   // 4 8 12 16 20
// Nothing was computed until this loop ran
```

No intermediate containers are created. Each element flows through the entire pipeline on demand. This is both memory-efficient and cache-friendly.

### Common Views

```cpp
namespace sv = std::views;

sv::filter(pred)          // keep only elements satisfying pred
sv::transform(fn)         // apply fn to each element
sv::take(n)               // first n elements
sv::take_while(pred)      // elements until pred is false
sv::drop(n)               // skip first n elements
sv::drop_while(pred)      // skip until pred is false
sv::reverse               // iterate in reverse
sv::keys                  // from a range of pairs/tuples: get first element
sv::values                // from a range of pairs/tuples: get second element
sv::enumerate             // (C++23) pairs (index, element)
sv::zip(r1, r2)           // (C++23) pair elements from two ranges
sv::iota(1, 10)           // generate 1, 2, 3, ..., 9 — no container needed
sv::repeat(val, n)        // (C++23) repeat val n times
sv::split(delim)          // split a range by delimiter
sv::join                  // flatten a range of ranges
sv::counted(it, n)        // view of n elements starting at it
```

### Practical Examples

```cpp
std::vector<std::string> words = {"hello", "world", "foo", "bar", "baz"};

// All words longer than 3 chars, uppercased
auto long_upper = words
    | sv::filter([](const auto& s) { return s.size() > 3; })
    | sv::transform([](auto s) {
        std::ranges::transform(s, s.begin(), ::toupper);
        return s;
    });

// Generate first 10 squares
auto squares = sv::iota(1, 11)
    | sv::transform([](int x) { return x * x; });
for (int x : squares) std::cout << x << " ";  // 1 4 9 16 25 36 49 64 81 100

// Sum of even squares less than 50
int total = 0;
for (int x : sv::iota(1) | sv::transform([](int x) { return x*x; })
                          | sv::take_while([](int x) { return x < 50; })
                          | sv::filter([](int x) { return x % 2 == 0; })) {
    total += x;
}
// total = 4 + 16 + 36 = 56
```

### Collecting Views into Containers

Views are lazy — they don't own data. To collect results into a container, C++23 provides `std::ranges::to`:

```cpp
// C++23
auto evens = v | sv::filter([](int x){ return x%2==0; })
               | std::ranges::to<std::vector>();

// C++20 workaround
std::vector<int> evens;
std::ranges::copy(v | sv::filter([](int x){ return x%2==0; }),
                  std::back_inserter(evens));
```

### Sentinels — More Flexible End Markers

Traditionally, `end()` must return the same type as `begin()` — an iterator. C++20 allows `end()` to return a **sentinel**, which is any type that can be compared with the iterator to signal "stop here." This enables natural expression of null-terminated strings, infinite sequences, and other open-ended ranges:

```cpp
// A sentinel that stops at a null character
struct NullTermSentinel {};
bool operator==(const char* p, NullTermSentinel) { return *p == '\0'; }

// Iterate a C string as a range without needing strlen
for (auto it = str; it != NullTermSentinel{}; ++it) { ... }
```

Standard views like `sv::iota(1)` (infinite sequence) work because their sentinel is never equal to the iterator — you stop by composing with `sv::take` or `sv::take_while`.
