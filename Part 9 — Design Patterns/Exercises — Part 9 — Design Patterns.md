# Exercises — Part 9 — Design Patterns

> Practice problems for [[Part 9 — Design Patterns]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 9.1 — SOLID Principles

**Ex 9.1.1** `[Medium]`

This class violates the Single Responsibility Principle. Identify all responsibilities it has and refactor into separate classes.

```cpp
class UserManager {
public:
    void create_user(const std::string& name, const std::string& email);
    bool authenticate(const std::string& email, const std::string& password);
    void send_welcome_email(const std::string& email);
    std::string to_json(int user_id);
    void save_to_database(int user_id);
    void log_action(const std::string& action);
};
```

```cpp
// your refactored solution
```

---

**Ex 9.1.2** `[Medium]`

Show an Open/Closed violation: a `DiscountCalculator` with hardcoded `if`/`else` for customer types. Refactor it to be open for extension (new types) without modifying the calculator class.

```cpp
// BEFORE — OCP violation
double calculate_discount(const Customer& c) {
    if (c.type == "VIP") return 0.3;
    if (c.type == "Regular") return 0.1;
    if (c.type == "New") return 0.05;
    return 0.0;
}

// AFTER — your OCP-compliant solution
```

---

**Ex 9.1.3** `[Medium]`

Show a Dependency Inversion violation in a `ReportGenerator` that directly instantiates a `PdfExporter`. Refactor using an `IExporter` abstraction. Show how the fixed code can be unit-tested by injecting a `MockExporter`.

```cpp
// your solution
```

---

**Ex 9.1.4** `[Hard]`

Design a `ShapeDrawer` system that satisfies all five SOLID principles. It must support Circle, Rectangle, and Triangle. Adding a new shape should require adding only one new class, not modifying any existing code.

```cpp
// your solution
```

---

## Section 9.2 — Creational Patterns: Singleton, Factory, Builder, Prototype

**Ex 9.2.1** `[Medium]`

Implement a thread-safe `ConfigManager` Singleton that:
- Reads a config file on first access
- Provides `get(key) -> std::string`
- Is implemented with the Meyers Singleton pattern (static local variable)
- Cannot be copied or moved

```cpp
class ConfigManager {
    // your solution
};
```

---

**Ex 9.2.2** `[Medium]`

Implement a self-registering factory for `Shape` objects. Each concrete shape class registers itself at startup using a static initializer. The factory creates shapes by name string ("circle", "rectangle", etc.).

```cpp
class ShapeFactory {
    // your solution
};

class Circle : public Shape {
    static bool registered_;  // triggers registration
};
```

---

**Ex 9.2.3** `[Medium]`

Implement a `QueryBuilder` using the Builder pattern for constructing SQL SELECT statements:

```cpp
auto query = QueryBuilder()
    .select({"id", "name", "email"})
    .from("users")
    .where("age > 18")
    .where("active = true")
    .order_by("name")
    .limit(100)
    .build();
// produces: "SELECT id, name, email FROM users WHERE age > 18 AND active = true ORDER BY name LIMIT 100"
```

```cpp
class QueryBuilder {
    // your solution
};
```

---

**Ex 9.2.4** `[Hard]`

Implement a Prototype registry for `Document` objects. The registry stores named prototypes and creates new documents by cloning them. Each `Document` subtype (`TextDocument`, `SpreadsheetDocument`) must implement `clone()`.

```cpp
class DocumentRegistry {
    // your solution
};
```

---

## Section 9.3 — Structural Patterns: Adapter, Decorator, Proxy, Facade, Composite

**Ex 9.3.1** `[Medium]`

Write an Adapter that makes a third-party `OldStack<T>` (with `push_val`, `pop_val`, `peek`) conform to a standard `IStack<T>` interface with `push`, `pop`, `top`.

```cpp
template<typename T>
class OldStack {
public:
    void push_val(T v);
    void pop_val();
    T& peek();
};

template<typename T>
class IStack {
public:
    virtual void push(T v) = 0;
    virtual void pop() = 0;
    virtual T& top() = 0;
};

template<typename T>
class StackAdapter : public IStack<T> {
    // your solution
};
```

---

**Ex 9.3.2** `[Medium]`

Implement a `LoggingDecorator` for any `IDatabase` implementation that logs every call to `query(sql)` with the SQL string and execution time before delegating to the real database.

```cpp
class IDatabase {
public:
    virtual std::string query(const std::string& sql) = 0;
    virtual ~IDatabase() = default;
};

class LoggingDatabase : public IDatabase {
    // your solution
};
```

---

**Ex 9.3.3** `[Medium]`

Implement a `CachingProxy` for an `IImageLoader` that caches loaded images by filename. On the first load of a filename, call the real loader; on subsequent calls, return the cached result.

```cpp
class IImageLoader {
public:
    virtual std::vector<uint8_t> load(const std::string& path) = 0;
    virtual ~IImageLoader() = default;
};

class CachingProxy : public IImageLoader {
    // your solution
};
```

---

**Ex 9.3.4** `[Hard]`

Build a `MenuSystem` using the Composite pattern. A `MenuItem` is a leaf (has a name and action). A `MenuGroup` is a composite (has a name and children). Both have `display(indent)` and `execute(name)` (finds and runs the named item anywhere in the tree).

```cpp
class MenuComponent {
    // your solution
};
```

---

## Section 9.4 — Behavioral Patterns: Observer, Strategy, Command, Iterator, State

**Ex 9.4.1** `[Medium]`

Implement an event system where `EventBus` allows subscribing to named events with a callback. Multiple subscribers for the same event are all notified. Use `std::function` and `std::string` event names.

```cpp
class EventBus {
public:
    using Handler = std::function<void(const std::any& data)>;
    void subscribe(const std::string& event, Handler h);
    void publish(const std::string& event, const std::any& data = {});
    // your solution
};
```

---

**Ex 9.4.2** `[Medium]`

Implement a `SortContext` that sorts a `std::vector<int>` using an interchangeable sort strategy. Provide: `BubbleSortStrategy`, `QuickSortStrategy`, and `StdSortStrategy`. Switch strategies at runtime.

```cpp
class ISortStrategy {
public:
    virtual void sort(std::vector<int>&) = 0;
    virtual ~ISortStrategy() = default;
};

class SortContext {
    // your solution
};
```

---

**Ex 9.4.3** `[Hard]`

Implement a text editor with full undo/redo using the Command pattern. Support: `InsertCommand`, `DeleteCommand`, `ReplaceCommand`. The history stack must handle branching (redo is cleared on new command).

```cpp
class TextEditor {
    std::string buffer_;
public:
    void apply(std::unique_ptr<Command> cmd);
    void undo();
    void redo();
    const std::string& text() const { return buffer_; }
};
```

---

**Ex 9.4.4** `[Hard]`

Implement a vending machine using the State pattern with states: `IdleState`, `HasMoneyState`, `DispensingState`, `OutOfStockState`. Transitions are triggered by `insert_coin()`, `select_item()`, `dispense()`, `cancel()`.

```cpp
class VendingMachine {
    // your solution
};
```

---

## Section 9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI

**Ex 9.5.1** `[Medium]`

Implement a `NetworkConnection` class using PIMPL. The public header must include nothing from `<sys/socket.h>` — all POSIX includes go only in the `.cpp`. Provide: `connect(host, port)`, `send(data)`, `receive() -> string`, `disconnect()`.

```cpp
// network_connection.h — your solution (no POSIX headers)
// network_connection.cpp — your solution (POSIX headers here only)
```

---

**Ex 9.5.2** `[Medium]`

Use CRTP to implement a `Cloneable<Derived>` mixin that provides a `clone() -> unique_ptr<Derived>` method to any class that inherits from it, using the derived class's copy constructor.

```cpp
template<typename Derived>
class Cloneable {
    // your solution
};

class Animal : public Cloneable<Animal> {
    std::string species_;
public:
    explicit Animal(std::string s) : species_(std::move(s)) {}
    // clone() is inherited and returns unique_ptr<Animal>
};
```

---

**Ex 9.5.3** `[Hard]`

Implement a `Callable` type-erased wrapper (simplified `std::function`) that can hold any callable taking an `int` and returning an `int`. Support copy and call. Implement using the Concept/Model pattern (virtual base + templated concrete).

```cpp
class Callable {
    struct Concept {
        virtual int invoke(int) const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
        virtual ~Concept() = default;
    };
    // your full solution
};
```

---

**Ex 9.5.4** `[Hard]`

Implement NVI on a `DataProcessor` base class. The public interface is `process(data)` which: validates input, calls the protected virtual `do_process`, then logs the result. Subclasses only override `do_process` — they cannot bypass validation or logging.

```cpp
class DataProcessor {
public:
    std::string process(const std::string& data) {
        // your solution — validate, call do_process, log
    }
private:
    virtual std::string do_process(const std::string& data) = 0;
    virtual bool validate(const std::string& data) { return !data.empty(); }
    virtual void log(const std::string& result) { std::cout << result << "\n"; }
};
```

---

**Ex 9.5.5** `[Hard]`

Design a small plugin architecture combining PIMPL + type erasure + factory:
- A `Plugin` type-erased interface with `name()` and `execute()`
- A `PluginLoader` that loads `.so` files with `dlopen` and wraps them as `Plugin` objects
- A `PluginRegistry` that manages loaded plugins by name

```cpp
// your solution — sketch the classes and key interactions
```
