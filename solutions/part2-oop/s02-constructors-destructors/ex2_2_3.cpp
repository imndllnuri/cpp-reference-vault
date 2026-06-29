// ============================================================
// Ex 2.2.3  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.2 — Constructors & Destructors
// ============================================================
//
// QUESTION:
//
// Mark `explicit` where appropriate and explain why each placement prevents a specific bug:
//
// ```cpp
// class Seconds {
// public:
//     Seconds(int s) : value_(s) {}    // should this be explicit?
//     // ...
// };
//
// class Buffer {
// public:
//     Buffer(int size) : data_(size) {} // and this?
//     // ...
// private:
//     std::vector<char> data_;
// };
//
// void take_seconds(Seconds s) {}
// void take_buffer(Buffer b) {}
//
// int main() {
//     take_seconds(42);   // is this desirable?
//     take_buffer(1024);  // is this desirable?
// }
// ```
//
// ```cpp
// // your solution with explanation
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
