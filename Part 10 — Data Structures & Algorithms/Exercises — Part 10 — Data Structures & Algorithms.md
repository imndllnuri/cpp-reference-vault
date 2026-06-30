# Exercises — Part 10 — Data Structures & Algorithms

> Practice problems for [[Part 10 — Data Structures & Algorithms]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 10.1 — Complexity Analysis: Big-O, Big-Theta, Big-Omega

**Ex 10.1.1** `[Easy]`

Give the Big-O time complexity of each:

```
- Accessing element at index i in std::vector
- Finding an element in std::set
- Inserting at the front of std::vector
- Inserting at the front of std::list
- std::sort on n elements
- Building a std::priority_queue from n elements
- std::unordered_map lookup (average)
- std::unordered_map lookup (worst case)
```

```
// your answer
```

---

**Ex 10.1.2** `[Medium]`

What is the time and space complexity of each function?

```cpp
// A
int sum(const std::vector<int>& v) {
    int s = 0;
    for (int x : v) s += x;
    return s;
}

// B
bool has_duplicate(const std::vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i)
        for (size_t j = i+1; j < v.size(); ++j)
            if (v[i] == v[j]) return true;
    return false;
}

// C — with extra memory
bool has_duplicate_fast(const std::vector<int>& v) {
    std::unordered_set<int> seen;
    for (int x : v)
        if (!seen.insert(x).second) return true;
    return false;
}
```

```
// your answer — time and space for each
```

---

**Ex 10.1.3** `[Medium]`

This function appears to be O(n²) but is actually O(n). Explain why.

```cpp
int count_pairs(const std::vector<int>& v) {
    int count = 0;
    std::unordered_set<int> seen;
    for (int x : v) {
        if (seen.count(x)) ++count;
        seen.insert(x);
    }
    return count;
}
```

```
// your answer
```

---

## Section 10.2 — Arrays & Strings

**Ex 10.2.1** `[Easy]`

Given a sorted `vector<int>`, find two indices `i < j` such that `v[i] + v[j] == target`. Return `{-1, -1}` if none. O(n) time, O(1) space using two pointers.

```cpp
std::pair<int,int> two_sum_sorted(const std::vector<int>& v, int target) {
    // your solution
}
```

---

**Ex 10.2.2** `[Medium]`

Given an array of integers, find the maximum product of any two elements. O(n) time.

```cpp
int max_product(const std::vector<int>& v) {
    // your solution — careful with negatives
}
```

---

**Ex 10.2.3** `[Medium]`

Given a string, find the length of the longest substring without repeating characters. O(n) using a sliding window.

```cpp
int longest_unique_substring(const std::string& s) {
    // your solution
}
```

---

**Ex 10.2.4** `[Medium]`

Implement `rotate_array(v, k)` that rotates `v` right by `k` positions in place using O(1) extra space (three reversal trick).

```cpp
void rotate_array(std::vector<int>& v, int k) {
    // your solution
}
// {1,2,3,4,5}, k=2  →  {4,5,1,2,3}
```

---

**Ex 10.2.5** `[Hard]`

Given a histogram (vector of bar heights), find the largest rectangle that fits inside it. O(n) using a monotonic stack.

```cpp
int largest_rectangle(const std::vector<int>& heights) {
    // your solution
}
// {2,1,5,6,2,3}  →  10
```

---

## Section 10.3 — Linked Lists: Singly, Doubly, Circular

**Ex 10.3.1** `[Easy]`

Implement a singly linked list with: `push_front`, `pop_front`, `push_back`, `size`, and a range-for compatible iterator.

```cpp
template<typename T>
class LinkedList {
    // your solution
};
```

---

**Ex 10.3.2** `[Medium]`

Given a singly linked list, reverse it in groups of `k` nodes. If the remaining nodes are fewer than `k`, leave them as-is.

```cpp
Node* reverse_k_group(Node* head, int k) {
    // your solution
}
// 1→2→3→4→5, k=2  →  2→1→4→3→5
```

---

**Ex 10.3.3** `[Medium]`

Given two singly linked lists, find the node where they intersect (same node by pointer, not value). O(n+m) time, O(1) space.

```cpp
Node* find_intersection(Node* a, Node* b) {
    // your solution — two-pointer approach: advance the longer list first
}
```

---

**Ex 10.3.4** `[Hard]`

Implement merge sort on a singly linked list. Use the fast/slow pointer technique to find the midpoint. O(n log n) time, O(log n) space.

```cpp
Node* merge_sort(Node* head) {
    // your solution
}
```

---

## Section 10.4 — Stacks & Queues

**Ex 10.4.1** `[Medium]`

Implement a `MinStack` that supports `push`, `pop`, `top`, and `get_min` all in O(1) time.

```cpp
class MinStack {
    // your solution — two stacks or stack of pairs
};
```

---

**Ex 10.4.2** `[Medium]`

Implement a queue using two stacks. `enqueue` is O(1) amortized; `dequeue` is O(1) amortized.

```cpp
template<typename T>
class QueueFromStacks {
    // your solution
};
```

---

**Ex 10.4.3** `[Hard]`

Given a sequence of integers, find the maximum in every contiguous subarray of size `k`. O(n) using a deque (monotonic deque pattern).

```cpp
std::vector<int> max_sliding_window(const std::vector<int>& v, int k) {
    // your solution
}
// {1,3,-1,-3,5,3,6,7}, k=3  →  {3,3,5,5,6,7}
```

---

## Section 10.5 — Hash Tables — Internals

**Ex 10.5.1** `[Medium]`

Explain with a diagram what happens when `std::unordered_map` reaches its load factor threshold. What is the load factor, and what does the rehash operation do to all existing elements?

```
// your answer
```

---

**Ex 10.5.2** `[Medium]`

Implement a simple hash map `HashMap<K, V>` using separate chaining with `std::vector<std::list<std::pair<K,V>>>`. Support `insert`, `find`, and `erase`. Rehash when load factor > 0.75.

```cpp
template<typename K, typename V>
class HashMap {
    // your solution
};
```

---

**Ex 10.5.3** `[Medium]`

Given a `vector<string>` of words, find the first word that appears exactly once. O(n) using an `unordered_map` + preserve insertion order.

```cpp
std::string first_unique(const std::vector<std::string>& words) {
    // your solution
}
```

---

## Section 10.6 — Trees: BST, AVL, Red-Black, Trie

**Ex 10.6.1** `[Medium]`

Implement a BST with: `insert`, `contains`, `erase` (handles all three cases: leaf, one child, two children), and an `inorder()` that returns sorted `vector<int>`.

```cpp
class BST {
    // your solution
};
```

---

**Ex 10.6.2** `[Medium]`

Given a BST, verify that it is a valid BST (all left subtree values < node < all right subtree values). A common wrong solution only compares with immediate parent — find the correct O(n) approach.

```cpp
bool is_valid_bst(TreeNode* root) {
    // your solution
}
```

---

**Ex 10.6.3** `[Medium]`

Implement a Trie that supports: `insert(word)`, `search(word)`, `starts_with(prefix)`, and `count_words_with_prefix(prefix)`.

```cpp
class Trie {
    // your solution
};
```

---

**Ex 10.6.4** `[Hard]`

Given a binary tree (not necessarily a BST), find the lowest common ancestor (LCA) of two given nodes. O(n) time.

```cpp
TreeNode* lca(TreeNode* root, TreeNode* p, TreeNode* q) {
    // your solution
}
```

---

**Ex 10.6.5** `[Hard]`

Serialize and deserialize a binary tree to/from a string. `serialize(root) -> string` and `deserialize(s) -> TreeNode*`. The round-trip must be lossless.

```cpp
std::string serialize(TreeNode* root) { /* your solution */ }
TreeNode* deserialize(const std::string& s) { /* your solution */ }
```

---

## Section 10.7 — Heaps & Priority Queues

**Ex 10.7.1** `[Medium]`

Implement `build_heap` from scratch: given an unsorted `vector<int>`, rearrange it in-place into a valid max-heap in O(n) time.

```cpp
void build_heap(std::vector<int>& v) {
    // your solution — sift-down from n/2-1 to 0
}
```

---

**Ex 10.7.2** `[Medium]`

Given `n` sorted arrays of total size `m`, merge them into a single sorted array in O(m log n) using a min-heap.

```cpp
std::vector<int> merge_k_sorted(const std::vector<std::vector<int>>& arrays) {
    // your solution
}
```

---

**Ex 10.7.3** `[Hard]`

Implement a median-maintenance data structure that, after each `insert(x)`, returns the current median in O(log n) per insertion. Use two heaps: a max-heap for the lower half and a min-heap for the upper half.

```cpp
class MedianTracker {
    // your solution
public:
    void insert(int x);
    double median() const;
};
```

---

## Section 10.8 — Graphs: Representation, BFS, DFS

**Ex 10.8.1** `[Medium]`

Given a grid of 0s (water) and 1s (land), count the number of islands using BFS or DFS. An island is a group of connected 1s (4-directional).

```cpp
int count_islands(std::vector<std::vector<int>>& grid) {
    // your solution
}
```

---

**Ex 10.8.2** `[Medium]`

Given a directed graph, detect if it has a cycle using DFS. Use a "visiting" (grey) / "visited" (black) coloring approach.

```cpp
bool has_cycle(int V, const std::vector<std::vector<int>>& adj) {
    // your solution
}
```

---

**Ex 10.8.3** `[Medium]`

Given a list of prerequisites `[course, prereq]`, determine if you can finish all courses (topological sort — detect cycle in a DAG). Return a valid order or empty vector if impossible.

```cpp
std::vector<int> course_schedule(int n, const std::vector<std::pair<int,int>>& prereqs) {
    // your solution — Kahn's algorithm
}
```

---

**Ex 10.8.4** `[Hard]`

Given an undirected graph, find all articulation points (vertices whose removal disconnects the graph). Use DFS with discovery time and low values.

```cpp
std::vector<int> articulation_points(int V, const std::vector<std::vector<int>>& adj) {
    // your solution
}
```

---

## Section 10.9 — Graph Algorithms: Dijkstra, Bellman-Ford, Floyd-Warshall, Kruskal, Prim

**Ex 10.9.1** `[Medium]`

Implement Dijkstra's algorithm returning not just distances but also the shortest path (predecessor array) from source to all reachable vertices.

```cpp
std::pair<std::vector<int>, std::vector<int>>
dijkstra_with_path(const std::vector<std::vector<std::pair<int,int>>>& adj, int src) {
    // your solution — returns {dist[], prev[]}
}

std::vector<int> reconstruct_path(const std::vector<int>& prev, int src, int dst) {
    // your solution
}
```

---

**Ex 10.9.2** `[Medium]`

Implement Kruskal's MST. Given a list of weighted edges, return the total MST weight and the edges in the MST.

```cpp
struct Edge { int u, v, w; };

std::pair<int, std::vector<Edge>>
kruskal(int V, std::vector<Edge> edges) {
    // your solution — Union-Find with path compression + union by rank
}
```

---

**Ex 10.9.3** `[Hard]`

Use Floyd-Warshall to find the shortest path between all pairs of cities. Then find the pair with the maximum shortest-path distance ("graph diameter").

```cpp
std::pair<int,int> graph_diameter(std::vector<std::vector<int>> dist) {
    // your solution — run FW, then find max finite dist[i][j]
}
```

---

## Section 10.10 — Sorting: Bubble, Merge, Quick, Heap, Radix, Tim

**Ex 10.10.1** `[Medium]`

Implement merge sort on `vector<int>`. Then adapt it to count the number of inversions (pairs `i < j` where `v[i] > v[j]`) during the merge step.

```cpp
long long merge_sort_count_inversions(std::vector<int>& v) {
    // your solution
}
```

---

**Ex 10.10.2** `[Medium]`

Implement QuickSelect — find the k-th smallest element in O(n) average time using the partition step of Quicksort. No full sort allowed.

```cpp
int quickselect(std::vector<int> v, int k) {
    // your solution — k is 0-indexed (k=0 means minimum)
}
```

---

**Ex 10.10.3** `[Hard]`

Sort a `vector<string>` of lowercase strings using radix sort (LSD — least significant digit first). Assume all strings have the same length.

```cpp
void radix_sort_strings(std::vector<std::string>& v) {
    // your solution
}
```

---

## Section 10.11 — Searching: Binary Search & variants

**Ex 10.11.1** `[Medium]`

A sorted array was rotated at an unknown pivot (e.g., `{4,5,6,7,0,1,2}`). Find the index of a target element in O(log n). Return -1 if not found.

```cpp
int search_rotated(const std::vector<int>& v, int target) {
    // your solution
}
```

---

**Ex 10.11.2** `[Medium]`

Given a sorted array with duplicates, find the first and last index of a target value in O(log n). Return `{-1, -1}` if not found.

```cpp
std::pair<int,int> search_range(const std::vector<int>& v, int target) {
    // your solution — two binary searches
}
```

---

**Ex 10.11.3** `[Hard]`

Given an array of integers sorted in ascending order and a function `bool condition(int x)` that is monotone (false then true), find the minimum `x` in the array where `condition(x)` is true. Apply it to find the minimum speed to deliver packages in D days.

```cpp
int min_eating_speed(const std::vector<int>& piles, int h) {
    // your solution — binary search on the answer
}
```

---

## Section 10.12 — Dynamic Programming: Memoization vs Tabulation

**Ex 10.12.1** `[Medium]`

Given a list of coin denominations and a target amount, find the minimum number of coins to make that amount. Return -1 if impossible. O(amount × coins) tabulation.

```cpp
int coin_change(const std::vector<int>& coins, int amount) {
    // your solution
}
```

---

**Ex 10.12.2** `[Medium]`

Given two strings, compute their longest common subsequence length. Then reconstruct the actual LCS string.

```cpp
std::string lcs(const std::string& a, const std::string& b) {
    // your solution — build dp table, then backtrack
}
```

---

**Ex 10.12.3** `[Medium]`

Given a `m×n` grid, count the number of unique paths from top-left to bottom-right. You can only move right or down.

```cpp
int unique_paths(int m, int n) {
    // your solution — O(m*n) DP, then optimize to O(n) space
}
```

---

**Ex 10.12.4** `[Hard]`

Given a string `s` and a dictionary `words`, determine if `s` can be segmented into space-separated dictionary words. Return all possible segmentations.

```cpp
std::vector<std::string> word_break(const std::string& s,
                                    const std::unordered_set<std::string>& words) {
    // your solution — memoized DFS
}
```

---

**Ex 10.12.5** `[Hard]`

Given a `n×n` matrix of integers, find the path from top-left to bottom-right that maximizes the sum. You can move right, down, or diagonally down-right. Return the path itself.

```cpp
std::pair<int, std::vector<std::pair<int,int>>>
max_path(const std::vector<std::vector<int>>& grid) {
    // your solution — DP + backtrack
}
```

---

## Section 10.13 — Greedy Algorithms

**Ex 10.13.1** `[Medium]`

Given intervals `[start, end]`, find the minimum number of intervals to remove so that no two intervals overlap.

```cpp
int min_removals_no_overlap(std::vector<std::pair<int,int>> intervals) {
    // your solution — sort by end time (activity selection)
}
```

---

**Ex 10.13.2** `[Medium]`

Given `n` tasks with deadlines and profits, schedule the maximum profit subset of tasks (each task takes 1 unit of time, and must be done by its deadline).

```cpp
int max_profit_schedule(std::vector<std::pair<int,int>> tasks_deadline_profit) {
    // your solution
}
```

---

**Ex 10.13.3** `[Hard]`

Implement Huffman encoding: given character frequencies, build the Huffman tree and output a `map<char, string>` of binary codes. Verify that the total encoded length is minimal.

```cpp
std::map<char, std::string> huffman_codes(const std::map<char, int>& freq) {
    // your solution
}
```

---

## Section 10.14 — Backtracking

**Ex 10.14.1** `[Medium]`

Generate all valid combinations of `k` numbers from 1 to `n` that sum to a target. Numbers can only be used once. Return all unique combinations.

```cpp
std::vector<std::vector<int>> combination_sum(int n, int k, int target) {
    // your solution
}
```

---

**Ex 10.14.2** `[Medium]`

Given a string of digits, return all possible letter combinations it could represent on a phone keypad (2=abc, 3=def, ...).

```cpp
std::vector<std::string> letter_combinations(const std::string& digits) {
    // your solution
}
```

---

**Ex 10.14.3** `[Hard]`

Implement a Sudoku solver. Given a partially filled 9×9 grid (0 = empty), fill in all cells using backtracking.

```cpp
bool solve_sudoku(std::vector<std::vector<int>>& board) {
    // your solution
}
```

---

**Ex 10.14.4** `[Hard]`

Given a `m×n` board and a word, return `true` if the word exists in the board following adjacent cells (horizontally or vertically). Each cell may only be used once.

```cpp
bool word_search(std::vector<std::vector<char>>& board, const std::string& word) {
    // your solution
}
```

---

## Section 10.15 — Bit Manipulation

**Ex 10.15.1** `[Easy]`

Implement the following without using any loops or conditionals:
- Check if a number is a power of 2
- Count the number of set bits (use `__builtin_popcount` or implement manually)
- Get the lowest set bit of `n`
- Clear the lowest set bit of `n`

```cpp
// your solution
```

---

**Ex 10.15.2** `[Medium]`

An array contains every integer from 1 to n exactly once, except one is missing. Find the missing number using XOR in O(n) time and O(1) space.

```cpp
int find_missing(const std::vector<int>& v) {
    // your solution
}
```

---

**Ex 10.15.3** `[Medium]`

Given an integer, reverse its bits (treat it as a 32-bit unsigned integer). E.g., `0b00000010100101000001111010011100` → `0b00111001011110000010100101000000`.

```cpp
uint32_t reverse_bits(uint32_t n) {
    // your solution
}
```

---

**Ex 10.15.4** `[Medium]`

Given an array where every element appears three times except one, find the element that appears only once. O(n) time, O(1) space. (XOR alone doesn't work here — think in terms of bit counting mod 3.)

```cpp
int single_number_iii(const std::vector<int>& v) {
    // your solution
}
```

---

**Ex 10.15.5** `[Hard]`

Given `n` items, enumerate all `2^n` subsets using bitmask iteration. For each subset, print the items it contains. Then find the subset with the maximum sum that does not exceed a given capacity.

```cpp
int max_subset_sum(const std::vector<int>& items, int capacity) {
    // your solution — bitmask over all subsets
}
```

---

**Ex 10.15.6** `[Hard]`

Implement the Travelling Salesman Problem using bitmask DP for `n ≤ 20` cities. Given a distance matrix, find the minimum cost Hamiltonian cycle.

```cpp
int tsp(const std::vector<std::vector<int>>& dist) {
    // your solution — O(2^n * n^2)
}
```
