// ============================================================
// Ex 2.8.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.8 — Polymorphism & Virtual Functions
// ============================================================
//
// QUESTION:
//
// This code has undefined behavior. Identify it and explain the runtime consequence.
//
// ```cpp
// class Animal {
// public:
//     virtual void speak() = 0;
//     ~Animal() {}              // <-- the problem
// };
// class Dog : public Animal {
//     std::string name_;
// public:
//     Dog(std::string n) : name_(std::move(n)) {}
//     void speak() override { std::cout << name_; }
//     ~Dog() { std::cout << "~Dog\n"; }
// };
// int main() {
//     Animal* a = new Dog("Rex");
//     delete a;
// }
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
