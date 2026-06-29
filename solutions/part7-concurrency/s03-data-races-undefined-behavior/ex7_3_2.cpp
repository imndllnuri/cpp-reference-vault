// ============================================================
// Ex 7.3.2  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.3 — Data Races & Undefined Behavior
// ============================================================
//
// QUESTION:
//
// Without running it, predict: can this program output 42? Can it output 0? Can it crash? Explain with reference to the C++ memory model.
//
// ```cpp
// int data = 0;
// bool ready = false;
//
// std::thread writer([&]{
//     data = 42;
//     ready = true;
// });
//
// std::thread reader([&]{
//     while (!ready) {}
//     std::cout << data;
// });
//
// writer.join(); reader.join();
// ```
//
// ```
// // your answer — and the correct fix
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
