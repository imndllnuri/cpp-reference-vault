// ============================================================
// Ex 7.2.4  [Hard]
// Part 7 — Concurrency & Multithreading
// Section 7.2 — `std::thread` — Creating & Joining
// ============================================================
//
// QUESTION:
//
// Write a function `parallel_for(first, last, f)` that divides the range `[first, last)` into `N = hardware_concurrency()` equal chunks and processes each chunk in a separate thread by calling `f(begin, end)` on each chunk. Join all threads before returning.
//
// ```cpp
// template<typename Iterator, typename Func>
// void parallel_for(Iterator first, Iterator last, Func f) {
//     // your solution
// }
// ```
//
// ---
//
// ## Section 7.3 — Data Races & Undefined Behavior
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
