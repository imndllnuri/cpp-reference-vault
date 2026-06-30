// ============================================================
// Ex 3.5.1  [Easy]
// Part 3 — Memory Management
// Section 3.5 — `std::shared_ptr`
// ============================================================
//
// QUESTION:
//
// What is the reference count after each line? What triggers the destructor?
//
// ```cpp
// auto a = std::make_shared<int>(10);    // count?
// auto b = a;                            // count?
// {
//     auto c = b;                        // count?
// }                                      // count after scope exit?
// a.reset();                             // count?
// b.reset();                             // count? destructor fires?
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
