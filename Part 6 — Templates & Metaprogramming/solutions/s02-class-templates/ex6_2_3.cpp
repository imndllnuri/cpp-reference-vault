// ============================================================
// Ex 6.2.3  [Hard]
// Part 6 — Templates & Metaprogramming
// Section 6.2 — Class Templates
// ============================================================
//
// QUESTION:
//
// Implement a `TypedBuffer<T, Alignment>` class that provides correctly-aligned storage for a single `T` object without default-constructing it (like `std::optional`'s storage). Use `alignas` and `std::aligned_storage_t` (or a plain `alignas` array). Support `construct(args...)`, `destroy()`, and `get()`.
//
// ```cpp
// template<typename T, std::size_t Alignment = alignof(T)>
// class TypedBuffer {
//     // your solution
// };
// ```
//
// ---
//
// ## Section 6.3 — Template Specialization: Full & Partial
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
