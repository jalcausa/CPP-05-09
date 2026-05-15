# CPP Module 09 — Guía de conceptos clave

> STL containers: `std::map`, `std::stack`, `std::vector`, `std::deque`

---

## Ex00 — Bitcoin Exchange

### Concepto clave: `std::map` como base de datos ordenada + `lower_bound` para búsqueda de fecha más cercana

El ejercicio modela una base de datos histórica de precios de Bitcoin. La clave de
diseño es que `std::map<std::string, float>` mantiene sus claves ordenadas
lexicográficamente — y las fechas en formato `YYYY-MM-DD` tienen la propiedad de
que el orden lexicográfico coincide exactamente con el orden cronológico. Esto
permite aprovechar `lower_bound` para encontrar la fecha más cercana sin iterar
todo el mapa.

### Búsqueda de la fecha más cercana con `lower_bound`

```cpp
// BitcoinExchange.cpp
float BitcoinExchange::getRate(const std::string &date) const
{
    std::map<std::string, float>::const_iterator it = _database.lower_bound(date);

    if (it != _database.end() && it->first == date)
        return it->second;       // coincidencia exacta

    if (it == _database.begin())
        return -1.0f;            // no hay ninguna fecha anterior

    --it;                        // retroceder al día más próximo anterior
    return it->second;
}
```

`lower_bound(key)` devuelve un iterador al **primer elemento cuya clave es >= key**.
Si la clave exacta no existe, apunta al siguiente. Decrementando el iterador se obtiene
el elemento más reciente anterior a la fecha solicitada — exactamente lo que pide el enunciado.

**Por qué `std::map` y no `std::unordered_map`:**
`unordered_map` no está ordenado, por lo que `lower_bound` no existe. El orden es
imprescindible para el algoritmo de búsqueda por proximidad.

### Carga de la base de datos (CSV)

```cpp
void BitcoinExchange::loadDatabase(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    std::string line;
    std::getline(file, line); // descarta la cabecera "date,exchange_rate"

    while (std::getline(file, line))
    {
        std::size_t sep = line.find(',');
        std::string date    = line.substr(0, sep);
        std::string rateStr = line.substr(sep + 1);
        float rate = static_cast<float>(std::atof(rateStr.c_str()));
        _database[date] = rate;
    }
}
```

Se usa `std::ifstream` para leer línea a línea. La cabecera se descarta con el primer
`getline`. Cada línea se parte por la coma con `find` + `substr`.

### Validación de fecha y valor

```cpp
bool BitcoinExchange::isValidDate(const std::string &date) const
{
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    // verificar que el resto son dígitos...
    int month = std::atoi(date.substr(5, 2).c_str());
    int day   = std::atoi(date.substr(8, 2).c_str());
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31)    return false;
    return true;
}
```

La validación cubre: longitud exacta de 10, guiones en posiciones 4 y 7, dígitos en
el resto, mes 1-12, día 1-31.

Para el valor se valida carácter a carácter (sólo dígitos y un único punto decimal),
sin negativos, y dentro del rango [0, 1000].

### Procesamiento de la entrada

```cpp
void BitcoinExchange::processInput(const std::string &filename)
{
    // ...
    std::size_t sep = line.find(" | ");
    if (sep == std::string::npos)
    {
        std::cout << "Error: bad input => " << line << std::endl;
        continue;
    }
    std::string date     = line.substr(0, sep);
    std::string valueStr = line.substr(sep + 3);
    // validar fecha, valor, rango...
    std::cout << date << " => " << value << " = " << value * rate << std::endl;
}
```

El separador es exactamente `" | "` (tres caracteres). Si no se encuentra, la línea
es inválida y se imprime el error correspondiente.

---

## Ex01 — Reverse Polish Notation

### Concepto clave: `std::stack` como evaluador de expresiones postfijas

La Notación Polaca Inversa (RPN) coloca los operadores después de sus operandos.
`std::stack<int>` es el contenedor ideal porque RPN sigue exactamente la disciplina
LIFO: empujamos números y, cuando llega un operador, sacamos los dos últimos para
operar sobre ellos.

