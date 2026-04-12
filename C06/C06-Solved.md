# C++ Module 06: C++ Casts — Solved

## Overview of the Module

Module 06 is all about **explicit type conversion** in C++. Whereas C provides a single, blunt cast operator `(Type)value`, C++ introduces **four named casts**, each with a specific, clearly-stated purpose:

| Cast | Purpose |
|---|---|
| `static_cast` | Compile-time type conversions between related types |
| `reinterpret_cast` | Bit-level reinterpretation — pointer ↔ integer, unrelated pointer types |
| `const_cast` | Add or remove `const`/`volatile` qualifiers |
| `dynamic_cast` | Safe runtime downcast in a polymorphic hierarchy |

Using the right cast is not a style choice — it is a *semantic* choice. Each cast tells the compiler (and the human reader) **why** the conversion is happening. A misused cast is an immediate red flag in code review and a common source of undefined behaviour.

This module introduces a different exercise for each of the three most important casts (`static_cast`, `reinterpret_cast`, `dynamic_cast`). `const_cast` is deliberately omitted because it is almost never needed in well-designed C++98 code.

---

## Exercise 00: Conversion of Scalar Types

### Main Concepts
- **`static_cast`**: the idiomatic C++ cast for well-defined, compile-time conversions between arithmetic types.
- **Type detection from a string literal**: parsing a command-line argument to decide which C++ literal type it represents.
- **Pseudo-literals**: special IEEE 754 values (`nan`, `inf`) that are not numbers in the traditional sense.
- **Overflow and non-displayability**: detecting when a value falls outside the target type's valid range.

### Theoretical Explanation

#### Why `static_cast` here?

When you convert `double` → `int` or `double` → `char`, you are making a *well-defined, explicit, narrowing* conversion. `static_cast` is the right tool because:

1. The relationship between the types is known **at compile time** (both are arithmetic types).
2. The compiler checks that the conversion is at least theoretically valid — unlike `reinterpret_cast`, which would blindly reinterpret bits.
3. It signals intent: "I know this might lose information; I am doing it on purpose."

```cpp
// C-style cast — compiles but gives no information about *why*
char c = (char)97.0;

// static_cast — clear, self-documenting, checked at compile time
char c = static_cast<char>(97.0);
```

#### IEEE 754 Pseudo-literals

Modern CPUs represent floating-point numbers using the IEEE 754 standard. This standard reserves special bit patterns for:
- **NaN** (Not-a-Number): result of `0/0`, `sqrt(-1)`, etc.
- **±Infinity**: result of overflow or `1/0`.

These values have no meaningful integer or character equivalent, so any attempt to convert them should print `"impossible"`.

In C++98, the functions `isnan()` and `isinf()` from `<cmath>` (originally C99 macros) are available on all POSIX platforms (Linux, macOS) and are the standard way to detect these values.

### How the Code is Done and Why

#### Architecture

`ScalarConverter` is a **utility class with only static members**. The subject explicitly states "the class must not be instantiable by users." We enforce this by declaring all four Orthodox Canonical Form methods (default constructor, copy constructor, assignment operator, destructor) as **`private`**. They exist — satisfying the form — but cannot be called from outside the class.

```cpp
class ScalarConverter
{
  private:
    ScalarConverter();                               // not instantiable
    ScalarConverter(const ScalarConverter &other);
    ScalarConverter &operator=(const ScalarConverter &other);
    ~ScalarConverter();

  public:
    static void convert(const std::string &literal);
};
```

#### Detection Logic

The `convert` method needs to determine which C++ literal type the string represents before it can parse and display it. The detection runs in this priority order:

1. **Pseudo-literals** (`nan`, `nanf`, `+inf`, `-inf`, `+inff`, `-inff`): exact string match, handled immediately without numeric parsing.
2. **Char literals**: either a single non-digit character (`a`) or the quoted form (`'a'` — three characters: `'`, the char, `'`).
3. **Float literals**: after pseudo-literal/char checks, if the string ends with `'f'`, it is a float.
4. **Double literals**: if the string contains a decimal point `.`, it is a double.
5. **Int literals**: everything else.

#### Parsing Strategy — Using `double` as the Universal Intermediate

Rather than writing separate parsing paths for int, float, and double, the implementation converts **everything into a `double`** first:

```
input string  →  parse to double  →  static_cast<char>
                                  →  static_cast<int>
                                  →  static_cast<float>
                                  →  (already double)
```

`double` can exactly represent all 32-bit integer values and all 32-bit floats, so no information is lost in the intermediate step for the common cases.

For char literals, we use `static_cast<double>(c)` directly. For numeric literals, we use `strtod()` (the C standard library function for parsing doubles). Crucially, we check `strtod`'s `endptr` output — if the pointer does not point to `'\0'` after parsing, the entire string was not consumed, meaning the input is invalid.

```cpp
char *end;
double value = strtod(toParse.c_str(), &end);
if (*end != '\0')
{
    // Input was not a valid number — print "impossible" for all types
}
```

#### Output Rules

