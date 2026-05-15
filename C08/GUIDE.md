# CPP Module 08 — Guía de conceptos clave

> Templated containers, iterators, algorithms

---

## Ex00 — Easy find

### Concepto clave: function templates sobre contenedores genéricos

El ejercicio introduce el uso de `std::find` como algoritmo de búsqueda genérico.
La clave está en que la función no sabe con qué contenedor trabaja: puede ser un
`vector`, `list`, `deque`… cualquier contenedor que tenga iteradores de inicio y fin.

```cpp
// easyfind.hpp
template <typename T>
typename T::iterator easyfind(T& container, int value)
{
    typename T::iterator it = std::find(container.begin(), container.end(), value);
    if (it == container.end())
        throw std::runtime_error("Value not found in container");
    return it;
}
```

**Por qué `typename T::iterator`?**
Cuando `T` es un tipo dependiente del parámetro de plantilla, el compilador no
sabe a priori si `T::iterator` es un tipo o un valor. La palabra clave `typename`
le indica explícitamente que es un tipo.

**Doble sobrecarga (mutable / const):**
Se definen dos versiones: una que recibe `T&` y devuelve `iterator`, y otra que
recibe `const T&` y devuelve `const_iterator`. Esto permite usar `easyfind` con
contenedores constantes sin romper la const-correctness.

**`std::find` (header `<algorithm>`):**
```
std::find(begin, end, value)  →  iterador al primer elemento igual a value,
                                  o end si no se encuentra
```
Si el resultado es `end()`, significa "no encontrado" y lanzamos la excepción.

---

## Ex01 — Span

### Concepto clave: contenedores internos + algoritmos STL + template sobre iteradores

La clase `Span` envuelve un `std::vector<int>` y expone una API de alto nivel
para calcular rangos entre números.

### Estructura de la clase

```cpp
// Span.hpp (fragmento)
class Span
{
public:
    Span(unsigned int n);
    void addNumber(int n);

    template <typename Iterator>
    void addNumbers(Iterator begin, Iterator end)   // método plantilla
    {
        while (begin != end)
        {
            addNumber(*begin);  // reutiliza la lógica de addNumber
            ++begin;
        }
    }

    int shortestSpan() const;
    int longestSpan() const;

    class FullContainerException : public std::exception { ... };
    class NotEnoughNumbersException : public std::exception { ... };

private:
    unsigned int       _maxSize;
    std::vector<int>   _numbers;
};
```

### Por qué `addNumbers` es una plantilla en el `.hpp` y no en el `.cpp`

Las implementaciones de métodos plantilla deben estar visibles en el punto de
instanciación (donde se usa la plantilla). Si se mueven a un `.cpp`, el linker
no encuentra la especialización. Por eso van en el header.

### `shortestSpan` — algoritmo clave

```cpp
// Span.cpp
int Span::shortestSpan() const
{
    if (_numbers.size() < 2)
        throw NotEnoughNumbersException();

    std::vector<int> sorted(_numbers);   // copia para no alterar el original
    std::sort(sorted.begin(), sorted.end());

    int shortest = sorted[1] - sorted[0];
    for (std::vector<int>::size_type i = 2; i < sorted.size(); ++i)
    {
        int diff = sorted[i] - sorted[i - 1];
        if (diff < shortest)
            shortest = diff;
    }
    return shortest;
}
```

Ordenar primero garantiza que los dos números más cercanos siempre son
**adyacentes** en el array ordenado. Sin ordenar habría que comparar todos los
pares posibles (O(n²)); ordenando, basta un único recorrido adicional (O(n log n)).

### `longestSpan` — algoritmos `min_element` / `max_element`

```cpp
int Span::longestSpan() const
{
    if (_numbers.size() < 2)
        throw NotEnoughNumbersException();

    int minVal = *std::min_element(_numbers.begin(), _numbers.end());
    int maxVal = *std::max_element(_numbers.begin(), _numbers.end());
    return maxVal - minVal;
}
```

`std::min_element` y `std::max_element` devuelven **iteradores** al elemento
mínimo/máximo. El operador `*` desreferencia el iterador para obtener el valor.

---

## Ex02 — Mutated abomination

### Concepto clave: herencia de contenedor STL + exposición de iteradores internos

`std::stack` es intencionalmente no iterable: es un adaptador que envuelve otro
contenedor (por defecto `std::deque`) y sólo expone `push`, `pop`, `top`, `size`
y `empty`.

### Cómo funciona `std::stack` internamente

```
std::stack<T>
    └── protected: container_type c   ← el std::deque<T> subyacente
```

La clave es que el contenedor interno se llama `c` y es `protected`, lo que
significa que las clases derivadas pueden acceder a él directamente.

### La solución: heredar y exponer `c`

```cpp
// MutantStack.hpp
template <typename T>
class MutantStack : public std::stack<T>
{
public:
    // Typedefs que reutilizan los tipos del contenedor subyacente
    typedef typename std::stack<T>::container_type::iterator               iterator;
    typedef typename std::stack<T>::container_type::const_iterator         const_iterator;
    typedef typename std::stack<T>::container_type::reverse_iterator       reverse_iterator;
    typedef typename std::stack<T>::container_type::const_reverse_iterator const_reverse_iterator;

    // Métodos que delegan al contenedor interno protegido
    iterator begin()              { return this->c.begin(); }
    iterator end()                { return this->c.end(); }
    const_iterator begin() const  { return this->c.begin(); }
    const_iterator end() const    { return this->c.end(); }

    reverse_iterator rbegin()              { return this->c.rbegin(); }
    reverse_iterator rend()                { return this->c.rend(); }
    const_reverse_iterator rbegin() const  { return this->c.rbegin(); }
    const_reverse_iterator rend() const    { return this->c.rend(); }
};
```

**`this->c`**: en plantillas con herencia, el compilador no busca automáticamente
en la clase base dependiente. Hay que calificar explícitamente con `this->` para
que encuentre el miembro `c` de `std::stack<T>`.

**`container_type`**: es el typedef que `std::stack` expone para referirse al
tipo del contenedor subyacente. Por defecto es `std::deque<T>`, pero podría ser
otro si se instancia la pila con un segundo parámetro de plantilla.

### Iteración en el mismo orden que `std::list`

El ejercicio pide que el output iterable de `MutantStack` coincida con el de
`std::list`. Como `std::deque` (el contenedor interno) preserva el orden de
inserción de principio a fin, y `std::list` también lo hace, ambos producen el
mismo recorrido:

```
push: 5, 3, 5, 737, 0   →   iteración: 5 3 5 737 0
```

`std::stack` usa el extremo final del deque como "cima": `push` hace `push_back`
y `pop` hace `pop_back`. Por eso `begin()` apunta al elemento más antiguo y
`end()` al más nuevo, lo que coincide con el orden de inserción.

---

## Resumen comparativo

| | Ex00 | Ex01 | Ex02 |
|---|---|---|---|
| **Técnica principal** | Function template genérica | Clase con STL interno + templates | Herencia de adaptador STL |
| **Algoritmo STL usado** | `std::find` | `std::sort`, `std::min/max_element` | Iteradores de `std::deque` |
| **Manejo de errores** | `std::runtime_error` | Excepciones anidadas custom | — |
| **Por qué en header** | Template → visible en punto de uso | Método `addNumbers` es template | Toda la clase es template |
| **Palabra clave clave** | `typename` | `typename` | `this->c`, `container_type` |
