// ============================================================
// Ex 9.4.3  [Hard]
// Part 9 — Design Patterns
// Section 9.4 — Behavioral Patterns: Observer, Strategy, Command, Iterator, State
// ============================================================
//
// QUESTION:
//
// Implement a text editor with full undo/redo using the Command pattern. Support: `InsertCommand`, `DeleteCommand`, `ReplaceCommand`. The history stack must handle branching (redo is cleared on new command).
//
// ```cpp
// class TextEditor {
//     std::string buffer_;
// public:
//     void apply(std::unique_ptr<Command> cmd);
//     void undo();
//     void redo();
//     const std::string& text() const { return buffer_; }
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
