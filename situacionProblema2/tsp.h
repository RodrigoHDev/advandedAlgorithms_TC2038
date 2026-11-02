/*
 * Title: tsp.h
 *
 * Description:
 * Creation of tsp object to solve the Traveling Salesman Problem (TSP) with
 * two methods:
 *   - tsp_process(): approximate solution with the greedy edge heuristic.
 *     Fast, works for large inputs, but does not guarantee the optimum.
 * 
 *   - held_karp(): exact solution with dynamic programming over subsets.
 *     Always finds the optimum, but is exponential (viable up to n ~ 20).
 *
 * Greedy edge heuristic: given a complete,
 * symmetric graph represented as an adjacency matrix, it builds a cycle that
 * visits every city exactly once and returns to the starting city.
 *
 * The greedy edge heuristic sorts every edge by weight and accepts them from
 * lightest to heaviest, as long as the edge does not:
 *   1. give any node a degree greater than 2 (each city needs exactly one
 *      incoming and one outgoing connection), or
 *   2. close a cycle before every city is included (checked with union-find).
 * After n - 1 edges are accepted they form a Hamiltonian path, which is closed
 * by joining its two endpoints (the only nodes with degree 1).
 *
 * This is a heuristic: it is fast but does not guarantee the optimal tour.
 * The class only processes the matrix it receives; reading input and
 * displaying results are handled by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Septiembre 3, 2026
 */

#ifndef TSP_H
#define TSP_H

#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <climits>

using namespace std;

/*
In order to analyze complexity:
n = number of cities (nodes).
E = number of edges in the complete graph = n * (n - 1) / 2, so E = O(n^2).
*/

class tsp {
     private:

     /**
      * find()
      * Returns the root (representative) of the fragment that contains x.
      * Each fragment is stored as a tree inside parent[], where a node whose
      * parent is itself is the root. While going up the tree, every visited
      * node is pointed directly to the root (path compression), so later
      * queries on the same fragment are faster.
      *
      * Complexity:
      *  Time: O(log n) amortized (path compression only)
      *  Space: O(log n) amortized for the recursion stack
      *
      * Params:
      * parent is the union-find array, passed by reference so path
      * compression persists between calls.
      * x is the node whose fragment root is searched.
      * Returns:
      * the index of the root of x's fragment.
      */
     int find(vector<int>& parent, int x) {
          if (parent[x] == x) return x;
          return parent[x] = find(parent, parent[x]);
     }

     /**
      * unite()
      * Merges the fragments that contain a and b. If both nodes already share
      * the same root, they are connected by a path, so adding the edge (a, b)
      * would close a cycle; in that case nothing is merged.
      *
      * Complexity:
      *  Time: O(log n) amortized (two calls to find)
      *  Space: O(log n) amortized for the recursion stack
      *
      * Params:
      * parent is the union-find array, passed by reference.
      * a and b are the endpoints of the edge being evaluated.
      * Returns:
      * true if the fragments were merged (the edge is valid), false if a and
      * b were already in the same fragment (the edge would close a cycle).
      */
     bool unite(vector<int>& parent, int a, int b) {
          a = find(parent, a);
          b = find(parent, b);
          if (a == b) return false;
          parent[a] = b;
          return true;
     }

     public:

