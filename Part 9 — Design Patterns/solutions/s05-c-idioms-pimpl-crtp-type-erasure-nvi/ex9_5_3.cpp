// ============================================================
// Ex 9.5.3  [Hard]
// Part 9 — Design Patterns
// Section 9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI
// ============================================================
//
// QUESTION:
//
// Implement a `Callable` type-erased wrapper (simplified `std::function`) that can hold any callable taking an `int` and returning an `int`. Support copy and call. Implement using the Concept/Model pattern (virtual base + templated concrete).
//
// ```cpp
// class Callable {
//     struct Concept {
//         virtual int invoke(int) const = 0;
//         virtual std::unique_ptr<Concept> clone() const = 0;
//         virtual ~Concept() = default;
//     };
//     // your full solution
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
