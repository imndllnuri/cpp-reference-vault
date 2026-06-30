// ============================================================
// Ex 2.2.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.2 — Constructors & Destructors
// ============================================================
//
// QUESTION:
//
// What does this program print? Trace construction and destruction order carefully.
//
// ```cpp
// #include <iostream>
// struct A {
//     A()  { std::cout << "A()\n"; }
//     ~A() { std::cout << "~A()\n"; }
// };
// struct B {
//     A a1_, a2_;
//     B()  { std::cout << "B()\n"; }
//     ~B() { std::cout << "~B()\n"; }
// };
// struct C : B {
//     A a3_;
//     C()  { std::cout << "C()\n"; }
//     ~C() { std::cout << "~C()\n"; }
// };
// int main() { C c; }
// ```
//
// ```
// // your answer — exact output
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
