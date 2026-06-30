# Part 8 — Systems Programming

> Low-level C++ — file I/O, POSIX, processes, signals, IPC, sockets, and the linker model. This part covers the interface between C++ and the operating system. Most of it is POSIX (Linux/macOS); Windows equivalents exist but are not covered.

## Table of Contents

- [[#8.1 — File I/O: C-style and C++ streams]]
- [[#8.2 — POSIX API Overview]]
- [[#8.3 — Processes: fork, exec, wait]]
- [[#8.4 — Signals]]
- [[#8.5 — IPC: Pipes, Shared Memory, Message Queues]]
- [[#8.6 — Sockets: TCP/UDP networking basics]]
- [[#8.7 — Memory-Mapped Files]]
- [[#8.8 — Linking: Static vs Dynamic Libraries]]

---

## 8.1 — File I/O: C-style and C++ streams

### C++ Stream I/O (`<fstream>`)

The standard C++ way to read and write files. Streams are type-safe, handle resources via RAII, and integrate with `operator<<`/`operator>>`.

```cpp
#include <fstream>
#include <string>

// Writing
std::ofstream out("output.txt");           // opens for writing, creates or truncates
std::ofstream out("log.txt", std::ios::app); // append mode — don't truncate
if (!out) throw std::runtime_error("Cannot open file");

out << "Hello, file!\n";
out << 42 << " " << 3.14 << "\n";
out.flush();   // explicit flush — destructor also flushes

// Reading — line by line
std::ifstream in("input.txt");
if (!in) throw std::runtime_error("Cannot open file");

std::string line;
while (std::getline(in, line)) {
    process(line);
}

// Reading — word by word
int n; double d;
in >> n >> d;   // reads whitespace-delimited tokens

// Reading entire file into string
std::ifstream f("data.txt");
std::string content((std::istreambuf_iterator<char>(f)),
                      std::istreambuf_iterator<char>());
```

**Open modes (combinable with `|`):**

| Mode | Effect |
|---|---|
| `std::ios::in` | Open for reading |
| `std::ios::out` | Open for writing (default for ofstream) |
| `std::ios::app` | Append — all writes go to end of file |
| `std::ios::trunc` | Truncate existing content (default with `out`) |
| `std::ios::binary` | Binary mode — no newline translation |
| `std::ios::ate` | Seek to end after opening |

**Binary I/O:**

```cpp
struct Record { int id; double value; char name[32]; };

// Write binary
std::ofstream out("data.bin", std::ios::binary);
Record r{42, 3.14, "hello"};
out.write(reinterpret_cast<const char*>(&r), sizeof(r));

// Read binary
std::ifstream in("data.bin", std::ios::binary);
Record r2;
in.read(reinterpret_cast<char*>(&r2), sizeof(r2));
if (in.gcount() != sizeof(r2)) { /* short read */ }
```

**Seeking:**

```cpp
std::fstream f("file.txt", std::ios::in | std::ios::out);
f.seekg(0, std::ios::end);     // seek get position to end
auto size = f.tellg();         // current get position = file size
f.seekg(0, std::ios::beg);     // back to beginning
f.seekp(100, std::ios::beg);   // seek put position to byte 100
```

**Stream state flags:**

```cpp
f.good()   // no errors
f.eof()    // reached end of file
f.fail()   // logical error (e.g., type mismatch on >>)
f.bad()    // I/O error (hardware/OS failure)
f.clear()  // reset all error flags
```

### `std::stringstream` — In-Memory Streams

```cpp
#include <sstream>

// Build a string using stream operators
std::ostringstream oss;
oss << "Value: " << 42 << ", Pi: " << std::fixed << std::setprecision(2) << 3.14159;
std::string result = oss.str();   // "Value: 42, Pi: 3.14"

// Parse a string using stream operators
std::istringstream iss("42 3.14 hello");
int n; double d; std::string s;
iss >> n >> d >> s;   // n=42, d=3.14, s="hello"

// Round-trip: any type with << and >> can be converted via stringstream
template<typename T>
std::string to_string(const T& val) {
    std::ostringstream oss;
    oss << val;
    return oss.str();
}
```

### C-style I/O (`<cstdio>`)

Faster than C++ streams for raw throughput. Used in performance-critical code and when interfacing with C libraries.

```cpp
#include <cstdio>

FILE* f = fopen("data.txt", "r");   // "r", "w", "a", "rb", "wb", "r+", "w+"
if (!f) { perror("fopen"); return -1; }

char buf[256];
while (fgets(buf, sizeof(buf), f)) {    // read line
    printf("Line: %s", buf);
}

// Formatted I/O
fprintf(f, "Value: %d, Pi: %.2f\n", 42, 3.14159);
fscanf(f, "%d %lf", &n, &d);

// Binary I/O
fwrite(&record, sizeof(record), 1, f);    // write 1 element of sizeof(record) bytes
fread(&record, sizeof(record), 1, f);

// Positioning
fseek(f, 0, SEEK_END);
long size = ftell(f);
fseek(f, 0, SEEK_SET);

fclose(f);
```

**RAII wrapper for FILE\*:**

```cpp
struct FileDeleter {
    void operator()(FILE* f) const { if (f) fclose(f); }
};
using UniqueFile = std::unique_ptr<FILE, FileDeleter>;

UniqueFile f(fopen("data.txt", "r"));
if (!f) throw std::runtime_error("Cannot open file");
// fclose called automatically when f goes out of scope
```

### Stream Formatting

```cpp
#include <iomanip>

// Integer formatting
std::cout << std::hex << 255;           // "ff"
std::cout << std::oct << 255;           // "377"
std::cout << std::dec << 255;           // "255"
std::cout << std::showbase << std::hex << 255;  // "0xff"
std::cout << std::uppercase << std::hex << 255; // "0XFF"

// Float formatting
std::cout << std::fixed     << std::setprecision(4) << 3.14159;  // "3.1416"
std::cout << std::scientific << std::setprecision(2) << 3.14159;  // "3.14e+00"

// Width and fill
std::cout << std::setw(10) << "hello";           // "     hello"
std::cout << std::left << std::setw(10) << "hi"; // "hi        "
std::cout << std::setfill('0') << std::setw(5) << 42;  // "00042"
```

In modern code, prefer `std::format` (C++20) over `<iomanip>` — it's cleaner and type-safe.

---

## 8.2 — POSIX API Overview

### File Descriptors

Everything in POSIX is a file — regular files, directories, sockets, pipes, devices. All are accessed through **file descriptors** (small integers):

| fd | Default |
|---|---|
| 0 | stdin |
| 1 | stdout |
| 2 | stderr |

```cpp
#include <fcntl.h>
#include <unistd.h>

// Open a file — returns a file descriptor or -1 on error
int fd = open("file.txt", O_RDONLY);
int fd = open("out.txt",  O_WRONLY | O_CREAT | O_TRUNC, 0644);  // mode when creating
if (fd == -1) { perror("open"); return -1; }

// Read / Write
char buf[4096];
ssize_t n = read(fd, buf, sizeof(buf));   // returns bytes read, 0 on EOF, -1 on error
ssize_t w = write(fd, buf, n);            // returns bytes written, -1 on error

// Always check return values — partial reads/writes are possible on pipes and sockets
while (bytes_remaining > 0) {
    ssize_t n = write(fd, ptr, bytes_remaining);
    if (n == -1) { if (errno == EINTR) continue; perror("write"); break; }
    ptr += n;
    bytes_remaining -= n;
}

// Seek
off_t pos = lseek(fd, 0, SEEK_END);   // SEEK_SET, SEEK_CUR, SEEK_END

// Close
close(fd);   // always close — file descriptor leak is like a memory leak
```

**RAII for file descriptors:**

```cpp
class FD {
    int fd_;
public:
    explicit FD(int fd) : fd_(fd) {}
    ~FD() { if (fd_ >= 0) close(fd_); }
    FD(const FD&) = delete;
    FD& operator=(const FD&) = delete;
    FD(FD&& o) noexcept : fd_(o.fd_) { o.fd_ = -1; }
    int get() const { return fd_; }
    operator int() const { return fd_; }
};
```

### Common Open Flags

```
O_RDONLY    read only
O_WRONLY    write only
O_RDWR      read and write
O_CREAT     create if not exists (requires mode argument)
O_TRUNC     truncate to zero length if exists
O_APPEND    all writes go to end of file
O_EXCL      with O_CREAT: fail if file already exists
O_NONBLOCK  non-blocking I/O (for pipes, sockets, devices)
O_SYNC      synchronous writes — wait for hardware write completion
O_CLOEXEC   close fd on exec() — prevents fd leaks across exec
```

### File Metadata (`stat`)

```cpp
#include <sys/stat.h>

struct stat st;
if (stat("file.txt", &st) == -1) { perror("stat"); return; }

st.st_size;      // file size in bytes
st.st_mtime;     // last modification time (time_t)
st.st_mode;      // permissions and file type bits
st.st_uid;       // owner user ID
st.st_gid;       // owner group ID
st.st_nlink;     // number of hard links

// Check file type
S_ISREG(st.st_mode)  // regular file
S_ISDIR(st.st_mode)  // directory
S_ISLNK(st.st_mode)  // symbolic link (use lstat to not follow links)

// Check permissions
bool readable   = st.st_mode & S_IRUSR;
bool writable   = st.st_mode & S_IWUSR;
bool executable = st.st_mode & S_IXUSR;
```

### Directory Operations

```cpp
#include <dirent.h>

DIR* dir = opendir("/home/user");
if (!dir) { perror("opendir"); return; }

struct dirent* entry;
while ((entry = readdir(dir)) != nullptr) {
    // entry->d_name — filename
    // entry->d_type — DT_REG (file), DT_DIR (dir), DT_LNK (symlink), DT_UNKNOWN
    if (entry->d_type == DT_REG) {
        std::cout << entry->d_name << "\n";
    }
}

closedir(dir);
```

Prefer `std::filesystem` (C++17) over raw POSIX directory operations — it's portable and safer. See [[Part 5 — Modern C++ (C++11 to C++23)]] section 5.9.

### `errno` and Error Handling

POSIX functions return -1 on error and set `errno` (a thread-local integer) to indicate the cause:

```cpp
#include <cerrno>
#include <cstring>

int fd = open("missing.txt", O_RDONLY);
if (fd == -1) {
    std::cerr << "open failed: " << strerror(errno) << "\n";
    // or: perror("open");  // prints "open: No such file or directory"
}

// Common errno values
// ENOENT   — no such file or directory
// EACCES   — permission denied
// EEXIST   — file already exists
// EINTR    — interrupted by signal — retry the call
// EAGAIN / EWOULDBLOCK — non-blocking operation would block — try again later
// EMFILE   — too many open files (process fd limit)
// ENOMEM   — out of memory
```

---

## 8.3 — Processes: `fork`, `exec`, `wait`

### Process Model

Each process has:
- A unique **PID** (process ID)
- Its own **virtual address space** — processes don't share memory by default
- A copy of the parent's file descriptors at fork time
- A working directory, environment variables, signal masks

### `fork` — Creating a Child Process

`fork()` creates an exact copy of the calling process. After `fork`, two processes run the same code from the same point, but have separate address spaces (copy-on-write):

```cpp
#include <unistd.h>
#include <sys/wait.h>

pid_t pid = fork();

if (pid < 0) {
    perror("fork");   // fork failed
} else if (pid == 0) {
    // Child process — pid is 0 here
    std::cout << "Child: my PID is " << getpid()
              << ", parent is " << getppid() << "\n";
    // do child work
    _exit(0);   // use _exit in child — not exit() — to avoid flushing parent's buffers
} else {
    // Parent process — pid is the child's PID
    std::cout << "Parent: child PID is " << pid << "\n";
    // wait for child
    int status;
    pid_t finished = waitpid(pid, &status, 0);  // block until child exits
    if (WIFEXITED(status)) {
        std::cout << "Child exited with code " << WEXITSTATUS(status) << "\n";
    } else if (WIFSIGNALED(status)) {
        std::cout << "Child killed by signal " << WTERMSIG(status) << "\n";
    }
}
```

**Copy-on-write:** The child's address space is initially shared with the parent — pages are only copied when either process writes to them. This makes `fork` fast even for large processes.

### `exec` — Replacing the Process Image

`exec` replaces the current process's program with a new one. The PID stays the same but everything else (code, data, stack, heap) is replaced:

```cpp
#include <unistd.h>

// execve — the fundamental exec call
char* const argv[] = {"/bin/ls", "-l", "/tmp", nullptr};
char* const envp[] = {nullptr};   // empty environment
execve("/bin/ls", argv, envp);

// Convenience wrappers
execl("/bin/ls", "ls", "-l", "/tmp", nullptr);   // variadic args, null terminated
execlp("ls", "ls", "-l", "/tmp", nullptr);        // search PATH for "ls"
execv("/bin/ls", argv);          // vector of args
execvp("ls", argv);              // search PATH + vector of args

// exec only returns on failure
perror("exec");
_exit(127);   // standard "command not found" exit code
```

### Fork + Exec Pattern

The standard Unix way to run a subprocess:

```cpp
pid_t pid = fork();
if (pid == 0) {
    // Child: replace ourselves with the new program
    execlp("grep", "grep", "-r", "pattern", "/etc", nullptr);
    perror("execlp");
    _exit(127);
} else if (pid > 0) {
    // Parent: wait for it
    int status;
    waitpid(pid, &status, 0);
}
```

### `wait` and `waitpid`

```cpp
// Wait for any child
pid_t pid = wait(&status);

// Wait for a specific child
pid_t pid = waitpid(child_pid, &status, 0);

// Non-blocking check
pid_t pid = waitpid(child_pid, &status, WNOHANG);
// Returns 0 if child hasn't finished yet

// Examine status
WIFEXITED(status)    // true if child exited normally
WEXITSTATUS(status)  // exit code (only valid if WIFEXITED)
WIFSIGNALED(status)  // true if killed by signal
WTERMSIG(status)     // signal number (only valid if WIFSIGNALED)
WIFSTOPPED(status)   // true if child was stopped (with WUNTRACED flag)
```

**Zombie processes:** When a child exits before the parent calls `wait`, the child becomes a zombie — its PID and exit status are retained until the parent calls `wait`. Long-running programs must call `waitpid` or install a `SIGCHLD` handler. Daemonizing processes typically double-fork to avoid zombies.

### `getpid`, `getppid`, `getuid`

```cpp
getpid()     // current process PID
getppid()    // parent PID
getuid()     // real user ID
geteuid()    // effective user ID (differs if setuid binary)
getgid()     // group ID
getenv("HOME")  // environment variable
```

---

## 8.4 — Signals

### What Signals Are

Signals are asynchronous notifications sent to a process. The OS delivers them at any point between instructions. Common signals:

| Signal | Default action | Meaning |
|---|---|---|
| `SIGTERM` | Terminate | Graceful termination request (`kill` default) |
| `SIGKILL` | Terminate | Forced termination — cannot be caught or ignored |
| `SIGINT` | Terminate | Ctrl+C from terminal |
| `SIGQUIT` | Core dump | Ctrl+\\ from terminal |
| `SIGHUP` | Terminate | Terminal closed; reload config (convention) |
| `SIGCHLD` | Ignore | Child process exited or stopped |
| `SIGSEGV` | Core dump | Invalid memory access (segfault) |
| `SIGABRT` | Core dump | `abort()` was called |
| `SIGPIPE` | Terminate | Write to broken pipe |
| `SIGALRM` | Terminate | Timer (from `alarm()`) expired |
| `SIGUSR1/2` | Terminate | User-defined — for application use |

### Installing Signal Handlers

```cpp
#include <signal.h>
#include <atomic>

std::atomic<bool> shutdown_requested{false};

void handle_sigterm(int sig) {
    shutdown_requested = true;   // async-signal-safe
}

// Install handler
struct sigaction sa{};
sa.sa_handler = handle_sigterm;
sigemptyset(&sa.sa_mask);       // don't block other signals during handler
sa.sa_flags = SA_RESTART;       // restart interrupted syscalls automatically
sigaction(SIGTERM, &sa, nullptr);
sigaction(SIGINT,  &sa, nullptr);

// Ignore a signal
signal(SIGPIPE, SIG_IGN);       // ignore SIGPIPE — common in network code

// Main loop
while (!shutdown_requested) {
    do_work();
}
```

### Async-Signal Safety

Signal handlers run asynchronously — they can interrupt any code, including `malloc`, `printf`, or C++ runtime functions. **Only async-signal-safe functions** can be called from a signal handler. The list is short: `write`, `read`, `_exit`, `signal`, `kill`, atomic operations, and a few others.

**Do not call** from a signal handler:
- `printf`, `std::cout`, any stdio function (not async-signal-safe)
- `malloc`, `new`, `delete`
- Any C++ exception mechanism
- Most STL functions

**Safe pattern:** set a `std::atomic<bool>` flag in the handler; check it in the main loop.

### `self-pipe` Trick

To wake a blocking `select`/`poll`/`epoll` from a signal handler:

```cpp
int pipe_fds[2];
pipe(pipe_fds);   // pipe_fds[0] = read end, pipe_fds[1] = write end

void signal_handler(int) {
    char byte = 1;
    write(pipe_fds[1], &byte, 1);   // write is async-signal-safe
}

// In the event loop — include pipe_fds[0] in the poll set
// When signal fires, the read end becomes readable, waking poll
```

### Sending Signals

```cpp
kill(pid, SIGTERM);    // send SIGTERM to process pid
kill(0,   SIGTERM);    // send to all processes in current process group
kill(-1,  SIGTERM);    // send to all processes current user can signal (dangerous)
raise(SIGUSR1);        // send signal to self
```

### Blocking Signals (`sigprocmask`)

Block signals during critical sections where you can't handle them:

```cpp
sigset_t mask, old_mask;
sigemptyset(&mask);
sigaddset(&mask, SIGTERM);
sigaddset(&mask, SIGINT);

sigprocmask(SIG_BLOCK, &mask, &old_mask);   // block SIGTERM, SIGINT
// critical section — signals are queued, not delivered
sigprocmask(SIG_SETMASK, &old_mask, nullptr); // restore — pending signals delivered now
```

---

## 8.5 — IPC: Pipes, Shared Memory, Message Queues

### Pipes — Unidirectional Byte Stream

```cpp
int fds[2];
pipe(fds);   // fds[0] = read end, fds[1] = write end

if (fork() == 0) {
    // Child: write to pipe
    close(fds[0]);                          // close unused read end
    write(fds[1], "hello", 5);
    close(fds[1]);
    _exit(0);
} else {
    // Parent: read from pipe
    close(fds[1]);                          // close unused write end
    char buf[64];
    ssize_t n = read(fds[0], buf, sizeof(buf));  // blocks until data available
    close(fds[0]);
    buf[n] = '\0';
    std::cout << "Got: " << buf << "\n";
}
```

**Key properties:**
- Unidirectional — one end reads, one end writes
- Blocking — `read` blocks when empty; `write` blocks when full (kernel buffer typically 64KB)
- `read` returns 0 (EOF) when all write ends are closed
- Anonymous pipes — only between related processes (parent/child)

### Named Pipes (FIFOs)

```cpp
mkfifo("/tmp/myfifo", 0644);   // create a named pipe — appears as a file

// Process A — opens and writes
int fd = open("/tmp/myfifo", O_WRONLY);
write(fd, "data", 4);

// Process B — opens and reads (blocks until A opens the write end)
int fd = open("/tmp/myfifo", O_RDONLY);
char buf[64];
read(fd, buf, sizeof(buf));
```

Named pipes survive across unrelated processes but not across reboots.

### POSIX Shared Memory

The fastest IPC — both processes map the same physical memory pages into their address spaces:

```cpp
#include <sys/mman.h>
#include <fcntl.h>

// Create/open shared memory object
int fd = shm_open("/myshm", O_CREAT | O_RDWR, 0600);
ftruncate(fd, sizeof(SharedData));   // set size

// Map into address space
void* ptr = mmap(nullptr, sizeof(SharedData),
                 PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
SharedData* data = static_cast<SharedData*>(ptr);

// Use it — same memory visible to all processes that map the same name
data->counter = 42;

// Cleanup
munmap(ptr, sizeof(SharedData));
close(fd);
shm_unlink("/myshm");   // delete the shared memory object
```

**Synchronization:** Shared memory gives no synchronization — you must use a `sem_t` (POSIX semaphore) or a `pthread_mutex_t` placed inside the shared region itself (initialized with `PTHREAD_PROCESS_SHARED`).

### POSIX Semaphores

```cpp
#include <semaphore.h>

// Named semaphore — visible across processes
sem_t* sem = sem_open("/mysem", O_CREAT, 0600, 1);  // initial value 1
sem_wait(sem);     // decrement — blocks if 0 (like lock)
// critical section
sem_post(sem);     // increment — wakes a waiter (like unlock)
sem_close(sem);
sem_unlink("/mysem");

// Unnamed semaphore — within a process or in shared memory
sem_t sem;
sem_init(&sem, 0, 1);   // 0 = not shared between processes, 1 = initial value
// 1 in pshared argument makes it work across forked processes (in shared memory)
sem_destroy(&sem);
```

### POSIX Message Queues

Structured message passing — each message has a priority:

```cpp
#include <mqueue.h>

// Open / create
mqd_t mq = mq_open("/myqueue", O_CREAT | O_RDWR, 0600, nullptr);

// Send
char msg[] = "hello";
mq_send(mq, msg, sizeof(msg), 0 /* priority */);

// Receive — blocks until a message is available
char buf[256];
unsigned prio;
ssize_t n = mq_receive(mq, buf, sizeof(buf), &prio);

// Cleanup
mq_close(mq);
mq_unlink("/myqueue");
```

### IPC Comparison

| Mechanism | Speed | Direction | Process boundary | Key use case |
|---|---|---|---|---|
| Pipe (anonymous) | Fast | Unidirectional | Parent-child only | Shell pipelines, subprocess I/O |
| Named pipe (FIFO) | Fast | Unidirectional | Any processes | Simple producer-consumer |
| Shared memory | Fastest | Any | Any processes | High-throughput data sharing |
| Message queue | Medium | Unidirectional | Any processes | Structured messages with priority |
| Unix socket | Fast | Bidirectional | Any processes on same host | General-purpose, stream or datagram |
| TCP socket | Moderate | Bidirectional | Any host | Network communication |

---

## 8.6 — Sockets: TCP/UDP networking basics

### The Socket API

Sockets are the POSIX interface for network communication. A socket is a file descriptor — you `read`/`write` it like any other fd.

```
Client                          Server
------                          ------
socket()                        socket()
                                bind()
                                listen()
connect() ─────────────────→   accept() → new fd
write()   ─── data ─────────→  read()
read()    ←── response ──────  write()
close()                         close()
```

### TCP Server

```cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Create socket
int server_fd = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET=IPv4, SOCK_STREAM=TCP

// Allow port reuse after crash
int opt = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

// Bind to address + port
sockaddr_in addr{};
addr.sin_family      = AF_INET;
addr.sin_addr.s_addr = INADDR_ANY;    // all interfaces
addr.sin_port        = htons(8080);   // port 8080, host-to-network byte order

bind(server_fd, (sockaddr*)&addr, sizeof(addr));

// Listen — accept up to 128 pending connections
listen(server_fd, 128);

// Accept loop
while (true) {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) { perror("accept"); continue; }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
    std::cout << "Connection from " << ip << ":" << ntohs(client_addr.sin_port) << "\n";

    // Handle client (in production: spawn a thread or use non-blocking I/O)
    char buf[4096];
    ssize_t n = read(client_fd, buf, sizeof(buf));
    write(client_fd, buf, n);   // echo back
    close(client_fd);
}
```

### TCP Client

```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);

sockaddr_in server{};
server.sin_family = AF_INET;
server.sin_port   = htons(8080);
inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);  // parse IP string

connect(sock, (sockaddr*)&server, sizeof(server));

write(sock, "Hello, server!", 14);

char buf[4096];
ssize_t n = read(sock, buf, sizeof(buf));
buf[n] = '\0';
std::cout << "Got: " << buf << "\n";

close(sock);
```

### UDP

UDP is connectionless — no handshake, no ordering guarantee, no reliability. Lower latency:

```cpp
// UDP server
int fd = socket(AF_INET, SOCK_DGRAM, 0);  // SOCK_DGRAM = UDP
// bind same as TCP...

sockaddr_in sender{};
socklen_t sender_len = sizeof(sender);
char buf[4096];
ssize_t n = recvfrom(fd, buf, sizeof(buf), 0, (sockaddr*)&sender, &sender_len);
sendto(fd, buf, n, 0, (sockaddr*)&sender, sender_len);   // reply to sender

// UDP client
int fd = socket(AF_INET, SOCK_DGRAM, 0);
// destination address set up same as TCP...
sendto(fd, "ping", 4, 0, (sockaddr*)&server, sizeof(server));
recvfrom(fd, buf, sizeof(buf), 0, nullptr, nullptr);
```

### Non-Blocking I/O and `epoll`

For high-performance servers that handle thousands of connections, blocking I/O is impractical. The modern Linux approach is `epoll` — event-driven I/O notification:

```cpp
#include <sys/epoll.h>

// Set socket to non-blocking
int flags = fcntl(server_fd, F_GETFL);
fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

// Create epoll instance
int epfd = epoll_create1(0);

// Register the server socket
epoll_event ev{};
ev.events  = EPOLLIN;             // notify when readable
ev.data.fd = server_fd;
epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

// Event loop
epoll_event events[64];
while (true) {
    int n = epoll_wait(epfd, events, 64, -1 /* timeout ms, -1=forever */);
    for (int i = 0; i < n; ++i) {
        if (events[i].data.fd == server_fd) {
            // New connection
            int client_fd = accept(server_fd, nullptr, nullptr);
            // set client_fd non-blocking, add to epoll
        } else {
            // Data on existing connection
            handle_client(events[i].data.fd);
        }
    }
}
```

`epoll` scales to millions of connections; `select`/`poll` do not (O(n) per call). This is the foundation of high-performance servers, ASIO, libevent, and libuv.

### Byte Order

Network protocols use big-endian ("network byte order"). x86 is little-endian. Always convert:

```cpp
htons(x)   // host-to-network short (16-bit)
htonl(x)   // host-to-network long (32-bit)
ntohs(x)   // network-to-host short
ntohl(x)   // network-to-host long

// C++23: std::byteswap — for explicit byte swapping
std::byteswap(uint32_t{0x12345678})  // 0x78563412
```

---

## 8.7 — Memory-Mapped Files

### `mmap` — Mapping Files into Address Space

`mmap` maps a file (or anonymous memory) directly into the process's virtual address space. Reading/writing the mapped region reads/writes the file — no explicit `read()`/`write()` calls needed:

```cpp
#include <sys/mman.h>
#include <fcntl.h>

int fd = open("large_file.dat", O_RDONLY);

struct stat st;
fstat(fd, &st);
size_t size = st.st_size;

// Map the entire file read-only
void* ptr = mmap(
    nullptr,          // let the OS choose the address
    size,             // how many bytes to map
    PROT_READ,        // protection: PROT_READ, PROT_WRITE, PROT_EXEC
    MAP_SHARED,       // MAP_SHARED (writes visible to others) or MAP_PRIVATE (copy-on-write)
    fd,               // file descriptor
    0                 // offset within the file
);

if (ptr == MAP_FAILED) { perror("mmap"); return; }

// Now access the file as memory
const char* data = static_cast<const char*>(ptr);
for (size_t i = 0; i < size; ++i) {
    process(data[i]);   // OS pages in the file data on demand (demand paging)
}

munmap(ptr, size);
close(fd);
```

### Anonymous `mmap` — Large Private Memory

```cpp
// Allocate large aligned memory — useful for custom allocators
void* mem = mmap(nullptr, 1024 * 1024 * 1024,   // 1 GB
                 PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS,     // not backed by a file
                 -1, 0);
// Pages are allocated lazily — only touched pages consume physical memory

munmap(mem, 1024 * 1024 * 1024);
```

### Advantages and Tradeoffs

**Advantages:**
- **Zero-copy:** data goes from disk to process memory without an intermediate kernel buffer copy
- **Demand paging:** only accessed pages are loaded into RAM — perfect for large files where you access a fraction
- **Shared memory between processes:** `MAP_SHARED` on the same file — both see each other's writes
- **Simpler code:** random access without `lseek` + `read` sequences

**Disadvantages:**
- **Page fault overhead:** each new page access triggers a page fault (minor, but measurable for highly scattered access)
- **No flow control:** can't rate-limit reads the way you can with `read()`
- **File size limits:** mapping more than physical + swap memory causes issues
- **Portability:** `mmap` is POSIX; Windows uses `CreateFileMapping`/`MapViewOfFile`

### `madvise` — Optimization Hints

```cpp
madvise(ptr, size, MADV_SEQUENTIAL);   // accessing pages sequentially — prefetch ahead
madvise(ptr, size, MADV_RANDOM);       // accessing randomly — don't prefetch
madvise(ptr, size, MADV_WILLNEED);     // will need these pages soon — start reading
madvise(ptr, size, MADV_DONTNEED);     // won't need these pages — can evict from RAM
```

### `msync` — Flush Changes to Disk

With `MAP_SHARED`, writes to the mapped region are not immediately written to disk:

```cpp
msync(ptr, size, MS_SYNC);     // flush and wait for completion
msync(ptr, size, MS_ASYNC);    // schedule flush, return immediately
```

---

## 8.8 — Linking: Static vs Dynamic Libraries

### What the Linker Does

After the compiler turns `.cpp` files into `.o` object files, the **linker** combines them into a final executable. It resolves **symbol references** — when `main.o` calls `foo()` defined in `util.o`, the linker patches in the address of `foo`.

```
main.cpp → [compiler] → main.o ─┐
util.cpp → [compiler] → util.o ─┤→ [linker] → executable
math.cpp → [compiler] → math.o ─┘
```

### Static Libraries (`.a`)

A static library is an archive of `.o` files. When you link against it, the linker copies the needed object files into your executable. The library becomes part of the binary.

```sh
# Create a static library
g++ -c util.cpp -o util.o
g++ -c math.cpp -o math.o
ar rcs libutil.a util.o math.o   # ar = archiver, rcs = create/update

# Link against it
g++ main.cpp -L. -lutil -o program
# -L. = look in current directory for libraries
# -lutil = link against libutil.a (or libutil.so — .a takes precedence with -static)
```

**Pros:**
- Single self-contained executable — no runtime dependencies
- Slightly faster function calls (no indirection through PLT)
- Easier deployment

**Cons:**
- Larger executables — every program using the library has its own copy
- Updating the library requires relinking all users
- Cannot share code between processes (each has its own copy in memory)

### Dynamic (Shared) Libraries (`.so`)

A shared library is loaded at program startup (or on demand) and shared between all processes using it. Only one copy in memory.

```sh
# Create a shared library
g++ -fPIC -c util.cpp -o util.o   # -fPIC = position-independent code (required for .so)
g++ -shared -o libutil.so util.o

# Link against it
g++ main.cpp -L. -lutil -o program
# At runtime, the linker needs to find libutil.so:
LD_LIBRARY_PATH=. ./program       # set search path at runtime
# or install to /usr/lib and run ldconfig
```

**Pros:**
- Smaller executables — code shared in memory between all users of the library
- Update library without relinking users (ABI compatibility permitting)
- Dynamic loading — load code at runtime with `dlopen`

**Cons:**
- Runtime dependency — library must be present and compatible at startup
- Slightly slower calls (PLT indirection on first call; PLT = Procedure Linkage Table)
- `DT_SONAME` / versioning complexity for ABI management

### `dlopen` — Runtime Dynamic Loading

```cpp
#include <dlfcn.h>

// Load a shared library at runtime
void* handle = dlopen("./plugin.so", RTLD_LAZY);
if (!handle) { std::cerr << dlerror() << "\n"; return; }

// Look up a symbol
using PluginFn = int(*)(const char*);
PluginFn plugin = reinterpret_cast<PluginFn>(dlsym(handle, "plugin_main"));
const char* err = dlerror();
if (err) { std::cerr << err << "\n"; dlclose(handle); return; }

// Call it
int result = plugin("input");

// Unload
dlclose(handle);
```

`dlopen` is the foundation of plugin systems — load extension code at runtime without recompiling the host.

### Symbol Visibility

By default, all non-static functions in a shared library are exported (visible to users). This exposes implementation details and slows link time. Control visibility with:

```cpp
// Mark a symbol as part of the public API
__attribute__((visibility("default"))) void public_api();

// Hide an implementation detail
__attribute__((visibility("hidden"))) void internal_helper();
```

Or compile with `-fvisibility=hidden` to hide everything by default and explicitly export:

```cpp
#define EXPORT __attribute__((visibility("default")))

EXPORT void public_function();
// internal_function() is hidden automatically
```

### Static Initialization Order Fiasco (SIOF)

The order of initialization of global/static variables across translation units is undefined. If global `A` in `a.cpp` depends on global `B` in `b.cpp`, `B` may not be initialized when `A`'s initializer runs:

```cpp
// a.cpp
extern int b_value;
int a_value = b_value + 1;   // b_value may be 0 (not yet initialized)

// b.cpp
int b_value = 42;
```

**Solutions:**
1. **Avoid non-trivial global variables.** Use `constexpr` for compile-time constants.
2. **Construct on first use idiom** — function-local static is initialized on first call (thread-safe since C++11):

```cpp
Config& get_config() {
    static Config cfg;   // initialized on first call, exactly once
    return cfg;
}
```

3. **`constinit`** (C++20) — ensures the variable is constant-initialized (no dynamic init).

### Linker Flags Reference

| Flag | Meaning |
|---|---|
| `-L/path` | Add directory to library search path |
| `-lname` | Link against `libname.a` or `libname.so` |
| `-static` | Force static linking |
| `-shared` | Produce a shared library |
| `-fPIC` | Generate position-independent code (required for shared libraries) |
| `-Wl,-rpath,/path` | Embed runtime library search path in the executable |
| `-Wl,--as-needed` | Only link libraries actually used (reduces startup time) |
| `-Wl,--version-script=v.map` | Control symbol visibility in shared libraries |
| `-rdynamic` | Export all symbols to the dynamic symbol table (for `dlopen`) |