### Algoritmo de evaluación

```cpp
// RPN.cpp
int RPN::evaluate(const std::string &expression)
{
    while (!_stack.empty()) _stack.pop(); // limpiar estado previo

    std::istringstream iss(expression);
    std::string token;

    while (iss >> token)
    {
        if (token.size() == 1 && token[0] >= '0' && token[0] <= '9')
        {
            _stack.push(token[0] - '0');   // dígito → apilar
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/")
        {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            int b = _stack.top(); _stack.pop();  // segundo operando
            int a = _stack.top(); _stack.pop();  // primer operando

            if (token == "+") _stack.push(a + b);
            else if (token == "-") _stack.push(a - b);
            else if (token == "*") _stack.push(a * b);
            else if (token == "/")
            {
                if (b == 0) throw std::runtime_error("Error");
                _stack.push(a / b);
            }
        }
        else { throw std::runtime_error("Error"); }
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");

    return _stack.top();
}
```

**Orden de extracción:** `b` se extrae antes que `a` porque la pila es LIFO. Si la
expresión es `"3 4 -"`, la pila tiene [3, 4] con 4 en la cima. Extraemos b=4, luego
a=3, y calculamos a-b = 3-4 = -1, que es el resultado correcto (3 − 4, no 4 − 3).

**Invariante de validez:** al final de la evaluación, la pila debe tener exactamente
un elemento (el resultado). Si tiene 0 o más de 1, la expresión es inválida.

### Casos de error que maneja

| Entrada inválida | Motivo |
|---|---|
| `"1 +"` | menos de 2 operandos al llegar al operador |
| `"1 2 3 +"` | quedan más de un elemento al final |
| `"1 2 foo"` | token no es dígito ni operador conocido |
| `"4 0 /"` | división por cero |

### Por qué `std::stack` y no `std::vector` o `std::deque`

El enunciado exige `std::stack`. Conceptualmente, `stack` fuerza el acceso LIFO
a nivel de API (sólo `push`, `pop`, `top`), lo que refleja el algoritmo RPN con
precisión semántica. Un `vector` también funcionaría mecánicamente, pero stack
expresa la intención del código de forma más clara y es el contenedor canónico
para este patrón.

---

## Ex02 — PmergeMe (Ford-Johnson / Merge-Insert Sort)

### Concepto clave: algoritmo Ford-Johnson para minimizar comparaciones + dos contenedores para medir rendimiento

El algoritmo Ford-Johnson (también llamado merge-insert sort) es teóricamente
óptimo en número de comparaciones para secuencias pequeñas. El ejercicio lo implementa
dos veces: con `std::vector<int>` y con `std::deque<int>`, midiendo el tiempo de cada
uno con `gettimeofday`.

### Estructura del algoritmo (paso a paso)

#### Paso 1: Emparejar y separar ganadores y perdedores

```cpp
// PmergeMe.cpp — fragmento de fordJohnsonVec
bool hasStraggler = (seq.size() % 2 != 0);
int  straggler    = 0;
if (hasStraggler) { straggler = seq.back(); seq.pop_back(); }

size_t numPairs = seq.size() / 2;
std::vector<std::pair<int,int> > pairs(numPairs);
for (size_t i = 0; i < numPairs; ++i)
{
    int a = seq[2 * i], b = seq[2 * i + 1];
    pairs[i] = (a >= b) ? std::make_pair(a, b) : std::make_pair(b, a);
    // pairs[i].first  = ganador (mayor)
    // pairs[i].second = perdedor (menor)
}
```

Si el número de elementos es impar, el último se guarda como "rezagado" (`straggler`)
y se inserta al final.

#### Paso 2: Ordenar recursivamente los ganadores

```cpp
std::vector<int> winners;
for (size_t i = 0; i < numPairs; ++i)
    winners.push_back(pairs[i].first);

fordJohnsonVec(winners);  // llamada recursiva sobre los ganadores
```

