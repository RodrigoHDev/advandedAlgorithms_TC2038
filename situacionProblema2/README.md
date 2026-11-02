# Situation Problem 2 — Analysis and Design of Advanced Algorithms

**Team 01**
- Rodrigo Alejandro Hurtado Cortes — A01713854
- Alexis Yaocalli Berthou Haas — A01713458
- Ricardo Rodriguez Medina — A01710266

---

## What needs to be done?

The program models a network of colonies and solves four related planning problems from input data:

1. **Minimum cable network:** find a minimum spanning tree (MST) over the colonies using the distance matrix, and report the selected connections and total cable length.
2. **Shortest round trip:** find a minimum-cost tour that starts at Colony 1, visits every colony exactly once, and returns to Colony 1.
3. **Maximum information flow:** calculate the maximum flow from the first colony to the last using the capacity matrix.
4. **Closest service center:** find the existing center with the smallest Euclidean distance to a new customer's coordinates.

The program reads one text file. By default it uses `input/input.txt`; an optional command-line argument can select a different input file. The file contains, in order: the number of colonies `N`, an `N × N` distance matrix, an `N × N` capacity matrix, the number of existing centers `K`, the `K` center coordinate pairs, and the new customer's coordinate pair. Matrix entries and coordinates are whitespace-separated, so blank lines are optional.

## Basic example

For the included `input/input.txt`, there are four colonies, three centers, and a new customer at `(5, 5)`. The program reports an MST requiring 14 km of cable, an optimal tour of 22 km, a maximum flow of 15, and Center 2 as closest at a distance of 1.00. The tour may be printed in either direction; both directions describe the same cycle and have the same cost.

Run the program from this directory:

```bash
g++ -std=c++17 main.cpp -o situation2
./situation2                         # reads input/input.txt
./situation2 input/flow_78.txt       # optional alternate input
```

## What has been developed

### General structure

`main.cpp` reads and validates the input file, prints the parsed data, invokes each algorithm, and formats the results. Algorithm implementations are kept in `algorithms/`:

- `kruskal.h` builds the minimum spanning tree.
- `tsp.h` contains both an edge-greedy tour heuristic and the exact Held–Karp dynamic programming solver. **The current `main.cpp` invokes `held_karp()`, not the greedy heuristic.**
- `edmonds_karp.h` computes the source-to-sink maximum flow.
- `nearest.h` finds the closest center by Euclidean distance.

### Notation

| Symbol | Meaning |
|---|---|
| `N` | Number of colonies / graph vertices. |
| `E` | Number of graph edges. In a complete undirected graph, `E = N(N−1)/2`. |
| `K` | Number of existing centers. |
| `C` | Capacity matrix; `C[u][v]` is the capacity from colony `u` to `v`. |
| `2^N` | Number of subsets of colonies represented in Held–Karp's DP table. |

### Part 1 — Minimum spanning tree: Kruskal's algorithm

The distance matrix is converted into an edge list by considering each pair once (the upper triangle). Edges with positive distance are sorted by weight. Kruskal's algorithm processes them from lightest to heaviest and uses a disjoint-set union (DSU) structure with path compression and union by rank to accept an edge only when it joins two previously disconnected components. It stops after selecting `N − 1` edges. If that many edges cannot connect the graph, the program reports that the graph is disconnected.

- **Time:** `O(E log E)` for sorting; DSU operations add near-linear amortized work (`O(E α(N))`).
- **Space:** `O(E + N)` for the edge list and DSU.

This implementation treats non-positive distances as absent edges. The distance graph is assumed to be undirected, as only one triangle of the matrix is read.

### Part 2 — Shortest round trip: Held–Karp dynamic programming

The exact solver represents a partial route by a pair `(mask, i)`: `mask` records the visited colonies and `i` is the last colony in the route. For each such state, the DP table stores the least cost of a route starting at Colony 1, visiting exactly the colonies in `mask`, and ending at `i`. Transitions append an unvisited colony; after all colonies have been visited, the return edge to Colony 1 is included. A predecessor table reconstructs the route.

- **Time:** `O(N² · 2^N)`.
- **Space:** `O(N · 2^N)` for the cost and predecessor tables.

