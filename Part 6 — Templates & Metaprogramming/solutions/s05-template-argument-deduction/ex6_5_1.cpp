// ============================================================
// Ex 6.5.1  [Medium]
// Part 6 — Templates & Metaprogramming
// Section 6.5 — Template Argument Deduction
// ============================================================
//
// QUESTION:
//
// For each call, state what `T` is deduced to and explain why:
//
// ```cpp
// template<typename T> void f(T x);
// template<typename T> void g(T& x);
// template<typename T> void h(T&& x);
//
// int i = 5;
// const int ci = 5;
// int& ri = i;
//
// f(i);   // T = ?
// f(ci);  // T = ?
// g(i);   // T = ?
// g(ci);  // T = ?
// h(i);   // T = ? (forwarding reference!)
// h(5);   // T = ?
// ```
//
// ```
// // your answer
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
