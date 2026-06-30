// ============================================================
// Ex 2.7.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.7 — Inheritance & the is-a Relationship
// ============================================================
//
// QUESTION:
//
// Does this violate LSP? Explain why or why not. If it does, propose a fix.
//
// ```cpp
// class Bird {
// public:
//     virtual void fly() { std::cout << "Flying\n"; }
// };
//
// class Penguin : public Bird {
// public:
//     void fly() override {
//         throw std::logic_error("Penguins can't fly!");
//     }
// };
//
// void make_it_fly(Bird& b) { b.fly(); }
// ```
//
// ```
// // your answer
// ```
//
// ---
//
// ## Section 2.8 — Polymorphism & Virtual Functions
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
