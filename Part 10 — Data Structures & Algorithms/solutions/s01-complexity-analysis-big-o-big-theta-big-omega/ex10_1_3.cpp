// ============================================================
// Ex 10.1.3  [Medium]
// Part 10 — Data Structures & Algorithms
// Section 10.1 — Complexity Analysis: Big-O, Big-Theta, Big-Omega
// ============================================================
//
// QUESTION:
//
// This function appears to be O(n²) but is actually O(n). Explain why.
//
// ```cpp
// int count_pairs(const std::vector<int>& v) {
//     int count = 0;
//     std::unordered_set<int> seen;
//     for (int x : v) {
//         if (seen.count(x)) ++count;
//         seen.insert(x);
//     }
//     return count;
// }
// ```
//
// ```
// // your answer
// ```
//
// ---
//
// ## Section 10.2 — Arrays & Strings
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
