// ============================================================
// Ex 0.5.2  [Medium]
// Part 0 — Environment & Toolchain
// Section 0.5 — Sanitizers
// ============================================================
//
// QUESTION:
//
// Each snippet below contains a bug. Which sanitizer (ASan, UBSan, or TSan) would catch it? Describe what the sanitizer output would say.
//
// ```cpp
// // Snippet A
// int arr[5];
// arr[5] = 42;
//
// // Snippet B
// int x = INT_MAX;
// x = x + 1;
//
// // Snippet C — two threads
// int counter = 0;
// // thread 1: counter++;
// // thread 2: counter++;
// ```
//
// ```
// // your solution
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
