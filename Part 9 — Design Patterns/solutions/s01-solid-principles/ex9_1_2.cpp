// ============================================================
// Ex 9.1.2  [Medium]
// Part 9 — Design Patterns
// Section 9.1 — SOLID Principles
// ============================================================
//
// QUESTION:
//
// Show an Open/Closed violation: a `DiscountCalculator` with hardcoded `if`/`else` for customer types. Refactor it to be open for extension (new types) without modifying the calculator class.
//
// ```cpp
// // BEFORE — OCP violation
// double calculate_discount(const Customer& c) {
//     if (c.type == "VIP") return 0.3;
//     if (c.type == "Regular") return 0.1;
//     if (c.type == "New") return 0.05;
//     return 0.0;
// }
//
// // AFTER — your OCP-compliant solution
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
