// ============================================================
// Ex 3.3.1  [Easy]
// Part 3 — Memory Management
// Section 3.3 — RAII: Resource Acquisition Is Initialization
// ============================================================
//
// QUESTION:
//
// Rewrite this function to be exception-safe using RAII. The current version leaks memory if `process()` throws.
//
// ```cpp
// void bad_function() {
//     int* buf = new int[1024];
//     process(buf);          // may throw
//     delete[] buf;
// }
// ```
//
// ```cpp
// // your solution — use std::vector or unique_ptr
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
