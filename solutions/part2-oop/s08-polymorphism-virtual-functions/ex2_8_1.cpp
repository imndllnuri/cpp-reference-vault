// ============================================================
// Ex 2.8.1  [Easy]
// Part 2 — Object-Oriented Programming
// Section 2.8 — Polymorphism & Virtual Functions
// ============================================================
//
// QUESTION:
//
// What does this print? Explain each line.
//
// ```cpp
// struct Base {
//     virtual void speak() { std::cout << "Base\n"; }
//     void identify() { std::cout << "I am Base\n"; }
// };
// struct Derived : Base {
//     void speak() override { std::cout << "Derived\n"; }
//     void identify() { std::cout << "I am Derived\n"; }
// };
// int main() {
//     Derived d;
//     Base* p = &d;
//     p->speak();      // line A
//     p->identify();   // line B
//     Base b = d;
//     b.speak();       // line C
//     b.identify();    // line D
// }
// ```
//
// ```
// // your answer — explain each line
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
