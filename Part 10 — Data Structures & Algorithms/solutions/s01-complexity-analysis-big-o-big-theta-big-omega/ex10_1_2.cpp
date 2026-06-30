// ============================================================
// Ex 10.1.2  [Medium]
// Part 10 — Data Structures & Algorithms
// Section 10.1 — Complexity Analysis: Big-O, Big-Theta, Big-Omega
// ============================================================
//
// QUESTION:
//
// What is the time and space complexity of each function?
//
// ```cpp
// // A
// int sum(const std::vector<int>& v) {
//     int s = 0;
//     for (int x : v) s += x;
//     return s;
// }
//
// // B
// bool has_duplicate(const std::vector<int>& v) {
//     for (size_t i = 0; i < v.size(); ++i)
//         for (size_t j = i+1; j < v.size(); ++j)
//             if (v[i] == v[j]) return true;
//     return false;
// }
//
// // C — with extra memory
// bool has_duplicate_fast(const std::vector<int>& v) {
//     std::unordered_set<int> seen;
//     for (int x : v)
//         if (!seen.insert(x).second) return true;
//     return false;
// }
// ```
//
// ```
// // your answer — time and space for each
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
