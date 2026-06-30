// ============================================================
// Ex 8.2.1  [Medium]
// Part 8 — Systems Programming
// Section 8.2 — POSIX API Overview
// ============================================================
//
// QUESTION:
//
// Write a RAII wrapper `PosixFile` around a POSIX file descriptor (int). It must: open in the constructor using `open()`, close in the destructor using `close()`, be non-copyable, be movable, and expose `read_all() -> std::string`.
//
// ```cpp
// class PosixFile {
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
