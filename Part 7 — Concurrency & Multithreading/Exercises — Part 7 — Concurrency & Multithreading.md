# Exercises — Part 7 — Concurrency & Multithreading

> Practice problems for [[Part 7 — Concurrency & Multithreading]]. Write your solutions in `.cpp` files. Compile with `-pthread`. No hints, no answers.

---

## Section 7.1 — Why Concurrency? Problems it Solves

**Ex 7.1.1** `[Easy]`

Name the two distinct reasons to use concurrency. Give a real-world example for each where the other reason would NOT apply.

```
// your answer
```

---

**Ex 7.1.2** `[Medium]`

What are the four classical concurrency hazards? For each, give a one-sentence definition and a concrete scenario where it occurs.

```
// your answer
```

---

## Section 7.2 — `std::thread` — Creating & Joining

**Ex 7.2.1** `[Easy]`

Write a program that launches 5 threads, each printing its own ID (0–4) and the result of computing `id * id`. Join all threads before exiting.

```cpp
// your solution
```

---

**Ex 7.2.2** `[Medium]`

This code has undefined behavior. Identify all problems and fix them.

```cpp
void do_work(int& result) { result = 42; }

int main() {
    int result;
    std::thread t(do_work, result);   // problem A
    std::cout << result;              // problem B
    // thread t is never joined      // problem C
}
```

```cpp
// your fixed version
```

---

**Ex 7.2.3** `[Medium]`

Implement a `JoinGuard` RAII wrapper for `std::thread` that automatically joins the thread in its destructor if it is still joinable. Make it movable but not copyable.

```cpp
class JoinGuard {
    // your solution
};
```

---

**Ex 7.2.4** `[Hard]`

Write a function `parallel_for(first, last, f)` that divides the range `[first, last)` into `N = hardware_concurrency()` equal chunks and processes each chunk in a separate thread by calling `f(begin, end)` on each chunk. Join all threads before returning.

```cpp
template<typename Iterator, typename Func>
void parallel_for(Iterator first, Iterator last, Func f) {
    // your solution
}
```

---

## Section 7.3 — Data Races & Undefined Behavior

**Ex 7.3.1** `[Easy]`

Define a data race precisely (three conditions). Does this code have a data race? Explain.

```cpp
int x = 0;
std::thread t1([&]{ x = 1; });
std::thread t2([&]{ x = 2; });
t1.join(); t2.join();
std::cout << x;
```

```
// your answer
```

---

**Ex 7.3.2** `[Medium]`

Without running it, predict: can this program output 42? Can it output 0? Can it crash? Explain with reference to the C++ memory model.

```cpp
int data = 0;
bool ready = false;

std::thread writer([&]{
    data = 42;
    ready = true;
});

std::thread reader([&]{
    while (!ready) {}
    std::cout << data;
});

writer.join(); reader.join();
```

```
// your answer — and the correct fix
```

---

**Ex 7.3.3** `[Medium]`

Write a program that reliably demonstrates a data race on a counter (incremented by multiple threads simultaneously). Then run it under TSan (compile with `-fsanitize=thread`) and describe what TSan reports.

```cpp
// your solution
```

---

## Section 7.4 — `std::mutex`, `std::lock_guard`, `std::unique_lock`

**Ex 7.4.1** `[Medium]`

Implement a thread-safe `Counter` class with `increment()`, `decrement()`, and `value()` using `std::mutex` and `std::lock_guard`.

```cpp
class Counter {
    // your solution
};
```

---

**Ex 7.4.2** `[Medium]`

This code will deadlock. Identify exactly why and fix it using `std::scoped_lock`.

```cpp
std::mutex mtx_a, mtx_b;

void thread1() {
    std::lock_guard la(mtx_a);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::lock_guard lb(mtx_b);
}

void thread2() {
    std::lock_guard lb(mtx_b);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::lock_guard la(mtx_a);
}
```

```cpp
// your fixed version
```

---

**Ex 7.4.3** `[Medium]`

Implement a `ReadWriteCache<K, V>` that uses `std::shared_mutex` to allow multiple concurrent readers but exclusive writers.

```cpp
template<typename K, typename V>
class ReadWriteCache {
    // your solution — shared_lock for reads, unique_lock for writes
};
```

---

**Ex 7.4.4** `[Hard]`

Implement a thread-safe `LRUCache<K, V>` with a fixed capacity. Evict the least recently used entry on overflow. Use a `std::mutex` for thread safety.

```cpp
template<typename K, typename V>
class LRUCache {
    // your solution
};
```

---

## Section 7.5 — Condition Variables

**Ex 7.5.1** `[Medium]`

Implement a simple `Semaphore` class using a `std::mutex` and `std::condition_variable`. Support `acquire()` (blocks when count is 0) and `release()`.

```cpp
class Semaphore {
    // your solution
};
```