     /**
      * tsp_process()
      * Builds a tour with the greedy edge heuristic in four stages:
      *  1. Generates the list of edges (weight, u, v) from the upper triangle
      *     of the matrix (no diagonal, no duplicates) and sorts it by weight.
      *  2. Traverses the sorted list once, accepting an edge only if both
      *     endpoints have degree < 2 and they belong to different fragments.
      *     A rejected edge can never become valid later (degrees only grow
      *     and fragments only merge), so a single pass is enough. It stops
      *     after accepting n - 1 edges.
      *  3. Closes the cycle by joining the two nodes with degree 1.
      *  4. Walks the cycle from node 0 through the adjacency list to obtain
      *     the visiting order.
      *
      * Complexity:
      *  Time: O(n^2 log n)
      *   - Building the edge list: O(E) = O(n^2)
      *   - Sorting the edges: O(E log E) = O(n^2 log n)  <- dominant term
      *   - Greedy pass: O(E log n) = O(n^2 log n) in the worst case
      *   - Finding the endpoints and walking the cycle: O(n)
      *  Space: O(n^2)
      *   - Edge list: O(E) = O(n^2)
      *   - deg, parent, adjacency list and finalPath: O(n)
      *
      * Params:
      * n is the number of cities.
      * matrix is the n x n symmetric adjacency matrix, where matrix[i][j] is
      * the distance between city i and city j. Passed by constant reference
      * to avoid copying it.
      * Returns:
      * a vector with the n cities in the order they are visited, starting at
      * city 0. The return to the starting city is implicit (the cost of the
      * tour includes matrix[path[n - 1]][path[0]]).
      */
     vector<int> tsp_process(int n, const vector<vector<int>>& matrix) {
          // With a single city there are no edges: the tour is just that city
          if (n == 1) return {0};

          vector<int> finalPath;

          // deg[i] = number of accepted edges that touch city i (max 2)
          vector<int> deg(n, 0);

          // Union-find: at the start every city is its own fragment
          vector<int> parent(n);
          iota(parent.begin(), parent.end(), 0);

          // Stage 1: edge list (weight, u, v). The weight goes first so that
          // sort() orders the tuples by weight without a custom comparator.
          vector<tuple<int,int,int>> aristas;
          for (int i = 0; i < n; i++)
               for (int y = i + 1; y < n; y++)
                    aristas.emplace_back(matrix[i][y], i, y);

          sort(aristas.begin(), aristas.end());

          // Stage 2: greedy pass over the sorted edges
          vector<vector<int>> adj(n);   // neighbors of each city in the tour
          int accepted = 0;
          long long total = 0;          // tour cost; long long avoids overflow
          for (auto [w, u, v] : aristas) {
               // Degree rule: a city cannot have more than 2 connections
               if (deg[u] == 2 || deg[v] == 2) continue;
               // Cycle rule: also merges the fragments when the edge is valid
               if (!unite(parent, u, v)) continue;

               deg[u]++;
               deg[v]++;
               adj[u].push_back(v);
               adj[v].push_back(u);
               total += w;

               // n - 1 edges without cycles form a Hamiltonian path
               if (++accepted == n - 1) break;
          }

          // Stage 3: close the cycle. After n - 1 edges, exactly two cities
          // have degree 1 (the endpoints of the path); all others have 2.
          int remaining[2] = {-1, -1};
          for (int i = 0; i < n; i++) {
               if (deg[i] == 1) {
                    if (remaining[0] == -1) remaining[0] = i;
                    else                    remaining[1] = i;
               }
          }
          total += matrix[remaining[0]][remaining[1]];
          adj[remaining[0]].push_back(remaining[1]);
          adj[remaining[1]].push_back(remaining[0]);

          // Stage 4: walk the cycle. Every city has exactly 2 neighbors, so
          // the next city is always the neighbor we did not come from.
          int prev = -1, cur = 0;
          for (int k = 0; k < n; k++) {
               finalPath.push_back(cur);
               int next = (adj[cur][0] != prev) ? adj[cur][0] : adj[cur][1];
               prev = cur;
               cur = next;
          }

          return finalPath;
     }




