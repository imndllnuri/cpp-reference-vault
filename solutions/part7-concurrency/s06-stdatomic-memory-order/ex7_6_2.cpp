// ============================================================
// Ex 7.6.2  [Medium]
// Part 7 — Concurrency & Multithreading
// Section 7.6 — `std::atomic` & Memory Order
// ============================================================
//
// QUESTION:
//
// Implement a lock-free stack using `std::atomic<Node*>` and compare-and-swap. Support `push` and `pop`.
//
// ```cpp
// template<typename T>
// class LockFreeStack {
//     struct Node { T val; Node* next; };
//     std::atomic<Node*> head_{nullptr};
// public:
//     void push(T val) {
//         // your solution — CAS loop
//     }
//     std::optional<T> pop() {
//         // your solution — CAS loop
//     }
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
