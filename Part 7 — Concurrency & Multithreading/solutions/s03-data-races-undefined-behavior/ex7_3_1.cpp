// ============================================================
// Ex 7.3.1  [Easy]
// Part 7 — Concurrency & Multithreading
// Section 7.3 — Data Races & Undefined Behavior
// ============================================================
//
// QUESTION:
//
// Define a data race precisely (three conditions). Does this code have a data race? Explain.
//
// ```cpp
// int x = 0;
// std::thread t1([&]{ x = 1; });
// std::thread t2([&]{ x = 2; });
// t1.join(); t2.join();
// std::cout << x;
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
