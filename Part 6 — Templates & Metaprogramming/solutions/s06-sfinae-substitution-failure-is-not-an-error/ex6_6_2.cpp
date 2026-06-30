// ============================================================
// Ex 6.6.2  [Medium]
// Part 6 — Templates & Metaprogramming
// Section 6.6 — SFINAE: Substitution Failure Is Not An Error
// ============================================================
//
// QUESTION:
//
// Use `std::void_t` to write a type trait `has_size_method<T>` that is true if `T` has a `.size()` member function returning something convertible to `size_t`.
//
// ```cpp
// template<typename T, typename = void>
// struct has_size_method : std::false_type {};
//
// // your specialization
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
