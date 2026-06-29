// ============================================================
// Ex 6.5.2  [Medium]
// Part 6 — Templates & Metaprogramming
// Section 6.5 — Template Argument Deduction
// ============================================================
//
// QUESTION:
//
// What is the difference between `auto` and `decltype(auto)` as a return type? Write a function `identity` using each, and show a case where they return different types.
//
// ```cpp
// template<typename T>
// auto         identity_auto(T&& x)         { return std::forward<T>(x); }
//
// template<typename T>
// decltype(auto) identity_decltype(T&& x)   { return std::forward<T>(x); }
//
// // Show where they differ
// ```
//
// ---
//
// ## Section 6.6 — SFINAE: Substitution Failure Is Not An Error
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
