# C++ Module 07: C++ Templates — Solved

## Overview of the Module

Module 07 introduces one of the most powerful features of C++: **templates**. A template is a blueprint that tells the compiler how to generate code for many different types automatically. Instead of writing a separate `swap` for `int`, `float`, and `std::string`, you write it once and the compiler generates all three versions at compile time.

There are two kinds of templates:

| Kind | What it parameterises | Defined in |
|---|---|---|
| **Function template** | A function that works for any type | `.hpp` / `.tpp` header |
| **Class template** | A class whose members depend on a type | `.hpp` / `.tpp` header |

The most critical rule is that **template definitions must live entirely in the header**. When the compiler encounters `Array<int>` in a `.cpp` file, it needs the full template source to generate the `int` version. If the implementation were hidden in a `.cpp` file, the linker would fail with "undefined reference" errors.

This module contains three exercises, each highlighting a different dimension of templates:

1. **ex00** — simple function templates (algorithms that work on any type)
2. **ex01** — a function template that takes another function as a parameter (higher-order template)
3. **ex02** — a class template with full Orthodox Canonical Form and memory management

---

## Exercise 00: Start with a few functions

### Main Concepts
- **Function templates**: writing a single algorithm that the compiler instantiates for every required type.
- **Template type deduction**: the compiler infers `T` from the argument types; you rarely need to spell it out as `swap<int>(a, b)`.
- **Returning `const T&`**: avoiding unnecessary copies by returning a reference to one of the inputs.
- **The `<` convention**: by relying only on `operator<`, the templates work for any type that supports comparison — integers, floats, strings, or any user-defined type with `operator<` defined.

### Theoretical Explanation

#### What is a function template?

A function template is written once with a placeholder type `T`. Each time the compiler sees the function called with a concrete type (e.g., `int` or `std::string`), it generates a fully-typed copy called a **template instantiation**. This process happens entirely at compile time, so there is zero runtime overhead compared to writing the same function manually for every type.

```cpp
// Written once — generates a version for every T used in the program
template <typename T>
void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
```

The `template <typename T>` line tells the compiler: "the following definition depends on a type `T` that will be supplied later."

#### The `<` convention — concept requirements in C++98

Modern C++ (C++20) uses *concepts* to formally declare the requirements a type must satisfy. In C++98 we have no such syntax, but the convention is the same: by only using `operator<` inside `min` and `max`, we implicitly require that any type passed to these functions has `operator<` defined. This keeps the templates maximally compatible.

```cpp
// Only operator< is needed — works for int, float, std::string, custom types...
template <typename T>
T const &min(T const &a, T const &b) { return (a < b) ? a : b; }

template <typename T>
T const &max(T const &a, T const &b) { return (b < a) ? a : b; }
```

#### Returning by `const T&`

`min` and `max` return a reference to one of their parameters instead of a copy. This avoids an unnecessary copy for complex types like `std::string`. The reference is valid as long as the original variables are in scope, which is always the case here since the parameters are references to the caller's variables.

The `const` qualifier prevents the caller from accidentally modifying the returned value through the reference.

### How the Code is Done and Why

**`whatever.hpp`** contains all three templates. There is no `.cpp` companion file — templates cannot be compiled in isolation.

**`swap`**: stores `a` in a temporary, overwrites `a` with `b`, overwrites `b` with the temporary. Classic three-step swap.

**`min(a, b)`**: uses `(a < b) ? a : b`. When `a == b`, `a < b` is false, so the ternary returns `b` — the *second* parameter. This matches the specification ("if equal, returns the second").

**`max(a, b)`**: uses `(b < a) ? a : b`. When `a == b`, `b < a` is false, so the ternary returns `b` — the *second* parameter again, as specified. When `b < a` (a is larger), it returns `a` correctly.

Both `min` and `max` are prefixed with `::` in `main.cpp` to call the global template version rather than any standard library overload that might otherwise be picked up by argument-dependent lookup.

### The Tests (`main.cpp`)

```
a = 3, b = 2           ← integers swapped
min(a, b) = 2          ← min of 3, 2 is 2
max(a, b) = 3          ← max of 3, 2 is 3

c = chaine2, d = chaine1    ← strings swapped
min(c, d) = chaine1         ← lexicographic min
max(c, d) = chaine2         ← lexicographic max

min(5, 5) returns second: yes   ← equality edge case
max(5, 5) returns second: yes   ← equality edge case
```

The equality test compares addresses (`&::min(x, y) == &y`) to confirm the reference returned is truly the *second* parameter when both are equal, not merely equal in value.

---

## Exercise 01: Iter

