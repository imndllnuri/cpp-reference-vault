// ============================================================
// Ex 9.5.2  [Medium]
// Part 9 — Design Patterns
// Section 9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI
// ============================================================
//
// QUESTION:
//
// Use CRTP to implement a `Cloneable<Derived>` mixin that provides a `clone() -> unique_ptr<Derived>` method to any class that inherits from it, using the derived class's copy constructor.
//
// ```cpp
// template<typename Derived>
// class Cloneable {
//     // your solution
// };
//
// class Animal : public Cloneable<Animal> {
//     std::string species_;
// public:
//     explicit Animal(std::string s) : species_(std::move(s)) {}
//     // clone() is inherited and returns unique_ptr<Animal>
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
