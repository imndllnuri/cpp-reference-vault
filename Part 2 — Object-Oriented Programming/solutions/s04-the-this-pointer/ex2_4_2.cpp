// ============================================================
// Ex 2.4.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.4 — The `this` Pointer
// ============================================================
//
// QUESTION:
//
// This `operator=` has a critical bug. What is it? Fix it.
//
// ```cpp
// class Buffer {
//     char* data_;
//     size_t size_;
// public:
//     Buffer& operator=(const Buffer& other) {
//         delete[] data_;
//         size_ = other.size_;
//         data_ = new char[size_];
//         std::memcpy(data_, other.data_, size_);
//         return *this;
//     }
// };
// ```
//
// ```cpp
// // your fixed version
// ```
//
// ---
//
// ## Section 2.5 — Static Members
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
