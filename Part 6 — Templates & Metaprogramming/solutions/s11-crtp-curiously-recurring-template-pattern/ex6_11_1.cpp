// ============================================================
// Ex 6.11.1  [Medium]
// Part 6 — Templates & Metaprogramming
// Section 6.11 — CRTP: Curiously Recurring Template Pattern
// ============================================================
//
// QUESTION:
//
// Use CRTP to create a `Comparable<Derived>` mixin that provides `!=`, `>`, `<=`, `>=` from just `==` and `<` defined in the derived class.
//
// ```cpp
// template<typename Derived>
// class Comparable {
//     // your solution
// };
//
// class Weight : public Comparable<Weight> {
//     double kg_;
// public:
//     explicit Weight(double kg) : kg_(kg) {}
//     bool operator==(const Weight& o) const { return kg_ == o.kg_; }
//     bool operator< (const Weight& o) const { return kg_ <  o.kg_; }
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