Los ganadores se extraen a un nuevo vector y se ordenan recursivamente con el mismo
algoritmo. El caso base es 1 o 2 elementos.

#### Paso 3: Reconstruir pares manteniendo la correspondencia ganador↔perdedor

Después de la recursión, `winners` está ordenado pero hemos perdido qué perdedor
corresponde a cada ganador. Lo recuperamos emparejando por valor:

```cpp
std::vector<bool> matched(numPairs, false);
std::vector<std::pair<int,int> > sortedPairs(numPairs);
for (size_t i = 0; i < numPairs; ++i)
{
    for (size_t j = 0; j < numPairs; ++j)
    {
        if (!matched[j] && pairs[j].first == winners[i])
        {
            sortedPairs[i] = pairs[j];
            matched[j] = true;
            break;
        }
    }
}
pairs = sortedPairs; // ahora pairs[] está ordenado por ganador
```

#### Paso 4: Construir la cadena principal e insertar el primer perdedor

```cpp
std::vector<int> chain;
for (size_t i = 0; i < numPairs; ++i)
    chain.push_back(pairs[i].first);  // todos los ganadores (ya ordenados)

// El perdedor de pairs[0] es <= pairs[0].first (el ganador mínimo)
// → es el mínimo global → se prepende directamente sin búsqueda binaria
chain.insert(chain.begin(), pairs[0].second);
```

**Punto clave:** el perdedor del par cuyo ganador es el más pequeño está
garantizado ser el mínimo de toda la secuencia, porque:
`pairs[0].second ≤ pairs[0].first ≤ todos los demás ganadores ≤ todos los demás perdedores`

Por eso se inserta en posición 0 sin ninguna comparación.

#### Paso 5: Insertar los perdedores restantes en orden Jacobsthal

La optimización central de Ford-Johnson está aquí. Los perdedores `pairs[1].second`,
`pairs[2].second`, ... no se insertan en orden secuencial, sino siguiendo la secuencia
de Jacobsthal, para minimizar el número de comparaciones en las búsquedas binarias.

**Secuencia de Jacobsthal:** 0, 1, 1, 3, 5, 11, 21, 43, 85, 171, 341, ...

```cpp
static size_t jacobsthal(size_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    // J(n) = J(n-1) + 2*J(n-2)
    size_t prev2 = 0, prev1 = 1, cur = 0;
    for (size_t i = 2; i <= n; ++i)
    {
        cur = prev1 + 2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
```

El orden de inserción usa los **grupos** definidos por la diferencia entre términos
consecutivos de Jacobsthal. Para cada grupo [J(k-1), J(k)), los elementos se insertan
de mayor a menor índice dentro del grupo:

```
Grupo 1: índice 1          (J(2)=1 a J(1)=1)
Grupo 2: índice 2          (J(3)=3 ... bajando a J(2)=1)  → insertar 2
Grupo 3: índices 4,3       (J(4)=5 ... bajando a J(3)=3)  → insertar 4, luego 3
Grupo 4: índices 10..5     (J(5)=11 ... bajando a J(4)=5) → insertar 10,9,..,5
```

**Por qué este orden reduce comparaciones:**
Al insertar en orden Jacobsthal, cada búsqueda binaria del pendiente `pairs[i].second`
puede acotar su rango superior al índice de `pairs[i].first` en la cadena (el perdedor
es siempre ≤ su ganador). Esto reduce el rango de búsqueda y, gracias al orden
Jacobsthal, el peor caso de comparaciones por grupo es óptimo.

