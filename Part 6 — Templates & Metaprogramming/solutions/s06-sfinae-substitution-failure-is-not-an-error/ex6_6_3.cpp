// ============================================================
// Ex 6.6.3  [Hard]
// Part 6 — Templates & Metaprogramming
// Section 6.6 — SFINAE: Substitution Failure Is Not An Error
// ============================================================
//
// QUESTION:
//
// Implement a `call_if_has_reserve(container, n)` function that calls `container.reserve(n)` only if the container has a `reserve` method (i.e., it's a vector-like container, not a list or map). Use SFINAE or `if constexpr`.
//
// ```cpp
// template<typename Container>
// void call_if_has_reserve(Container& c, size_t n) {
//     // your solution
// }
// ```
//
// ---
//
// ## Section 6.7 — Type Traits (`<type_traits>`)
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
