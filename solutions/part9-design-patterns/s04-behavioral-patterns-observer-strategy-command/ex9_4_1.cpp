// ============================================================
// Ex 9.4.1  [Medium]
// Part 9 — Design Patterns
// Section 9.4 — Behavioral Patterns: Observer, Strategy, Command, Iterator, State
// ============================================================
//
// QUESTION:
//
// Implement an event system where `EventBus` allows subscribing to named events with a callback. Multiple subscribers for the same event are all notified. Use `std::function` and `std::string` event names.
//
// ```cpp
// class EventBus {
// public:
//     using Handler = std::function<void(const std::any& data)>;
//     void subscribe(const std::string& event, Handler h);
//     void publish(const std::string& event, const std::any& data = {});
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
