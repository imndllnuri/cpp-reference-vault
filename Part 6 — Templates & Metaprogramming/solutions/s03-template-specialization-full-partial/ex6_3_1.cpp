// ============================================================
// Ex 6.3.1  [Medium]
// Part 6 — Templates & Metaprogramming
// Section 6.3 — Template Specialization: Full & Partial
// ============================================================
//
// QUESTION:
//
// Fully specialize `Serializer<T>` for `bool` (output "true"/"false") and `std::string` (output with surrounding quotes). The primary template should use `operator<<`.
//
// ```cpp
// template<typename T>
// struct Serializer {
//     static std::string serialize(const T& val) {
//         std::ostringstream oss;
//         oss << val;
//         return oss.str();
//     }
// };
//
// // your specializations
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
