// ============================================================
// Ex 3.6.1  [Medium]
// Part 3 — Memory Management
// Section 3.6 — `std::weak_ptr` — Breaking Cycles
// ============================================================
//
// QUESTION:
//
// This code has a memory leak. Identify the cycle and fix it using `weak_ptr`.
//
// ```cpp
// struct Node {
//     std::shared_ptr<Node> next;
//     std::shared_ptr<Node> prev;
//     ~Node() { std::cout << "~Node\n"; }
// };
//
// int main() {
//     auto a = std::make_shared<Node>();
//     auto b = std::make_shared<Node>();
//     a->next = b;
//     b->prev = a;
//     // destructors never called — memory leaked
// }
// ```
//
// ```cpp
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