     /**
      * held_karp()
      * Finds the OPTIMAL tour using dynamic programming over subsets
      *
      * Key idea:
      * For the rest of the tour, the only things that matter about a partial
      * path are (1) which cities were already visited and (2) the city we are
      * standing on. The order used to visit them does not matter, so among all
      * partial paths with the same set and the same last city only the
      * cheapest one needs to be kept.
      *
      * State:
      * dp[mask][i] = minimum cost of starting at city 0, visiting exactly theTod
      * cities in mask, and ending at city i.
      * mask is an integer used as a set: bit j is 1 if city j was visited.
      *   Example with n = 4: mask = 1101 = 13 -> visited {0, 2, 3}.
      *
      * Recurrence:
      *   Base:       dp[{0}][0] = 0   (mask = 1, standing at the start)
      *   Transition: dp[mask | {j}][j] = min(dp[mask | {j}][j],
      *                                       dp[mask][i] + matrix[i][j])
      *               for every reachable state (mask, i) and every city j
      *               not in mask.
      *   Answer:     min over i of dp[all][i] + matrix[i][0]
      *               (all cities visited, then return to city 0).
      *
      * Complexity:
      *  Time: O(n^2 * 2^n)
      *   - Table initialization: O(n * 2^n)
      *   - Filling the table: 2^n masks * n last cities * n next cities
      *     = O(n^2 * 2^n)  <- dominant term
      *   - Choosing the best last city: O(n)
      *   - Rebuilding the path: O(n)
      * 
      *  Space: O(n * 2^n)
      *   - dp table: 2^n rows * n columns of long long
      *   - from table: 2^n rows * n columns of int
      *  In practice the limit is memory: n = 20 needs ~250 MB, so this method
      *  is only viable up to n ~ 20-22.
      *
      * Params:
      * n is the number of cities.
      * matrix is the n x n adjacency matrix, where matrix[i][j] is the
      * distance from city i to city j. Passed by constant reference to avoid
      * copying it. It also works for asymmetric matrices.
      * Returns:
      * a vector with the n cities of the optimal tour in visiting order,
      * starting at city 0. The return to the starting city is implicit.
      */
     vector<int> held_karp(int n, const vector<vector<int>>& matrix) {
          // With a single city the tour is just that city
          if (n == 1) return {0};

          // Stage 1: table initialization -> O(n * 2^n) time and space
          const long long INF = LLONG_MAX / 2;
          int FULL = 1 << n;                    // number of subsets = 2^n

          // dp[mask][i]: best cost for the state (visited set, current city)
          vector<vector<long long>> dp(FULL, vector<long long>(n, INF));
          // from[mask][i]: city visited right before i in that best path,
          // used at the end to rebuild the tour
          vector<vector<int>> from(FULL, vector<int>(n, -1));

          // Base case: only city 0 visited, standing at 0, cost 0
          dp[1][0] = 0;

          // Stage 2: fill the table -> O(n^2 * 2^n) time
          for (int mask = 1; mask < FULL; mask++) {              // 2^n masks
               // Every valid path starts at city 0, so bit 0 must be set
               if (!(mask & 1)) continue;

               for (int i = 0; i < n; i++) {                     // n last cities
                    if (!(mask & (1 << i)) || dp[mask][i] == INF) continue;

                    for (int j = 0; j < n; j++) {                // n next cities
                         if (mask & (1 << j)) continue;          // already visited

                         int nextMask = mask | (1 << j);         // add j to the set
                         long long cost = dp[mask][i] + matrix[i][j];

                         // Keep only the cheapest way to reach (nextMask, j)
                         if (cost < dp[nextMask][j]) {
                              dp[nextMask][j] = cost;
                              from[nextMask][j] = i;
                         }
                    }
               }
          }

          // Stage 3: close the cycle -> O(n) time
          int last = -1;
          long long best = INF;
          for (int i = 1; i < n; i++) {
               long long cost = dp[FULL - 1][i] + matrix[i][0];
               if (cost < best) {
                    best = cost;
                    last = i;
               }
          }

          // Stage 4: rebuild the path -> O(n) time and space
          // Walk backwards with from[][]: save the current city, remove it
          vector<int> path;
          int mask = FULL - 1;
          int cur = last;
          while (cur != -1) {
               path.push_back(cur);
               int prev = from[mask][cur];
               mask ^= (1 << cur);                  // remove cur from the set
               cur = prev;
          }

          // The path was built from the end to the start
          reverse(path.begin(), path.end());
          return path;
     }
};

#endif