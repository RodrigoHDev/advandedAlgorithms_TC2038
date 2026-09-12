# DataNova Index Selector (Branch & Bound)

**Author: Rodrigo Alejandro Hurtado Cortés - A01713854**


This program solves DataNova's index-selection problem: given a set of
candidate indexes, each with a disk-space cost and an estimated performance
benefit, it selects the subset that maximizes total benefit without
exceeding the available storage capacity. The problem is a 0/1 knapsack
variant, solved with the Branch & Bound technique.


The test cases are located in [`tests/`](tests/). The program reads its
input from standard input in the format described below; it does not open
files by name (see [Execution](#execution) for how to feed it a `.txt`
file anyway).


<br>

## Input format

```
N C
E1 B1
E2 B2
...
EN BN
```

- `N`: number of candidate indexes.
- `C`: maximum available storage capacity, in MB.
- `Ei`, `Bi`: space (MB) and estimated benefit of index `i`.

### Restrictions

| Value | Range |
|---|---:|
| `N` | `1` – `30` |
| `C` | `1` – `10 000` |
| `Ei` | `1` – `C` |
| `Bi` | `1` – `100 000` |

All values must be positive integers. If the header, an item, or the input
format itself (non-numeric tokens, missing values) violates any of these
restrictions, the program prints `Invalid entry format` and stops — no
partial output is produced.

<br>

## How the algorithm works

Let `n = N` be the number of candidate indexes.

1. `sortByRatio()` orders the indexes in descending order of `Bi / Ei`
   (benefit per unit of space), using cross-multiplication instead of
   floating-point division to avoid precision loss.
2. `BranchBound::explore()` builds a binary decision tree: at each index it
   branches into "include" and "exclude". A branch that would exceed `C` is
   never taken.
3. `BranchBound::maxValue()` computes, at each node, an optimistic upper
   bound on the best value reachable from there — the classic fractional
   knapsack bound: take remaining indexes whole while they fit, then add
   only the fraction of the first one that doesn't. Because the vector is
   sorted by ratio, this fraction is guaranteed to be an upper bound on any
   real (0/1) combination, so `explore()` can safely prune any branch whose
   bound does not exceed the best value found so far.
4. Once `explore()` finishes, `sortByIndex()` restores the selected indexes
   to their original input order (1-indexed) for printing.

<br>

## Complexity of the algorithm only

This section excludes input validation and console output (matrix/status
printing does not apply here, but reading and formatting the result do).
It describes the work needed to find the optimal subset.

| Operation | Time | Additional space | Reason |
|---|---:|---:|---|
| `sortByRatio()` | `O(n log n)` | `O(log n)` | In-place sort with a constant-time comparator. |
| `maxValue()` | `O(n)` per call | `O(1)` | Single pass over the remaining, not-yet-decided indexes. |
| `explore()` | `O(n · 2^n)` worst case | `O(n)` | Up to `2^n` decision-tree nodes, each doing an `O(n)` `maxValue()` scan; recursion depth and `path`/`bestPath` are `O(n)`. |
| `sortByIndex()` | `O(k log k)`, `k ≤ n` | `O(log k)` | Sorts only the selected indexes (the result path) back into input order. |
| **Algorithm total** | **`O(n · 2^n)` worst case** | **`O(n)`** | `explore()` dominates; the knapsack problem is NP-hard, so no Branch & Bound scheme guarantees better than exponential worst-case time. |

Because `1 ≤ n ≤ 30`, the theoretical worst case (`2^30` nodes) is a loose
upper bound: sorting by `Bi/Ei` before the search makes the fractional
bound tight early, so in practice `explore()` visits far fewer nodes than
the full tree, especially when items differ noticeably in their
value/space ratio (see the [test cases](#expected-versus-actual-results)
below).

<br>

## Complexity of the complete program

This analysis includes everything performed by `main()`, header/item
validation, and `BranchBound`'s own defensive re-validation:

- Reading and validating the header and each item: `O(n)` time, `O(1)`
  additional space (validation is done per token as it's read).
- Copying the items into `BranchBound` and re-validating them:
  `O(n)` time and `O(n)` space.
- `sortByRatio()`: `O(n log n)` time, `O(log n)` space.
- `explore()`: `O(n · 2^n)` worst-case time, `O(n)` space.
- `sortByIndex()` and printing the result: `O(n log n)` time, `O(n)` space.

The complete program therefore has:

| Resource | Complexity |
|---|---:|
| **Total time, worst case** | **`O(n · 2^n)`** |
| **Peak working space** | **`O(n)`** |

`explore()` dominates both totals; every other step is polynomial in `n`
and negligible by comparison. Since `n ≤ 30`, peak memory stays small
regardless of input (a handful of `vector<tuple<int,int,int>>` of size at
most 30).

<br>

## Compilation

Run this command from the project directory. With a g++-compatible
compiler; the program was tested with `-std=c++17`.

```powershell
g++ -std=c++17 main.cpp -o branchbound.exe
```

The only code file includes `BranchBound.h`, so no additional source files need
to be listed in the compile command.

<br>

## Execution

The program reads `N C` and the `N` following lines from standard input;
it does not accept a filename as an argument. To type the input by hand:

```powershell
.\branchbound.exe
```

```text
5 20
6 30
8 42
5 25
10 50
4 18
```

To run one of the provided test files without retyping it, redirect it
into standard input from the shell — the program never notices the
difference between a keyboard and a file:

```powershell
.\branchbound.exe < A18_A01713854_Skill_tests/A18_A01713854_Skill_test1.txt
```

<br>

## Expected versus actual results

The expected values were independently recomputed with a brute-force
(and, for larger cases, dynamic-programming) 0/1 knapsack solver over the
same input files. The actual values are what the compiled program prints.
`caso1` is an intentional restriction violation (`N = 0`), included to
confirm the program rejects invalid input instead of producing a partial
or incorrect result.

| Case | File | Entrada proporcionada | Resultado esperado | Resultado obtenido | Comentarios sobre el desempeño | Result | Image |
|---|---|---|---|---|---|---|---|
| caso1 (extreme: 0 elements) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test1.txt` | `0 100` | `Invalid entry format`<br>(N = 0 viola 1 ≤ N ≤ 30) | `Invalid entry format` | Restricción violada detectada en `isValidHeader()` antes de construir el objeto `BranchBound`; `explore()` nunca se ejecuta (0 nodos), por lo que el rechazo es inmediato (O(1)). | PASS | <img width="400" height="auto" alt="Screenshot 2026-09-11 234823" src="https://github.com/user-attachments/assets/b83a481e-5fb3-48a0-b7e3-94a606dd9f9a" /> |
| caso2 (extreme: many elements, easy) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test2.txt` | `30 10000`<br>`100 1000`<br>`105 1050`<br>`110 1100`<br>`115 1150`<br>`120 1200`<br>`125 1250`<br>`130 1300`<br>`135 1350`<br>`140 1400`<br>`145 1450`<br>`150 1500`<br>`155 1550`<br>`160 1600`<br>`165 1650`<br>`170 1700`<br>`175 1750`<br>`180 1800`<br>`185 1850`<br>`190 1900`<br>`195 1950`<br>`200 2000`<br>`205 2050`<br>`210 2100`<br>`215 2150`<br>`220 2200`<br>`225 2250`<br>`230 2300`<br>`235 2350`<br>`240 2400`<br>`245 2450` | Beneficio máximo: `51750`<br>Espacio utilizado: `5175 MB`<br>Índices esperados: 1–30 (la suma de todos los espacios, 5175 MB, cabe en la capacidad de 10000 MB, así que incluirlos todos es la única solución óptima) | Beneficio maximo: `51750`<br>Espacio utilizado: `5175 MB`<br>Indices seleccionados: `1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30` | Con 30 índices el árbol de decisión completo tendría hasta 2^30 nodos; al ser la capacidad muy holgada, la cota fraccionaria de `maxValue()` permite resolver el caso visitando solo 61 nodos, confirmando que la poda evita exploración innecesaria incluso en el caso "todos caben". | PASS | <img width="400" height="auto" alt="Screenshot 2026-09-11 234841" src="https://github.com/user-attachments/assets/5e14fcca-b7b9-429d-816a-af6e7ec2b994" /> |
| caso3 (intermediate classic) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test3.txt` | `10 50`<br>`10 60`<br>`20 100`<br>`30 120`<br>`15 70`<br>`5 25`<br>`25 90`<br>`8 40`<br>`12 55`<br>`18 80`<br>`7 32` | Beneficio máximo: `257`<br>Espacio utilizado: `50 MB` (recomputado con solución de fuerza bruta sobre las mismas 10 tuplas) | Beneficio maximo: `257`<br>Espacio utilizado: `50 MB`<br>Indices seleccionados: `1 2 5 7 10` | El espacio usado coincide exactamente con la capacidad (50 MB), forzando al algoritmo a descartar varias combinaciones cercanas al óptimo. Se visitaron solo 16 de los 2^10 = 1024 nodos posibles, mostrando una poda efectiva incluso en un caso "ajustado". | PASS | <img width="400" height="auto" alt="Screenshot 2026-09-11 234851" src="https://github.com/user-attachments/assets/104b424e-f146-4634-9abf-5be6912c149d" /> |
| caso4 (intermediate - same ratio) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test4.txt` | `12 40`<br>`5 10`<br>`10 20`<br>`15 30`<br>`8 16`<br>`6 18`<br>`4 16`<br>`12 24`<br>`3 6`<br>`7 21`<br>`9 9`<br>`2 8`<br>`11 11` | Beneficio máximo: `105`<br>Espacio utilizado: `40 MB` (recomputado con fuerza bruta; varios índices comparten razón beneficio/espacio, por lo que existen múltiples subconjuntos óptimos) | Beneficio maximo: `105`<br>Espacio utilizado: `40 MB`<br>Indices seleccionados: `2 4 5 6 8 9 11` | Al haber ítems con la misma relación valor/espacio, la cota fraccionaria queda "empatada" en varias ramas; el algoritmo aun así converge a un óptimo válido visitando 46 de 4096 nodos posibles, sin verse afectado por el empate de razones. | PASS | <img width="400" height="auto" alt="Screenshot 2026-09-11 234900" src="https://github.com/user-attachments/assets/e3f2ec14-0d12-4129-90a0-bd1d5278306d" /> |
| caso5 (intermediate) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test5.txt` | `15 30`<br>`25 100`<br>`20 90`<br>`15 70`<br>`10 55`<br>`30 100`<br>`5 20`<br>`8 35`<br>`12 48`<br>`18 75`<br>`22 85`<br>`6 22`<br>`14 60`<br>`9 38`<br>`17 65`<br>`3 10` | Beneficio máximo: `145`<br>Espacio utilizado: `30 MB` (recomputado con fuerza bruta sobre las mismas 15 tuplas) | Beneficio maximo: `145`<br>Espacio utilizado: `30 MB`<br>Indices seleccionados: `3 4 6` | Con capacidad muy ajustada (30 MB) frente a 15 candidatos, muchos de ellos con alto beneficio individual pero espacio excesivo, el algoritmo descarta rápidamente esas ramas: solo 19 de 32768 (2^15) nodos posibles fueron visitados. | PASS | <img width="400" height="auto" alt="Screenshot 2026-09-11 234907" src="https://github.com/user-attachments/assets/51a8c2c0-daa7-400a-8a3e-13d781e1b4dc" /> |
| caso6 (intermediate) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test6.txt` | `20 200`<br>`50 90000`<br>`80 50000`<br>`30 40000`<br>`20 15000`<br>`60 70000`<br>`10 8000`<br>`100 95000`<br>`5 3000`<br>`45 42000`<br>`70 60000`<br>`15 12000`<br>`90 80000`<br>`25 20000`<br>`35 33000`<br>`55 48000`<br>`8 6000`<br>`120 100000`<br>`40 37000`<br>`65 59000`<br>`12 9000` | Beneficio máximo: `254000`<br>Espacio utilizado: `200 MB` (recomputado con programación dinámica sobre las mismas 20 tuplas) | Beneficio maximo: `254000`<br>Espacio utilizado: `200 MB`<br>Indices seleccionados: `1 3 5 9 11` | Con beneficios grandes (hasta 100000) y 20 índices, el desempeño se mantuvo eficiente: solo 58 de 2^20 (≈1 048 576) nodos posibles fueron visitados, gracias a que ordenar por razón beneficio/espacio deja la cota fraccionaria muy ajustada desde las primeras ramas. | PASS | <img width="400" height="auto" alt="Screenshot 2026-09-11 234914" src="https://github.com/user-attachments/assets/4baf71a6-d4e6-4e26-8248-c3ae6f667d58" /> |


Los conteos de nodos citados en la columna de comentarios se obtuvieron
instrumentando temporalmente `explore()` con un contador global incrementado
en cada llamada; se reportan aquí como evidencia del comportamiento de la
poda, no forman parte de la salida estándar del programa entregado.

<br>

## Known limitations / not implemented

- **File input by path**: the program only reads from standard input; use
  shell redirection (`< file.txt`) to feed it a text file, as shown above.
- **Reto adicional (one index per table)**: the extension described in the
  problem statement — restricting selection to at most one index per
  database table — is not implemented in the current version. Each index
  is treated independently, with no table grouping constraint.
