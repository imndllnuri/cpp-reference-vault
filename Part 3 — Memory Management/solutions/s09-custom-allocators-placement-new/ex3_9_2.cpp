// ============================================================
// Ex 3.9.2  [Hard]
// Part 3 — Memory Management
// Section 3.9 — Custom Allocators & Placement New
// ============================================================
//
// QUESTION:
//
// Implement a simple `ArenaAllocator` that:
// - Takes a fixed-size `char` buffer at construction
// - Allocates objects by bumping a pointer (no individual frees)
// - Has a `reset()` that makes all memory available again
// - Works with `std::pmr::monotonic_buffer_resource` or as a standalone class
//
// ```cpp
// class ArenaAllocator {
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
