// ============================================================
// Ex 9.3.3  [Medium]
// Part 9 — Design Patterns
// Section 9.3 — Structural Patterns: Adapter, Decorator, Proxy, Facade, Composite
// ============================================================
//
// QUESTION:
//
// Implement a `CachingProxy` for an `IImageLoader` that caches loaded images by filename. On the first load of a filename, call the real loader; on subsequent calls, return the cached result.
//
// ```cpp
// class IImageLoader {
// public:
//     virtual std::vector<uint8_t> load(const std::string& path) = 0;
//     virtual ~IImageLoader() = default;
// };
//
// class CachingProxy : public IImageLoader {
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
