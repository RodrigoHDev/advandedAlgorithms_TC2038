# Maze Traversal Algorithms

Authors:
- Alexis Yaocalli Berthou Haas - A01713458
- Rodrigo Alejandro Hurtado Cortés - A01713854

<br>

## Introduction

Given a maze represented as a grid of `0`s and `1`s, where `0` marks a wall
and `1` marks a free tile, the goal is to find the shortest path — measured
in number of steps — from the origin `[0][0]` to the exit `[rows-1][columns-1]`.
 Movement is restricted to the four orthogonal directions (up, down, left, 
 right) and a tile cannot be revisited within the same path.

This project explores two ways of solving this problem, backtracking implemented in
`!!![INSERTA TU ARCHIVO AQUI ALEXIS]!!!` and branch & bound implemented in`branchBound.h`:

## Algorithms

### Backtracking

*(To be implemented.)*

### Branch and Bound

`findWay` explores the maze recursively, treating every tile as a node with
up to four possible children (its orthogonal neighbors). The algorithm keeps
two pieces of state as it explores:

- `way`: the path currently being explored, represented as a stack of
  coordinates. A tile is pushed onto `way` when it is entered and popped
  once every direction from that tile has been explored, so `way` always
  reflects only the branch currently under consideration.
- `bestSteps` / `bestWay`: the cost and path of the best complete solution
  found so far.

At every recursive call, before exploring further, the current accumulated
cost is compared against `bestSteps`. If the current path can no longer beat
the best solution found so far, the branch is discarded immediately — this
is the "bound" step that distinguishes the algorithm. Whenever a path reaches
the exit with a lower cost than `bestSteps`, `bestSteps` and `bestWay` are
updated, tightening the bound for every branch explored afterward. A `visited`
matrix is kept alongside `way`so that membership checks (has this tile 
already been used in the current path?) run in constant time instead of 
requiring a scan of the stack.

**Complexity**

Let `M` = number of rows and `N` = number of columns.

- **Time: `O(3^{M×N})`** in the worst case. From any tile, one of the four
  directions always leads back to the tile just visited, which `visited`
  blocks, leaving an effective branching factor of 3. The deepest a path can
  go without repeating a tile is `M×N` (the whole maze). This bound is only
  reached in the worst case, before any full path has been found and the
  cost comparison has something to prune against — once a solution exists,
  later branches are cut earlier, and `visited` also prevents revisiting
  any tile already used earlier in the current path, not just the
  immediately previous one, so real execution is typically much smaller than
  this bound.
- **Space: `O(M×N)`.** Passing the maze by reference avoids copying it at
  every recursive call. What remains are the recursion stack (depth up to
  `M×N`, constant space per frame), the `visited` matrix (`O(M×N)`), and the
  `way` / `bestWay` stacks (each up to `O(M×N)` coordinates) — all of the
  same order, so the total stays `O(M×N)`.



## How to compile and run from CMD

Open Command Prompt and move to the project folder. Then run the following
commands:

Compile the program:
```cmd
g++ main.cpp -o maze.exe
```

Run one test case:

```cmd
maze.exe < input\input1.txt
```

Run all test cases:

```cmd
for %f in (input\*.txt) do maze.exe < %f
```

## How to compile and run from Bash

Open a terminal and move to the project folder. Then run the following
commands:

Compile the program:
```bash
g++ main.cpp -o maze
```

Run one test case:

```bash
./maze < input/input1.txt
```

Run all test cases:

```bash
for f in input/*.txt; do ./maze < "$f"; done
```

## Input format

Each input file contains, in order:

1. The number of rows.
2. The number of columns.
3. One line per row, with the row's values (`0` or `1`) separated by
   spaces.

`main.cpp` reads these values, builds the maze, and passes it to
`branchBound::travelMaze`, which returns the traversed path as a grid of
the same size where `1` marks a tile used in the best path found and `0`
marks tiles that are not part of it. If no path exists, the result is
`{{-1}}` and the program reports that no valid solution was found.

## Test case

Input (4 rows, 5 columns):

```text
4
5
1 0 1 1 1
1 1 1 0 1
1 0 1 1 1
1 1 1 0 1
```

This test case was built to require a detour around two separate walls
(the `0` in the first column below the origin and the `0`s in the fourth
column) while still leaving exactly one route from `[0][0]` to `[3][4]`,
so the resulting path is easy to verify by inspection against the printed
"Original" maze.

Result obtained with Branch and Bound:

```text
1 0 0 0 0
1 1 1 0 0
0 0 1 1 1
0 0 0 0 1
```

| Test case | Rows | Columns | Branch and Bound | Backtracking |
| --- | ---: | ---: | ---: | --- |
| `input1.txt` | 4 | 5 | `[Insert image here]` | `[Insert image here]` |
 