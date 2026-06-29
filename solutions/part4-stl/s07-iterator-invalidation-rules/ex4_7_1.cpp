// ============================================================
// Ex 4.7.1  [Medium]
// Part 4 — Standard Template Library (STL)
// Section 4.7 — Iterator Invalidation Rules
// ============================================================
//
// QUESTION:
//
// Each snippet has an iterator invalidation bug. Identify and fix each:
//
// ```cpp
// // A — erasing while iterating
// std::vector<int> v = {1, 2, 3, 4, 5};
// for (auto it = v.begin(); it != v.end(); ++it) {
//     if (*it % 2 == 0) v.erase(it);
// }
//
// // B — map iteration with erase
// std::map<int, int> m = {{1,1},{2,2},{3,3}};
// for (auto it = m.begin(); it != m.end(); ++it) {
//     if (it->first % 2 == 0) m.erase(it);
// }
// ```
//
// ```cpp
// // your fixed versions
// ```
//
// ---
//
// ## Section 4.8 — Algorithms (`<algorithm>`)
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