```cpp
// Fragmento de insertLosersVec
size_t jacobIdx = 2;
while (true)
{
    size_t jCurr = jacobsthal(jacobIdx);
    size_t jPrev = jacobsthal(jacobIdx - 1);

    if (jPrev >= pendingSize) break;

    size_t hiIdx = (jCurr <= pendingSize) ? jCurr - 1 : pendingSize - 1;

    for (size_t idx = hiIdx + 1; idx > jPrev; --idx)
    {
        size_t i = idx - 1; // índice 0-based en pending
        int val          = pairs[i + 1].second;
        int pairedWinner = pairs[i + 1].first;

        // Acotar la búsqueda binaria: val <= pairedWinner
        size_t upperBound = chain.size();
        for (size_t k = 0; k < chain.size(); ++k)
            if (chain[k] == pairedWinner) { upperBound = k + 1; break; }

        binaryInsertVec(chain, val, upperBound);
        inserted[i] = true;
    }
    ++jacobIdx;
}
```

#### Inserción binaria

```cpp
void PmergeMe::binaryInsertVec(std::vector<int> &seq, int val, size_t end)
{
    if (end > seq.size()) end = seq.size();
    size_t lo = 0, hi = end;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (seq[mid] < val) lo = mid + 1;
        else                hi = mid;
    }
    seq.insert(seq.begin() + static_cast<std::vector<int>::difference_type>(lo), val);
}
```

Búsqueda binaria estándar sobre el prefijo `[0, end)`. Se usa `lo + (hi-lo)/2` para
evitar overflow. El resultado `lo` es la posición de inserción que mantiene el orden.

### Por qué dos contenedores (`std::vector` y `std::deque`)

El enunciado exige medir el tiempo con ambos. La diferencia estructural es:

| | `std::vector` | `std::deque` |
|---|---|---|
| Almacenamiento | bloque contiguo | bloques fijos enlazados |
| Inserción al principio | O(n) — desplaza todo | O(1) amortizado |
| Acceso aleatorio | O(1) | O(1) |
| Localidad de caché | excelente | buena |

Para una secuencia pequeña (hasta ~100 elementos), `vector` suele ser más rápido
por mejor localidad de caché. Para secuencias grandes con muchas inserciones al
principio, `deque` puede ser más rápido.

### Medición del tiempo con `gettimeofday`

```cpp
struct timeval tstart, tend;
gettimeofday(&tstart, NULL);
sortVector();
gettimeofday(&tend, NULL);
double vecTime = (tend.tv_sec - tstart.tv_sec) * 1e6
               + (tend.tv_usec - tstart.tv_usec);
```

`gettimeofday` (header `<sys/time.h>`) devuelve segundos y microsegundos desde
el epoch Unix. Restando los dos valores y multiplicando los segundos por 10⁶, se
obtiene el tiempo transcurrido en microsegundos.

**Por qué no `clock()`:** `clock()` mide tiempo de CPU, que puede ser diferente
al tiempo real en sistemas multitarea. `gettimeofday` mide tiempo de pared (wall
clock), que es lo que el evaluador normalmente compara visualmente.

### Ejemplo de salida

```
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 14 us
Time to process a range of 5 elements with std::deque  : 21 us
```

---

## Resumen comparativo

| | Ex00 | Ex01 | Ex02 |
|---|---|---|---|
| **Contenedor principal** | `std::map<string,float>` | `std::stack<int>` | `std::vector<int>` + `std::deque<int>` |
| **Algoritmo clave** | `lower_bound` + decremento | Evaluación LIFO de tokens | Ford-Johnson (merge-insert) |
| **Por qué ese contenedor** | Claves ordenadas → búsqueda por proximidad de fecha | Acceso LIFO refleja semántica RPN | Comparar rendimiento de dos estructuras distintas |
| **Caso especial importante** | Fecha sin coincidencia exacta → usar día anterior | Orden de `pop` (b antes que a) | Rezagado (straggler) al final; primer perdedor sin comparación |
| **Optimización central** | `lower_bound` en O(log n) vs. búsqueda lineal | Ninguna (algoritmo lineal en tokens) | Orden Jacobsthal minimiza comparaciones en inserción binaria |
| **Manejo de errores** | `std::cout` con mensaje descriptivo | `throw std::runtime_error("Error")` | `std::cerr << "Error"` y retorno inmediato |
