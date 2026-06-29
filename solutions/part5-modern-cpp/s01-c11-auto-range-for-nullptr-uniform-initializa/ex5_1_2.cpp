// ============================================================
// Ex 5.1.2  [Medium]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.1 — C++11: `auto`, range-for, `nullptr`, uniform initialization
// ============================================================
//
// QUESTION:
//
// Explain the "Most Vexing Parse". Why does `Foo f()` not declare a variable? Show how uniform initialization fixes it. Then explain the `std::initializer_list` trap with `std::vector`.
//
// ```cpp
// // Demo the most vexing parse:
// struct Foo { Foo() {} };
//
// Foo f();        // what is this?
// Foo g{};        // what is this?
//
// std::vector<int> v1(10, 0);   // vs
// std::vector<int> v2{10, 0};   // what's the difference?
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
