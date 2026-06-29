// ============================================================
// Ex 5.4.3  [Medium]
// Part 5 — Modern C++ (C++11 to C++23)
// Section 5.4 — C++11: `constexpr`, `static_assert`, `noexcept`
// ============================================================
//
// QUESTION:
//
// Use `static_assert` to enforce that a `NetworkPacket` struct is exactly 8 bytes and has standard layout (for binary serialization over a socket).
//
// ```cpp
// struct NetworkPacket {
//     uint8_t  type;
//     uint8_t  flags;
//     uint16_t length;
//     uint32_t checksum;
// };
//
// // your static_assert(s)
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
