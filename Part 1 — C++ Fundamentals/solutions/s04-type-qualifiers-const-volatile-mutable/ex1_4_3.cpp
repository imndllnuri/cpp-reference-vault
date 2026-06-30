// ============================================================
// Ex 1.4.3  [Medium]
// Part 1 — C++ Fundamentals
// Section 1.4 — Type Qualifiers: `const`, `volatile`, `mutable`
// ============================================================
//
// QUESTION:
//
// This class has a bug. Find it, explain why it's a bug, and fix it using `mutable`.
//
// ```cpp
// class TextProcessor {
//     std::string text_;
//     mutable int word_count_ = -1;  // -1 means not yet computed
// public:
//     explicit TextProcessor(std::string t) : text_(std::move(t)) {}
//
//     int word_count() {              // <-- bug is here
//         if (word_count_ == -1) {
//             word_count_ = count_words(text_);
//         }
//         return word_count_;
//     }
// private:
//     static int count_words(const std::string& s);
// };
//
// void print_stats(const TextProcessor& tp) {
//     std::cout << tp.word_count();   // fails to compile — why?
// }
// ```
//
// ```cpp
// // your fixed version
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
