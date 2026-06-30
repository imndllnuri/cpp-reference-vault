// ============================================================
// Ex 5.2.2  [Medium]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.2 — C++11: Rvalue References & Move Semantics
// ============================================================
//
// QUESTION:
//
// Explain why returning `std::move(v)` is wrong in this function and what the compiler does instead:
//
// ```cpp
// std::vector<int> make_vector() {
//     std::vector<int> v = {1, 2, 3, 4, 5};
//     return std::move(v);  // why is this wrong?
// }
// ```
//
// ```
// // your answer
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
