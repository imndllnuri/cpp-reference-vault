// ============================================================
// Ex 6.7.2  [Medium]
// Part 6 — Templates & Metaprogramming
// Section 6.7 — Type Traits (`<type_traits>`)
// ============================================================
//
// QUESTION:
//
// Write a function `safe_cast<To>(from)` that uses `std::is_convertible` and `std::is_arithmetic` to only compile when the conversion is safe (both arithmetic types, no narrowing detected via `sizeof` comparison).
//
// ```cpp
// template<typename To, typename From>
// To safe_cast(From val) {
//     // your solution — static_assert to prevent unsafe casts
// }
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