---

**Ex 7.5.2** `[Medium]`

Why is this condition variable usage incorrect? Fix it.

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// Consumer
std::unique_lock lock(mtx);
cv.wait(lock);          // bug — what's missing?
use_data();
```

```cpp
// your fixed version with explanation
```

---

**Ex 7.5.3** `[Hard]`

Implement a `Pipeline<T>` with two stages connected by a bounded queue: one producer thread feeds items into the queue, one consumer thread processes them. Both threads run until the producer signals completion. Use condition variables.

```cpp
template<typename T>
class Pipeline {
    // your solution
};
```

---

## Section 7.6 — `std::atomic` & Memory Order

**Ex 7.6.1** `[Easy]`

Why is `volatile` NOT a substitute for `std::atomic` in multithreaded code? What specific guarantee does `volatile` provide, and what doesn't it provide?

```
// your answer
```

---

**Ex 7.6.2** `[Medium]`

Implement a lock-free stack using `std::atomic<Node*>` and compare-and-swap. Support `push` and `pop`.

```cpp
template<typename T>
class LockFreeStack {
    struct Node { T val; Node* next; };
    std::atomic<Node*> head_{nullptr};
public:
    void push(T val) {
        // your solution — CAS loop
    }
    std::optional<T> pop() {
        // your solution — CAS loop
    }
};
```

---

**Ex 7.6.3** `[Hard]`

Implement a lock-free single-producer single-consumer (SPSC) queue of fixed capacity using `std::atomic` with `memory_order_acquire`/`release`. Explain each memory order choice.

```cpp
template<typename T, size_t N>
class SPSCQueue {
    // your solution
};
```

---

## Section 7.7 — `std::future`, `std::promise`, `std::async`

**Ex 7.7.1** `[Medium]`

Launch a computation of `fib(40)` asynchronously with `std::async`. While it runs, print "computing..." every 100ms. When done, print the result.

```cpp
// your solution
```

---

**Ex 7.7.2** `[Medium]`

Use `std::promise` and `std::future` to implement a one-shot channel where one thread produces a string and another consumes it. The consumer must block until the string is ready.

```cpp
// your solution
```

---

**Ex 7.7.3** `[Hard]`

Implement `parallel_transform(input, f)` using `std::async` that applies `f` to each element of `input` in parallel and returns a `std::vector` of results. All tasks must complete before returning.

```cpp
template<typename T, typename F>
auto parallel_transform(const std::vector<T>& input, F f) {
    // your solution
}
```

---

## Section 7.8 — Thread Pools

**Ex 7.8.1** `[Medium]`

Using the thread pool from the reference, submit 20 tasks each sleeping for 10ms and returning a unique integer. Collect all results via futures and print them in submission order.

```cpp
// your solution
```

---

**Ex 7.8.2** `[Hard]`

Write a minimal `ThreadPool` from scratch with: a fixed number of worker threads, a task queue, `submit(f)` returning a `std::future`, and a destructor that drains and joins all threads.

```cpp
class ThreadPool {
    // your solution
};
```

---

## Section 7.9 — Coroutines (C++20): `co_await`, `co_yield`, `co_return`

**Ex 7.9.1** `[Easy]`

Explain the difference between suspending a coroutine and blocking a thread. Why can you have millions of coroutines but only thousands of threads?

```
// your answer
```

---

**Ex 7.9.2** `[Hard]`

Using `std::generator<T>` (C++23) or a minimal generator implementation, write a coroutine `prime_generator()` that lazily yields prime numbers indefinitely.

```cpp
// your solution — use it like:
// for (int p : prime_generator() | std::views::take(10)) ...
```

---

## Section 7.10 — Common Pitfalls: Deadlock, Livelock, Starvation

**Ex 7.10.1** `[Medium]`

Write a program that reliably deadlocks in under 1 second. Then fix it using `std::scoped_lock`. Explain why `std::scoped_lock` is deadlock-free.

```cpp
// your solution — deadlocked version and fixed version
```

---

**Ex 7.10.2** `[Medium]`

Why is the double-checked locking pattern (DCLP) broken without `std::atomic`? Show the broken code, explain the race, and show the correct alternatives (static local and `call_once`).

```cpp
// BROKEN:
Singleton* instance() {
    if (ptr == nullptr) {
        std::lock_guard lock(mtx);
        if (ptr == nullptr) ptr = new Singleton();
    }
    return ptr;
}

// CORRECT version 1 — static local
// your solution

// CORRECT version 2 — call_once
// your solution
```

---

**Ex 7.10.3** `[Hard]`

Implement a `Barrier` class that blocks a set of `n` threads until all `n` have arrived, then releases them all simultaneously. (Like `std::barrier` in C++20 but implemented manually.)

```cpp
class Barrier {
    // your solution — use mutex + condition_variable + generation counter
};
```
