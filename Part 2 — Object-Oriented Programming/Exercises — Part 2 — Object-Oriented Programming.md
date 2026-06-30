# Exercises — Part 2 — Object-Oriented Programming

> Practice problems for [[Part 2 — Object-Oriented Programming]]. Write your solutions in `.cpp` files. No hints, no answers.

---

## Section 2.1 — Classes vs Structs

**Ex 2.1.1** `[Easy]`

What is the only technical difference between `struct` and `class` in C++? Rewrite this `struct` as a `class` with equivalent behavior:

```cpp
struct Point {
    double x, y;
    double length() const;
};
```

```cpp
// your solution
```

---

**Ex 2.1.2** `[Medium]`

Design a `BankAccount` class (not struct) with a private `balance_` and the invariant that balance can never go negative. Provide: a constructor taking an initial balance, `deposit`, `withdraw` (returns `bool` — fails if insufficient funds), and `balance`. Why is a `struct` inappropriate here?

```cpp
class BankAccount {
    // your solution
};
```

---

## Section 2.2 — Constructors & Destructors

**Ex 2.2.1** `[Easy]`

What is wrong with this constructor? Rewrite it correctly using a member initializer list.

```cpp
class Rectangle {
    const int width_;
    const int height_;
    int area_;
public:
    Rectangle(int w, int h) {
        width_ = w;       // problem A
        height_ = h;
        area_ = w * h;
    }
};
```

```cpp
// your solution
```

---

**Ex 2.2.2** `[Medium]`

What does this program print? Trace construction and destruction order carefully.

```cpp
#include <iostream>
struct A {
    A()  { std::cout << "A()\n"; }
    ~A() { std::cout << "~A()\n"; }
};
struct B {
    A a1_, a2_;
    B()  { std::cout << "B()\n"; }
    ~B() { std::cout << "~B()\n"; }
};
struct C : B {
    A a3_;
    C()  { std::cout << "C()\n"; }
    ~C() { std::cout << "~C()\n"; }
};
int main() { C c; }
```

```
// your answer — exact output
```

---

**Ex 2.2.3** `[Medium]`

Mark `explicit` where appropriate and explain why each placement prevents a specific bug:

```cpp
class Seconds {
public:
    Seconds(int s) : value_(s) {}    // should this be explicit?
    // ...
};

class Buffer {
public:
    Buffer(int size) : data_(size) {} // and this?
    // ...
private:
    std::vector<char> data_;
};

void take_seconds(Seconds s) {}
void take_buffer(Buffer b) {}

int main() {
    take_seconds(42);   // is this desirable?
    take_buffer(1024);  // is this desirable?
}
```

```cpp
// your solution with explanation
```

---

**Ex 2.2.4** `[Hard]`

Implement a `FileHandle` class that wraps a `FILE*`. It must: open in the constructor, close in the destructor, be non-copyable, be movable (transferring ownership), and expose a `write(std::string_view)` method. Use `fopen`/`fclose`/`fwrite`.

```cpp
class FileHandle {
    // your solution
};
```

---

## Section 2.3 — Access Control

**Ex 2.3.1** `[Easy]`

Explain why `private` member data is essential for maintaining class invariants. Give an example where making data `public` allows an invariant to be violated.

```
// your answer
```

---

**Ex 2.3.2** `[Medium]`

This class uses `friend`. Will it compile? What does `friend` grant here — is it too much access or exactly what's needed?

```cpp
class Matrix {
    int data_[4][4];
public:
    friend Matrix operator+(const Matrix& a, const Matrix& b);
};

Matrix operator+(const Matrix& a, const Matrix& b) {
    Matrix result;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            result.data_[i][j] = a.data_[i][j] + b.data_[i][j];
    return result;
}
```

```
// your answer
```

---

## Section 2.4 — The `this` Pointer

**Ex 2.4.1** `[Easy]`

Implement a `Builder` class with three setter methods (`set_name`, `set_age`, `set_email`) that each return `*this` to allow method chaining. Show a usage example.

```cpp
class Builder {
    // your solution
};

// Usage:
// Builder b;
// b.set_name("Alice").set_age(30).set_email("alice@example.com");
```

---

**Ex 2.4.2** `[Medium]`

This `operator=` has a critical bug. What is it? Fix it.