### Main Concepts
- **Higher-order function templates**: a template that takes another function (or function template instantiation) as a parameter.
- **Two-parameter template `<typename T, typename F>`**: making the function type F a separate template parameter so it is freely deduced, handling both `void f(T&)` and `void f(const T&)` without overloads.
- **Array traversal without STL**: applying a function to every element of a raw C-style array.
- **const vs. non-const arrays**: demonstrating that the same `iter` works for both mutable and immutable arrays.

### Theoretical Explanation

#### What is a higher-order function?

A higher-order function is one that takes another function as an argument (or returns a function). In C++98, functions can be passed as function pointers. Templates allow us to abstract over *which* function pointer is passed, so `iter` can accept any callable that matches the element type.

```cpp
// F is deduced from whatever is passed as the third argument:
// iter(arr, 5, printElem<int>)  →  F = void(*)(int const&)
// iter(arr, 5, doubleElem<int>) →  F = void(*)(int&)
template <typename T, typename F>
void iter(T *arr, const std::size_t len, F f)
{
    for (std::size_t i = 0; i < len; ++i)
        f(arr[i]);
}
```

#### Why two template parameters instead of one?

The subject states that the third parameter "can receive its argument by `const` reference or non-`const` reference depending on context." Two approaches are possible:

**Option A — one template parameter, two overloads:**
```cpp
template <typename T>
void iter(T *arr, size_t len, void (*f)(T &));

template <typename T>
void iter(const T *arr, size_t len, void (*f)(const T &));
```
This works but is verbose and forces an exact match between the array's const-ness and the function signature.

**Option B — two template parameters (chosen approach):**
```cpp
template <typename T, typename F>
void iter(T *arr, const std::size_t len, F f);
```
`F` is deduced to whatever the caller passes. For a mutable array of `int`, passing `printElem<int>` (which takes `const int&`) works because `int` is implicitly convertible to `const int&`. For a `const std::string` array, passing `printElem<std::string>` (which takes `const std::string&`) also works because T deduces to `const std::string`. The same single template handles all cases cleanly.

#### `std::size_t` for array length

`std::size_t` (from `<cstddef>`) is the unsigned integer type returned by `sizeof` and used throughout C and C++ for sizes and indices. Using `std::size_t` instead of `int` avoids signed/unsigned comparison warnings and correctly represents arrays of any size on any platform.

### How the Code is Done and Why

**`iter.hpp`** defines the single template. The entire implementation is in the header because it is a function template — the compiler needs the full source at instantiation time.

In `main.cpp`, two helper function templates are defined:
- `printElem<T>`: takes `const T&` (read-only, works on both const and non-const arrays).
- `doubleElem<T>`: takes `T&` (modifying, only works on non-const arrays).

The tests call `::iter(intArr, intLen, doubleElem<int>)` to mutate the array, then `::iter` again with `printElem<int>` to print the mutated result. For the const string array, only `printElem` is used (which is the only callable that accepts `const std::string&`).

### The Tests (`main.cpp`)

```
Integer array:           ← original values 1–5
1  2  3  4  5

After doubling:          ← values mutated in-place by doubleElem
2  4  6  8  10

Const string array:      ← read-only, only printElem works
hello  world  42

Float array:
1.1  2.2  3.3
```

This demonstrates that `iter` is type-agnostic: it works with `int`, `const std::string`, and `float` without any code duplication.

---

## Exercise 02: Array

### Main Concepts
- **Class templates**: defining a generic container whose element type is a template parameter `T`.
- **Orthodox Canonical Form for templates**: the same four methods (default constructor, copy constructor, assignment operator, destructor), but each prefixed with `template <typename T>`.
- **Deep copy**: when copying an `Array`, allocating new memory and copying elements, so that mutations to the copy do not affect the original.
- **`new T[n]()`**: value-initialization of array elements (zeros for POD types, default construction for class types).
- **`operator[]` with bounds checking**: throwing a custom exception instead of causing undefined behaviour.
- **`.tpp` file pattern**: separating the template declaration (`.hpp`) from its implementation (`.tpp`) for readability, while still including the `.tpp` from the `.hpp` so the compiler sees both.

### Theoretical Explanation

#### Class templates vs. function templates

A class template is a blueprint for a *class* rather than a function. When you write `Array<int>`, the compiler generates a complete class with `int` substituted for `T`. When you write `Array<std::string>`, it generates a different complete class with `std::string` substituted. These are two separate types as far as C++ is concerned — neither is a subtype of the other.

```cpp
template <typename T>
class Array
{
  // ... T is used wherever the element type is needed
};
```

#### Orthodox Canonical Form with templates

Every member function of the class template must be prefixed with the template declaration when defined outside the class body:

```cpp
template <typename T>
Array<T>::Array() : _data(NULL), _size(0) {}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &other) { /* ... */ }
```

