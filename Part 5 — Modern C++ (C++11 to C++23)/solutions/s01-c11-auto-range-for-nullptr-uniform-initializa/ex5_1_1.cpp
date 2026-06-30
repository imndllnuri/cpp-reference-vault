// ============================================================
// Ex 5.1.1  [Easy]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.1 — C++11: `auto`, range-for, `nullptr`, uniform initialization
// ============================================================
//
// QUESTION:
//
// What type does `auto` deduce for each variable? Does any of them produce a copy when a reference was intended?
//
// ```cpp
// std::vector<int> v = {1, 2, 3};
//
// auto a = v;              // type?
// auto b = v[0];           // type?
// auto c = v.begin();      // type?
//
// const std::vector<int>& get_v() { return v; }
// auto d = get_v();        // type? copy or reference?
// auto& e = get_v();       // type?
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
