// ============================================================
// Ex 4.2.3  [Medium]
// Part 4 — Standard Template Library (STL)
// Section 4.2 — Sequence Containers: `vector`, `deque`, `list`, `array`
// ============================================================
//
// QUESTION:
//
// When does `push_back` invalidate iterators? Write code that demonstrates the bug, then fix it.
//
// ```cpp
// std::vector<int> v = {1, 2, 3};
// auto it = v.begin();
// for (int i = 4; i <= 10; ++i) v.push_back(i);
// std::cout << *it;   // is this safe? why?
// ```
//
// ```cpp
// // your fixed version
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
