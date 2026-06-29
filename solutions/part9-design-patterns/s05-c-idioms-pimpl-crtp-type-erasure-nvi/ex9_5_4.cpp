// ============================================================
// Ex 9.5.4  [Hard]
// Part 9 — Design Patterns
// Section 9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI
// ============================================================
//
// QUESTION:
//
// Implement NVI on a `DataProcessor` base class. The public interface is `process(data)` which: validates input, calls the protected virtual `do_process`, then logs the result. Subclasses only override `do_process` — they cannot bypass validation or logging.
//
// ```cpp
// class DataProcessor {
// public:
//     std::string process(const std::string& data) {
//         // your solution — validate, call do_process, log
//     }
// private:
//     virtual std::string do_process(const std::string& data) = 0;
//     virtual bool validate(const std::string& data) { return !data.empty(); }
//     virtual void log(const std::string& result) { std::cout << result << "\n"; }
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