| Output | When |
|---|---|
| `char: 'X'` | ASCII 32–126 (printable) |
| `char: Non displayable` | ASCII 0–31 or 127 (control characters) |
| `char: impossible` | Value outside 0–127, or NaN/Inf |
| `int: impossible` | Value > `INT_MAX`, < `INT_MIN`, or NaN/Inf |
| `float: nanf` / `±inff` | Float is NaN or infinite |
| `float: 42.0f` | Normal float (1 decimal place minimum) |
| `double: nan` / `±inf` | Double is NaN or infinite |
| `double: 42.0` | Normal double (1 decimal place minimum) |

The `std::fixed << std::setprecision(1)` stream manipulators are set once and affect all subsequent float/double output, ensuring values like `42` are displayed as `42.0` rather than just `42`.

### The Tests (`main.cpp`)

The program takes a single command-line argument:

```bash
./convert 0          # char: Non displayable / int: 0 / float: 0.0f / double: 0.0
./convert nan        # char: impossible / int: impossible / float: nanf / double: nan
./convert 42.0f      # char: '*' / int: 42 / float: 42.0f / double: 42.0
./convert 'a'        # char: 'a' / int: 97 / float: 97.0f / double: 97.0
./convert +inf       # char: impossible / int: impossible / float: +inff / double: +inf
./convert 2147483648 # char: impossible / int: impossible (overflow) / float: 2147483648.0f
```

---

## Exercise 01: Serialization

### Main Concepts
- **`reinterpret_cast`**: bit-level reinterpretation of a pointer as an integer and back.
- **`uintptr_t`**: the platform-specific unsigned integer type guaranteed to be wide enough to hold any pointer.
- **Pointer identity**: confirming that serialize → deserialize is a lossless round-trip.

### Theoretical Explanation

#### Why `reinterpret_cast` here?

A pointer is, at the hardware level, just an integer: the address of a memory location. `reinterpret_cast` lets us treat that address as a plain `uintptr_t` integer — without any arithmetic or type-checked conversion. No bits change; only the C++ type label changes.

```cpp
// pointer → integer: take the address and label it as uintptr_t
uintptr_t raw = reinterpret_cast<uintptr_t>(ptr);

// integer → pointer: take the integer and label it as Data*
Data* p = reinterpret_cast<Data*>(raw);
```

This is the *only* correct cast to use here:
- `static_cast` cannot convert between unrelated pointer types and integer types.
- `dynamic_cast` requires a polymorphic hierarchy.
- `const_cast` only removes const — useless here.

#### What is `uintptr_t`?

`uintptr_t` (from `<stdint.h>` / `<cstdint>`) is an **unsigned integer type whose size is guaranteed to equal the size of a pointer** on the current platform. On a 64-bit system it is 64 bits wide; on a 32-bit system it is 32 bits wide. This guarantee is essential: if we used `unsigned int` (always 32 bits), we would silently truncate pointer values on 64-bit systems and corrupt the round-trip.

#### Why does it work?

The C++ standard guarantees that:

> A pointer can be converted to an integer type large enough to hold it. [...] Converting the resulting integer back to the original pointer type will produce the original pointer value. — C++11 §5.2.10/4 (same rule in C++98)

So the serialize → deserialize round-trip is well-defined behaviour: we get back the exact same pointer we started with.

### How the Code is Done and Why

The `Serializer` class follows the same non-instantiable pattern as `ScalarConverter`: all OCF methods are private, and the two methods are static:

```cpp
uintptr_t Serializer::serialize(Data *ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);  // pointer bits → integer
}

Data *Serializer::deserialize(uintptr_t raw)
{
    return reinterpret_cast<Data *>(raw);     // integer bits → pointer
}
```

The `Data` struct has three members (`name`, `id`, `score`) to make it non-trivially-empty and to prove the round-trip preserves access to the actual data.

### The Tests (`main.cpp`)

```
Original pointer : 0x16d002c60
Data content     : name=Ford Prefect, id=42, score=9.8

Serialized value : 6123695200

Deserialized ptr : 0x16d002c60
Pointers match   : yes
Data content     : name=Ford Prefect, id=42, score=9.8
```

The serialized integer is just the decimal representation of the memory address. After deserialization, the pointer is identical to the original, and all data fields are still accessible through the recovered pointer.

---

## Exercise 02: Identify Real Type

### Main Concepts
- **`dynamic_cast`**: runtime type checking in a polymorphic hierarchy using the vtable.
- **Pointer vs. reference semantics for casts**: pointer cast returns `nullptr` on failure; reference cast throws `std::bad_cast`.
- **Why `<typeinfo>` is forbidden**: using `typeid` is the naive solution the exercise explicitly prohibits, forcing you to use `dynamic_cast` properly.

### Theoretical Explanation

#### Why `dynamic_cast` here?

`dynamic_cast` is the **only** C++ cast that performs a check at **runtime**. It inspects the object's vtable to determine the real type of the object at the other end of a `Base*` pointer. It requires:

