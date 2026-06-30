// ============================================================
// Ex 3.4.2  [Medium]
// Part 3 — Memory Management
// Section 3.4 — `std::unique_ptr`
// ============================================================
//
// QUESTION:
//
// `unique_ptr` is non-copyable. This code doesn't compile. Fix it the correct way — don't make it copyable.
//
// ```cpp
// std::unique_ptr<int> make_int() { return std::make_unique<int>(42); }
//
// void take_ownership(std::unique_ptr<int> p) {
//     std::cout << *p << "\n";
// }
//
// int main() {
//     auto p = make_int();
//     take_ownership(p);    // error
// }
// ```
//
// ```cpp
// // your solution
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
