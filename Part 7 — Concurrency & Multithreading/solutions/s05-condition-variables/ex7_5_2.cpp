// ============================================================
// Ex 7.5.2  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.5 — Condition Variables
// ============================================================
//
// QUESTION:
//
// Why is this condition variable usage incorrect? Fix it.
//
// ```cpp
// std::mutex mtx;
// std::condition_variable cv;
// bool ready = false;
//
// // Consumer
// std::unique_lock lock(mtx);
// cv.wait(lock);          // bug — what's missing?
// use_data();
// ```
//
// ```cpp
// // your fixed version with explanation
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
