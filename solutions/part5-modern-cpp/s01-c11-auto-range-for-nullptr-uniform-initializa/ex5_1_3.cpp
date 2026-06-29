// ============================================================
// Ex 5.1.3  [Medium]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.1 — C++11: `auto`, range-for, `nullptr`, uniform initialization
// ============================================================
//
// QUESTION:
//
// This code has a narrowing conversion bug that `{}` would catch but `()` does not. Demonstrate it and explain the rule.
//
// ```cpp
// double pi = 3.14159;
// int a = pi;           // silent truncation — show the value
// int b{pi};            // what happens?
// int c(pi);            // what happens?
//
// long big = 1'000'000'000'000LL;
// short s1 = big;       // silent truncation
// short s2{big};        // what happens?
// ```
//
// ```cpp
// // your answer with demonstration
// ```
//
// ---
//
// ## Section 5.2 — C++11: Rvalue References & Move Semantics
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
