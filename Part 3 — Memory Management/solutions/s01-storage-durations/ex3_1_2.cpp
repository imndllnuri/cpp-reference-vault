// ============================================================
// Ex 3.1.2  [Medium]
// Part 3 — Memory Management
// Section 3.1 — Storage Durations
// ============================================================
//
// QUESTION:
//
// This function has a bug. What is it? What would a debugger or ASan show?
//
// ```cpp
// int* get_value() {
//     int x = 42;
//     return &x;
// }
// int main() {
//     int* p = get_value();
//     std::cout << *p << "\n";   // UB
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
