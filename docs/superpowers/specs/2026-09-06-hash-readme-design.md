# Hash README design

## Objective

Create `Hash/README.md` to explain the matrix-based hash implementation and document reproducible execution results for the two supplied cases using `n = 16`.

## Scope

- Document the transformation from file text to matrix, column sums modulo 256, and hexadecimal key.
- Separate the asymptotic complexity of the hash algorithm from the complexity of the complete program, including file I/O, string copying, and console output.
- Define `m` as the number of characters read after carriage returns are removed, `n` as the number of columns, and `r = ceil(m / n)` as the number of matrix rows.
- Explain time and auxiliary-space complexity for both views, while noting that the implementation restricts `n` to 16 through 64.
- Provide PowerShell and general command-line compile/run instructions.
- Record expected and observed hash results for `tests/caso1.txt` and `tests/caso_02.txt`, both with `n = 16`.

## Proposed README structure

1. Title and purpose.
2. Algorithm overview.
3. Complexity of the algorithm only (without printing).
4. Complexity of the complete program (including reading and printing).
5. Requirements and compilation.
6. Execution instructions.
7. Test cases and expected-versus-actual results.
8. Input/file naming note for `caso_02.txt`.

## Verification criteria

- The README is located at `Hash/README.md`.
- Complexity claims match `main.cpp`, `hash.h`, and `printer.h`.
- The documented commands compile the current C++ sources.
- The documented hashes match executions with `caso1`, `16` and `caso_02`, `16`.
- The parent README modification present before this task is not changed.
