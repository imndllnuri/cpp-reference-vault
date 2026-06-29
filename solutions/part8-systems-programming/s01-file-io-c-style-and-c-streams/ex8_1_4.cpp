// ============================================================
// Ex 8.1.4  [Hard]
// Part 8 — Systems Programming
// Section 8.1 — File I/O: C-style and C++ streams
// ============================================================
//
// QUESTION:
//
// Implement a binary file format for serializing a `std::vector<struct Record { int id; float value; char name[32]; }>`. Write `save(path, records)` and `load(path) -> vector<Record>`. Use `static_assert` to verify `Record` is trivially copyable.
//
// ```cpp
// struct Record { int id; float value; char name[32]; };
// // your solution
// ```
//
// ---
//
// ## Section 8.2 — POSIX API Overview
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
