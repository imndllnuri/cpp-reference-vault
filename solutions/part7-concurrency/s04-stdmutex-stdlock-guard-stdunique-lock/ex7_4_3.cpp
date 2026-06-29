// ============================================================
// Ex 7.4.3  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.4 — `std::mutex`, `std::lock_guard`, `std::unique_lock`
// ============================================================
//
// QUESTION:
//
// Implement a `ReadWriteCache<K, V>` that uses `std::shared_mutex` to allow multiple concurrent readers but exclusive writers.
//
// ```cpp
// template<typename K, typename V>
// class ReadWriteCache {
//     // your solution — shared_lock for reads, unique_lock for writes
// };
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
