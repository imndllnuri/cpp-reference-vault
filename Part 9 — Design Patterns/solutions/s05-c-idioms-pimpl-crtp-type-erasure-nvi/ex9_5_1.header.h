// ============================================================
// Ex 9.5.1  [Medium]  — HEADER FILE
// Part 9 — Design Patterns
// Section 9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI
// ============================================================
//
// QUESTION (see companion .cpp for full notes):
//
// Implement a `NetworkConnection` class using PIMPL. The public header must include nothing from `<sys/socket.h>` — all POSIX includes go only in the `.cpp`. Provide: `connect(host, port)`, `send(data)`, `receive() -> string`, `disconnect()`.
//
// ```cpp
// // network_connection.h — your solution (no POSIX headers)
// // network_connection.cpp — your solution (POSIX headers here only)
// ```
//
// ============================================================

#pragma once
#ifndef EX9_5_1_H
#define EX9_5_1_H

// TODO: public interface declarations go here

#endif // EX9_5_1_H