```cpp
class Buffer {
    char* data_;
    size_t size_;
public:
    Buffer& operator=(const Buffer& other) {
        delete[] data_;
        size_ = other.size_;
        data_ = new char[size_];
        std::memcpy(data_, other.data_, size_);
        return *this;
    }
};
```

```cpp
// your fixed version
```

---

## Section 2.5 — Static Members

**Ex 2.5.1** `[Easy]`

Implement a class `IdGenerator` where every instance gets a unique integer ID (starting from 1) assigned at construction. The ID must be stored per-instance but the counter must be shared across all instances.

```cpp
class IdGenerator {
    // your solution
};
```

---

**Ex 2.5.2** `[Medium]`

What does this program print? Is `instance_count_` initialized before `main` runs?

```cpp
class Widget {
    static int instance_count_;
public:
    Widget()  { ++instance_count_; }
    ~Widget() { --instance_count_; }
    static int count() { return instance_count_; }
};
int Widget::instance_count_ = 0;

Widget global_widget;

int main() {
    std::cout << Widget::count() << "\n";
    Widget a, b;
    std::cout << Widget::count() << "\n";
    {
        Widget c;
        std::cout << Widget::count() << "\n";
    }
    std::cout << Widget::count() << "\n";
}
```

```
// your answer — exact output
```

---

## Section 2.6 — Operator Overloading

**Ex 2.6.1** `[Medium]`

Implement a `Vector2D` class with `operator+`, `operator-`, `operator*` (scalar multiply, both `v * s` and `s * v`), `operator==`, and `operator<<` for `std::ostream`. Follow the canonical forms.

```cpp
class Vector2D {
    // your solution
};
```

---

**Ex 2.6.2** `[Medium]`

Implement a `Matrix2x2` with `operator[]` returning a reference to the inner array so that `m[0][1] = 5;` works correctly. Provide both const and non-const overloads.

```cpp
class Matrix2x2 {
    // your solution
};
```

---

**Ex 2.6.3** `[Hard]`

Implement `operator<=>` (spaceship) for a `Version` class holding `major`, `minor`, `patch`. Verify that `<`, `>`, `<=`, `>=`, `==`, `!=` all work automatically from it.

```cpp
#include <compare>
struct Version {
    int major, minor, patch;
    // your solution
};
```

---

## Section 2.7 — Inheritance & the is-a Relationship

**Ex 2.7.1** `[Easy]`

What is the difference between `public`, `protected`, and `private` inheritance? For each, state what access the derived class has to base `public` and `protected` members, and what access external code has.

```
// your answer
```

---

**Ex 2.7.2** `[Medium]`

Does this violate LSP? Explain why or why not. If it does, propose a fix.

```cpp
class Bird {
public:
    virtual void fly() { std::cout << "Flying\n"; }
};

class Penguin : public Bird {
public:
    void fly() override {
        throw std::logic_error("Penguins can't fly!");
    }
};

void make_it_fly(Bird& b) { b.fly(); }
```

```
// your answer
```

---

## Section 2.8 — Polymorphism & Virtual Functions

**Ex 2.8.1** `[Easy]`

What does this print? Explain each line.

```cpp
struct Base {
    virtual void speak() { std::cout << "Base\n"; }
    void identify() { std::cout << "I am Base\n"; }
};
struct Derived : Base {
    void speak() override { std::cout << "Derived\n"; }
    void identify() { std::cout << "I am Derived\n"; }
};
int main() {
    Derived d;
    Base* p = &d;
    p->speak();      // line A
    p->identify();   // line B
    Base b = d;
    b.speak();       // line C
    b.identify();    // line D
}
```

```
// your answer — explain each line
```

---

**Ex 2.8.2** `[Medium]`

This code has undefined behavior. Identify it and explain the runtime consequence.

```cpp
class Animal {
public:
    virtual void speak() = 0;
    ~Animal() {}              // <-- the problem
};
class Dog : public Animal {
    std::string name_;
public:
    Dog(std::string n) : name_(std::move(n)) {}
    void speak() override { std::cout << name_; }
    ~Dog() { std::cout << "~Dog\n"; }
};
int main() {
    Animal* a = new Dog("Rex");
    delete a;
}
```

```cpp
// your fixed version with explanation
```

---

**Ex 2.8.3** `[Hard]`

