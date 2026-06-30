// ============================================================
// Ex 9.3.1  [Medium]
// Part 9 — Design Patterns
// Section 9.3 — Structural Patterns: Adapter, Decorator, Proxy, Facade, Composite
// ============================================================
//
// QUESTION:
//
// Write an Adapter that makes a third-party `OldStack<T>` (with `push_val`, `pop_val`, `peek`) conform to a standard `IStack<T>` interface with `push`, `pop`, `top`.
//
// ```cpp
// template<typename T>
// class OldStack {
// public:
//     void push_val(T v);
//     void pop_val();
//     T& peek();
// };
//
// template<typename T>
// class IStack {
// public:
//     virtual void push(T v) = 0;
//     virtual void pop() = 0;
//     virtual T& top() = 0;
// };
//
// template<typename T>
// class StackAdapter : public IStack<T> {
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
