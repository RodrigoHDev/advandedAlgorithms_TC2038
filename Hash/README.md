# Hash

This program reads a text file, arranges its contents in a matrix, sums the
values of each column modulo `256`, and converts those sums to an uppercase
hexadecimal key.

The input files are located in [`tests/`](tests/). The program expects the
filename without the `.txt` extension and a number of columns `n`.

## How the algorithm works

Let:

- `m` be the number of bytes read from the file after carriage returns (`\r`)
  are ignored.
- `n` be the number of matrix columns. The program accepts multiples of `4`
  from `16` through `64`.
- `r = ceil(m / n)` be the number of matrix rows.

The `Hash` class performs these steps:

1. `assembleMatrix()` fills an `r × n` matrix row by row with the file
   contents. If the last row is incomplete, the remaining cells contain the
   value `n`.
2. `addColumns()` adds the values in each column and applies modulo `256`.
3. `convertToHex()` writes each of the `n` sums as two hexadecimal digits,
   producing a key with `2n` characters.

## Complexity of the algorithm only

This section excludes `printMatrix()`, `printArray()`, and the other console
output. It describes the work needed to build the hash key.

| Operation | Time | Additional space | Reason |
|---|---:|---:|---|
| `assembleMatrix()` | `O(rn)` | `O(rn)` | Visits every matrix cell. |
| `addColumns()` | `O(rn)` | `O(n)` | Visits every cell and stores one sum per column. |
| `convertToHex()` | `O(n)` | `O(n)` | Converts one value per column and builds the key. |
| **Algorithm total** | **`O(rn)`** | **`O(rn)`** | The matrix dominates the other terms. |

Because `r = ceil(m / n)`, the matrix contains `rn` cells and `m ≤ rn < m+n`
for non-empty input. Therefore, the total can also be written as `O(m+n)`.
Since this program restricts `n` to the constant range `16`–`64`, its algorithm
is linear in the input size: `O(m)` time and `O(m)` space.

## Complexity of the complete program

This analysis includes everything performed by `main()` and `obtainKey()`:

- Reading the file and growing `fileContent`: `O(m)` time and `O(m)` space.
- Copying the text into the `Hash` object: `O(m)` time and `O(m)` space.
- Computing the key: `O(rn)` time and `O(rn)` space.
- Printing the original file content: `O(m)` time.
- Printing the matrix: `O(rn)` time, including separators and line breaks.
- Printing the column sums and the hexadecimal key: `O(n)` time.

The complete program therefore has:

| Resource | Complexity |
|---|---:|
| **Total time, including printing** | **`O(m + rn + n)`** |
| **Peak working space** | **`O(m + rn + n)`** |

The output itself contains `O(m + rn + n)` characters, so printing remains
linear in the input size. Printing does not require storing all output first;
the peak memory is dominated by `fileContent`, the copied text in `Hash`, the
matrix, and the sum/key structures. With `n` bounded to `16`–`64`, both totals
are also `O(m)`.

## Compilation

Run these commands from the `Hash` directory. With g++ compatible compiler. The program was tested with std=c++17

```powershell
g++ main.cpp -o hash.exe
```

The source includes `hash.h` and `printer.h`, so no additional source files
need to be listed in the compile command.

## Execution

The program uses paths relative to the current directory. 

```powershell
.\hash.exe
```

For example, enter:

```text
caso1
16
```

## Expected versus actual results

Both cases below use `n = 16`. The expected values were independently
recomputed from the file bytes after removing carriage returns. The actual
values are the final hexadecimal lines printed by the compiled program. The
column sums are included to make the comparison reproducible before the final
hexadecimal conversion.

| Case | File | `m` | `r` | Expected column sums | Expected key | Actual key | Result |
|---|---|---:|---:|---|---|---|---|
| caso1 | `tests/caso1.txt` | 83 | 6 | `44 130 228 128 199 208 167 211 233 236 38 149 38 224 44 197` | `2C82E480C7D0A7D3E9EC269526E02CC5` | `2C82E480C7D0A7D3E9EC269526E02CC5` | PASS |
| caso2 | `tests/caso_02.txt` | 164 | 11 | `160 204 70 96 241 133 120 162 143 98 246 132 28 159 71 2` | `A0CC4660F18578A28F62F6841C9F4702` | `A0CC4660F18578A28F62F6841C9F4702` | PASS |

The program also prints the original content, the matrix, and the column sums
before printing the final key. Those diagnostic lines are expected and are not
part of the key itself.
