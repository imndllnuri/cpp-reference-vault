// ============================================================
// Ex 2.10.2  [Hard]
// Part 2 — Object-Oriented Programming
// Section 2.10 — Multiple Inheritance & Diamond Problem
// ============================================================
//
// QUESTION:
//
// Fix this diamond problem using virtual inheritance. Then show how to access `value` from `Diamond` unambiguously.
//
// ```cpp
// struct Device {
//     int device_id;
//     void identify() { std::cout << "Device " << device_id << "\n"; }
// };
// struct NetworkDevice : Device {};
// struct StorageDevice  : Device {};
// struct NetworkStorage : NetworkDevice, StorageDevice {};
//
// int main() {
//     NetworkStorage ns;
//     ns.identify();         // ambiguous — fix it
//     ns.device_id = 1;      // ambiguous — fix it
// }
// ```
//
// ```cpp
// // your solution
// ```
//
// ---
//
// ## Section 2.11 — Copy Constructor & Copy Assignment
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
