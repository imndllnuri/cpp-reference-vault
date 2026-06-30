// ============================================================
// Ex 2.5.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.5 — Static Members
// ============================================================
//
// QUESTION:
//
// What does this program print? Is `instance_count_` initialized before `main` runs?
//
// ```cpp
// class Widget {
//     static int instance_count_;
// public:
//     Widget()  { ++instance_count_; }
//     ~Widget() { --instance_count_; }
//     static int count() { return instance_count_; }
// };
// int Widget::instance_count_ = 0;
//
// Widget global_widget;
//
// int main() {
//     std::cout << Widget::count() << "\n";
//     Widget a, b;
//     std::cout << Widget::count() << "\n";
//     {
//         Widget c;
//         std::cout << Widget::count() << "\n";
//     }
//     std::cout << Widget::count() << "\n";
// }
// ```
//
// ```
// // your answer — exact output
// ```
//
// ---
//
// ## Section 2.6 — Operator Overloading
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
