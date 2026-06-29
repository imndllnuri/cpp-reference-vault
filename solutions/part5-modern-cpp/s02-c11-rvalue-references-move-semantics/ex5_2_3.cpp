// ============================================================
// Ex 5.2.3  [Hard]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.2 — C++11: Rvalue References & Move Semantics
// ============================================================
//
// QUESTION:
//
// Implement a `forward` function yourself that preserves the value category of its argument (perfect forwarding). Explain the reference collapsing rules used.
//
// ```cpp
// template<typename T>
// T&& my_forward(std::remove_reference_t<T>& t) noexcept {
//     // your solution
// }
//
// template<typename T>
// T&& my_forward(std::remove_reference_t<T>&& t) noexcept {
//     // your solution
// }
// ```
//
// ---
//
// ## Section 5.3 — C++11: Lambdas
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
