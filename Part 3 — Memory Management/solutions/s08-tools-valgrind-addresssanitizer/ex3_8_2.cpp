// ============================================================
// Ex 3.8.2  [Medium]
// Part 3 — Memory Management
// Section 3.8 — Tools: Valgrind, AddressSanitizer
// ============================================================
//
// QUESTION:
//
// Given an ASan output that says:
//
// ```
// ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010
// READ of size 4 at 0x602000000010
//   #0 main main.cpp:7
// ```
//
// Write the minimal C++ program that would produce exactly this error (an integer heap allocation, freed, then read on line 7 of main.cpp).
//
// ```cpp
// // your solution
// ```
//
// ---
//
// ## Section 3.9 — Custom Allocators & Placement New
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
