# Exercises — Part 4 — Standard Template Library (STL)

> Practice problems for [[Part 4 — Standard Template Library (STL)]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 4.1 — STL Design Philosophy

**Ex 4.1.1** `[Easy]`

Name the three pillars of STL design and explain how they interact. Give one example of each pillar using standard library components.

```
// your answer
```

---

## Section 4.2 — Sequence Containers: `vector`, `deque`, `list`, `array`

**Ex 4.2.1** `[Easy]`

What is the time complexity of each operation for `std::vector`? access by index, `push_back`, `insert` at middle, `erase` at middle, `find` (linear scan).

```
// your answer
```

---

**Ex 4.2.2** `[Medium]`

Remove all even numbers from a `std::vector<int>` using the erase-remove idiom. Then do the same using C++20 `std::erase_if`. Show both versions.

```cpp
#include <vector>
#include <algorithm>

// your solution
```

---

**Ex 4.2.3** `[Medium]`

When does `push_back` invalidate iterators? Write code that demonstrates the bug, then fix it.

```cpp
std::vector<int> v = {1, 2, 3};
auto it = v.begin();
for (int i = 4; i <= 10; ++i) v.push_back(i);
std::cout << *it;   // is this safe? why?
```

```cpp
// your fixed version
```

---

**Ex 4.2.4** `[Medium]`

Implement a function `flatten(const std::vector<std::vector<int>>&)` that returns a single `std::vector<int>` with all elements. Pre-reserve the exact needed capacity before inserting.

```cpp
std::vector<int> flatten(const std::vector<std::vector<int>>& vv) {
    // your solution
}
```

---

## Section 4.3 — Associative Containers: `map`, `set`, `multimap`, `multiset`

**Ex 4.3.1** `[Easy]`

What is the time complexity of lookup, insertion, and deletion in `std::map`? What data structure does it use internally?

```
// your answer
```

---

**Ex 4.3.2** `[Medium]`

This code has a subtle bug. Find it and explain the consequence.

```cpp
std::map<std::string, int> word_count;
std::vector<std::string> words = {"a", "b", "a", "c"};

for (const auto& w : words) {
    if (word_count.find(w) == word_count.end()) {
        word_count[w] = 1;
    } else {
        word_count[w]++;    // is there a problem here vs find?
    }
    // vs: just doing word_count[w]++; directly — any difference?
}
```

```
// your answer — explain the operator[] insertion trap
```

---

**Ex 4.3.3** `[Medium]`

Using `std::map`, write a function that takes a `std::string` and returns a `std::map<char, int>` counting the frequency of each character. Then print the top 3 most frequent characters.

```cpp
std::map<char, int> char_frequency(const std::string& s) {
    // your solution
}
```

---

**Ex 4.3.4** `[Hard]`

Implement a `MultiIndex` container that stores strings and supports O(log n) lookup both by index (insertion order) and by value. You may use a `std::vector` and a `std::map` internally.

```cpp
class MultiIndex {
    // your solution
};
```

---

## Section 4.4 — Unordered Containers

**Ex 4.4.1** `[Easy]`

What is the average and worst-case complexity for `unordered_map` lookup? What causes worst-case behavior?

```
// your answer
```

---

**Ex 4.4.2** `[Medium]`

Write a custom hash for `std::pair<int, int>` so it can be used as a key in `std::unordered_map`. Use the hash-combine pattern.

```cpp
struct PairHash {
    // your solution
};

std::unordered_map<std::pair<int,int>, std::string, PairHash> grid_labels;
```

---

**Ex 4.4.3** `[Medium]`

Given a `std::vector<int>`, find all pairs of indices `(i, j)` where `i < j` and `a[i] + a[j] == target`. Use an `unordered_map` for O(n) time.

```cpp
std::vector<std::pair<int,int>> two_sum(const std::vector<int>& a, int target) {
    // your solution
}
```

---

## Section 4.5 — Container Adaptors: `stack`, `queue`, `priority_queue`

**Ex 4.5.1** `[Medium]`

Implement a function that uses a `std::stack` to reverse a `std::string` in place.

```cpp
std::string reverse_string(std::string s) {
    // your solution
}
```

---

**Ex 4.5.2** `[Medium]`

Using `std::priority_queue`, implement a function that finds the `k` largest integers in an unsorted `std::vector<int>` in O(n log k) time.

```cpp
std::vector<int> k_largest(const std::vector<int>& v, int k) {
    // your solution
}
```

---

## Section 4.6 — Iterators & Iterator Categories

**Ex 4.6.1** `[Easy]`

For each container, state the iterator category it provides and whether it supports `operator[]` (random access):
`std::vector`, `std::list`, `std::set`, `std::forward_list`, `std::deque`.

```
// your answer
```

---

**Ex 4.6.2** `[Medium]`

Implement a custom iterator for a `NumberRange` class that represents `[begin, end)` and can be used in a range-for loop:

```cpp
NumberRange r(1, 6);
for (int x : r) std::cout << x << " ";  // prints: 1 2 3 4 5
```

```cpp
class NumberRange {
    // your solution
};
```

---

**Ex 4.6.3** `[Hard]`

Write a `FilterIterator` adapter that wraps any forward iterator and skips elements not matching a predicate. It must satisfy the forward iterator requirements.

```cpp
template<typename Iter, typename Pred>
class FilterIterator {
    // your solution
};
```

---

## Section 4.7 — Iterator Invalidation Rules

**Ex 4.7.1** `[Medium]`

Each snippet has an iterator invalidation bug. Identify and fix each:

```cpp
// A — erasing while iterating
std::vector<int> v = {1, 2, 3, 4, 5};
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it % 2 == 0) v.erase(it);
}

// B — map iteration with erase
std::map<int, int> m = {{1,1},{2,2},{3,3}};
for (auto it = m.begin(); it != m.end(); ++it) {
    if (it->first % 2 == 0) m.erase(it);
}
```

```cpp
// your fixed versions
```

---

## Section 4.8 — Algorithms (`<algorithm>`)

**Ex 4.8.1** `[Easy]`

Using only standard algorithms (no raw loops), write a pipeline that: takes a `vector<int>`, keeps only values > 10, doubles each remaining value, and sums the result.

```cpp
int process(std::vector<int> v) {
    // your solution — use copy_if, transform, accumulate
}
```

---

**Ex 4.8.2** `[Medium]`

Implement `std::partition` yourself. It should reorder elements so all elements satisfying the predicate come before those that don't. Return an iterator to the first element of the second group.

```cpp
template<typename It, typename Pred>
It my_partition(It first, It last, Pred pred) {
    // your solution
}
```

---

**Ex 4.8.3** `[Medium]`

Use `std::nth_element` to find the median of a `vector<int>` without fully sorting it. Explain the time complexity.

```cpp
double median(std::vector<int> v) {
    // your solution
}
```

---

## Section 4.9 — `std::string` and `std::string_view`

**Ex 4.9.1** `[Medium]`

This function returns a `string_view`. Find the bug and explain why it causes undefined behavior.

```cpp
std::string_view get_prefix(const std::string& s, size_t n) {
    return s.substr(0, n);  // bug?
}
```

```
// your answer
```

---

**Ex 4.9.2** `[Medium]`

Implement `split(std::string_view s, char delim)` that returns a `std::vector<std::string_view>`. All returned views must refer into the original `s` — no copies.

```cpp
std::vector<std::string_view> split(std::string_view s, char delim) {
    // your solution
}
```

---

**Ex 4.9.3** `[Hard]`

Implement a simple `StringBuilder` class that appends strings efficiently (amortized O(1) per append) and returns the final `std::string`. Internally use `std::vector<std::string>` and join them at the end with a single allocation.

```cpp
class StringBuilder {
    // your solution
};
```

---

## Section 4.10 — `std::span`

**Ex 4.10.1** `[Medium]`

Rewrite this function to take `std::span<const int>` instead of `const std::vector<int>&`, so it works with vectors, arrays, and raw pointer+size pairs without copies:

```cpp
int sum(const std::vector<int>& v) {
    int total = 0;
    for (int x : v) total += x;
    return total;
}
```

```cpp
// your solution
```

---

**Ex 4.10.2** `[Medium]`

Write a function `first_n(std::span<int> data, size_t n)` that returns a `std::span<int>` of the first `n` elements, and `last_n` for the last n. Then write a function that splits a span into two halves.

```cpp
// your solution
```

---

## Section 4.11 — Ranges and Views (C++20)

**Ex 4.11.1** `[Medium]`

Using ranges and views, write a one-liner that takes a `vector<string>`, keeps strings with length > 3, converts each to uppercase, and collects them into a new `vector<string>`.

```cpp
#include <ranges>
#include <algorithm>

// your solution
```

---

**Ex 4.11.2** `[Medium]`

Implement a lazy `fibonacci_view` using `std::views::iota` or a custom range that yields the first `n` Fibonacci numbers without materializing them all in memory.

```cpp
// your solution
```

---

**Ex 4.11.3** `[Hard]`

Implement a custom range adaptor `stride_view(range, n)` that yields every n-th element of a range. It must be composable with `|` pipe syntax.

```cpp
std::vector<int> v = {0,1,2,3,4,5,6,7,8,9};
for (int x : v | stride_view(3)) std::cout << x << " ";  // 0 3 6 9

// your solution
```
