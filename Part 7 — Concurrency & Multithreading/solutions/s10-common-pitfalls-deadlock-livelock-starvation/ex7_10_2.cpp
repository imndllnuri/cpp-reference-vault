// ============================================================
// Ex 7.10.2  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.10 — Common Pitfalls: Deadlock, Livelock, Starvation
// ============================================================
//
// QUESTION:
//
// Why is the double-checked locking pattern (DCLP) broken without `std::atomic`? Show the broken code, explain the race, and show the correct alternatives (static local and `call_once`).
//
// ```cpp
// // BROKEN:
// Singleton* instance() {
//     if (ptr == nullptr) {
//         std::lock_guard lock(mtx);
//         if (ptr == nullptr) ptr = new Singleton();
//     }
//     return ptr;
// }
//
// // CORRECT version 1 — static local
// // your solution
//
// // CORRECT version 2 — call_once
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
