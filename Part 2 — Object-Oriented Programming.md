# Part 2 — Object-Oriented Programming

> C++ OOP is not just about classes — it's about controlling how objects are created, copied, moved, and destroyed, and about using inheritance and polymorphism correctly without shooting yourself in the foot.

## Table of Contents

- [[#2.1 — Classes vs Structs]]
- [[#2.2 — Constructors & Destructors]]
- [[#2.3 — Access Control: public, protected, private]]
- [[#2.4 — The this Pointer]]
- [[#2.5 — Static Members]]
- [[#2.6 — Operator Overloading]]
- [[#2.7 — Inheritance & the is-a Relationship]]
- [[#2.8 — Polymorphism & Virtual Functions]]
- [[#2.9 — Abstract Classes & Pure Virtual Functions]]
- [[#2.10 — Multiple Inheritance & Diamond Problem]]
- [[#2.11 — Copy Constructor & Copy Assignment]]
- [[#2.12 — Move Constructor & Move Assignment (Rule of 5)]]

---

## 2.1 — Classes vs Structs

### The Only Real Difference

In C++, `class` and `struct` are nearly identical. The **only** difference is default access:

- `struct`: members are **public** by default
- `class`: members are **private** by default

The same applies to inheritance:
- `struct Derived : Base` — public inheritance by default
- `class Derived : Base` — private inheritance by default

```cpp
struct Point {
    int x;   // public by default
    int y;
};

class Counter {
    int value;   // private by default
public:
    int get() const { return value; }
};
```

That's it. Everything else — constructors, destructors, virtual functions, inheritance — works the same in both.

### Convention: When to Use Which

The choice is stylistic, but the convention is consistent across almost all C++ codebases:

- **`struct`** — for passive data containers with no invariants. Members can be accessed directly. Think: coordinate pairs, configuration objects, POD types (Plain Old Data), return values bundling multiple items.
- **`class`** — for objects that have invariants to protect. Private data, public interface. The class controls its own state.

```cpp
struct Config {
    int port = 8080;
    std::string host = "localhost";
    bool verbose = false;
};

class Connection {
    int socket_fd = -1;
    bool connected = false;
public:
    bool connect(const Config& cfg);
    void disconnect();
    bool is_connected() const { return connected; }
};
```

An **invariant** is a condition that must always be true about an object's state. `Connection` has an invariant: `socket_fd` is valid if and only if `connected` is true. A `struct` with public members can't protect that.

### Member Layout

Members are laid out in memory in the order they are declared. The compiler may insert **padding** between members to satisfy alignment requirements:

```cpp
struct Bad {
    char a;      // 1 byte
    // 7 bytes padding (on 64-bit)
    double b;    // 8 bytes — must be 8-byte aligned
    char c;      // 1 byte
    // 7 bytes padding
};
// sizeof(Bad) = 24

struct Good {
    double b;    // 8 bytes
    char a;      // 1 byte
    char c;      // 1 byte
    // 6 bytes padding
};
// sizeof(Good) = 16
```

For performance-critical code (cache lines, network serialization), order your members largest to smallest.

### Forward Declarations

A forward declaration tells the compiler "this type exists" without providing its full definition:

```cpp
class Foo;   // forward declaration

class Bar {
    Foo* ptr;   // OK — pointer to Foo, doesn't need full definition
    // Foo obj; // ERROR — full definition needed to know size
};

class Foo {
    // ...
};
```

Use forward declarations to break circular include dependencies. A pointer or reference to a type only needs a forward declaration; a value or calling a method needs the full definition.

---

## 2.2 — Constructors & Destructors

### What a Constructor Is

A constructor is a special member function that is called automatically when an object is created. It initializes the object. It has the same name as the class and no return type.

```cpp
class Box {
    double width, height, depth;
public:
    Box(double w, double h, double d) {
        width = w; height = h; depth = d;
    }
};

Box b(2.0, 3.0, 4.0);   // constructor called automatically
```

### Default Constructor

A constructor that takes no arguments (or has all defaults). The compiler generates one implicitly if you define no constructors at all — but the moment you define ANY constructor, the compiler stops generating the default one.

```cpp
class Foo {
public:
    Foo() {}             // explicit default constructor
    Foo() = default;     // explicitly ask the compiler to generate it (C++11)
    Foo() = delete;      // explicitly forbid default construction (C++11)
};
```

### Member Initializer List — Always Prefer This

The member initializer list initializes members before the constructor body runs:

```cpp
class Box {
    double width, height, depth;
public:
    // Initializer list — preferred
    Box(double w, double h, double d)
        : width(w), height(h), depth(d) {}

    // Assignment in body — suboptimal
    Box(double w, double h, double d) {
        width = w;   // default-initialized first, then assigned
        height = h;
        depth = d;
    }
};
```

**Why the initializer list matters — three reasons:**

1. **`const` members can only be initialized, not assigned.** They must be in the initializer list.
2. **Reference members can only be initialized, not assigned.** Same reason.
3. **Performance.** For class-type members, the body version default-constructs them first, then copy-assigns. The initializer list direct-constructs — one step instead of two.

```cpp
class Logger {
    const std::string name;   // const — MUST use initializer list
    std::ostream& out;        // reference — MUST use initializer list
    std::vector<std::string> history;

public:
    Logger(std::string n, std::ostream& o)
        : name(std::move(n)), out(o), history() {}
};
```

**Initialization order:** Members are initialized in the order they are **declared in the class**, not the order they appear in the initializer list. This is a subtle source of bugs — always declare members in the order you want them initialized.

```cpp
class Danger {
    int b;
    int a;   // declared after b
public:
    Danger(int x) : a(x), b(a) {}   // b is initialized FIRST (declared first)
                                     // b(a) uses uninitialized a — undefined behavior
};
```

### Constructor Delegation (C++11)

One constructor can call another constructor of the same class:

```cpp
class Config {
    int port;
    std::string host;
public:
    Config(int p, std::string h) : port(p), host(std::move(h)) {}
    Config() : Config(8080, "localhost") {}   // delegates to the above
    Config(int p) : Config(p, "localhost") {} // delegates to the first
};
```

### `explicit` — Preventing Implicit Conversions

By default, a single-argument constructor can be used as an implicit conversion:

```cpp
class Radius {
    double value;
public:
    Radius(double v) : value(v) {}
};

void draw_circle(Radius r) { ... }

draw_circle(5.0);    // OK but surprising — 5.0 implicitly converted to Radius
```

`explicit` prevents this:

```cpp
class Radius {
    double value;
public:
    explicit Radius(double v) : value(v) {}
};

draw_circle(5.0);           // error: no implicit conversion
draw_circle(Radius(5.0));   // OK: explicit construction
```

Use `explicit` on any single-argument constructor unless implicit conversion is intentional and obvious. It prevents hard-to-spot bugs where the wrong type is passed silently.

### Destructors

The destructor is called automatically when an object goes out of scope, is deleted, or its containing object is destroyed. It has no parameters and no return type:

```cpp
class FileHandle {
    FILE* file;
public:
    FileHandle(const char* path) : file(fopen(path, "r")) {}
    ~FileHandle() {
        if (file) fclose(file);   // cleanup guaranteed
    }
};
```

This is the heart of **RAII** (Resource Acquisition Is Initialization) — acquire in the constructor, release in the destructor. The destructor is guaranteed to run, even if an exception is thrown. See [[Part 3 — Memory Management]] for the full treatment.

### Construction and Destruction Order

- **Members** are constructed in declaration order, destructed in reverse order.
- **Base classes** are constructed before derived classes, destructed after.
- **Local variables** are destructed in reverse order of construction (LIFO).
- **Global/static objects** are constructed before `main` runs (in an unspecified order between translation units) and destructed after `main` returns.

### The Static Initialization Order Fiasco

Global objects in different translation units are constructed in an unspecified order. If one global depends on another in a different `.cpp` file, you have a race:

```cpp
// file_a.cpp
int global_a = 42;

// file_b.cpp
extern int global_a;
int global_b = global_a + 1;   // undefined — global_a may not be initialized yet
```

The fix: wrap globals in a function (Meyers' singleton pattern). Function-local statics are initialized on first call, guaranteed:

```cpp
int& get_a() {
    static int a = 42;   // initialized first time get_a() is called
    return a;
}
```

---

## 2.3 — Access Control: public, protected, private

### The Three Levels

| Specifier | Accessible from |
|-----------|----------------|
| `public` | Anywhere — this class, derived classes, external code |
| `protected` | This class and derived classes only |
| `private` | This class only |

```cpp
class Animal {
public:
    void breathe() { ... }       // anyone can call this
protected:
    void sleep() { ... }         // only Animal and subclasses
private:
    int heart_rate = 70;         // only Animal
};

class Dog : public Animal {
    void rest() {
        sleep();         // OK: protected
        breathe();       // OK: public
        // heart_rate;   // ERROR: private
    }
};
```

### Why Access Control Exists

Access control enforces **invariants** and hides implementation details. If `heart_rate` were public, any code could set it to `-1000`, violating the invariant that heart rate must be positive. By keeping it private and providing controlled access, the class guarantees its own consistency.

This also enables **refactoring**: if `heart_rate` is private and you later decide to store it differently (e.g., as a float, or computed from other data), the change is contained entirely inside the class. Public data leaks implementation everywhere.

### `friend`

`friend` grants a specific function or class access to all private and protected members:

```cpp
class Matrix {
    double data[4][4];
    friend Matrix operator*(const Matrix& a, const Matrix& b);   // free function friend
    friend class MatrixSerializer;   // entire class friend
};

Matrix operator*(const Matrix& a, const Matrix& b) {
    // can access a.data and b.data directly
}
```

**`friend` is not inherited** — friendship is not transitive. A class that inherits from `Matrix` does not get the friend's access, and a class that `friend`s `Matrix` doesn't automatically friend `Matrix`'s friends.

Use `friend` sparingly. It breaks encapsulation intentionally — only use it when the alternative (making something public or adding accessors) is worse.

### Accessor/Mutator Pattern — and When to Avoid It

Getters and setters are the most common way to provide controlled access to private data:

```cpp
class Temperature {
    double celsius;
public:
    double get_celsius() const { return celsius; }
    void set_celsius(double c) {
        if (c < -273.15) throw std::invalid_argument("below absolute zero");
        celsius = c;
    }
    double get_fahrenheit() const { return celsius * 9.0 / 5.0 + 32.0; }
};
```

The setter validates input. The getter for Fahrenheit is computed — clients don't need to know how it's stored.

**When to avoid getters/setters:** If your class has a getter and setter for every private member and the setter just does `member = value` with no validation or logic, the class has no meaningful invariants and the private members are just public members with extra friction. In that case, just use a `struct`.

A well-designed class provides **behavioral methods** (`connect()`, `deposit()`, `resize()`) rather than raw property access.

---

## 2.4 — The `this` Pointer

### What `this` Is

Every non-static member function receives a hidden parameter: a pointer to the object it was called on. This pointer is called `this`.

```cpp
class Foo {
    int value;
public:
    void set(int v) {
        this->value = v;   // this is a Foo*
        value = v;         // equivalent — this-> is implicit
    }
};
```

You never see `this` in the function signature, but it's always there. For a `Foo` object, `this` has type `Foo*` in non-const methods and `const Foo*` in const methods.

### When You Need to Use `this` Explicitly

**Disambiguating shadowed names:**

```cpp
class Box {
    int width;
public:
    void set_width(int width) {
        this->width = width;   // this->width is the member; width is the parameter
    }
};
```

**Returning the object itself — method chaining:**

```cpp
class Builder {
    std::string result;
public:
    Builder& add(const std::string& s) {
        result += s;
        return *this;   // return reference to self
    }
    std::string build() const { return result; }
};

std::string s = Builder().add("Hello").add(", ").add("World").build();
```

This pattern — returning `*this` from methods — enables **fluent interfaces** and method chaining. `std::ostream` uses it: `std::cout << a << b << c`.

**Checking for self-assignment:**

```cpp
MyClass& operator=(const MyClass& other) {
    if (this == &other) return *this;   // guard against a = a
    // ... copy ...
    return *this;
}
```

### `this` Cannot Be Reassigned

`this` is a pointer, not a reference, but you cannot change what it points to. It's implicitly `T* const` (const pointer, non-const pointee — in a non-const method).

---

## 2.5 — Static Members

### Static Data Members

A `static` data member belongs to the **class itself**, not to any instance. All instances share the same variable. It exists even if no instances have been created.

```cpp
class Counter {
    static int count;   // declaration — not a definition
    int id;
public:
    Counter() : id(++count) {}
    static int get_count() { return count; }
};

int Counter::count = 0;   // definition — must be in exactly one .cpp file
```

In C++17, you can define it inline:

```cpp
class Counter {
    inline static int count = 0;   // declaration AND definition (C++17)
};
```

### Static Member Functions

A `static` member function has no `this` pointer. It can only access static members and other static functions:

```cpp
class MathHelper {
public:
    static double square(double x) { return x * x; }
    static double cube(double x) { return x * x * x; }
};

double result = MathHelper::square(5.0);   // called on the class, not an instance
```

**Use cases:**
- Factory methods that create instances without a pre-existing instance
- Utility functions that logically belong to the class but don't need object state
- Accessing class-level configuration or counts

### Static Local Variables

A `static` variable inside a function is initialized once, on the first call, and persists for the lifetime of the program. In C++11 and later, this initialization is **thread-safe**:

```cpp
Logger& get_logger() {
    static Logger instance("app.log");   // initialized exactly once
    return instance;
}
```

This is the **Meyers' Singleton** pattern — the recommended way to have a single instance of something in C++. It avoids the static initialization order fiasco (see [[#2.2 — Constructors & Destructors]]) because initialization is deferred to first use.

### The Singleton Anti-Pattern

A class that restricts itself to one instance using a static member and a private constructor:

```cpp
class Singleton {
    static Singleton* instance;
    Singleton() {}
public:
    static Singleton* get() {
        if (!instance) instance = new Singleton();
        return instance;
    }
};
```

This is widely considered an anti-pattern because:
- It makes code hard to test (global mutable state)
- It hides dependencies
- Thread safety requires extra work
- The destructor may never be called

Prefer dependency injection — pass the shared object as a parameter.

---

## 2.6 — Operator Overloading

### What It Means

You can define the behavior of built-in operators (`+`, `==`, `<<`, etc.) for your own types. The compiler translates `a + b` into a function call:

```cpp
Point operator+(const Point& a, const Point& b) {
    return Point{a.x + b.x, a.y + b.y};
}

Point p1{1, 2}, p2{3, 4};
Point p3 = p1 + p2;   // calls operator+(p1, p2)
```

### Which Operators Can and Cannot Be Overloaded

**Can be overloaded:** `+`, `-`, `*`, `/`, `%`, `=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `<=>`, `<<`, `>>`, `[]`, `()`, `->`, `->*`, `++`, `--`, `!`, `&&`, `||`, `,`, `new`, `delete`, and more.

**Cannot be overloaded:** `::` (scope resolution), `.` (member access), `.*` (member pointer), `?:` (ternary), `sizeof`, `typeid`, `alignof`.

### Member vs Free Function

Binary operators can be either member functions or free (non-member) functions:

```cpp
// As member function — a.operator+(b)
Point operator+(const Point& b) const;

// As free function — operator+(a, b)
Point operator+(const Point& a, const Point& b);
```

**The rule:** If the operator must work symmetrically with implicit conversions on both sides, use a free function. For example, `a + 5` and `5 + a` should both work for a numeric type — a member function can only handle `a + 5` (left operand is always `*this`).

Operators that must be members: `=`, `[]`, `()`, `->`.

### Canonical Forms for Common Operators

**Equality and comparison:**

```cpp
class Money {
    int cents;
public:
    bool operator==(const Money& other) const { return cents == other.cents; }
    bool operator!=(const Money& other) const { return !(*this == other); }
    bool operator<(const Money& other) const { return cents < other.cents; }
    bool operator>(const Money& other) const { return other < *this; }
    bool operator<=(const Money& other) const { return !(other < *this); }
    bool operator>=(const Money& other) const { return !(*this < other); }
};
```

In C++20, the **spaceship operator** `<=>` replaces all of this:

```cpp
#include <compare>

class Money {
    int cents;
public:
    auto operator<=>(const Money&) const = default;   // generates all 6 comparisons
    bool operator==(const Money&) const = default;    // also needed (C++20 rule)
};
```

**Arithmetic with compound assignment:**

```cpp
class Vec2 {
    float x, y;
public:
    Vec2& operator+=(const Vec2& other) {
        x += other.x; y += other.y;
        return *this;
    }
    // Define + in terms of += — canonical pattern
    friend Vec2 operator+(Vec2 lhs, const Vec2& rhs) {
        return lhs += rhs;   // lhs is a copy, so this is safe
    }
};
```

**Stream output — always a free function:**

```cpp
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

std::cout << p;   // calls operator<<(std::cout, p)
```

It must return `std::ostream&` to support chaining (`std::cout << a << b`). The left operand is `std::ostream`, which you don't own — so it cannot be a member of `Point`.

**Subscript operator:**

```cpp
class Matrix {
    double data[4][4];
public:
    double& operator[](int i) { return data[i][0]; }          // non-const version
    const double& operator[](int i) const { return data[i][0]; } // const version
};
```

Always provide both const and non-const versions so the subscript works on both const and non-const objects.

**Function call operator — makes a type callable (functor):**

```cpp
class Multiplier {
    int factor;
public:
    Multiplier(int f) : factor(f) {}
    int operator()(int x) const { return x * factor; }
};

Multiplier triple(3);
int result = triple(10);   // calls triple.operator()(10) = 30
```

This is how lambdas work internally — the compiler generates a class with `operator()`.

### Pitfalls

- **Don't overload `&&`, `||`, or `,`** — they lose short-circuit evaluation. The built-in `&&` evaluates the right side only if the left is true; an overloaded `&&` always evaluates both.
- **Don't make operators do surprising things.** `+` should add, `<<` should shift or output. Unusual operator semantics make code unreadable.
- **`++` prefix vs postfix:** prefix returns `T&`, postfix returns `T` (by value, the old value):

```cpp
T& operator++() { ++value; return *this; }   // prefix: ++x
T operator++(int) { T old = *this; ++(*this); return old; }  // postfix: x++
```

---

## 2.7 — Inheritance & the is-a Relationship

### The Concept

Inheritance models the **is-a relationship**: a `Dog` is an `Animal`. The derived class inherits all members of the base class and can extend or specialize them.

```cpp
class Animal {
public:
    std::string name;
    void breathe() { std::cout << name << " breathes\n"; }
};

class Dog : public Animal {
public:
    void bark() { std::cout << name << " barks\n"; }  // can use inherited name
};

Dog d;
d.name = "Rex";
d.breathe();   // inherited
d.bark();      // own
```

### Inheritance Access Specifiers

| `class Derived : ?? Base` | public members of Base become | protected members of Base become |
|--------------------------|-------------------------------|----------------------------------|
| `public` | public in Derived | protected in Derived |
| `protected` | protected in Derived | protected in Derived |
| `private` | private in Derived | private in Derived |

- **`public` inheritance** — the standard. Models is-a. `Derived` IS-A `Base` — anywhere a `Base` is expected, a `Derived` can be used.
- **`protected` inheritance** — rare. Derived and its subclasses know about the base, but outside code doesn't.
- **`private` inheritance** — models **has-a** (implemented-in-terms-of). The relationship is purely implementation, not interface. Almost always better replaced by composition.

### What Is and Isn't Inherited

**Inherited:** all data members (including private ones, though they're inaccessible), all member functions (public and protected).

**Not automatically inherited:** constructors, destructor, copy/move constructors, copy/move assignment operators.

You must call the base constructor explicitly from the derived constructor's initializer list:

```cpp
class Animal {
    std::string name;
public:
    Animal(std::string n) : name(std::move(n)) {}
};

class Dog : public Animal {
    std::string breed;
public:
    Dog(std::string n, std::string b)
        : Animal(std::move(n)),   // call base constructor
          breed(std::move(b)) {}
};
```

If you don't call the base constructor explicitly, the base's **default constructor** is called. If the base has no default constructor, it's a compile error.

### Inheriting Constructors (C++11)

You can bring base constructors into scope with `using`:

```cpp
class Dog : public Animal {
public:
    using Animal::Animal;   // inherit all of Animal's constructors
};

Dog d("Rex");   // calls Animal("Rex")
```

### Overriding vs Hiding

Without `virtual`, a derived class function with the same name as a base class function **hides** the base version — it does not override it:

```cpp
class Base {
public:
    void speak() { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    void speak() { std::cout << "Derived\n"; }   // hides Base::speak
};

Base* p = new Derived();
p->speak();   // prints "Base" — hiding, not overriding
```

To get true overriding (runtime polymorphism), use `virtual`. See [[#2.8 — Polymorphism & Virtual Functions]].

To call the base version explicitly: `base_obj.Base::speak()`.

### Liskov Substitution Principle (LSP)

A derived class object must be fully substitutable for a base class object without breaking the program. If `Dog` is-a `Animal`, then any code that works with an `Animal` must also work correctly with a `Dog`.

Violating LSP is a design error, not a C++ error. Classic example: a `Square` that inherits from `Rectangle` and overrides `set_width` to also set height — this breaks any code that assumes width and height are independent on a `Rectangle`.

---

## 2.8 — Polymorphism & Virtual Functions

### Runtime Polymorphism

The power of inheritance comes from polymorphism: calling a method through a base class pointer or reference, and having it dispatch to the correct derived class implementation at runtime.

Without `virtual`, you get static dispatch (resolved at compile time based on the pointer's type). With `virtual`, you get dynamic dispatch (resolved at runtime based on the actual object type):

```cpp
class Animal {
public:
    virtual void speak() { std::cout << "...\n"; }
};

class Dog : public Animal {
public:
    void speak() override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() override { std::cout << "Meow!\n"; }
};

Animal* a = new Dog();
a->speak();   // prints "Woof!" — dynamic dispatch to Dog::speak
a = new Cat();
a->speak();   // prints "Meow!" — dynamic dispatch to Cat::speak
```

### How the vtable Works

Every class with at least one virtual function gets a **vtable** (virtual dispatch table) — an array of function pointers, one per virtual function. Each instance of such a class gets a hidden pointer (`vptr`) to its class's vtable.

When you call `a->speak()`, the compiler emits:
1. Load the `vptr` from `*a`
2. Look up `speak` in the vtable
3. Call through that pointer

This indirection costs one pointer dereference. It's negligible in almost all code, but measurable in tight loops with many virtual calls.

### `override` — Always Use It

`override` tells the compiler you intend to override a virtual function. If the base doesn't have a matching virtual function (e.g., you misspelled it), the compiler errors:

```cpp
class Dog : public Animal {
public:
    void speek() override { ... }   // error: 'Animal' has no virtual function 'speek'
    void speak() override { ... }   // OK
};
```

Without `override`, a typo would silently hide the base function rather than overriding it — a bug that's very hard to find. **Always write `override`.**

### `final`

`final` prevents a virtual function from being overridden further, or prevents a class from being inherited from:

```cpp
class Dog : public Animal {
public:
    void speak() override final { ... }   // no subclass of Dog can override speak
};

class Animal final { ... };   // no class can inherit from Animal
```

### The Slicing Problem

Copying a derived object into a base object by value loses the derived part:

```cpp
Dog d;
Animal a = d;   // SLICING — only the Animal part is copied; Dog-specific data is lost
a.speak();      // calls Animal::speak, not Dog::speak
```

Polymorphism requires **pointers or references** to the base class, never values:

```cpp
Animal& ref = d;    // OK — no slicing
ref.speak();        // calls Dog::speak
```

Never store polymorphic objects in containers by value. Use `std::unique_ptr<Animal>` or `std::shared_ptr<Animal>`.

### Virtual Destructors — Critical

If you delete a derived object through a base class pointer and the base destructor is not virtual, only the base destructor runs — the derived destructor is never called. This is **undefined behavior** and typically causes resource leaks:

```cpp
class Base {
public:
    ~Base() { std::cout << "Base destroyed\n"; }   // NOT virtual — dangerous
};

class Derived : public Base {
    int* data;
public:
    Derived() : data(new int[100]) {}
    ~Derived() { delete[] data; }   // never called!
};

Base* p = new Derived();
delete p;   // only ~Base() runs — memory leak from data
```

**Rule: If a class is designed to be inherited from and used polymorphically, its destructor must be virtual.** Even if the destructor does nothing:

```cpp
class Base {
public:
    virtual ~Base() = default;   // virtual destructor — correct
};
```

If a class is not meant to be a base class, mark it `final` instead.

---

## 2.9 — Abstract Classes & Pure Virtual Functions

### Pure Virtual Functions

A pure virtual function has no implementation in the base class and forces all concrete derived classes to provide one:

```cpp
class Shape {
public:
    virtual double area() const = 0;      // pure virtual
    virtual double perimeter() const = 0; // pure virtual
    virtual ~Shape() = default;
};
```

The `= 0` syntax declares the function as pure virtual. A class with at least one pure virtual function is an **abstract class**.

### Abstract Classes Cannot Be Instantiated

```cpp
Shape s;           // error: Shape is abstract
Shape* p;          // OK: pointer to Shape is fine
Shape* p = new Circle(5.0);  // OK: Circle is a concrete class
```

Any derived class that doesn't implement all pure virtual functions is also abstract.

### The Interface Pattern

C++ has no `interface` keyword, but you can simulate it with a class that has only pure virtual functions and a virtual destructor:

```cpp
class Serializable {
public:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    virtual ~Serializable() = default;
};

class Config : public Serializable {
    std::map<std::string, std::string> data;
public:
    std::string serialize() const override { ... }
    void deserialize(const std::string& s) override { ... }
};
```

This is exactly how Java/C# interfaces work, achieved through the language's existing mechanisms.

### Pure Virtual with a Body

A pure virtual function can have an implementation. It's still pure (derived classes must override it), but the base implementation can be called explicitly:

```cpp
class Animal {
public:
    virtual void speak() = 0;
};

void Animal::speak() {
    std::cout << "...generic animal sound...\n";
}

class Dog : public Animal {
public:
    void speak() override {
        Animal::speak();   // call the base implementation explicitly
        std::cout << "Woof!\n";
    }
};
```

Use this to provide a default that subclasses can optionally call, while still requiring them to override.

### What Abstract Classes Are For

An abstract class defines a **contract**: it says "any concrete thing of this type must be able to do these operations." The abstract class may also provide shared data and non-pure methods that all derived classes share.

```cpp
class Stream {
protected:
    bool is_open = false;
public:
    virtual ~Stream() = default;
    virtual void open(const std::string& path) = 0;
    virtual void close() = 0;
    virtual size_t read(char* buf, size_t n) = 0;
    bool opened() const { return is_open; }   // concrete method shared by all
};
```

---

## 2.10 — Multiple Inheritance & Diamond Problem

### Multiple Inheritance

C++ allows a class to inherit from more than one base class:

```cpp
class Flyable {
public:
    virtual void fly() { std::cout << "Flying\n"; }
};

class Swimmable {
public:
    virtual void swim() { std::cout << "Swimming\n"; }
};

class Duck : public Flyable, public Swimmable {
public:
    void quack() { std::cout << "Quack!\n"; }
};

Duck d;
d.fly();    // from Flyable
d.swim();   // from Swimmable
d.quack();  // own
```

### The Diamond Problem

The diamond problem occurs when two base classes share a common ancestor and a class inherits from both:

```
        Animal
       /      \
    Bird      Fish
       \      /
        Duck
```

Without virtual inheritance, `Duck` ends up with **two copies** of `Animal`'s members — one from `Bird`, one from `Fish`:

```cpp
class Animal { public: int age; };
class Bird   : public Animal {};
class Fish   : public Animal {};
class Duck   : public Bird, public Fish {};

Duck d;
d.age;         // error: ambiguous — Bird::Animal::age or Fish::Animal::age?
d.Bird::age;   // OK — explicit disambiguation
d.Fish::age;   // OK
```

### Virtual Inheritance — The Fix

`virtual` in the inheritance declaration tells the compiler to share a single instance of the base:

```cpp
class Animal { public: int age; };
class Bird : public virtual Animal {};
class Fish : public virtual Animal {};
class Duck : public Bird, public Fish {};

Duck d;
d.age = 5;   // OK — only one Animal subobject
```

With virtual inheritance, `Duck` has exactly one `Animal` subobject shared between `Bird` and `Fish`.

**The cost:** Virtual inheritance adds indirection (similar to vtable). Use it only when the diamond really occurs.

### Alternatives to Multiple Inheritance

Most real-world multiple inheritance cases are better handled with:

1. **Interfaces (pure abstract classes)** as one base + a single concrete base for data
2. **Composition** — contain the other type as a member instead of inheriting it
3. **Mixins** — small abstract classes that add behavior (like `Flyable`, `Serializable`) alongside one concrete base

```cpp
class Duck : public Animal, public Flyable, public Swimmable {
    // Animal is the concrete base with data
    // Flyable and Swimmable are pure interfaces
};
```

This avoids the diamond as long as `Flyable` and `Swimmable` don't share a common ancestor.

---

## 2.11 — Copy Constructor & Copy Assignment

### Signatures

```cpp
class MyClass {
public:
    MyClass(const MyClass& other);            // copy constructor
    MyClass& operator=(const MyClass& other); // copy assignment operator
};
```

- **Copy constructor** — called when a new object is created as a copy: `MyClass b = a;` or `MyClass b(a);` or passing by value.
- **Copy assignment** — called when an existing object is assigned: `b = a;` (both `a` and `b` already exist).

### The Rule of Three

If you define **any one** of these three, you should define all three:
1. Destructor
2. Copy constructor
3. Copy assignment operator

The reason: if you need a custom destructor (usually to release a resource), the default copy operations will do a shallow copy — copying the pointer, not what it points to. Two objects now own the same resource, and the second destructor to run will double-free it.

### Compiler-Generated Copy Operations

The compiler generates copy constructor and copy assignment implicitly if you don't define them. The generated versions do **memberwise copy** — each member is copied using its own copy constructor or assignment.

This is correct when all members can be safely copied by value (ints, strings, vectors). It breaks down when a member is a raw pointer that owns heap memory.

### Shallow vs Deep Copy

```cpp
class Buffer {
    char* data;
    size_t size;
public:
    Buffer(size_t n) : data(new char[n]), size(n) {}
    ~Buffer() { delete[] data; }

    // WRONG — compiler-generated copy does this:
    // Buffer(const Buffer& other) : data(other.data), size(other.size) {}
    // Now two Buffer objects point to the same heap memory — double free!

    // CORRECT — deep copy:
    Buffer(const Buffer& other) : data(new char[other.size]), size(other.size) {
        std::memcpy(data, other.data, size);
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;   // self-assignment guard
        delete[] data;                       // release old resource
        data = new char[other.size];
        size = other.size;
        std::memcpy(data, other.data, size);
        return *this;
    }
};
```

### Self-Assignment Guard

Always check for self-assignment at the start of `operator=`. Without it, `a = a` would delete `a`'s data and then try to copy from the now-invalid memory:

```cpp
MyClass& operator=(const MyClass& other) {
    if (this == &other) return *this;   // nothing to do
    // ... safe to proceed ...
    return *this;
}
```

### Copy-and-Swap Idiom

The cleanest, exception-safe way to implement copy assignment — swap the current object with a copy of the source:

```cpp
class Buffer {
    char* data;
    size_t size;
public:
    // ... constructor, destructor ...

    Buffer(const Buffer& other);   // copy constructor (deep copy)

    friend void swap(Buffer& a, Buffer& b) noexcept {
        std::swap(a.data, b.data);
        std::swap(a.size, b.size);
    }

    Buffer& operator=(Buffer other) {   // other is a copy (passed by value)
        swap(*this, other);             // swap this with the copy
        return *this;                   // other (old this) is destroyed at end of scope
    }
};
```

Benefits: automatically handles self-assignment, is strongly exception-safe (copy either succeeds or throws before any modification), and reuses the copy constructor.

---

## 2.12 — Move Constructor & Move Assignment (Rule of 5)

### The Problem Move Semantics Solves

Copying is expensive when a class owns a large resource (a heap buffer, a file handle, a socket). When a temporary object is about to be destroyed, there's no need to copy its resource — we can **steal** it:

```cpp
std::vector<int> make_big_vector() {
    std::vector<int> v(1'000'000);
    // ... fill v ...
    return v;   // before C++11: copy 1M elements. After: move (zero cost)
}

auto v = make_big_vector();   // move constructor called — instant
```

### Rvalue References

C++11 introduced rvalue references (`T&&`) to distinguish temporaries (rvalues) from named objects (lvalues):

```cpp
int x = 5;
int&  lref = x;    // lvalue reference — binds to named objects
int&& rref = 5;    // rvalue reference — binds to temporaries
int&& rref2 = x;   // error — cannot bind rvalue reference to lvalue
```

Move constructor and move assignment take an rvalue reference — they're only called when the source is a temporary or explicitly marked as one.

### Move Constructor

```cpp
class Buffer {
    char* data;
    size_t size;
public:
    // Move constructor — steal resources from other
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;   // leave other in a valid-but-empty state
        other.size = 0;
    }
};
```

After the move, `other` must be in a **valid but unspecified state** — you can safely destroy it or reassign it, but you cannot assume it still holds its data.

### Move Assignment

```cpp
Buffer& operator=(Buffer&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;             // release current resource
    data = other.data;         // steal
    size = other.size;
    other.data = nullptr;      // leave source empty
    other.size = 0;
    return *this;
}
```

### `std::move` — Enabling Moves Explicitly

`std::move` does not move anything. It casts its argument to an rvalue reference, enabling the move constructor or move assignment to be called:

```cpp
Buffer a(1000);
Buffer b = std::move(a);   // calls move constructor — a's data is stolen
// a is now empty — do not use a as if it still has data
```

Use `std::move` when:
- Returning a local variable (though NRVO usually handles this automatically)
- Passing a local to a function that should take ownership
- Implementing move constructor/assignment themselves

### `noexcept` on Move Operations — Critical for Containers

`std::vector` (and other containers) may need to reallocate their buffer as they grow. When doing so, they move or copy elements to the new buffer. If the move constructor is `noexcept`, the vector uses moves (fast). If it might throw, the vector falls back to copies (safe but slow):

```cpp
class Buffer {
public:
    Buffer(Buffer&&) noexcept;            // vector WILL move these
    Buffer& operator=(Buffer&&) noexcept; // same
};
```

**Always mark move constructor and move assignment `noexcept`** unless they genuinely can throw (rare). Without `noexcept`, your type is moved-from but stored in vectors by copy.

### The Rule of Five

If you define any one of these five, you should define all five:

| Special member | When called |
|----------------|-------------|
| Destructor | Object goes out of scope or is deleted |
| Copy constructor | Creating a new object as a copy |
| Copy assignment | Assigning to an existing object |
| Move constructor | Creating from a temporary (rvalue) |
| Move assignment | Assigning from a temporary (rvalue) |

```cpp
class Buffer {
    char* data;
    size_t size;
public:
    Buffer(size_t n);                                  // constructor
    ~Buffer();                                         // destructor
    Buffer(const Buffer& other);                       // copy constructor
    Buffer& operator=(const Buffer& other);            // copy assignment
    Buffer(Buffer&& other) noexcept;                   // move constructor
    Buffer& operator=(Buffer&& other) noexcept;        // move assignment
};
```

### The Rule of Zero — Prefer This

The best class defines **none** of the five special members and instead uses RAII types (smart pointers, `std::string`, `std::vector`) that already implement them correctly. The compiler-generated versions then do the right thing automatically:

```cpp
class Config {
    std::string name;
    std::vector<std::string> entries;
    std::unique_ptr<Connection> conn;
public:
    Config(std::string n) : name(std::move(n)) {}
    // No destructor, no copy/move defined — compiler generates all of them correctly
    // copy is deleted because unique_ptr is not copyable
    // move works because all members are movable
};
```

The Rule of Zero is the goal. The Rule of Five is for when you're managing a raw resource (raw pointer, file descriptor, OS handle) that the standard library doesn't wrap for you. See [[Part 3 — Memory Management]] for smart pointers that let you stay at zero.
