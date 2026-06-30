# Part 10 — Data Structures & Algorithms

> Complexity analysis, fundamental data structures, and the core algorithms every C++ developer needs to know. The C++ implementations here build on the STL containers from [[Part 4 — Standard Template Library (STL)]] — this part explains the internals and algorithmic foundations behind them.

## Table of Contents

- [[#10.1 — Complexity Analysis: Big-O, Big-Theta, Big-Omega]]
- [[#10.2 — Arrays & Strings]]
- [[#10.3 — Linked Lists: Singly, Doubly, Circular]]
- [[#10.4 — Stacks & Queues]]
- [[#10.5 — Hash Tables — Internals]]
- [[#10.6 — Trees: BST, AVL, Red-Black, Trie]]
- [[#10.7 — Heaps & Priority Queues]]
- [[#10.8 — Graphs: Representation, BFS, DFS]]
- [[#10.9 — Graph Algorithms: Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim]]
- [[#10.10 — Sorting: Bubble, Merge, Quick, Heap, Radix, Tim]]
- [[#10.11 — Searching: Binary Search & variants]]
- [[#10.12 — Dynamic Programming: Memoization vs Tabulation]]
- [[#10.13 — Greedy Algorithms]]
- [[#10.14 — Backtracking]]
- [[#10.15 — Bit Manipulation]]

---

## 10.1 — Complexity Analysis: Big-O, Big-Theta, Big-Omega

### Why Complexity Analysis Matters

An algorithm that takes 1ms on 100 elements might take 100 seconds on 10,000 elements — or it might take 1.01ms. Complexity analysis tells you which without running it. It's the language for comparing algorithms at scale and is fundamental to every engineering decision about data structure selection.

### Asymptotic Notation

Complexity describes how resource usage (time or space) grows as input size `n` grows. We drop constants and lower-order terms because they matter less as `n` gets large.

**Big-O — Upper Bound (worst case)**

`f(n) = O(g(n))` means `f` grows *no faster* than `g`. Used most commonly because we care about worst-case behavior.

```
O(1)        constant     — array index, hash lookup (average)
O(log n)    logarithmic  — binary search, balanced BST operations
O(n)        linear       — linear scan, iterating a list
O(n log n)  linearithmic — merge sort, heap sort, std::sort
O(n²)       quadratic    — bubble sort, naive string matching, nested loops
O(n³)       cubic        — naive matrix multiplication, Floyd-Warshall
O(2ⁿ)       exponential  — brute-force subset enumeration
O(n!)       factorial    — brute-force permutations, traveling salesman
```

**Big-Omega — Lower Bound (best case)**

`f(n) = Ω(g(n))` means `f` grows *at least* as fast as `g`. Any comparison-based sort is `Ω(n log n)` — you can't do better asymptotically.

**Big-Theta — Tight Bound**

`f(n) = Θ(g(n))` means `f` grows *at the same rate* as `g` — both O and Ω. Merge sort is `Θ(n log n)` — always takes n log n time regardless of input.

### Complexity Hierarchy

```
O(1) < O(log n) < O(√n) < O(n) < O(n log n) < O(n²) < O(n³) < O(2ⁿ) < O(n!)
```

At n = 1,000,000:
- O(log n) ≈ 20 operations
- O(n) = 1,000,000 operations
- O(n²) = 10¹² operations — infeasible
- O(2ⁿ) = 10³⁰⁰⁰⁰⁰ operations — physically impossible

### Amortized Complexity

Some operations are occasionally expensive but cheap on average. `std::vector::push_back` is O(1) amortized — most calls are O(1), but every time the capacity doubles, it's O(n). Averaged over n pushes, each is O(1).

```cpp
// n push_back calls on an empty vector — total work is O(n), not O(n²)
// Capacity doubles: 1, 2, 4, 8, 16, ...
// Total copies: 1 + 2 + 4 + ... + n = 2n = O(n)
// Per-call amortized cost: O(n)/n = O(1)
```

### Space Complexity

Space complexity counts auxiliary memory — memory beyond the input itself.

```
O(1) — iterative algorithms that use a fixed number of variables
O(log n) — recursive algorithms with log-depth recursion (e.g., binary search recursive)
O(n) — storing a copy of the input, or a hash table proportional to input
O(n²) — storing an adjacency matrix for n vertices
```

### Common Complexities of STL Operations

| Operation | `vector` | `list` | `map` | `unordered_map` |
|---|---|---|---|---|
| Access by index | O(1) | O(n) | — | — |
| Find | O(n) | O(n) | O(log n) | O(1) avg |
| Insert at end | O(1) amort | O(1) | O(log n) | O(1) avg |
| Insert at middle | O(n) | O(1)* | O(log n) | O(1) avg |
| Delete at middle | O(n) | O(1)* | O(log n) | O(1) avg |
| Sort | O(n log n) | O(n log n) | always sorted | — |

*Given an iterator to the position. Finding the position is O(n).

---

## 10.2 — Arrays & Strings

### Arrays — The Foundation

An array is a contiguous block of memory holding elements of the same type. All other data structures build on this or contrast with it. Random access is O(1) because the address of element `i` is `base_address + i * sizeof(T)`.

```cpp
int arr[5] = {1, 2, 3, 4, 5};  // stack-allocated, fixed size
arr[2];       // O(1) access
&arr[0];      // base address
sizeof(arr);  // 20 bytes

// Heap-allocated
int* dyn = new int[n];
// Prefer std::vector — same performance, manages lifetime
std::vector<int> v(n, 0);   // n elements, all 0
```

**Two-pointer technique** — O(n) algorithms on sorted arrays:

```cpp
// Find pair summing to target in sorted array
bool has_pair_with_sum(const std::vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {
        int sum = a[lo] + a[hi];
        if (sum == target) return true;
        if (sum < target)  ++lo;
        else               --hi;
    }
    return false;
}

// Reverse in place
void reverse(std::vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) std::swap(a[lo++], a[hi--]);
}
```

**Sliding window** — O(n) algorithms on subarrays:

```cpp
// Maximum sum subarray of length k
int max_sum_window(const std::vector<int>& a, int k) {
    int window = 0;
    for (int i = 0; i < k; ++i) window += a[i];  // initial window
    int best = window;
    for (int i = k; i < (int)a.size(); ++i) {
        window += a[i] - a[i - k];  // slide: add new, remove old
        best = std::max(best, window);
    }
    return best;
}
```

**Kadane's Algorithm** — Maximum subarray sum in O(n):

```cpp
int max_subarray(const std::vector<int>& a) {
    int max_so_far = a[0], max_ending_here = a[0];
    for (int i = 1; i < (int)a.size(); ++i) {
        max_ending_here = std::max(a[i], max_ending_here + a[i]);
        max_so_far = std::max(max_so_far, max_ending_here);
    }
    return max_so_far;
}
// max_ending_here: best subarray ending at i
// if extending is worse than starting fresh, start fresh
```

**Prefix sums** — O(1) range sum queries after O(n) preprocessing:

```cpp
std::vector<int> prefix(a.size() + 1, 0);
for (size_t i = 0; i < a.size(); ++i) prefix[i+1] = prefix[i] + a[i];

// Sum of a[l..r] inclusive — O(1)
int range_sum(int l, int r) { return prefix[r+1] - prefix[l]; }
```

### Strings

Strings are arrays of characters. Key algorithms:

**Palindrome check:**

```cpp
bool is_palindrome(const std::string& s) {
    int lo = 0, hi = (int)s.size() - 1;
    while (lo < hi) {
        if (s[lo] != s[hi]) return false;
        ++lo; --hi;
    }
    return true;
}
```

**Anagram check:**

```cpp
bool is_anagram(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    std::array<int, 256> freq{};
    for (char c : a) ++freq[(unsigned char)c];
    for (char c : b) { if (--freq[(unsigned char)c] < 0) return false; }
    return true;
}
```

**KMP — Substring search in O(n+m) instead of O(nm):**

```cpp
// Failure function: how much of the prefix also matches a suffix at each position
std::vector<int> kmp_failure(const std::string& pat) {
    int n = pat.size();
    std::vector<int> f(n, 0);
    for (int i = 1, j = 0; i < n; ++i) {
        while (j > 0 && pat[i] != pat[j]) j = f[j-1];
        if (pat[i] == pat[j]) ++j;
        f[i] = j;
    }
    return f;
}

int kmp_search(const std::string& text, const std::string& pat) {
    auto f = kmp_failure(pat);
    for (int i = 0, j = 0; i < (int)text.size(); ++i) {
        while (j > 0 && text[i] != pat[j]) j = f[j-1];
        if (text[i] == pat[j]) ++j;
        if (j == (int)pat.size()) return i - j + 1;  // match at index i-j+1
    }
    return -1;  // not found
}
```

---

## 10.3 — Linked Lists: Singly, Doubly, Circular

### Structure

```cpp
// Singly linked
struct SNode {
    int val;
    SNode* next{nullptr};
    explicit SNode(int v) : val(v) {}
};

// Doubly linked
struct DNode {
    int val;
    DNode* prev{nullptr};
    DNode* next{nullptr};
    explicit DNode(int v) : val(v) {}
};
```

**Advantage over arrays:** O(1) insert/delete anywhere given a pointer to the node — no shifting. **Disadvantage:** No random access (O(n) to reach element i), poor cache locality (nodes scattered in heap memory).

### Key Operations

**Reverse a singly linked list in O(n), O(1) space:**

```cpp
SNode* reverse(SNode* head) {
    SNode* prev = nullptr;
    SNode* curr = head;
    while (curr) {
        SNode* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;  // new head
}
```

**Find cycle — Floyd's tortoise and hare:**

```cpp
bool has_cycle(SNode* head) {
    SNode* slow = head;
    SNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;  // cycle detected
    }
    return false;
}

// Find start of cycle
SNode* cycle_start(SNode* head) {
    SNode* slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow != fast) continue;
        // Reset slow to head; they meet at cycle start
        slow = head;
        while (slow != fast) { slow = slow->next; fast = fast->next; }
        return slow;
    }
    return nullptr;
}
```

**Find middle (for merge sort):**

```cpp
SNode* find_middle(SNode* head) {
    SNode* slow = head, *fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;  // slow is at middle
}
```

**Merge two sorted lists:**

```cpp
SNode* merge_sorted(SNode* a, SNode* b) {
    SNode dummy{0};
    SNode* tail = &dummy;
    while (a && b) {
        if (a->val <= b->val) { tail->next = a; a = a->next; }
        else                  { tail->next = b; b = b->next; }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}
```

**Merge sort on linked list — O(n log n), O(log n) space:**

```cpp
SNode* merge_sort(SNode* head) {
    if (!head || !head->next) return head;
    SNode* mid  = find_middle(head);
    SNode* right = mid->next;
    mid->next = nullptr;  // split
    return merge_sorted(merge_sort(head), merge_sort(right));
}
```

### STL Equivalents

- `std::list<T>` — doubly linked list
- `std::forward_list<T>` — singly linked, slightly more memory-efficient

Both are rarely the right choice — `std::vector` beats them in practice due to cache locality, even for frequent insertions, because the memory overhead and cache misses of linked lists dominate.

---

## 10.4 — Stacks & Queues

### Stack — LIFO

```cpp
// STL
std::stack<int> s;
s.push(1); s.push(2); s.push(3);
s.top();    // 3 — peek without removing
s.pop();    // removes 3
s.size();   // 2
s.empty();  // false
```

**Classic applications:**

```cpp
// Balanced parentheses
bool balanced(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') st.push(c);
        else {
            if (st.empty()) return false;
            char top = st.top(); st.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) return false;
        }
    }
    return st.empty();
}

// Evaluate RPN (reverse Polish notation)
int eval_rpn(const std::vector<std::string>& tokens) {
    std::stack<int> st;
    for (const auto& t : tokens) {
        if (t == "+" || t == "-" || t == "*" || t == "/") {
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            if (t == "+") st.push(a + b);
            else if (t == "-") st.push(a - b);
            else if (t == "*") st.push(a * b);
            else st.push(a / b);
        } else {
            st.push(std::stoi(t));
        }
    }
    return st.top();
}

// Monotonic stack — next greater element in O(n)
std::vector<int> next_greater(const std::vector<int>& a) {
    int n = a.size();
    std::vector<int> result(n, -1);
    std::stack<int> st;  // stack of indices
    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[i] > a[st.top()]) {
            result[st.top()] = a[i];
            st.pop();
        }
        st.push(i);
    }
    return result;
}
```

### Queue — FIFO

```cpp
std::queue<int> q;
q.push(1); q.push(2); q.push(3);
q.front();  // 1 — first in
q.back();   // 3 — last in
q.pop();    // removes front (1)
```

**Deque (double-ended queue):**

```cpp
std::deque<int> dq;
dq.push_front(1);  // insert at front — O(1)
dq.push_back(2);   // insert at back  — O(1)
dq.pop_front();
dq.pop_back();
dq[i];             // random access — O(1)
```

**Sliding window maximum using deque — O(n):**

```cpp
std::vector<int> sliding_max(const std::vector<int>& a, int k) {
    std::deque<int> dq;   // indices, decreasing values front to back
    std::vector<int> result;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (!dq.empty() && dq.front() == i - k) dq.pop_front();  // remove out-of-window
        while (!dq.empty() && a[dq.back()] < a[i]) dq.pop_back(); // remove smaller
        dq.push_back(i);
        if (i >= k - 1) result.push_back(a[dq.front()]);  // front is max of window
    }
    return result;
}
```

---

## 10.5 — Hash Tables — Internals

### How Hash Tables Work

A hash table maps keys to values in O(1) average time. It uses a **hash function** to convert the key to an index in an array (the **bucket array**), then stores the value there.

```
key → hash_function(key) → index % bucket_count → bucket → value
```

### Collision Resolution

Two different keys can hash to the same index (**collision**). The two main strategies:

**Separate chaining** — each bucket holds a linked list (or vector) of all key-value pairs that map to that bucket:

```cpp
// Simplified chained hash table
template<typename K, typename V>
class HashMap {
    static const int BUCKETS = 16;
    std::vector<std::list<std::pair<K, V>>> table_{BUCKETS};
    std::hash<K> hasher_;
    size_t count_{0};

public:
    void insert(K key, V value) {
        auto& bucket = table_[hasher_(key) % BUCKETS];
        for (auto& [k, v] : bucket)
            if (k == key) { v = value; return; }  // update existing
        bucket.emplace_back(std::move(key), std::move(value));
        ++count_;
    }

    V* find(const K& key) {
        auto& bucket = table_[hasher_(key) % BUCKETS];
        for (auto& [k, v] : bucket)
            if (k == key) return &v;
        return nullptr;
    }
};
```

**Open addressing** — all entries stored in the bucket array itself. On collision, probe for the next empty slot:

- *Linear probing*: check index, index+1, index+2, ...
- *Quadratic probing*: check index, index+1, index+4, index+9, ...
- *Double hashing*: step size is itself a hash of the key

**Load factor** = `n / capacity` (number of entries / number of buckets). As load factor rises, collisions increase and O(1) degrades toward O(n). Standard practice: rehash (double the bucket array) when load factor exceeds ~0.7.

### Hash Functions

A good hash function distributes keys uniformly across buckets. For custom types, combine the hashes of all members:

```cpp
struct Point { int x, y; };

template<>
struct std::hash<Point> {
    size_t operator()(const Point& p) const noexcept {
        size_t hx = std::hash<int>{}(p.x);
        size_t hy = std::hash<int>{}(p.y);
        // FNV-style combination — each field shifts and XORs
        return hx ^ (hy * 2654435761ULL);  // Knuth's multiplicative hash
    }
};

// Boost-style hash_combine (common pattern):
template<typename T>
void hash_combine(size_t& seed, const T& v) {
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

size_t seed = 0;
hash_combine(seed, p.x);
hash_combine(seed, p.y);
```

### `std::unordered_map` — Internals

`std::unordered_map` uses separate chaining. Important operations:

```cpp
std::unordered_map<std::string, int> freq;
freq.reserve(1000);     // pre-allocate for 1000 elements — avoids rehashes
freq.max_load_factor(0.5f);  // rehash when >50% full (default ~1.0)

// Count frequencies
for (const auto& word : words) ++freq[word];

// Iteration order is unspecified — do not rely on it
```

**Worst-case O(n) for `unordered_map`:** Hash collision attacks (deliberately crafted keys that all map to the same bucket) degrade performance to O(n). In security-sensitive code, use a randomized hash (e.g., adding a random seed) or `std::map`.

---

## 10.6 — Trees: BST, AVL, Red-Black, Trie

### Binary Search Tree (BST)

A BST stores values such that for every node, all values in its left subtree are smaller and all in its right subtree are larger. This enables O(log n) search, insert, and delete — if the tree is balanced.

```cpp
struct BST {
    struct Node {
        int val;
        std::unique_ptr<Node> left, right;
        explicit Node(int v) : val(v) {}
    };

    std::unique_ptr<Node> root_;

    void insert(int val) { root_ = insert(std::move(root_), val); }
    bool contains(int val) const { return contains(root_.get(), val); }

private:
    static std::unique_ptr<Node> insert(std::unique_ptr<Node> node, int val) {
        if (!node) return std::make_unique<Node>(val);
        if (val < node->val) node->left  = insert(std::move(node->left),  val);
        else if (val > node->val) node->right = insert(std::move(node->right), val);
        return node;
    }
    static bool contains(const Node* node, int val) {
        if (!node) return false;
        if (val == node->val) return true;
        return val < node->val ? contains(node->left.get(), val)
                               : contains(node->right.get(), val);
    }
};
```

**The balance problem:** Inserting sorted data (1, 2, 3, 4, ...) into a BST produces a linked list — height O(n), not O(log n). Self-balancing trees fix this.

### Tree Traversals

```cpp
void inorder(Node* n) {    // left, root, right — gives SORTED output for BST
    if (!n) return;
    inorder(n->left.get());
    process(n->val);
    inorder(n->right.get());
}
void preorder(Node* n) {   // root, left, right — used to copy/serialize trees
    if (!n) return;
    process(n->val);
    preorder(n->left.get());
    preorder(n->right.get());
}
void postorder(Node* n) {  // left, right, root — used to delete trees
    if (!n) return;
    postorder(n->left.get());
    postorder(n->right.get());
    process(n->val);   // node is processed after its children
}

// Level-order (BFS)
void level_order(Node* root) {
    if (!root) return;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* n = q.front(); q.pop();
        process(n->val);
        if (n->left)  q.push(n->left.get());
        if (n->right) q.push(n->right.get());
    }
}
```

### AVL Tree

Guarantees height ≤ 1.44 log₂(n) by maintaining a **balance factor** (height difference between left and right subtrees) of -1, 0, or 1 at every node. When an insertion violates this, it performs one or two **rotations** to restore balance.

**Rotations:**
- **Right rotation** — when left subtree is too heavy
- **Left rotation** — when right subtree is too heavy
- **Left-Right rotation** — double rotation: left on child, then right on node
- **Right-Left rotation** — double rotation: right on child, then left on node

AVL trees have stricter balance than Red-Black trees, so lookups are faster. Insertions are slightly slower (more rotations). Used in std::map on some implementations.

### Red-Black Tree

A Red-Black tree maintains five invariants:
1. Every node is red or black
2. The root is black
3. Every leaf (null) is black
4. If a node is red, both children are black (no two consecutive reds)
5. All paths from any node to its descendant null leaves have the same number of black nodes

These guarantee height ≤ 2 log₂(n+1). Red-Black trees require at most 2 rotations per insertion and deletion — better than AVL's potential for more rotations. This is why `std::map`, `std::set`, `std::multimap`, `std::multiset` are universally implemented as Red-Black trees.

### Trie (Prefix Tree)

A trie stores strings by their prefixes. Each node represents a character; paths from root to marked nodes spell words. O(m) operations where m is the string length — independent of how many strings are stored.

```cpp
struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> children;
    bool is_end{false};
};

class Trie {
    TrieNode root_;
public:
    void insert(const std::string& word) {
        TrieNode* node = &root_;
        for (char c : word) {
            if (!node->children.count(c))
                node->children[c] = std::make_unique<TrieNode>();
            node = node->children[c].get();
        }
        node->is_end = true;
    }

    bool search(const std::string& word) const {
        const TrieNode* node = &root_;
        for (char c : word) {
            auto it = node->children.find(c);
            if (it == node->children.end()) return false;
            node = it->second.get();
        }
        return node->is_end;
    }

    bool starts_with(const std::string& prefix) const {
        const TrieNode* node = &root_;
        for (char c : prefix) {
            auto it = node->children.find(c);
            if (it == node->children.end()) return false;
            node = it->second.get();
        }
        return true;
    }
};
```

**Applications:** Autocomplete, spell checking, IP routing (prefix matching), dictionary lookups. STL equivalent for ordered prefix iteration: `std::map` supports `lower_bound` on strings, which gives prefix range queries.

---

## 10.7 — Heaps & Priority Queues

### Binary Heap

A binary heap is a complete binary tree stored as an array, satisfying the **heap property**:
- **Max-heap**: every node ≥ both children → root is maximum
- **Min-heap**: every node ≤ both children → root is minimum

For a node at index `i` (1-indexed):
- Left child: `2i`
- Right child: `2i + 1`
- Parent: `i / 2`

(0-indexed: left = `2i+1`, right = `2i+2`, parent = `(i-1)/2`)

```cpp
class MaxHeap {
    std::vector<int> data_;

    void sift_up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (data_[p] >= data_[i]) break;
            std::swap(data_[p], data_[i]);
            i = p;
        }
    }

    void sift_down(int i) {
        int n = data_.size();
        while (true) {
            int largest = i, l = 2*i+1, r = 2*i+2;
            if (l < n && data_[l] > data_[largest]) largest = l;
            if (r < n && data_[r] > data_[largest]) largest = r;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }

public:
    void push(int val) {
        data_.push_back(val);
        sift_up(data_.size() - 1);   // O(log n)
    }

    int top() const { return data_[0]; }   // O(1)

    void pop() {
        data_[0] = data_.back();
        data_.pop_back();
        if (!data_.empty()) sift_down(0);  // O(log n)
    }

    // Build heap from array — O(n) (better than n pushes = O(n log n))
    void build(std::vector<int> v) {
        data_ = std::move(v);
        for (int i = (data_.size() - 2) / 2; i >= 0; --i) sift_down(i);
    }
};
```

**Build heap is O(n), not O(n log n).** This is non-obvious — the proof uses the fact that most nodes are near the bottom and sift down very little.

### `std::priority_queue`

```cpp
// Max-heap (default)
std::priority_queue<int> max_pq;
max_pq.push(3); max_pq.push(1); max_pq.push(4);
max_pq.top();   // 4
max_pq.pop();   // removes 4

// Min-heap
std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;

// Custom comparator
auto cmp = [](const Task& a, const Task& b){ return a.priority < b.priority; };
std::priority_queue<Task, std::vector<Task>, decltype(cmp)> task_queue(cmp);
```

### K-th Largest Element

Use a min-heap of size k — keep only the k largest elements seen so far:

```cpp
int kth_largest(const std::vector<int>& a, int k) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
    for (int x : a) {
        min_heap.push(x);
        if ((int)min_heap.size() > k) min_heap.pop();
    }
    return min_heap.top();   // k-th largest
}
// O(n log k) — better than sorting O(n log n) when k << n
```

### Merge K Sorted Lists

```cpp
std::vector<int> merge_k_sorted(std::vector<std::vector<int>>& lists) {
    using T = std::tuple<int, int, int>;  // (value, list_index, element_index)
    std::priority_queue<T, std::vector<T>, std::greater<T>> pq;

    for (int i = 0; i < (int)lists.size(); ++i)
        if (!lists[i].empty()) pq.emplace(lists[i][0], i, 0);

    std::vector<int> result;
    while (!pq.empty()) {
        auto [val, li, ei] = pq.top(); pq.pop();
        result.push_back(val);
        if (ei + 1 < (int)lists[li].size())
            pq.emplace(lists[li][ei+1], li, ei+1);
    }
    return result;   // O(n log k) where n = total elements, k = number of lists
}
```

---

## 10.8 — Graphs: Representation, BFS, DFS

### Graph Representations

**Adjacency List** — store each vertex's neighbors in a list. Space: O(V + E). Fast neighbor iteration. Preferred for sparse graphs.

```cpp
int V = 6;
std::vector<std::vector<int>> adj(V);   // adj[u] = list of neighbors of u

// Add edge u → v (directed)
adj[0].push_back(1);
adj[0].push_back(2);

// Undirected: add both directions
void add_edge(std::vector<std::vector<int>>& adj, int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

// Weighted
std::vector<std::vector<std::pair<int,int>>> wadj(V);  // (neighbor, weight)
wadj[0].emplace_back(1, 5);  // edge 0→1 with weight 5
```

**Adjacency Matrix** — `matrix[u][v]` = weight (or 1/0). Space: O(V²). O(1) edge lookup. Preferred for dense graphs, Floyd-Warshall, or when edge existence queries dominate.

```cpp
const int INF = 1e9;
std::vector<std::vector<int>> dist(V, std::vector<int>(V, INF));
for (int i = 0; i < V; ++i) dist[i][i] = 0;
dist[0][1] = 5;   // edge 0→1 weight 5
```

### BFS — Breadth-First Search

Explores all neighbors at the current depth before going deeper. Uses a queue. Gives shortest paths in unweighted graphs.

```cpp
// Shortest path in unweighted graph (BFS)
std::vector<int> bfs_distances(const std::vector<std::vector<int>>& adj, int src) {
    int V = adj.size();
    std::vector<int> dist(V, -1);
    std::queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {   // not visited
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;   // dist[v] = shortest hops from src to v, or -1 if unreachable
}
```

**BFS properties:**
- Time: O(V + E)
- Space: O(V) for the queue and visited array
- Finds shortest path in unweighted graphs
- Level-order traversal of trees

### DFS — Depth-First Search

Explores as deep as possible before backtracking. Uses a stack (explicit or call stack via recursion).

```cpp
// Recursive DFS
void dfs(const std::vector<std::vector<int>>& adj, int u, std::vector<bool>& visited) {
    visited[u] = true;
    process(u);
    for (int v : adj[u]) {
        if (!visited[v]) dfs(adj, v, visited);
    }
}

// Iterative DFS (explicit stack — avoids stack overflow on deep graphs)
void dfs_iterative(const std::vector<std::vector<int>>& adj, int src) {
    std::vector<bool> visited(adj.size(), false);
    std::stack<int> st;
    st.push(src);
    while (!st.empty()) {
        int u = st.top(); st.pop();
        if (visited[u]) continue;
        visited[u] = true;
        process(u);
        for (int v : adj[u]) if (!visited[v]) st.push(v);
    }
}
```

**DFS applications:**
- Cycle detection
- Topological sort (on DAGs)
- Connected components
- Finding articulation points and bridges
- Maze solving

### Topological Sort (DFS-based, for DAGs)

```cpp
void topo_dfs(const std::vector<std::vector<int>>& adj, int u,
              std::vector<bool>& visited, std::stack<int>& result) {
    visited[u] = true;
    for (int v : adj[u]) if (!visited[v]) topo_dfs(adj, v, visited, result);
    result.push(u);   // push AFTER processing all descendants
}

std::vector<int> topological_sort(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::stack<int> result;
    for (int i = 0; i < V; ++i)
        if (!visited[i]) topo_dfs(adj, i, visited, result);
    std::vector<int> order;
    while (!result.empty()) { order.push_back(result.top()); result.pop(); }
    return order;
}
```

**Kahn's algorithm** — BFS-based topological sort using in-degree counts:

```cpp
std::vector<int> kahn_topo(const std::vector<std::vector<int>>& adj) {
    int V = adj.size();
    std::vector<int> in_degree(V, 0);
    for (int u = 0; u < V; ++u)
        for (int v : adj[u]) ++in_degree[v];

    std::queue<int> q;
    for (int i = 0; i < V; ++i) if (in_degree[i] == 0) q.push(i);

    std::vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u])
            if (--in_degree[v] == 0) q.push(v);
    }
    // If order.size() != V: graph has a cycle
    return order;
}
```

---

## 10.9 — Graph Algorithms: Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim

### Dijkstra's Algorithm — Single-Source Shortest Path (Non-negative weights)

Greedy: always extend the shortest known path. O((V + E) log V) with a min-heap.

```cpp
std::vector<int> dijkstra(const std::vector<std::vector<std::pair<int,int>>>& adj, int src) {
    int V = adj.size();
    std::vector<int> dist(V, INT_MAX);
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;  // min-heap: (dist, vertex)
    dist[src] = 0;
    pq.emplace(0, src);

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;   // stale entry — skip

        for (auto [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
        }
    }
    return dist;
}
```

**Limitation:** Does not work with negative edge weights. Use Bellman-Ford instead.

### Bellman-Ford — Single-Source, Handles Negative Weights

Relaxes all edges V-1 times. O(VE). Also detects negative cycles.

```cpp
struct Edge { int u, v, w; };

std::vector<int> bellman_ford(int V, const std::vector<Edge>& edges, int src) {
    std::vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    for (int i = 0; i < V - 1; ++i) {     // V-1 passes
        for (const auto& [u, v, w] : edges) {
            if (dist[u] != INT_MAX && dist[u] + w < dist[v])
                dist[v] = dist[u] + w;
        }
    }

    // Check for negative cycles — a V-th pass that still relaxes = negative cycle
    for (const auto& [u, v, w] : edges) {
        if (dist[u] != INT_MAX && dist[u] + w < dist[v])
            throw std::runtime_error("Negative cycle detected");
    }
    return dist;
}
```

### Floyd-Warshall — All-Pairs Shortest Paths

O(V³) — finds shortest paths between all pairs of vertices. Works with negative weights, detects negative cycles. Best for dense graphs where V is small.

```cpp
void floyd_warshall(std::vector<std::vector<int>>& dist) {
    int V = dist.size();
    for (int k = 0; k < V; ++k)
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
    // After: dist[i][j] = shortest path from i to j
    // Negative cycle: dist[i][i] < 0 for some i
}
```

### Minimum Spanning Tree

An MST of a weighted undirected graph connects all vertices with the minimum total edge weight. |MST| = V-1 edges.

**Kruskal's Algorithm** — sort edges by weight, add each if it doesn't form a cycle (Union-Find):

```cpp
struct UnionFind {
    std::vector<int> parent, rank;
    explicit UnionFind(int n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);  // path compression
        return parent[x];
    }
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;   // already same component — would form a cycle
        if (rank[px] < rank[py]) std::swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) ++rank[px];
        return true;
    }
};

int kruskal_mst(int V, std::vector<Edge> edges) {
    std::sort(edges.begin(), edges.end(), [](auto& a, auto& b){ return a.w < b.w; });
    UnionFind uf(V);
    int total_weight = 0, edges_added = 0;
    for (auto& [u, v, w] : edges) {
        if (uf.unite(u, v)) {
            total_weight += w;
            if (++edges_added == V - 1) break;
        }
    }
    return total_weight;   // O(E log E)
}
```

**Prim's Algorithm** — grow MST one vertex at a time, always adding the cheapest edge that connects a new vertex:

```cpp
int prim_mst(const std::vector<std::vector<std::pair<int,int>>>& adj) {
    int V = adj.size();
    std::vector<int> key(V, INT_MAX);
    std::vector<bool> in_mst(V, false);
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    key[0] = 0;
    pq.emplace(0, 0);
    int total = 0;
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (in_mst[u]) continue;
        in_mst[u] = true;
        total += d;
        for (auto [v, w] : adj[u]) {
            if (!in_mst[v] && w < key[v]) {
                key[v] = w;
                pq.emplace(w, v);
            }
        }
    }
    return total;   // O((V + E) log V)
}
```

**Kruskal vs Prim:** Kruskal is better for sparse graphs (sort fewer edges). Prim with a Fibonacci heap is asymptotically O(E + V log V) — better for dense graphs.

---

## 10.10 — Sorting: Bubble, Merge, Quick, Heap, Radix, Tim

### Complexity Overview

| Algorithm | Best | Average | Worst | Space | Stable |
|---|---|---|---|---|---|
| Bubble sort | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Insertion sort | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Selection sort | O(n²) | O(n²) | O(n²) | O(1) | No |
| Merge sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick sort | O(n log n) | O(n log n) | O(n²) | O(log n) | No |
| Heap sort | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Radix sort | O(nk) | O(nk) | O(nk) | O(n+k) | Yes |
| Tim sort | O(n) | O(n log n) | O(n log n) | O(n) | Yes |
| `std::sort` | O(n) | O(n log n) | O(n log n) | O(log n) | No |
| `std::stable_sort` | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |

### Merge Sort

Divide-and-conquer. Guaranteed O(n log n). The standard for linked lists and stable sorting.

```cpp
void merge(std::vector<int>& a, int lo, int mid, int hi) {
    std::vector<int> tmp(a.begin() + lo, a.begin() + hi + 1);
    int i = 0, j = mid - lo + 1, k = lo;
    while (i <= mid - lo && j <= hi - lo) {
        if (tmp[i] <= tmp[j]) a[k++] = tmp[i++];
        else                   a[k++] = tmp[j++];
    }
    while (i <= mid - lo) a[k++] = tmp[i++];
    while (j <= hi - lo)  a[k++] = tmp[j++];
}

void merge_sort(std::vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(a, lo, mid);
    merge_sort(a, mid + 1, hi);
    merge(a, lo, mid, hi);
}
```

### Quick Sort

Average O(n log n) in practice — better constants than merge sort. Worst-case O(n²) on already-sorted input with bad pivot choice. Pivot selection matters enormously.

```cpp
int partition(std::vector<int>& a, int lo, int hi) {
    // Median-of-three pivot selection
    int mid = lo + (hi - lo) / 2;
    if (a[mid] < a[lo]) std::swap(a[mid], a[lo]);
    if (a[hi]  < a[lo]) std::swap(a[hi],  a[lo]);
    if (a[mid] < a[hi]) std::swap(a[mid], a[hi]);
    int pivot = a[hi];  // pivot is now median of lo, mid, hi

    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        if (a[j] <= pivot) std::swap(a[++i], a[j]);
    }
    std::swap(a[i+1], a[hi]);
    return i + 1;
}

void quick_sort(std::vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(a, lo, hi);
    quick_sort(a, lo, p - 1);
    quick_sort(a, p + 1, hi);
}
```

### Heap Sort

Uses a max-heap. In-place, O(n log n) guaranteed, but poor cache behavior (random access pattern) makes it slower than quicksort in practice.

```cpp
void heap_sort(std::vector<int>& a) {
    // Build max-heap in-place
    int n = a.size();
    for (int i = n/2 - 1; i >= 0; --i) {
        // sift_down inline:
        int j = i;
        while (true) {
            int largest = j, l = 2*j+1, r = 2*j+2;
            if (l < n && a[l] > a[largest]) largest = l;
            if (r < n && a[r] > a[largest]) largest = r;
            if (largest == j) break;
            std::swap(a[j], a[largest]);
            j = largest;
        }
    }
    // Extract max repeatedly
    for (int i = n - 1; i > 0; --i) {
        std::swap(a[0], a[i]);   // move max to end
        // sift_down(a, 0, i)... abbreviated
    }
}
```

### Radix Sort

Non-comparison sort — O(nk) where k is the number of digits. Beats O(n log n) for integers with bounded key size.

```cpp
void counting_sort_by_digit(std::vector<int>& a, int exp) {
    std::vector<int> output(a.size()), count(10, 0);
    for (int x : a) ++count[(x / exp) % 10];
    for (int i = 1; i < 10; ++i) count[i] += count[i-1];
    for (int i = a.size() - 1; i >= 0; --i) {
        output[--count[(a[i] / exp) % 10]] = a[i];
    }
    a = output;
}

void radix_sort(std::vector<int>& a) {
    int max_val = *std::max_element(a.begin(), a.end());
    for (int exp = 1; max_val / exp > 0; exp *= 10)
        counting_sort_by_digit(a, exp);
}
```

### `std::sort` — TimSort

`std::sort` is IntroSort in most implementations: quicksort that switches to heapsort if recursion depth exceeds 2 log n (preventing worst-case), and insertion sort for small subarrays (≤16 elements, faster due to cache). `std::stable_sort` is typically TimSort or merge sort.

---

## 10.11 — Searching: Binary Search & variants

### Binary Search

Requires sorted array. Eliminates half the search space each step. O(log n).

```cpp
// Returns index of target, or -1
int binary_search(const std::vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;   // avoids overflow vs (lo+hi)/2
        if (a[mid] == target) return mid;
        if (a[mid] < target)  lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}
```

### Lower Bound and Upper Bound

These are the most useful binary search variants:

```cpp
// lower_bound: first index where a[i] >= target
int lower_bound(const std::vector<int>& a, int target) {
    int lo = 0, hi = a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < target) lo = mid + 1;
        else                 hi = mid;
    }
    return lo;   // a[lo] is first element >= target (or a.size() if none)
}

// upper_bound: first index where a[i] > target
int upper_bound(const std::vector<int>& a, int target) {
    int lo = 0, hi = a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] <= target) lo = mid + 1;
        else                  hi = mid;
    }
    return lo;   // a[lo] is first element > target
}

// Count occurrences of target
int count_target = upper_bound(a, target) - lower_bound(a, target);
```

The STL provides `std::lower_bound`, `std::upper_bound`, `std::binary_search`, and `std::equal_range`.

### Binary Search on the Answer

A powerful technique: when a function `f(x)` is monotone (false for small x, true for large x), binary search to find the crossover point.

```cpp
// Minimum capacity to ship packages within D days
int ship_within_days(const std::vector<int>& weights, int D) {
    int lo = *std::max_element(weights.begin(), weights.end());  // min possible capacity
    int hi = std::accumulate(weights.begin(), weights.end(), 0); // max: ship all in 1 day

    auto can_ship = [&](int capacity) {
        int days = 1, current = 0;
        for (int w : weights) {
            if (current + w > capacity) { ++days; current = 0; }
            current += w;
        }
        return days <= D;
    };

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (can_ship(mid)) hi = mid;
        else               lo = mid + 1;
    }
    return lo;
}
```

**Pattern:** Whenever you need to minimize or maximize a value subject to a condition that is monotone with respect to the value, binary search on the answer.

---

## 10.12 — Dynamic Programming: Memoization vs Tabulation

### What is DP

Dynamic programming solves problems by breaking them into overlapping subproblems and storing solutions to each subproblem — avoiding recomputation. Two conditions must hold:
1. **Optimal substructure** — optimal solution to the problem contains optimal solutions to subproblems
2. **Overlapping subproblems** — the same subproblems are solved multiple times without DP

### Memoization (Top-Down)

Recursive solution + cache. Natural to write; only computes subproblems actually needed.

```cpp
// Fibonacci with memoization
std::unordered_map<int, long long> memo;
long long fib(int n) {
    if (n <= 1) return n;
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    return memo[n] = fib(n-1) + fib(n-2);
}
```

### Tabulation (Bottom-Up)

Fill a table iteratively from smallest subproblem to largest. Usually faster (no recursion overhead, better cache behavior).

```cpp
// Fibonacci tabulation
long long fib(int n) {
    if (n <= 1) return n;
    std::vector<long long> dp(n+1);
    dp[0] = 0; dp[1] = 1;
    for (int i = 2; i <= n; ++i) dp[i] = dp[i-1] + dp[i-2];
    return dp[n];
}

// Space optimized — only need last two values
long long fib_opt(int n) {
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { long long c = a + b; a = b; b = c; }
    return b;
}
```

### 0/1 Knapsack

Classic DP — given items with weights and values, maximize value with weight capacity W.

```cpp
int knapsack(const std::vector<int>& weights, const std::vector<int>& values, int W) {
    int n = weights.size();
    std::vector<std::vector<int>> dp(n+1, std::vector<int>(W+1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= W; ++w) {
            dp[i][w] = dp[i-1][w];   // don't take item i
            if (weights[i-1] <= w)
                dp[i][w] = std::max(dp[i][w], dp[i-1][w - weights[i-1]] + values[i-1]);
        }
    }
    return dp[n][W];   // O(nW) time and space
}

// Space-optimized: O(W)
int knapsack_opt(const std::vector<int>& weights, const std::vector<int>& values, int W) {
    std::vector<int> dp(W+1, 0);
    for (int i = 0; i < (int)weights.size(); ++i)
        for (int w = W; w >= weights[i]; --w)   // reverse to avoid using item twice
            dp[w] = std::max(dp[w], dp[w - weights[i]] + values[i]);
    return dp[W];
}
```

### Longest Common Subsequence (LCS)

```cpp
int lcs(const std::string& a, const std::string& b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) {
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
            else                   dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
        }
    return dp[m][n];
}
```

### Longest Increasing Subsequence (LIS)

O(n²) DP:

```cpp
int lis(const std::vector<int>& a) {
    int n = a.size();
    std::vector<int> dp(n, 1);
    for (int i = 1; i < n; ++i)
        for (int j = 0; j < i; ++j)
            if (a[j] < a[i]) dp[i] = std::max(dp[i], dp[j] + 1);
    return *std::max_element(dp.begin(), dp.end());
}
```

O(n log n) using patience sorting (binary search):

```cpp
int lis_fast(const std::vector<int>& a) {
    std::vector<int> tails;   // tails[i] = smallest tail of IS of length i+1
    for (int x : a) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else                   *it = x;
    }
    return tails.size();
}
```

### Edit Distance (Levenshtein)

Minimum insertions, deletions, substitutions to transform string a into b:

```cpp
int edit_distance(const std::string& a, const std::string& b) {
    int m = a.size(), n = b.size();
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1));
    for (int i = 0; i <= m; ++i) dp[i][0] = i;
    for (int j = 0; j <= n; ++j) dp[0][j] = j;
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j) {
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1];
            else dp[i][j] = 1 + std::min({dp[i-1][j],    // delete from a
                                          dp[i][j-1],    // insert into a
                                          dp[i-1][j-1]}); // substitute
        }
    return dp[m][n];
}
```

---

## 10.13 — Greedy Algorithms

### What Makes a Problem Greedy

A greedy algorithm makes the locally optimal choice at each step and never reconsiders. It works only when the **greedy choice property** holds: a locally optimal choice leads to a globally optimal solution. This must be proven for each problem — greedy fails on many problems where DP is required.

### Activity Selection

Given intervals, pick the maximum number of non-overlapping intervals:

```cpp
int activity_selection(std::vector<std::pair<int,int>> intervals) {
    // Sort by end time — greedy: pick earliest-ending activity
    std::sort(intervals.begin(), intervals.end(),
              [](auto& a, auto& b){ return a.second < b.second; });

    int count = 0, last_end = INT_MIN;
    for (auto [start, end] : intervals) {
        if (start >= last_end) {   // doesn't overlap with last selected
            ++count;
            last_end = end;
        }
    }
    return count;
}
```

**Why greedy works here:** Choosing the earliest-ending activity leaves maximum time for remaining activities. A formal exchange argument proves no other choice can do better.

### Huffman Coding

Optimal prefix-free encoding: more frequent symbols get shorter codes. Build a tree greedily using a min-heap.

```cpp
struct HuffNode {
    char ch;
    int freq;
    std::unique_ptr<HuffNode> left, right;
    bool is_leaf() const { return !left && !right; }
};

auto cmp = [](const HuffNode* a, const HuffNode* b){ return a->freq > b->freq; };

std::unique_ptr<HuffNode> build_huffman(const std::unordered_map<char, int>& freq_map) {
    std::priority_queue<HuffNode*,
                        std::vector<HuffNode*>,
                        decltype(cmp)> pq(cmp);

    for (auto [ch, freq] : freq_map) {
        auto node = std::make_unique<HuffNode>();
        node->ch = ch; node->freq = freq;
        pq.push(node.release());
    }

    while (pq.size() > 1) {
        auto r = std::unique_ptr<HuffNode>(pq.top()); pq.pop();
        auto l = std::unique_ptr<HuffNode>(pq.top()); pq.pop();
        auto merged = std::make_unique<HuffNode>();
        merged->freq = l->freq + r->freq;
        merged->left = std::move(l);
        merged->right = std::move(r);
        pq.push(merged.release());
    }
    return std::unique_ptr<HuffNode>(pq.top());
}
```

### Fractional Knapsack

Unlike 0/1 knapsack (requires DP), you can take fractions of items. Greedy: pick highest value/weight ratio first.

```cpp
double fractional_knapsack(std::vector<std::pair<int,int>> items, int W) {
    // Sort by value/weight ratio descending
    std::sort(items.begin(), items.end(),
              [](auto& a, auto& b){ return (double)a.first/a.second > (double)b.first/b.second; });
    double total = 0;
    for (auto [value, weight] : items) {
        if (W >= weight) { total += value; W -= weight; }
        else             { total += (double)value * W / weight; break; }
    }
    return total;
}
```

---

## 10.14 — Backtracking

### What Backtracking Is

Backtracking is a general algorithm for finding all (or some) solutions to constraint satisfaction problems. It incrementally builds a solution and abandons ("backtracks") a partial solution as soon as it determines that it cannot lead to a valid complete solution.

**Pattern:**
```
choose → explore → unchoose (backtrack)
```

### N-Queens

Place N queens on an N×N board so no two queens attack each other:

```cpp
class NQueens {
    int n_;
    std::vector<std::vector<std::string>> solutions_;

    bool is_safe(const std::vector<int>& placement, int row, int col) {
        for (int r = 0; r < row; ++r) {
            if (placement[r] == col) return false;
            if (std::abs(placement[r] - col) == row - r) return false;  // diagonal
        }
        return true;
    }

    void solve(std::vector<int>& placement, int row) {
        if (row == n_) {
            // Convert placement to board representation
            std::vector<std::string> board(n_, std::string(n_, '.'));
            for (int r = 0; r < n_; ++r) board[r][placement[r]] = 'Q';
            solutions_.push_back(board);
            return;
        }
        for (int col = 0; col < n_; ++col) {
            if (is_safe(placement, row, col)) {
                placement[row] = col;    // choose
                solve(placement, row+1); // explore
                // unchoose happens implicitly — placement[row] is overwritten next iteration
            }
        }
    }

public:
    std::vector<std::vector<std::string>> solve(int n) {
        n_ = n;
        std::vector<int> placement(n);
        solve(placement, 0);
        return solutions_;
    }
};
```

### Sudoku Solver

```cpp
bool is_valid(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num) return false;  // row
        if (board[i][col] == num) return false;  // column
        int box_r = 3*(row/3) + i/3, box_c = 3*(col/3) + i%3;
        if (board[box_r][box_c] == num) return false;  // 3×3 box
    }
    return true;
}

bool solve_sudoku(std::vector<std::vector<int>>& board) {
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (board[r][c] != 0) continue;
            for (int num = 1; num <= 9; ++num) {
                if (is_valid(board, r, c, num)) {
                    board[r][c] = num;             // choose
                    if (solve_sudoku(board)) return true;  // explore
                    board[r][c] = 0;               // unchoose (backtrack)
                }
            }
            return false;  // no valid number — backtrack to previous cell
        }
    }
    return true;  // all cells filled
}
```

### Subsets and Permutations

```cpp
// All subsets (power set)
void subsets(const std::vector<int>& nums, int start,
             std::vector<int>& current, std::vector<std::vector<int>>& result) {
    result.push_back(current);   // add current subset (including empty)
    for (int i = start; i < (int)nums.size(); ++i) {
        current.push_back(nums[i]);      // choose
        subsets(nums, i+1, current, result);  // explore
        current.pop_back();              // unchoose
    }
}

// All permutations
void permutations(std::vector<int>& nums, int start, std::vector<std::vector<int>>& result) {
    if (start == (int)nums.size()) { result.push_back(nums); return; }
    for (int i = start; i < (int)nums.size(); ++i) {
        std::swap(nums[start], nums[i]);         // choose
        permutations(nums, start+1, result);     // explore
        std::swap(nums[start], nums[i]);         // unchoose
    }
}
```

---

## 10.15 — Bit Manipulation

### Fundamental Operations

```cpp
// Set bit n
x |= (1 << n);

// Clear bit n
x &= ~(1 << n);

// Toggle bit n
x ^= (1 << n);

// Check bit n
bool is_set = (x >> n) & 1;

// Extract lowest set bit
int lsb = x & (-x);    // or: x & (~x + 1)

// Clear lowest set bit
x &= x - 1;

// Check power of 2
bool is_pow2 = x > 0 && (x & (x - 1)) == 0;

// Count set bits (popcount)
int count = __builtin_popcount(x);    // GCC/Clang
int count = std::popcount(x);         // C++20 <bit>

// Count trailing zeros
int ctz = __builtin_ctz(x);          // GCC/Clang
int ctz = std::countr_zero(x);        // C++20

// Count leading zeros
int clz = __builtin_clz(x);
int clz = std::countl_zero(x);        // C++20

// Bit length (floor log2 + 1)
int bits = 32 - __builtin_clz(x);

// Reverse bits — swap nibbles, then bytes
uint32_t reverse_bits(uint32_t x) {
    x = ((x & 0xAAAAAAAA) >> 1)  | ((x & 0x55555555) << 1);
    x = ((x & 0xCCCCCCCC) >> 2)  | ((x & 0x33333333) << 2);
    x = ((x & 0xF0F0F0F0) >> 4)  | ((x & 0x0F0F0F0F) << 4);
    x = ((x & 0xFF00FF00) >> 8)  | ((x & 0x00FF00FF) << 8);
    x = (x >> 16) | (x << 16);
    return x;
}
```

### XOR Tricks

XOR has useful properties: `a ^ a = 0`, `a ^ 0 = a`, commutative and associative.

```cpp
// Swap without temporary
a ^= b; b ^= a; a ^= b;

// Find the single number (all others appear twice)
int single_number(const std::vector<int>& nums) {
    int result = 0;
    for (int x : nums) result ^= x;
    return result;   // all pairs cancel: a^a=0; single element remains
}

// Find two numbers that appear once (all others appear twice)
std::pair<int,int> two_singles(const std::vector<int>& nums) {
    int xor_all = 0;
    for (int x : nums) xor_all ^= x;
    // xor_all = a ^ b. Find a bit that differs (any set bit of xor_all)
    int diff_bit = xor_all & (-xor_all);
    int a = 0;
    for (int x : nums)
        if (x & diff_bit) a ^= x;
    return {a, xor_all ^ a};
}
```

### Bitmask DP — Subset Enumeration

Enumerate all subsets of a set of n elements using bitmasks:

```cpp
int n = 4;
for (int mask = 0; mask < (1 << n); ++mask) {
    // mask represents a subset — bit i is set iff element i is in subset
    for (int i = 0; i < n; ++i) {
        if (mask & (1 << i)) {
            // element i is in this subset
        }
    }
}

// Enumerate all subsets of a given mask (useful in DP)
for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
    // sub is a submask of mask
    // This loop visits every submask — O(3^n) total over all masks
}
```

**Travelling Salesman with bitmask DP** — O(2ⁿ · n²):

```cpp
int tsp(const std::vector<std::vector<int>>& dist) {
    int n = dist.size();
    // dp[mask][i] = min cost to visit all cities in mask, ending at city i
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INT_MAX/2));
    dp[1][0] = 0;   // start at city 0 (bit 0 set)

    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u)) || dp[mask][u] == INT_MAX/2) continue;
            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue;  // already visited
                int new_mask = mask | (1 << v);
                dp[new_mask][v] = std::min(dp[new_mask][v], dp[mask][u] + dist[u][v]);
            }
        }
    }

    int full = (1 << n) - 1;
    int ans = INT_MAX;
    for (int u = 1; u < n; ++u)
        if (dp[full][u] != INT_MAX/2) ans = std::min(ans, dp[full][u] + dist[u][0]);
    return ans;
}
```

### C++20 `<bit>` Header

```cpp
#include <bit>

std::popcount(x)         // number of 1 bits
std::countl_zero(x)      // count leading zeros
std::countr_zero(x)      // count trailing zeros
std::countl_one(x)       // count leading ones
std::countr_one(x)       // count trailing ones
std::bit_width(x)        // ⌊log₂(x)⌋ + 1 = position of highest set bit
std::has_single_bit(x)   // true if exactly one bit set (power of 2)
std::bit_ceil(x)         // smallest power of 2 >= x
std::bit_floor(x)        // largest power of 2 <= x
std::rotl(x, n)          // rotate bits left by n
std::rotr(x, n)          // rotate bits right by n
std::byteswap(x)         // reverse byte order (C++23)
```

All these functions work with unsigned integer types. They map directly to single CPU instructions on modern hardware (POPCNT, BSF, BSR, ROL, ROL, BSWAP).