Held–Karp guarantees an optimal tour but is exponential in `N`, so memory and runtime limit its practical input size. The same header also provides `tsp_process()`, a greedy edge-based heuristic with `O(N² log N)` time and `O(N²)` space. That method is faster for larger inputs but is not guaranteed to find the optimal tour; it is currently not selected by `main.cpp`. The TSP implementation expects a complete, symmetric distance matrix.

### Part 3 — Maximum information flow: Edmonds–Karp

Edmonds–Karp repeatedly searches the residual network with breadth-first search (BFS) for a path from colony 1 (index 0) to colony `N` (index `N−1`). It augments the flow by the smallest remaining capacity on that path, updates forward and reverse residual capacities, and stops when no augmenting path remains. Reverse residual edges make it possible to revise earlier flow choices. The function rejects a non-square matrix, negative capacities, and invalid endpoints with `-1`.

- **Standard bound:** `O(VE²)` time with adjacency-list traversal, where `V` is the number of vertices.
- **This implementation:** stores and scans the residual graph as a matrix. Each BFS costs `O(N²)` and there are `O(NE)` augmentations, so its matrix-based worst-case bound is `O(N³E)` (up to `O(N⁵)` for a dense graph).
- **Space:** `O(N²)` for the residual matrix, plus `O(N)` for BFS state.

### Part 4 — Closest center: linear scan

For each center, the program computes the Euclidean distance to the new customer's coordinates using `hypot`, retaining the smallest distance and its index.

- **Time:** `O(K)`.
- **Extra space:** `O(1)`.

## Testing strategy

The included tests focus on the exact TSP solver and Edmonds–Karp. The TSP cases verify that the returned route visits every city once and that its cycle cost matches the known optimum, including cases where the greedy heuristic would not be optimal. The flow tests cover the supplied example, an unreachable sink, a single vertex, large capacities, reverse residual edges, and invalid inputs. The default input was also run through the complete program.

| Test suite / input | Coverage | Result |
|---|---|---|
| `tests/test_tsp.cpp` | Three instances (4, 6, and 5 cities); route validity and optimal cycle cost | All 3 passed |
| `tests/test_edmonds_karp.cpp` | Nine normal, edge, and invalid-input scenarios | All 9 passed |
| `input/input.txt` | End-to-end execution of MST, TSP, maximum flow, and closest-center sections | Completed; outputs: 14 km, 22 km, 15, and Center 2 at 1.00 |

Compile and run the test programs from `situacionProblema2/`:

```bash
g++ -std=c++17 tests/test_tsp.cpp -Ialgorithms -o test_tsp && ./test_tsp
g++ -std=c++17 tests/test_edmonds_karp.cpp -Ialgorithms -o test_edmonds_karp && ./test_edmonds_karp
```

The sample files `input/flow_78.txt` and `input/flow_disconnected.txt` provide alternative flow and connectivity data for manual end-to-end checks.

## Alternative approaches

- **MST:** Prim's algorithm is another option, particularly convenient with an adjacency matrix (`O(N²)` for a dense graph). Kruskal's edge sorting is a natural fit when the edge list is sparse.
- **TSP:** the included edge-greedy heuristic is a faster approximate alternative. For larger exact instances, branch-and-bound can sometimes help in practice but still has exponential worst-case behavior. Held–Karp is appropriate when correctness and manageable input size matter.
- **Maximum flow:** Dinic's algorithm can improve performance on larger networks, especially with adjacency lists. Edmonds–Karp is comparatively straightforward and its BFS paths give a clear polynomial bound.
- **Closest center:** a spatial index such as a k-d tree can improve repeated nearest-neighbor queries for a large, mostly static set of centers; a linear scan is simpler for a single query or small `K`.

## General conclusion

This solution combines graph algorithms and geometric distance calculations to address four distinct planning needs. Kruskal's algorithm gives a minimum-cost network, Held-Karp guarantees the least-cost round trip, Edmonds-Karp finds the maximum feasible information flow, and a linear scan selects the nearest center. Their tradeoffs illustrate why algorithm choice depends on the required guarantee and input scale: the exact TSP solution is optimal but exponential, while the other selected methods offer practical polynomial-time solutions for their respective tasks.