Implement a minimal vtable manually in C — no C++ classes or virtual keyword — to demonstrate how virtual dispatch works under the hood. Define a `Shape` "class" with `area()` and `draw()` function pointers in a struct, then implement `Circle` and `Rectangle` using it.

```c
// your solution in C style
```

---

## Section 2.9 — Abstract Classes & Pure Virtual Functions

**Ex 2.9.1** `[Medium]`

Design an `ISerializer` abstract interface with pure virtual methods `serialize(const std::string& data) -> std::string` and `deserialize(const std::string& data) -> std::string`. Implement two concrete classes: `Base64Serializer` and `JsonWrapSerializer` (just wraps string in `{"data":"..."}` for simplicity).

```cpp
class ISerializer {
    // your solution
};
```

---

**Ex 2.9.2** `[Medium]`

Can a pure virtual function have a body? Write an example where it does and explain a valid use case for it.

```cpp
// your solution
```

---

## Section 2.10 — Multiple Inheritance & Diamond Problem

**Ex 2.10.1** `[Medium]`

Draw (in ASCII or text) the object layout for this hierarchy without and with virtual inheritance. How many copies of `Base` exist in each case?

```cpp
struct Base { int x; };
struct Left  : Base {};
struct Right : Base {};
struct Diamond : Left, Right {};
```

```
// your diagram and explanation
```

---

**Ex 2.10.2** `[Hard]`

Fix this diamond problem using virtual inheritance. Then show how to access `value` from `Diamond` unambiguously.

```cpp
struct Device {
    int device_id;
    void identify() { std::cout << "Device " << device_id << "\n"; }
};
struct NetworkDevice : Device {};
struct StorageDevice  : Device {};
struct NetworkStorage : NetworkDevice, StorageDevice {};

int main() {
    NetworkStorage ns;
    ns.identify();         // ambiguous — fix it
    ns.device_id = 1;      // ambiguous — fix it
}
```

```cpp
// your solution
```

---

## Section 2.11 — Copy Constructor & Copy Assignment

**Ex 2.11.1** `[Medium]`

Implement a `DynamicArray` class that manages a heap-allocated `int` array. It must have a working copy constructor and copy assignment operator (deep copy). The Rule of Three applies.

```cpp
class DynamicArray {
    int* data_;
    size_t size_;
public:
    explicit DynamicArray(size_t n);
    // your solution: destructor, copy ctor, copy assignment
};
```

---

**Ex 2.11.2** `[Hard]`

Implement the copy-and-swap idiom for `DynamicArray` from the previous exercise. Show why it handles self-assignment and provides strong exception guarantee automatically.

```cpp
// your solution — reimplement copy assignment using copy-and-swap
```

---

## Section 2.12 — Move Constructor & Move Assignment (Rule of Five)

**Ex 2.12.1** `[Medium]`

Add move constructor and move assignment to `DynamicArray`. After a move, the source object must be in a valid but empty state (size 0, null pointer). Mark both `noexcept` and explain why that matters for `std::vector`.

```cpp
class DynamicArray {
    // extend your Rule of Three solution with Rule of Five
};
```

---

**Ex 2.12.2** `[Medium]`

What does this print? Trace each call to constructor/destructor.

```cpp
struct Tracker {
    std::string name_;
    Tracker(std::string n) : name_(std::move(n)) { std::cout << "ctor " << name_ << "\n"; }
    Tracker(const Tracker& o) : name_(o.name_)    { std::cout << "copy " << name_ << "\n"; }
    Tracker(Tracker&& o) noexcept : name_(std::move(o.name_)) { std::cout << "move " << name_ << "\n"; }
    ~Tracker() { std::cout << "dtor " << name_ << "\n"; }
};

Tracker make() { return Tracker("X"); }

int main() {
    Tracker a("A");
    Tracker b = a;
    Tracker c = std::move(a);
    Tracker d = make();
}
```

```
// your answer — exact output
```

---

**Ex 2.12.3** `[Hard]`

Implement a `UniquePtr<T>` — a simplified version of `std::unique_ptr`. It must: own a heap-allocated `T`, delete it in the destructor, be non-copyable, support move, provide `operator*` and `operator->`, and support `reset()` and `release()`.

```cpp
template<typename T>
class UniquePtr {
    // your solution
};
```
