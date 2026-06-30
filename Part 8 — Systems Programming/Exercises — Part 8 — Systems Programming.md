# Exercises — Part 8 — Systems Programming

> Practice problems for [[Part 8 — Systems Programming]]. Write your solutions in `.cpp` or `.c` files. Compile on Linux. No hints, no answers.

---

## Section 8.1 — File I/O: C-style and C++ streams

**Ex 8.1.1** `[Easy]`

Write a program that reads a text file line by line and prints each line prefixed with its line number (1-indexed). Handle the case where the file doesn't exist gracefully.

```cpp
// your solution
```

---

**Ex 8.1.2** `[Medium]`

Implement `copy_file(src, dst)` using C++ streams that copies a binary file in chunks of 4KB. Return the number of bytes copied. Use `std::ios::binary` and handle errors.

```cpp
size_t copy_file(const std::string& src, const std::string& dst) {
    // your solution
}
```

---

**Ex 8.1.3** `[Medium]`

Using `std::stringstream`, implement `serialize(const std::map<std::string, int>&)` that produces a key=value text format (one per line) and `deserialize(const std::string&)` that parses it back.

```cpp
std::string serialize(const std::map<std::string, int>& m) {
    // your solution
}
std::map<std::string, int> deserialize(const std::string& s) {
    // your solution
}
```

---

**Ex 8.1.4** `[Hard]`

Implement a binary file format for serializing a `std::vector<struct Record { int id; float value; char name[32]; }>`. Write `save(path, records)` and `load(path) -> vector<Record>`. Use `static_assert` to verify `Record` is trivially copyable.

```cpp
struct Record { int id; float value; char name[32]; };
// your solution
```

---

## Section 8.2 — POSIX API Overview

**Ex 8.2.1** `[Medium]`

Write a RAII wrapper `PosixFile` around a POSIX file descriptor (int). It must: open in the constructor using `open()`, close in the destructor using `close()`, be non-copyable, be movable, and expose `read_all() -> std::string`.

```cpp
class PosixFile {
    // your solution
};
```

---

**Ex 8.2.2** `[Medium]`

Write a function `file_info(path)` using `stat()` that prints: file size, permissions (as `rwxrwxrwx` string), last modified time, and whether it's a regular file, directory, or symlink.

```cpp
void file_info(const std::string& path) {
    // your solution
}
```

---

**Ex 8.2.3** `[Medium]`

Write a function using `opendir`/`readdir` that returns a `std::vector<std::string>` of all regular files in a directory (non-recursive). Sort the result alphabetically.

```cpp
std::vector<std::string> list_files(const std::string& dir) {
    // your solution
}
```

---

## Section 8.3 — Processes: `fork`, `exec`, `wait`

**Ex 8.3.1** `[Medium]`

Write a program that forks a child process. The child prints its PID and exits with code 0. The parent waits for the child, then prints the child's exit code.

```cpp
// your solution
```

---

**Ex 8.3.2** `[Medium]`

Implement `run_command(cmd, args)` that forks a child, execs `cmd` with `args` using `execvp`, and returns the child's exit code. The parent must wait for the child.

```cpp
int run_command(const std::string& cmd, const std::vector<std::string>& args) {
    // your solution
}
```

---

**Ex 8.3.3** `[Hard]`

Implement `run_with_output(cmd, args) -> std::string` that captures the child process's stdout output using a pipe. The parent reads from the read end of the pipe after the child writes to the write end (redirected via `dup2`).

```cpp
std::string run_with_output(const std::string& cmd, const std::vector<std::string>& args) {
    // your solution — use pipe() + fork() + dup2() + exec() + read()
}
```

---

**Ex 8.3.4** `[Hard]`

What is a zombie process? Write a program that creates one intentionally. Then fix it by properly calling `waitpid`. Alternatively, show how to use a `SIGCHLD` handler to avoid zombies.

```cpp
// your solution — both versions
```

---

## Section 8.4 — Signals

**Ex 8.4.1** `[Medium]`

Write a program with a main loop that runs until `SIGTERM` or `SIGINT` is received. Use `sigaction` to install a handler that sets an `std::atomic<bool>` flag. The main loop checks this flag.

```cpp
// your solution
```

---

