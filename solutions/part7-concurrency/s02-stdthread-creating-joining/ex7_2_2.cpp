// ============================================================
// Ex 7.2.2  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.2 — `std::thread` — Creating & Joining
// ============================================================
//
// QUESTION:
//
// This code has undefined behavior. Identify all problems and fix them.
//
// ```cpp
// void do_work(int& result) { result = 42; }
//
// int main() {
//     int result;
//     std::thread t(do_work, result);   // problem A
//     std::cout << result;              // problem B
//     // thread t is never joined      // problem C
// }
// ```
//
// ```cpp
// // your fixed version
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
