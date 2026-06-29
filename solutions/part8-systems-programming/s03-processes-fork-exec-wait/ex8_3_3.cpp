// ============================================================
// Ex 8.3.3  [Hard]
// Part 8 — Systems Programming
// Section 8.3 — Processes: `fork`, `exec`, `wait`
// ============================================================
//
// QUESTION:
//
// Implement `run_with_output(cmd, args) -> std::string` that captures the child process's stdout output using a pipe. The parent reads from the read end of the pipe after the child writes to the write end (redirected via `dup2`).
//
// ```cpp
// std::string run_with_output(const std::string& cmd, const std::vector<std::string>& args) {
//     // your solution — use pipe() + fork() + dup2() + exec() + read()
// }
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
