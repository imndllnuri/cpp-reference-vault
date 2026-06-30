# Part 9 — Design Patterns

> Classical GoF patterns and C++-specific idioms — how to structure code for flexibility, correctness, and maintainability. Patterns are solutions to recurring problems, not templates to apply blindly. Understanding the problem each pattern solves matters more than memorizing its structure.

## Table of Contents

- [[#9.1 — SOLID Principles]]
- [[#9.2 — Creational Patterns: Singleton, Factory, Builder, Prototype]]
- [[#9.3 — Structural Patterns: Adapter, Decorator, Proxy, Facade, Composite]]
- [[#9.4 — Behavioral Patterns: Observer, Strategy, Command, Iterator, State]]
- [[#9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI]]

---

## 9.1 — SOLID Principles

SOLID is five design principles for writing maintainable object-oriented code. Violating them leads to systems that are rigid (hard to change), fragile (one change breaks many things), and immobile (can't reuse components).

### S — Single Responsibility Principle

**A class should have one reason to change.** If a class handles multiple concerns, a change to one concern might break the other.

```cpp
// WRONG — two responsibilities: data storage + serialization
class User {
public:
    void set_name(std::string n) { name_ = n; }
    std::string get_name() const { return name_; }

    void save_to_database();  // database concern
    std::string to_json();    // serialization concern
    void send_email();        // notification concern
private:
    std::string name_;
    int id_;
};

// CORRECT — separated responsibilities
class User {
public:
    void set_name(std::string n) { name_ = n; }
    std::string get_name() const { return name_; }
private:
    std::string name_;
    int id_;
};

class UserRepository { void save(const User&); };
class UserSerializer { std::string to_json(const User&); };
class NotificationService { void send_welcome_email(const User&); };
```

### O — Open/Closed Principle

**Open for extension, closed for modification.** You should be able to add new behavior without changing existing code.

```cpp
// WRONG — adding a new shape requires modifying draw()
void draw(Shape* s) {
    if (s->type == CIRCLE)    draw_circle(s);
    else if (s->type == RECT) draw_rect(s);
    // Adding Triangle requires editing this function
}

// CORRECT — each shape knows how to draw itself
class Shape {
public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    void draw() const override { /* draw circle */ }
};

class Triangle : public Shape {
public:
    void draw() const override { /* draw triangle */ }  // added without touching Shape or Circle
};

void draw(const Shape& s) { s.draw(); }  // this function never changes
```

### L — Liskov Substitution Principle

**A derived class must be substitutable for its base class** — every property that holds for the base must hold for the derived. If substitution breaks the program's correctness, the inheritance relationship is wrong.

```cpp
// WRONG — Square violates LSP when substituted for Rectangle
class Rectangle {
public:
    virtual void set_width(int w)  { w_ = w; }
    virtual void set_height(int h) { h_ = h; }
    int area() const { return w_ * h_; }
protected:
    int w_, h_;
};

class Square : public Rectangle {
public:
    void set_width(int w)  override { w_ = h_ = w; }  // also sets height!
    void set_height(int h) override { w_ = h_ = h; }  // also sets width!
};

// This function works for Rectangle but breaks for Square
void test(Rectangle& r) {
    r.set_width(5);
    r.set_height(4);
    assert(r.area() == 20);  // fails for Square — area is 16, not 20
}
```

The fix: don't model a Square as a subtype of Rectangle. Geometric "is-a" relationships don't map to behavioral substitutability.

### I — Interface Segregation Principle

**Don't force clients to depend on interfaces they don't use.** Large "fat" interfaces create unnecessary coupling.

```cpp
// WRONG — Robot must implement eat() even though robots don't eat
class Worker {
public:
    virtual void work() = 0;
    virtual void eat()  = 0;
    virtual void sleep() = 0;
};

class Robot : public Worker {
    void work()  override { /* works */ }
    void eat()   override { /* throws? is empty? */ }  // forced to implement nonsense
    void sleep() override { /* same problem */ }
};

// CORRECT — segregated interfaces
class Workable  { public: virtual void work() = 0; };
class Feedable  { public: virtual void eat() = 0; };
class Sleepable { public: virtual void sleep() = 0; };

class Human : public Workable, public Feedable, public Sleepable { ... };
class Robot : public Workable { /* only what's relevant */ };
```

### D — Dependency Inversion Principle

**Depend on abstractions, not concretions.** High-level modules should not import from low-level modules; both should depend on abstractions.

```cpp
// WRONG — high-level module directly depends on a concrete low-level class
class MySQLDatabase {
public:
    void save(const std::string& data);
};

class OrderService {
    MySQLDatabase db_;   // tightly coupled to MySQL — can't swap for PostgreSQL or mock
public:
    void place_order(Order o) { db_.save(o.serialize()); }
};

// CORRECT — depend on an abstraction
class IDatabase {
public:
    virtual void save(const std::string& data) = 0;
    virtual ~IDatabase() = default;
};

class MySQLDatabase  : public IDatabase { void save(const std::string&) override; };
class InMemoryDB     : public IDatabase { void save(const std::string&) override; };

class OrderService {
    IDatabase& db_;   // depends on abstraction — any IDatabase works
public:
    explicit OrderService(IDatabase& db) : db_(db) {}
    void place_order(Order o) { db_.save(o.serialize()); }
};

// In production
MySQLDatabase mysql;
OrderService svc(mysql);

// In tests
InMemoryDB test_db;
OrderService test_svc(test_db);
```

---

## 9.2 — Creational Patterns: Singleton, Factory, Builder, Prototype

Creational patterns abstract the instantiation process — who creates objects, how they're configured, and when.

### Singleton

**Intent:** Ensure a class has exactly one instance and provide global access to it.

**When to use:** For truly unique resources: a logger, a thread pool, a registry. Often overused — if you just want one instance, a global variable or dependency injection may be cleaner.

```cpp
class Logger {
public:
    static Logger& instance() {
        static Logger logger;   // C++11: initialized exactly once, thread-safe
        return logger;
    }

    void log(const std::string& msg) {
        std::lock_guard lock(mtx_);
        std::cerr << "[LOG] " << msg << "\n";
    }

    // Non-copyable, non-movable
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;   // private constructor
    std::mutex mtx_;
};

// Usage
Logger::instance().log("Application started");
```

**The Meyers Singleton** (above) is the idiomatic C++ approach — the static local variable is initialized once on first call. See [[Part 7 — Concurrency & Multithreading]] section 7.10 for why other implementations (DCLP) are broken.

**Testability problem:** Singletons make testing hard — you can't substitute a mock. Prefer dependency injection when testability matters.

### Factory Method

**Intent:** Define an interface for creating an object, but let subclasses or parameters decide which class to instantiate. Decouples object creation from object use.

```cpp
// Abstract product
class Button {
public:
    virtual void render() = 0;
    virtual void on_click() = 0;
    virtual ~Button() = default;
};

// Concrete products
class WindowsButton : public Button {
    void render()   override { std::cout << "[Windows button]\n"; }
    void on_click() override { /* windows look & feel */ }
};

class MacButton : public Button {
    void render()   override { std::cout << "[Mac button]\n"; }
    void on_click() override { /* mac look & feel */ }
};

// Factory — creation is centralized and can vary
class GUIFactory {
public:
    static std::unique_ptr<Button> create_button(const std::string& platform) {
        if (platform == "windows") return std::make_unique<WindowsButton>();
        if (platform == "mac")     return std::make_unique<MacButton>();
        throw std::invalid_argument("Unknown platform: " + platform);
    }
};

auto btn = GUIFactory::create_button("mac");
btn->render();
```

**Parameterized factory with registration** — open for extension (add new types without modifying the factory):

```cpp
class ButtonFactory {
public:
    using Creator = std::function<std::unique_ptr<Button>()>;

    static void register_type(const std::string& name, Creator creator) {
        creators()[name] = std::move(creator);
    }

    static std::unique_ptr<Button> create(const std::string& name) {
        auto it = creators().find(name);
        if (it == creators().end()) throw std::runtime_error("Unknown: " + name);
        return it->second();
    }

private:
    static std::map<std::string, Creator>& creators() {
        static std::map<std::string, Creator> m;
        return m;
    }
};

// Registration (e.g., at startup)
ButtonFactory::register_type("windows", []{ return std::make_unique<WindowsButton>(); });
ButtonFactory::register_type("mac",     []{ return std::make_unique<MacButton>(); });
```

### Abstract Factory

**Intent:** Create families of related objects without specifying their concrete classes.

```cpp
class GUIFactory {
public:
    virtual std::unique_ptr<Button>    create_button()    = 0;
    virtual std::unique_ptr<Checkbox>  create_checkbox()  = 0;
    virtual std::unique_ptr<TextField> create_textfield() = 0;
    virtual ~GUIFactory() = default;
};

class WindowsFactory : public GUIFactory {
    std::unique_ptr<Button>    create_button()    override { return std::make_unique<WindowsButton>(); }
    std::unique_ptr<Checkbox>  create_checkbox()  override { return std::make_unique<WindowsCheckbox>(); }
    std::unique_ptr<TextField> create_textfield() override { return std::make_unique<WindowsTextField>(); }
};

class MacFactory : public GUIFactory { /* analogous Mac implementations */ };

// Application uses only the factory interface — completely decoupled from platform
void build_ui(GUIFactory& factory) {
    auto btn   = factory.create_button();
    auto check = factory.create_checkbox();
    btn->render();
}
```

### Builder

**Intent:** Construct a complex object step-by-step, separating construction from representation.

**When to use:** When an object has many optional parameters, or when the construction process has multiple steps that must happen in a specific order.

```cpp
class HttpRequest {
public:
    std::string method, url, body;
    std::map<std::string, std::string> headers;
    int timeout_ms{5000};
};

class HttpRequestBuilder {
public:
    HttpRequestBuilder& method(std::string m)  { req_.method = std::move(m); return *this; }
    HttpRequestBuilder& url(std::string u)      { req_.url    = std::move(u); return *this; }
    HttpRequestBuilder& body(std::string b)     { req_.body   = std::move(b); return *this; }
    HttpRequestBuilder& header(std::string k, std::string v) {
        req_.headers[std::move(k)] = std::move(v);
        return *this;
    }
    HttpRequestBuilder& timeout(int ms)         { req_.timeout_ms = ms; return *this; }

    HttpRequest build() {
        if (req_.url.empty()) throw std::logic_error("URL is required");
        return std::move(req_);
    }

private:
    HttpRequest req_;
};

// Clean, readable construction
auto request = HttpRequestBuilder{}
    .method("POST")
    .url("https://api.example.com/data")
    .header("Content-Type", "application/json")
    .header("Authorization", "Bearer token123")
    .body(R"({"key": "value"})")
    .timeout(10000)
    .build();
```

Each setter returns `*this` — enabling the fluent (method-chaining) interface. The builder validates required fields in `build()`.

### Prototype

**Intent:** Create new objects by copying an existing object (the prototype). Useful when construction is expensive or complex.

```cpp
class Shape {
public:
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    int x_, y_, radius_;
public:
    Circle(int x, int y, int r) : x_(x), y_(y), radius_(r) {}

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);   // copy constructor
    }
    void draw() const override {
        std::cout << "Circle at (" << x_ << "," << y_ << ") r=" << radius_ << "\n";
    }
};

// Usage — clone without knowing the concrete type
std::unique_ptr<Shape> original = std::make_unique<Circle>(10, 20, 5);
auto copy1 = original->clone();
auto copy2 = original->clone();
// copy1 and copy2 are independent Circle objects
```

---

## 9.3 — Structural Patterns: Adapter, Decorator, Proxy, Facade, Composite

Structural patterns deal with how classes and objects are composed to form larger structures.

### Adapter

**Intent:** Convert the interface of a class into another interface that clients expect. Makes incompatible interfaces work together.

```cpp
// Existing class with an incompatible interface
class LegacyLogger {
public:
    void write_log(int level, const char* message, const char* file, int line);
};

// Target interface that the rest of the code uses
class ILogger {
public:
    virtual void debug(const std::string& msg) = 0;
    virtual void info (const std::string& msg) = 0;
    virtual void error(const std::string& msg) = 0;
    virtual ~ILogger() = default;
};

// Adapter — wraps LegacyLogger and implements ILogger
class LegacyLoggerAdapter : public ILogger {
    LegacyLogger legacy_;
public:
    void debug(const std::string& msg) override { legacy_.write_log(0, msg.c_str(), "", 0); }
    void info (const std::string& msg) override { legacy_.write_log(1, msg.c_str(), "", 0); }
    void error(const std::string& msg) override { legacy_.write_log(2, msg.c_str(), "", 0); }
};

// Code that uses ILogger now works with the legacy system
void do_work(ILogger& log) {
    log.info("Starting work");
    log.debug("Detail...");
}

LegacyLoggerAdapter adapter;
do_work(adapter);
```

### Decorator

**Intent:** Attach additional responsibilities to an object dynamically. Provides a flexible alternative to subclassing for extending functionality.

```cpp
class DataSource {
public:
    virtual void write_data(const std::string& data) = 0;
    virtual std::string read_data() = 0;
    virtual ~DataSource() = default;
};

class FileDataSource : public DataSource {
    std::string filename_;
public:
    explicit FileDataSource(std::string fn) : filename_(std::move(fn)) {}
    void write_data(const std::string& data) override { /* write to file */ }
    std::string read_data() override { /* read from file */ return {}; }
};

// Base decorator — wraps another DataSource
class DataSourceDecorator : public DataSource {
protected:
    std::unique_ptr<DataSource> wrappee_;
public:
    explicit DataSourceDecorator(std::unique_ptr<DataSource> ds)
        : wrappee_(std::move(ds)) {}
    void write_data(const std::string& d) override { wrappee_->write_data(d); }
    std::string read_data() override { return wrappee_->read_data(); }
};

// Concrete decorators — each adds one responsibility
class EncryptionDecorator : public DataSourceDecorator {
public:
    using DataSourceDecorator::DataSourceDecorator;
    void write_data(const std::string& d) override {
        wrappee_->write_data(encrypt(d));
    }
    std::string read_data() override { return decrypt(wrappee_->read_data()); }
private:
    std::string encrypt(const std::string& s) { return s + "_enc"; }
    std::string decrypt(const std::string& s) { return s.substr(0, s.size() - 4); }
};

class CompressionDecorator : public DataSourceDecorator {
public:
    using DataSourceDecorator::DataSourceDecorator;
    void write_data(const std::string& d) override { wrappee_->write_data(compress(d)); }
    std::string read_data() override { return decompress(wrappee_->read_data()); }
private:
    std::string compress(const std::string& s)   { return s + "_z"; }
    std::string decompress(const std::string& s) { return s.substr(0, s.size() - 2); }
};

// Compose behaviors at runtime
auto source = std::make_unique<FileDataSource>("data.txt");
auto encrypted = std::make_unique<EncryptionDecorator>(std::move(source));
auto compressed_and_encrypted = std::make_unique<CompressionDecorator>(std::move(encrypted));

compressed_and_encrypted->write_data("hello");  // compressed, then encrypted, then written
```

### Proxy

**Intent:** Provide a surrogate or placeholder for another object to control access to it. Same interface as the real object.

**Types of proxy:**
- **Virtual proxy** — lazy initialization (create the expensive object only when needed)
- **Remote proxy** — represents an object in another address space
- **Protection proxy** — controls access based on permissions
- **Caching proxy** — caches results of expensive operations

```cpp
class Image {
public:
    virtual void display() = 0;
    virtual ~Image() = default;
};

class RealImage : public Image {
    std::string filename_;
public:
    explicit RealImage(std::string fn) : filename_(std::move(fn)) {
        load_from_disk();   // expensive
    }
    void display() override { std::cout << "Displaying " << filename_ << "\n"; }
private:
    void load_from_disk() { std::cout << "Loading " << filename_ << " from disk\n"; }
};

// Virtual proxy — lazy loading
class ImageProxy : public Image {
    std::string filename_;
    mutable std::unique_ptr<RealImage> real_image_;   // mutable for lazy init in const
public:
    explicit ImageProxy(std::string fn) : filename_(std::move(fn)) {}

    void display() override {
        if (!real_image_) {
            real_image_ = std::make_unique<RealImage>(filename_);  // load on first use
        }
        real_image_->display();
    }
};

ImageProxy proxy("photo.jpg");   // no loading yet
// ... time passes ...
proxy.display();   // loads NOW, then displays
proxy.display();   // already loaded — just displays
```

### Facade

**Intent:** Provide a simplified interface to a complex subsystem. The facade doesn't add functionality — it makes the subsystem easier to use.

```cpp
// Complex subsystem classes
class VideoDecoder   { public: void decode(const std::string& file); };
class AudioDecoder   { public: void decode(const std::string& file); };
class VideoRenderer  { public: void render_frame(int frame); };
class AudioRenderer  { public: void play_sample(int sample); };
class Synchronizer   { public: void sync_av(int frame, int sample); };

// Facade — hides all this complexity
class MediaPlayer {
    VideoDecoder  video_dec_;
    AudioDecoder  audio_dec_;
    VideoRenderer video_ren_;
    AudioRenderer audio_ren_;
    Synchronizer  sync_;
public:
    void play(const std::string& filename) {
        video_dec_.decode(filename);
        audio_dec_.decode(filename);
        // sync and render loop...
        sync_.sync_av(0, 0);
        video_ren_.render_frame(0);
        audio_ren_.play_sample(0);
    }
};

// Client only sees the facade
MediaPlayer player;
player.play("movie.mp4");   // all complexity hidden
```

### Composite

**Intent:** Compose objects into tree structures. Lets clients treat individual objects and compositions of objects uniformly.

```cpp
class FileSystemEntry {
public:
    virtual std::string name() const = 0;
    virtual size_t size() const = 0;
    virtual void print(int indent = 0) const = 0;
    virtual ~FileSystemEntry() = default;
};

// Leaf
class File : public FileSystemEntry {
    std::string name_;
    size_t size_;
public:
    File(std::string n, size_t s) : name_(std::move(n)), size_(s) {}
    std::string name() const override { return name_; }
    size_t size() const override { return size_; }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << name_ << " (" << size_ << " bytes)\n";
    }
};

// Composite — contains children, which may themselves be composites
class Directory : public FileSystemEntry {
    std::string name_;
    std::vector<std::unique_ptr<FileSystemEntry>> children_;
public:
    explicit Directory(std::string n) : name_(std::move(n)) {}

    void add(std::unique_ptr<FileSystemEntry> entry) {
        children_.push_back(std::move(entry));
    }

    std::string name() const override { return name_; }
    size_t size() const override {
        size_t total = 0;
        for (const auto& c : children_) total += c->size();
        return total;
    }
    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ') << "[" << name_ << "]\n";
        for (const auto& c : children_) c->print(indent + 2);
    }
};

auto root = std::make_unique<Directory>("root");
auto src  = std::make_unique<Directory>("src");
src->add(std::make_unique<File>("main.cpp", 1024));
src->add(std::make_unique<File>("util.cpp", 512));
root->add(std::move(src));
root->add(std::make_unique<File>("README.md", 256));

root->print();   // recursive — treats files and dirs uniformly
root->size();    // 1024 + 512 + 256 = 1792
```

---

## 9.4 — Behavioral Patterns: Observer, Strategy, Command, Iterator, State

Behavioral patterns define how objects communicate and distribute responsibility.

### Observer

**Intent:** Define a one-to-many dependency so that when one object changes state, all dependents are notified automatically.

```cpp
class Observer {
public:
    virtual void on_event(const std::string& event, const std::any& data) = 0;
    virtual ~Observer() = default;
};

class EventSource {
    std::vector<std::weak_ptr<Observer>> observers_;
public:
    void subscribe(std::shared_ptr<Observer> obs) {
        observers_.push_back(obs);
    }

    void notify(const std::string& event, const std::any& data = {}) {
        // Remove expired observers, notify live ones
        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                [](const std::weak_ptr<Observer>& wp){ return wp.expired(); }),
            observers_.end()
        );
        for (auto& wp : observers_) {
            if (auto obs = wp.lock()) obs->on_event(event, data);
        }
    }
};

class Dashboard : public Observer {
public:
    void on_event(const std::string& event, const std::any& data) override {
        std::cout << "Dashboard: " << event << "\n";
    }
};

class Logger : public Observer {
public:
    void on_event(const std::string& event, const std::any& data) override {
        std::cout << "[LOG] " << event << "\n";
    }
};

EventSource stock_feed;
auto dash = std::make_shared<Dashboard>();
auto log  = std::make_shared<Logger>();
stock_feed.subscribe(dash);
stock_feed.subscribe(log);

stock_feed.notify("AAPL_PRICE_UPDATE", 185.42);
// Dashboard: AAPL_PRICE_UPDATE
// [LOG] AAPL_PRICE_UPDATE
```

Using `weak_ptr` for observers prevents the event source from keeping observers alive past their intended lifetime — a common source of leaks in the naive (raw pointer) implementation.

### Strategy

**Intent:** Define a family of algorithms, encapsulate each one, and make them interchangeable. Lets the algorithm vary independently from the clients that use it.

```cpp
class SortStrategy {
public:
    virtual void sort(std::vector<int>& data) = 0;
    virtual ~SortStrategy() = default;
};

class QuickSort : public SortStrategy {
    void sort(std::vector<int>& data) override {
        std::sort(data.begin(), data.end());
    }
};

class BubbleSort : public SortStrategy {
    void sort(std::vector<int>& data) override {
        for (size_t i = 0; i < data.size(); ++i)
            for (size_t j = 0; j + 1 < data.size() - i; ++j)
                if (data[j] > data[j+1]) std::swap(data[j], data[j+1]);
    }
};

class Sorter {
    std::unique_ptr<SortStrategy> strategy_;
public:
    explicit Sorter(std::unique_ptr<SortStrategy> s) : strategy_(std::move(s)) {}
    void set_strategy(std::unique_ptr<SortStrategy> s) { strategy_ = std::move(s); }
    void sort(std::vector<int>& data) { strategy_->sort(data); }
};

// In C++ — prefer using std::function or a template parameter for strategies
// when the strategy is set at construction and doesn't change:
template<typename SortFn = decltype(std::ranges::sort)>
class FastSorter {
    SortFn fn_;
public:
    explicit FastSorter(SortFn fn = std::ranges::sort) : fn_(fn) {}
    void sort(std::vector<int>& data) { fn_(data); }
};
```

### Command

**Intent:** Encapsulate a request as an object, allowing parameterization, queuing, logging, and undo/redo.

```cpp
class Command {
public:
    virtual void execute() = 0;
    virtual void undo()    = 0;
    virtual ~Command() = default;
};

class TextEditor {
    std::string text_;
public:
    void insert(size_t pos, const std::string& s) {
        text_.insert(pos, s);
    }
    void erase(size_t pos, size_t len) {
        text_.erase(pos, len);
    }
    const std::string& text() const { return text_; }
};

class InsertCommand : public Command {
    TextEditor& editor_;
    size_t pos_;
    std::string text_;
public:
    InsertCommand(TextEditor& ed, size_t pos, std::string text)
        : editor_(ed), pos_(pos), text_(std::move(text)) {}

    void execute() override { editor_.insert(pos_, text_); }
    void undo()    override { editor_.erase(pos_, text_.size()); }
};

class CommandHistory {
    std::vector<std::unique_ptr<Command>> history_;
    size_t cursor_{0};
public:
    void execute(std::unique_ptr<Command> cmd) {
        history_.erase(history_.begin() + cursor_, history_.end()); // discard redo stack
        cmd->execute();
        history_.push_back(std::move(cmd));
        ++cursor_;
    }
    void undo() {
        if (cursor_ == 0) return;
        history_[--cursor_]->undo();
    }
    void redo() {
        if (cursor_ == history_.size()) return;
        history_[cursor_++]->execute();
    }
};

TextEditor editor;
CommandHistory history;
history.execute(std::make_unique<InsertCommand>(editor, 0, "Hello"));
history.execute(std::make_unique<InsertCommand>(editor, 5, " World"));
// editor.text() == "Hello World"
history.undo();
// editor.text() == "Hello"
history.redo();
// editor.text() == "Hello World"
```

### Strategy via `std::function` (Modern C++)

In modern C++, lightweight strategies don't need a class hierarchy — `std::function` or templates suffice:

```cpp
class Validator {
    std::function<bool(const std::string&)> validate_;
public:
    explicit Validator(std::function<bool(const std::string&)> fn)
        : validate_(std::move(fn)) {}
    bool validate(const std::string& input) { return validate_(input); }
};

Validator email_validator([](const std::string& s){
    return s.find('@') != std::string::npos;
});

Validator length_validator([](const std::string& s){
    return s.size() >= 8;
});
```

### State

**Intent:** Allow an object to alter its behavior when its internal state changes. The object will appear to change its class.

```cpp
class TrafficLight;

class LightState {
public:
    virtual void handle(TrafficLight& light) = 0;
    virtual std::string name() const = 0;
    virtual ~LightState() = default;
};

class TrafficLight {
    std::unique_ptr<LightState> state_;
public:
    explicit TrafficLight(std::unique_ptr<LightState> initial_state)
        : state_(std::move(initial_state)) {}

    void transition_to(std::unique_ptr<LightState> new_state) {
        state_ = std::move(new_state);
    }

    void update() { state_->handle(*this); }
    std::string current_state() const { return state_->name(); }
};

class RedState   : public LightState {
    std::string name() const override { return "RED"; }
    void handle(TrafficLight& light) override;
};
class GreenState : public LightState {
    std::string name() const override { return "GREEN"; }
    void handle(TrafficLight& light) override;
};
class YellowState: public LightState {
    std::string name() const override { return "YELLOW"; }
    void handle(TrafficLight& light) override;
};

void RedState::handle(TrafficLight& light) {
    light.transition_to(std::make_unique<GreenState>());
}
void GreenState::handle(TrafficLight& light) {
    light.transition_to(std::make_unique<YellowState>());
}
void YellowState::handle(TrafficLight& light) {
    light.transition_to(std::make_unique<RedState>());
}

TrafficLight tl(std::make_unique<RedState>());
tl.current_state();   // RED
tl.update();
tl.current_state();   // GREEN
tl.update();
tl.current_state();   // YELLOW
```

---

## 9.5 — C++ Idioms: PIMPL, CRTP, Type Erasure, NVI

### PIMPL — Pointer to Implementation

**Problem:** When a class's private data includes heavy headers (like Windows API, Boost, or large library types), those headers pollute every file that includes your class header. Changing a private member forces recompilation of all includers.

**Solution:** Move all private implementation details into a separate class, and hold only a forward-declared pointer to it.

```cpp
// widget.h — clean, minimal dependencies
#include <memory>
#include <string>

class Widget {
public:
    explicit Widget(std::string name);
    ~Widget();                     // must be defined in .cpp — Impl is incomplete here

    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;
    Widget(const Widget&);
    Widget& operator=(const Widget&);

    void render();
    void resize(int w, int h);

private:
    class Impl;                    // forward declaration — no definition needed in header
    std::unique_ptr<Impl> pimpl_; // pointer to implementation
};
```

```cpp
// widget.cpp — heavy includes only here
#include "widget.h"
#include <opengl/gl.h>        // not leaked to widget.h includers
#include <heavy_library.h>    // not leaked either

struct Widget::Impl {
    std::string name;
    int width{800}, height{600};
    HeavyObject heavy_data;   // from heavy_library.h — no leak

    void render()        { /* opengl calls */ }
    void resize(int w, int h) { width = w; height = h; }
};

Widget::Widget(std::string name) : pimpl_(std::make_unique<Impl>()) {
    pimpl_->name = std::move(name);
}
Widget::~Widget() = default;    // defined here — Impl is complete now
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;
Widget::Widget(const Widget& o) : pimpl_(std::make_unique<Impl>(*o.pimpl_)) {}
Widget& Widget::operator=(const Widget& o) {
    *pimpl_ = *o.pimpl_; return *this;
}

void Widget::render()           { pimpl_->render(); }
void Widget::resize(int w, int h) { pimpl_->resize(w, h); }
```

**Benefits:**
- **Compilation firewall** — changes to Impl don't trigger recompilation of headers
- **ABI stability** — library users are insulated from internal changes
- **Clean headers** — no leaked implementation details

**Cost:** One extra heap allocation per Widget, one extra pointer dereference per call. Acceptable for UI objects, libraries, and anything compilation-speed-sensitive. Not worth it for performance-critical inner-loop objects.

### CRTP — Curiously Recurring Template Pattern

Covered fully in [[Part 6 — Templates & Metaprogramming]] section 6.11. The key points for the idiom context:

```cpp
// CRTP enables static polymorphism and mixin behavior
template<typename Derived>
class Cloneable {
public:
    std::unique_ptr<Derived> clone() const {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};

class Document : public Cloneable<Document> {
    // inherits clone() — works correctly for Document without virtual
};

Document doc;
auto copy = doc.clone();   // returns unique_ptr<Document>, not unique_ptr<Base>
```

CRTP gives you:
- Compile-time polymorphism (no vtable)
- Mixins — add capabilities by inheriting from a base that knows the derived type
- The "curiously recurring" part: `class Derived : public Base<Derived>` — the derived class is its own template argument

### Type Erasure

**Problem:** You want to store heterogeneous callable types or objects with a common interface, but you don't want to commit to a specific type hierarchy or lose performance by using virtual dispatch for every small operation.

**Solution:** Type erasure — hide the concrete type behind a uniform interface, storing the type information internally.

`std::function`, `std::any`, and `std::shared_ptr` are all type erasure implementations in the standard library.

**Writing your own type erasure — a `Drawable` that works with any type that has a `draw()` method:**

```cpp
class Drawable {
    struct Concept {
        virtual void draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
        virtual ~Concept() = default;
    };

    template<typename T>
    struct Model : Concept {
        T obj_;
        explicit Model(T obj) : obj_(std::move(obj)) {}
        void draw() const override { obj_.draw(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model<T>>(*this);
        }
    };

    std::unique_ptr<Concept> concept_;

public:
    template<typename T>
    Drawable(T obj) : concept_(std::make_unique<Model<T>>(std::move(obj))) {}

    Drawable(const Drawable& o) : concept_(o.concept_->clone()) {}
    Drawable(Drawable&&) noexcept = default;

    void draw() const { concept_->draw(); }
};

// Any type with a draw() method works — no inheritance required
struct Circle { void draw() const { std::cout << "O\n"; } };
struct Square { void draw() const { std::cout << "[]\n"; } };
struct Star   { void draw() const { std::cout << "*\n"; } };

std::vector<Drawable> shapes;
shapes.emplace_back(Circle{});  // no common base class required
shapes.emplace_back(Square{});
shapes.emplace_back(Star{});

for (const auto& s : shapes) s.draw();
```

**Comparison with virtual dispatch:**

| | Virtual dispatch | Type erasure |
|---|---|---|
| Inheritance required | Yes | No — any type with the right methods |
| Heap allocation | Only for the object | Object + vtable concept wrapper |
| Copyable | Depends | Yes — clone() in concept |
| Intrusive | Yes — must inherit | Non-intrusive |
| Use case | Known hierarchy | Third-party types, value semantics |

### NVI — Non-Virtual Interface

**Problem:** When you make public interface functions virtual, you give subclasses the power to change behavior in ways that break invariants — and you can't add pre/post-condition checks without modifying every subclass.

**Solution:** Public methods are non-virtual. They call private virtual methods that subclasses override. The public method keeps control.

```cpp
class Logger {
public:
    // Non-virtual public interface — enforces invariants for all subclasses
    void log(const std::string& msg) {
        if (msg.empty()) return;              // invariant: never log empty strings
        std::string formatted = format(msg);  // hook for customization
        do_log(formatted);                    // subclass decides HOW to log
    }

private:
    virtual std::string format(const std::string& msg) {
        return "[" + timestamp() + "] " + msg;   // default formatting
    }
    virtual void do_log(const std::string& msg) = 0;  // pure: must override

    std::string timestamp() { return "12:00:00"; }
};

class ConsoleLogger : public Logger {
private:
    void do_log(const std::string& msg) override {
        std::cout << msg << "\n";
    }
    // Optionally override format() too
};

class FileLogger : public Logger {
    std::ofstream file_;
private:
    void do_log(const std::string& msg) override {
        file_ << msg << "\n";
    }
};
```

**Benefits:**
- The base class controls pre/post conditions, invariants, and sequencing
- Subclasses can customize behavior without bypassing guards
- The virtual interface is private — callers can't call `do_log` directly and bypass the checks
- Adding hooks (pre-log, post-log) to the base doesn't require touching subclasses

**NVI vs Strategy:** NVI uses inheritance; Strategy uses composition. For stable hierarchies where the type relationship is fundamental, NVI is simpler. For interchangeable behaviors that don't share identity, Strategy is more flexible.

### Pattern Selection Guide

| Problem | Pattern |
|---|---|
| Need exactly one instance | Singleton |
| Decouple creation from use, vary concrete type | Factory Method / Abstract Factory |
| Complex multi-step construction | Builder |
| Copy without knowing concrete type | Prototype |
| Incompatible interface from existing class | Adapter |
| Add behavior without changing class | Decorator |
| Control access / lazy init | Proxy |
| Simplify a complex subsystem | Facade |
| Uniform tree structures | Composite |
| Notify multiple dependents of change | Observer |
| Interchangeable algorithms | Strategy |
| Encapsulate requests, support undo | Command |
| Behavior changes with internal state | State |
| Hide impl details, stabilize ABI | PIMPL |
| Compile-time polymorphism / mixin | CRTP |
| Generic interface without inheritance | Type Erasure |
| Enforce invariants in virtual hierarchy | NVI |
