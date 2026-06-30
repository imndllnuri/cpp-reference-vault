// ============================================================
// Ex 6.9.3  [Hard]
// Part 6 — Templates & Metaprogramming
// Section 6.9 — Concepts (C++20) — Replacing SFINAE
// ============================================================
//
// QUESTION:
//
// Write a concept `OutputStream` satisfied by any type that supports `operator<<` for `int`, `double`, and `std::string`. Then write a generic `Logger` class template constrained by it.
//
// ```cpp
// template<typename T>
// concept OutputStream = /* your solution */;
//
// template<OutputStream Out>
// class Logger {
//     // your solution
// };
// ```
//
// ---
//
// ## Section 6.10 — `consteval` & Compile-Time Computation
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
