// ============================================================
// Ex 9.2.3  [Medium]
// Part 9 — Design Patterns
// Section 9.2 — Creational Patterns: Singleton, Factory, Builder, Prototype
// ============================================================
//
// QUESTION:
//
// Implement a `QueryBuilder` using the Builder pattern for constructing SQL SELECT statements:
//
// ```cpp
// auto query = QueryBuilder()
//     .select({"id", "name", "email"})
//     .from("users")
//     .where("age > 18")
//     .where("active = true")
//     .order_by("name")
//     .limit(100)
//     .build();
// // produces: "SELECT id, name, email FROM users WHERE age > 18 AND active = true ORDER BY name LIMIT 100"
// ```
//
// ```cpp
// class QueryBuilder {
//     // your solution
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
