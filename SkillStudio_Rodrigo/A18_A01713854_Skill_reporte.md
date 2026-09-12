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

| Case | File | N | C | Expected benefit / space | Actual benefit / space | Result | Image |
|---|---|---:|---:|---|---|---|---|
| caso1 (extreme: 0 elements) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test1.txt` | 0 | 100 | `Invalid entry format` | `Invalid entry format` | PASS | |
| caso2 (extreme: many elements, easy) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test2.txt` | 30 | 10 000 | `51750` / `5175 MB` | `51750` / `5175 MB` | PASS | |
| caso3 (intermediate classic) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test3.txt` | 10 | 50 | `257` / `50 MB` | `257` / `50 MB` | PASS | |
| caso4 (intermediate - same ratio) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test4.txt` | 12 | 40 | `105` / `40 MB` | `105` / `40 MB` | PASS | |
| caso5 (intermediate) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test5.txt` | 15 | 30 | `145` / `30 MB` | `145` / `30 MB` | PASS | |
| caso6 (intermediate) | `A18_A01713854_Skill_tests/A18_A01713854_Skill_test6.txt` | 20 | 200 | `254000` / `200 MB` | `254000` / `200 MB` | PASS | |

The program also prints the selected index identifiers (1-indexed, restored
to input order) after the benefit and space lines; those are omitted from
the table above for brevity but are included in the program's actual
output.

<br>

## Known limitations / not implemented

- **File input by path**: the program only reads from standard input; use
  shell redirection (`< file.txt`) to feed it a text file, as shown above.
- **Reto adicional (one index per table)**: the extension described in the
  problem statement — restricting selection to at most one index per
  database table — is not implemented in the current version. Each index
  is treated independently, with no table grouping constraint.