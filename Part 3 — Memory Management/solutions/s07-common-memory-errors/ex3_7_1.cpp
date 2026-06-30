// ============================================================
// Ex 3.7.1  [Medium]
// Part 3 — Memory Management
// Section 3.7 — Common Memory Errors
// ============================================================
//
// QUESTION:
//
// Identify the memory error type (leak, dangling pointer, double-free, buffer overflow, use-after-move) in each snippet:
//
// ```cpp
// // A
// std::string s = "hello";
// std::string t = std::move(s);
// std::cout << s.size();  // is this UB?
//
// // B
// std::vector<int> v = {1, 2, 3};
// auto it = v.begin();
// v.push_back(4);
// std::cout << *it;
//
// // C
// int* p = new int(5);
// delete p;
// *p = 10;
//
// // D
// char buf[4];
// strcpy(buf, "hello world");
//
// // E
// void* p = malloc(100);
// // p is never freed, function returns
// ```
//
// ```
// // your answer — name each error type and consequence
// ```
//
// ============================================================

// ── WHAT ─────────────────────────────────────────────────────
// What concept / feature / language rule is this about?
//
//   TODO

// ── WHY ──────────────────────────────────────────────────────
// Why does this matter? What real problem does it solve?
// When would you encounter this in production code?
//
//   TODO

// ── HOW ──────────────────────────────────────────────────────
// Approach / algorithm / steps to reach the solution.
//
//   TODO

// ── KEY INSIGHT ───────────────────────────────────────────────
// The non-obvious invariant, rule, or trade-off here.
//
//   TODO

// ── GOTCHAS / COMMON MISTAKES ─────────────────────────────────
// What breaks? What UB lurks? What would a junior get wrong?
//
//   TODO

// ------------------------------------------------------------

#include <iostream>
// TODO: add other headers as needed

// ── SOLUTION ──────────────────────────────────────────────────

// TODO: write your solution here

// ── EXAMPLE / TEST ────────────────────────────────────────────
/*
int main() {
    // TODO: demonstrate / test your solution
    return 0;
}
*/
