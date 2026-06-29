// ============================================================
// Ex 2.3.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.3 — Access Control
// ============================================================
//
// QUESTION:
//
// This class uses `friend`. Will it compile? What does `friend` grant here — is it too much access or exactly what's needed?
//
// ```cpp
// class Matrix {
//     int data_[4][4];
// public:
//     friend Matrix operator+(const Matrix& a, const Matrix& b);
// };
//
// Matrix operator+(const Matrix& a, const Matrix& b) {
//     Matrix result;
//     for (int i = 0; i < 4; ++i)
//         for (int j = 0; j < 4; ++j)
//             result.data_[i][j] = a.data_[i][j] + b.data_[i][j];
//     return result;
// }
// ```
//
// ```
// // your answer
// ```
//
// ---
//
// ## Section 2.4 — The `this` Pointer
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