Note that the scope resolution `Array<T>::` requires the `<T>` because the class itself is a template and its full name is `Array<T>`.

#### Value-initialization with `new T[n]()`

The subject hint says to try `int* a = new int()` and observe that `*a` is `0`. This is *value-initialization*:

- For **POD types** (`int`, `double`, etc.): sets all bytes to zero.
- For **class types**: calls the default constructor.

Without the parentheses (`new T[n]`), POD elements would be *default-initialized* — their values are indeterminate (garbage). The parentheses `()` make all elements predictably zero or default-constructed, which is the correct behaviour for a general-purpose container.

#### Deep copy — why it matters

A *shallow copy* would copy the pointer `_data`, making both the original and the copy point to the same memory. Modifying one would silently corrupt the other — a dangerous bug that is hard to track down.

A *deep copy* allocates a fresh block of memory and copies elements one by one:

```cpp
template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &other)
{
    if (this != &other)
    {
        delete[] _data;          // free old memory first
        _size = other._size;
        _data = new T[_size];    // allocate new block
        for (unsigned int i = 0; i < _size; ++i)
            _data[i] = other._data[i];  // copy each element
    }
    return *this;
}
```

The `if (this != &other)` self-assignment guard is essential: without it, `delete[] _data` would destroy the source data before copying it, silently corrupting the array.

#### `operator[]` with bounds checking

Raw C arrays silently access memory out of bounds, causing undefined behaviour. The templated `operator[]` prevents this:

```cpp
template <typename T>
T &Array<T>::operator[](unsigned int idx)
{
    if (idx >= _size)
        throw IndexOutOfBoundsException();
    return _data[idx];
}
```

Two overloads are provided: one returning `T&` (for mutation) and one returning `const T&` (callable on a `const Array`). Both perform the same bounds check.

The exception class is nested inside `Array` and inherits from `std::exception`:

```cpp
class IndexOutOfBoundsException : public std::exception {
  public:
    const char *what() const throw();
};
```

This follows the same pattern established in Module 05: custom, descriptive exceptions that integrate naturally with standard `catch (std::exception &)` handlers.

#### `.tpp` file pattern

Template implementations in a separate `.tpp` file improve readability — the `.hpp` shows the interface at a glance without the noise of implementations. The `.tpp` is *included* at the bottom of the `.hpp` (not compiled separately), so the compiler still sees everything it needs:

```cpp
// Array.hpp (last line)
# include "Array.tpp"
```

The `ARRAY_TPP` include guard in `.tpp` prevents double-inclusion if anything else were to include it directly.

### How the Code is Done and Why

**`Array.hpp`** declares the class template: the nested exception class, the four OCF methods, both `operator[]` overloads, and `size()`. It ends with `#include "Array.tpp"`.

**`Array.tpp`** provides the implementations. Each method is prefixed `template <typename T>` and uses `Array<T>::` as the class scope.

Key implementation choices:
- `_data(NULL)` in the default constructor: safe to `delete[] NULL` (no-op), so the destructor is always correct.
- `_data(new T[n]())` in the n-constructor: value-initializes all elements.
- Copy constructor delegates to `operator=` via `*this = other` after setting `_data = NULL`. This avoids duplicating the deep-copy logic.

### The Tests (`main.cpp`)

```
Empty array size: 0          ← default constructor

Array<int>(5), default values:
  intArr[0] = 0              ← value-initialization works
  intArr[1] = 0
  ...

Original after copy[0] = 999:
  intArr[0] = 0              ← original unchanged — deep copy confirmed
Copy:
  copy[0] = 999              ← only the copy was mutated

Original intArr[1] after assigned[1]=777: 10   ← assignment also deep copies

Exception caught: Array: index out of bounds   ← out-of-range access handled

String array:
  strArr[0] = hello          ← works with std::string, not just int
  strArr[1] = world
  strArr[2] = 42
```

Each test targets a specific behaviour: value-initialization, copy independence (both for copy constructor and assignment operator), exception safety, and template generality with a non-trivial type (`std::string`).

---

## Summary: Template Concepts by Exercise

| Exercise | Template kind | Key concept | Why it matters |
|---|---|---|---|
| ex00 — swap/min/max | Function template | Type-generic algorithms with `operator<` | Write once, reuse for any comparable type |
| ex01 — iter | Function template (2 params) | Higher-order template: function as parameter | Decouples traversal from action — same pattern as `std::for_each` |
| ex02 — Array | Class template + OCF | Generic container with deep copy & bounds check | Foundation for understanding how `std::vector` works internally |

The golden rule of C++ templates: **define everything in the header**. The compiler cannot generate instantiations from a `.cpp` file it never sees when compiling the translation unit that uses the template.

---

*Templates: write once, compile for everything.*
