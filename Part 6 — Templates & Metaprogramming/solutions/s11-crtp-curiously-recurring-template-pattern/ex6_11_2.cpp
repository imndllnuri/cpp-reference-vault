// ============================================================
// Ex 6.11.2  [Hard]
// Part 6 — Templates & Metaprogramming
// Section 6.11 — CRTP: Curiously Recurring Template Pattern
// ============================================================
//
// QUESTION:
//
// Implement a CRTP `Singleton<Derived>` base class that ensures only one instance can be created. The instance should be accessible via `Derived::instance()`.
//
// ```cpp
// template<typename Derived>
// class Singleton {
//     // your solution
// };
//
// class Config : public Singleton<Config> {
//     // only one Config can ever exist
// };
// ```
//
// ---
//
// ## Section 6.12 — Policy-Based Design
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