1. The base class to have at least one **virtual function** (or a virtual destructor — which counts). Without this, there is no vtable and `dynamic_cast` cannot work.
2. A polymorphic hierarchy: the cast target must be a class in the same inheritance tree.

```
Base (virtual ~Base)
 ├── A
 ├── B
 └── C
```

`Base` has only a `virtual ~Base()`. This single virtual function is enough to make `Base` polymorphic and to allow `dynamic_cast` to function.

#### Pointer cast vs. reference cast — the critical difference

| Scenario | Result |
|---|---|
| `dynamic_cast<A*>(p)` — success | Returns a valid `A*` |
| `dynamic_cast<A*>(p)` — failure | Returns `nullptr` |
| `dynamic_cast<A&>(p)` — success | Returns a valid `A&` |
| `dynamic_cast<A&>(p)` — failure | **Throws `std::bad_cast`** |

The subject requires the **reference overload** of `identify` to avoid using pointers. Since a reference cannot be null, failure must be signalled via an exception:

```cpp
void identify(Base &p)
{
    try
    {
        (void)dynamic_cast<A &>(p);  // throws if p is not really an A
        std::cout << "A" << std::endl;
        return;
    }
    catch (std::bad_cast &) {}
    // ... repeat for B and C
}
```

The `(void)` cast is needed because GCC would emit an "unused variable" warning otherwise.

#### Why is `<typeinfo>` forbidden?

The naive solution to "identify the real type of a Base pointer" is:

```cpp
#include <typeinfo>
if (typeid(*p) == typeid(A))  // FORBIDDEN
```

This works, but it is a brute-force approach: it gives you the exact runtime type, bypassing the normal polymorphism mechanism. The subject forbids it to force you to use `dynamic_cast` — the **proper, hierarchy-aware** tool for this job. `dynamic_cast` also handles intermediate types in deep hierarchies correctly (`dynamic_cast<A*>` succeeds even if `A` is a class derived from another intermediate class), whereas `typeid` equality only matches the exact type.

### How the Code is Done and Why

#### Class structure

`Base` has only one member: the virtual destructor. This makes it polymorphic without any unnecessary complexity.

```cpp
// Base.hpp
class Base { public: virtual ~Base(); };

// A.hpp / B.hpp / C.hpp  — completely empty bodies
class A : public Base {};
class B : public Base {};
class C : public Base {};
```

Orthodox Canonical Form is **not required** for A, B, C (as stated in the subject), since they carry no resources and have no meaningful copy semantics.

#### `generate()`

Seeds `rand()` with `time(NULL)` in `main()` (once per run), then picks one of three classes:

```cpp
Base *generate(void)
{
    int r = rand() % 3;
    if (r == 0) return new A();
    if (r == 1) return new B();
    return new C();
}
```

The caller receives a `Base*` and has no compile-time way to know which concrete type it holds.

#### `identify(Base *p)` — pointer version

```cpp
void identify(Base *p)
{
    if (dynamic_cast<A *>(p)) std::cout << "A" << std::endl;
    else if (dynamic_cast<B *>(p)) std::cout << "B" << std::endl;
    else if (dynamic_cast<C *>(p)) std::cout << "C" << std::endl;
}
```

Each `dynamic_cast` attempt returns either a valid pointer (truthy) or `nullptr` (falsy). A simple `if/else` chain is enough.

#### `identify(Base &p)` — reference version

```cpp
void identify(Base &p)
{
    try { (void)dynamic_cast<A &>(p); std::cout << "A" << std::endl; return; }
    catch (std::bad_cast &) {}
    try { (void)dynamic_cast<B &>(p); std::cout << "B" << std::endl; return; }
    catch (std::bad_cast &) {}
    try { (void)dynamic_cast<C &>(p); std::cout << "C" << std::endl; }
    catch (std::bad_cast &) {}
}
```

Three independent `try/catch` blocks are used. The `return` after each success ensures only one type is printed. The final `C` block has no `return` because it is the last candidate.

### The Tests (`main.cpp`)

```
=== Real-Type Identification ===
Round 1  |  pointer: C
         |  reference: C
Round 2  |  pointer: A
         |  reference: A
...
```

Both `identify` overloads agree on every object, confirming that both the pointer-based and reference-based `dynamic_cast` paths work correctly. The output is random but consistent within each round (the same object is inspected twice — once by pointer, once by reference).

---

## Summary: Which Cast, When, and Why

| Exercise | Cast used | Why that cast |
|---|---|---|
| ex00 — scalar conversion | `static_cast` | Arithmetic conversions between related types, checked at compile time |
| ex01 — serialization | `reinterpret_cast` | Raw bit-level reinterpretation of pointer ↔ integer |
| ex02 — type identification | `dynamic_cast` | Runtime polymorphic type check via vtable, safe downcast |

The golden rule: **use the weakest cast that does the job**. `static_cast` is safer than `reinterpret_cast`. `dynamic_cast` is slower but safer than `static_cast` for downcasts. `reinterpret_cast` bypasses all safety — use it only when you have no other option (low-level serialization, hardware addresses, etc.).

---

*Cast wisely, and may your undefined behaviour be zero.*
