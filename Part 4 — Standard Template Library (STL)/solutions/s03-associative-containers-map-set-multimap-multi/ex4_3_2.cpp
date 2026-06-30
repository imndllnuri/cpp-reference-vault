// ============================================================
// Ex 4.3.2  [Medium]
// Part 4 — Standard Template Library (STL)
// Section 4.3 — Associative Containers: `map`, `set`, `multimap`, `multiset`
// ============================================================
//
// QUESTION:
//
// This code has a subtle bug. Find it and explain the consequence.
//
// ```cpp
// std::map<std::string, int> word_count;
// std::vector<std::string> words = {"a", "b", "a", "c"};
//
// for (const auto& w : words) {
//     if (word_count.find(w) == word_count.end()) {
//         word_count[w] = 1;
//     } else {
//         word_count[w]++;    // is there a problem here vs find?
//     }
//     // vs: just doing word_count[w]++; directly — any difference?
// }
// ```
//
// ```
// // your answer — explain the operator[] insertion trap
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
