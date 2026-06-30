// ============================================================
// Ex 7.4.2  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.4 — `std::mutex`, `std::lock_guard`, `std::unique_lock`
// ============================================================
//
// QUESTION:
//
// This code will deadlock. Identify exactly why and fix it using `std::scoped_lock`.
//
// ```cpp
// std::mutex mtx_a, mtx_b;
//
// void thread1() {
//     std::lock_guard la(mtx_a);
//     std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     std::lock_guard lb(mtx_b);
// }
//
// void thread2() {
//     std::lock_guard lb(mtx_b);
//     std::this_thread::sleep_for(std::chrono::milliseconds(1));
//     std::lock_guard la(mtx_a);
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
