// ============================================================
// Ex 2.12.2  [Medium]
// Part 2 — Object-Oriented Programming
// Section 2.12 — Move Constructor & Move Assignment (Rule of Five)
// ============================================================
//
// QUESTION:
//
// What does this print? Trace each call to constructor/destructor.
//
// ```cpp
// struct Tracker {
//     std::string name_;
//     Tracker(std::string n) : name_(std::move(n)) { std::cout << "ctor " << name_ << "\n"; }
//     Tracker(const Tracker& o) : name_(o.name_)    { std::cout << "copy " << name_ << "\n"; }
//     Tracker(Tracker&& o) noexcept : name_(std::move(o.name_)) { std::cout << "move " << name_ << "\n"; }
//     ~Tracker() { std::cout << "dtor " << name_ << "\n"; }
// };
//
// Tracker make() { return Tracker("X"); }
//
// int main() {
//     Tracker a("A");
//     Tracker b = a;
//     Tracker c = std::move(a);
//     Tracker d = make();
// }
// ```
//
// ```
// // your answer — exact output
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
