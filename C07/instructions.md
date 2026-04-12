# C++ - Module 07: C++ Templates

## General Rules

**Compilación:**
- Compilar con `c++` y flags `-Wall -Wextra -Werror`
- Debe compilar con `-std=c++98`

**Prohibido:**
- `using namespace <ns_name>` y `friend`
- STL (Containers, Algorithms) — no permitido hasta el Module 08/09
- `*printf()`, `*alloc()`, `free()`
- C++11 y Boost

**Diseño:**
- Orthodox Canonical Form (Modules 02–09), salvo indicación contraria
- No implementaciones de funciones en headers (excepto function templates)
- Include guards en todos los headers
- Sin memory leaks

> **Nota importante:** Los templates de función/clase SÍ deben definirse en los headers (`.hpp` / `.tpp`).

---

## Exercise 00: Start with a few functions

**Directorio:** `ex00/`  
**Archivos:** `Makefile, main.cpp, whatever.{h,hpp}`  
**Prohibido:** Nada

### Tarea

Implementar las siguientes **function templates**:

- `swap`: Intercambia los valores de dos parámetros. No retorna nada.
- `min`: Retorna el menor de dos valores. Si son iguales, retorna el segundo.
- `max`: Retorna el mayor de dos valores. Si son iguales, retorna el segundo.

Las funciones deben funcionar con cualquier tipo, siempre que ambos argumentos sean del mismo tipo y soporten los operadores de comparación.

**Los templates deben definirse en el header.**

### Output esperado

```cpp
int a = 2, b = 3;
::swap(a, b);
// a = 3, b = 2
// min(a, b) = 2
// max(a, b) = 3

std::string c = "chaine1", d = "chaine2";
::swap(c, d);
// c = chaine2, d = chaine1
// min(c, d) = chaine1
// max(c, d) = chaine2
```

---

## Exercise 01: Iter

**Directorio:** `ex01/`  
**Archivos:** `Makefile, main.cpp, iter.{h,hpp}`  
**Prohibido:** Nada

### Tarea

Implementar una **function template** `iter` que recibe 3 parámetros y no retorna nada:

1. La dirección de un array.
2. La longitud del array, pasada como valor `const`.
3. Una función que se llamará sobre cada elemento del array.

`iter` debe funcionar con cualquier tipo de array. El tercer parámetro puede ser una función template instanciada.

La función del tercer parámetro puede recibir su argumento por referencia `const` o no-`const` según el contexto.

> **Pista:** Piensa bien cómo soportar tanto elementos `const` como no-`const` en tu función `iter`.

---

## Exercise 02: Array

**Directorio:** `ex02/`  
**Archivos:** `Makefile, main.cpp, Array.{h,hpp}` (opcional: `Array.tpp`)  
**Prohibido:** Nada

### Tarea

Desarrollar una **class template** `Array` que contiene elementos de tipo `T` con el siguiente comportamiento:

- **Constructor sin parámetros:** crea un array vacío.
- **Constructor con `unsigned int n`:** crea un array de `n` elementos inicializados por defecto.  
  *(Tip: prueba `int* a = new int();` y muestra `*a`.)*
- **Copy constructor y assignment operator:** modificar el original o la copia después de copiar no debe afectar al otro (deep copy).
- **Obligatorio usar `new[]`** para alojar memoria. Queda prohibida la preallocación. Nunca acceder a memoria no alocada.
- **Operador `[]`** para acceder a elementos. Si el índice está fuera de rango, lanzar `std::exception`.
- **Función miembro `size()`:** retorna el número de elementos. Sin parámetros, no modifica la instancia.
