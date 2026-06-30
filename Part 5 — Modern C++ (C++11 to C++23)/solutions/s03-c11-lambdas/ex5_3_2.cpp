// ============================================================
// Ex 5.3.2  [Medium]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.3 — C++11: Lambdas
// ============================================================
//
// QUESTION:
//
// This lambda has a dangling reference bug. Explain when and why it crashes.
//
// ```cpp
// std::function<int()> make_adder() {
//     int base = 100;
//     return [&base]() { return base + 1; };  // bug?
// }
//
// int main() {
//     auto adder = make_adder();
//     std::cout << adder();   // what happens?
// }
// ```
//
// ```
// // your answer and fix
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
