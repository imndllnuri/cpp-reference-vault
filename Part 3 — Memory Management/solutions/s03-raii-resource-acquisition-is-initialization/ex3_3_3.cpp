// ============================================================
// Ex 3.3.3  [Medium]
// Part 3 — Memory Management
// Section 3.3 — RAII: Resource Acquisition Is Initialization
// ============================================================
//
// QUESTION:
//
// Implement a `ScopeGuard` class that takes any callable and invokes it in its destructor — used for cleanup actions at scope exit regardless of how the scope exits (normal return or exception).
//
// ```cpp
// class ScopeGuard {
//     // your solution
// };
//
// // Usage example:
// void foo() {
//     auto cleanup = ScopeGuard([]{ std::cout << "cleaning up\n"; });
//     // cleanup fires even if an exception is thrown
// }
// ```
//
// ---
//
// ## Section 3.4 — `std::unique_ptr`
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