**Ex 8.4.2** `[Medium]`

Explain why calling `printf` from a signal handler is dangerous. Write the signal handler correctly using only async-signal-safe operations. Show the self-pipe trick to wake a blocking `poll()` from a signal handler.

```cpp
// your solution
```

---

**Ex 8.4.3** `[Hard]`

Implement a timer using `SIGALRM` and `alarm()`. Every second, the handler increments a counter. After 5 seconds, the main function prints the count and exits. Use `sigprocmask` to block `SIGALRM` during the counter read to avoid a race.

```cpp
// your solution
```

---

## Section 8.5 — IPC: Pipes, Shared Memory, Message Queues

**Ex 8.5.1** `[Medium]`

Write a program with a producer and consumer connected by an anonymous pipe. The producer generates 10 integers and writes them to the pipe. The consumer reads them and prints their sum.

```cpp
// your solution
```

---

**Ex 8.5.2** `[Hard]`

Implement a shared memory counter shared between two processes: one process increments it 10,000 times, the other reads and prints it at the end. Use `shm_open`, `mmap`, and a POSIX semaphore for synchronization. Clean up with `shm_unlink` and `sem_unlink`.

```cpp
// your solution — producer process and consumer process (or both in one program with fork)
```

---

**Ex 8.5.3** `[Hard]`

Implement a named pipe (FIFO) based logging system: a `log_server` process reads log messages from a FIFO and writes them to a file with timestamps. A `log_client` sends messages. Both run as separate programs.

```cpp
// log_server.cpp — your solution
// log_client.cpp — your solution
```

---

## Section 8.6 — Sockets: TCP/UDP networking basics

**Ex 8.6.1** `[Medium]`

Write a TCP echo server that listens on port 9000. For each connection: read up to 4096 bytes, echo them back, close the connection. Handle one connection at a time (no concurrency needed).

```cpp
// your solution
```

---

**Ex 8.6.2** `[Medium]`

Write a TCP client that connects to `localhost:9000`, sends the string "hello server\n", reads the response, and prints it.

```cpp
// your solution
```

---

**Ex 8.6.3** `[Hard]`

Extend the echo server to handle multiple clients concurrently using `fork()` — each accepted connection gets its own child process. The parent must reap children (no zombies).

```cpp
// your solution
```

---

**Ex 8.6.4** `[Hard]`

Rewrite the echo server to use `epoll` for non-blocking I/O — a single thread handles many clients. Register the server socket and each accepted client socket with `epoll`. Set all sockets to `O_NONBLOCK`.

```cpp
// your solution
```

---

## Section 8.7 — Memory-Mapped Files

**Ex 8.7.1** `[Medium]`

Write a program that memory-maps a file and counts the occurrences of a given byte value (e.g., newlines = `'\n'`) without using `read()`. Compare performance of mmap vs reading in 4KB chunks using a timer.

```cpp
// your solution
```

---

**Ex 8.7.2** `[Hard]`

Implement a persistent key-value store backed by a memory-mapped file. The file stores entries as fixed-size records `{char key[32]; char value[256];}`. Support `set(key, value)`, `get(key) -> optional<string>`, and persist changes via `msync`.

```cpp
class MmapKVStore {
    // your solution
};
```

---

## Section 8.8 — Linking: Static vs Dynamic Libraries

**Ex 8.8.1** `[Medium]`

Build a static library `libmath.a` from `add.cpp` and `multiply.cpp`. Then write `main.cpp` that links against it. Show all commands from source to executable.

```sh
# your solution — all shell commands
```

---

**Ex 8.8.2** `[Medium]`

Build a shared library `libmath.so` from the same sources. Link and run `main.cpp` against it. Show how to set `LD_LIBRARY_PATH` or use `-Wl,-rpath` so the executable finds the library at runtime.

```sh
# your solution
```

---

**Ex 8.8.3** `[Hard]`

Write a simple plugin system: a host program uses `dlopen` to load a shared library `plugin.so` at runtime, looks up a function `int plugin_run(const char*)` with `dlsym`, calls it with "hello", and prints the return value. Write both the host and the plugin.

```cpp
// plugin.cpp — your solution
// host.cpp — your solution
// Makefile or build commands — your solution
```
