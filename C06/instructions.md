# C++ - Module 06: C++ Casts

## General Rules

**Compilation:**
- Compile with `c++` and flags `-Wall -Wextra -Werror`
- Must compile with `-std=c++98`

**Forbidden:**
- `using namespace <ns_name>` and `friend` keywords
- STL (Containers, Algorithms) — not allowed until Module 08/09
- `*printf()`, `*alloc()`, `free()`
- C++11 and Boost libraries

**Design requirements:**
- Orthodox Canonical Form (Modules 02–09), unless stated otherwise
- No function implementations in header files (except templates)
- Include guards in all headers
- Avoid memory leaks

---

## Additional Rule (Mandatory)

For **each exercise**, type conversion must be handled using a **specific C++ cast**. Your choice will be reviewed during the defense.

---

## Exercise 00: Conversion of Scalar Types

**Directory:** `ex00/`  
**Files:** `Makefile, *.cpp, *.{h,hpp}`  
**Authorized:** Any string-to-int/float/double conversion function

### Task

Write a class `ScalarConverter` with only one **static** method `convert` that takes a string representation of a C++ literal and prints its value as:
- `char`
- `int`
- `float`
- `double`

The class must **not be instantiable** by users.

Only decimal notation is used, except for `char` literals.

**Handle these pseudo-literals:**
- Float: `-inff`, `+inff`, `nanf`
- Double: `-inf`, `+inf`, `nan`

If a conversion is impossible or overflows, print an informative message. Include headers for numeric limits and special values.

### Expected Output

```
./convert 0
char: Non displayable
int: 0
float: 0.0f
double: 0.0

./convert nan
char: impossible
int: impossible
float: nanf
double: nan

./convert 42.0f
char: '*'
int: 42
float: 42.0f
double: 42.0
```

---

## Exercise 01: Serialization

**Directory:** `ex01/`  
**Files:** `Makefile, *.cpp, *.{h,hpp}`  
**Forbidden:** None

### Task

Implement a class `Serializer`, not initializable by the user, with these **static** methods:

```cpp
uintptr_t serialize(Data* ptr);
Data*      deserialize(uintptr_t raw);
```

- `serialize`: converts a pointer to `uintptr_t`
- `deserialize`: converts a `uintptr_t` back to a `Data*`

Create a **non-empty** `Data` struct (with data members). Call `serialize()` on a `Data*`, pass the result to `deserialize()`, and verify the returned pointer equals the original.

Turn in the `Data` struct files.

---

## Exercise 02: Identify Real Type

**Directory:** `ex02/`  
**Files:** `Makefile, *.cpp, *.{h,hpp}`  
**Forbidden:** `std::typeinfo` / `<typeinfo>` header

### Task

Implement a `Base` class with only a **public virtual destructor**. Create three empty classes `A`, `B`, `C` that publicly inherit from `Base`. (Orthodox Canonical Form not required for these.)

Implement:

```cpp
Base* generate(void);       // randomly returns new A, B, or C as Base*
void  identify(Base* p);    // prints "A", "B", or "C"
void  identify(Base& p);    // prints "A", "B", or "C" — no pointer use allowed inside
```

- `identify(Base*)` → use `dynamic_cast` on pointer (returns `nullptr` on failure)
- `identify(Base&)` → use `dynamic_cast` on reference (throws `std::bad_cast` on failure)
