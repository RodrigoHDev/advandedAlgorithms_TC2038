/*
 * Title: tsp.h
 *
 * Description:
 * TspSolver class to solve the Traveling Salesman Problem (TSP): visit every
 * colony exactly once and return to the starting colony with the minimum
 * total distance. The graph is complete and given as an adjacency matrix.
 * It offers two methods:
 *   - heldKarp(): exact solution with dynamic programming over subsets.
 *     Always finds the optimum, but is exponential (viable up to n ~ 20).
 *     This is the method used by the main program.
 *   - greedyEdge(): approximate solution with the greedy edge heuristic.
 *     Fast and suitable for large inputs, but does not guarantee the optimum.
 * The class only processes the matrix it receives; reading the input and
 * displaying the results are handled by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#ifndef TSP_H
#define TSP_H

#include <algorithm>
#include <climits>
#include <numeric>
#include <tuple>
#include <vector>

using namespace std;

/*
In order to analyze complexity:
n = number of colonies (nodes).
E = number of edges in the complete graph = n * (n - 1) / 2, so E = O(n^2).
*/

class TspSolver {
	private:

	/**
	 * find()
	 * Returns the root (representative) of the fragment that contains x.
	 * Each fragment is a tree stored in parent, where a node whose parent is
	 * itself is the root. Every node visited on the way up is pointed
	 * directly to the root (path compression) to speed up later queries.
	 *
	 * Complexity:
	 *  Time: O(log n) amortized
	 *  Space: O(log n) amortized for the recursion stack
	 *
	 * Params:
	 * parent is the union-find array, passed by reference so the path
	 * compression persists between calls.
	 * x is the node whose fragment root is searched.
	 * Returns:
	 * the index of the root of the fragment that contains x.
	 */
	int find(vector<int>& parent, int x) {
		if (parent[x] != x) {
			parent[x] = find(parent, parent[x]);
		}
		return parent[x];
	}

	/**
	 * unite()
	 * Merges the fragments that contain a and b. If both nodes already share
	 * the same root they are connected by a path, so the edge (a, b) would
	 * close a cycle and nothing is merged.
	 *
	 * Complexity:
	 *  Time: O(log n) amortized (two calls to find)
	 *  Space: O(log n) amortized for the recursion stack
	 *
	 * Params:
	 * parent is the union-find array, passed by reference.
	 * a and b are the endpoints of the edge being evaluated.
	 * Returns:
	 * true if the fragments were merged (the edge is valid), false if they
	 * were already the same fragment (the edge would close a cycle).
	 */
	bool unite(vector<int>& parent, int a, int b) {
		int rootA = find(parent, a);
		int rootB = find(parent, b);
		bool merged = rootA != rootB;
		if (merged) {
			parent[rootA] = rootB;
		}
		return merged;
	}

	/**
	 * containsCity()
	 * Checks whether a colony belongs to the set represented by a bitmask,
	 * where bit j is 1 if colony j was visited.
	 *
	 * Complexity:
	 *  Time: O(1)
	 *  Space: O(1)
	 *
	 * Params:
	 * mask is the set of visited colonies.
	 * city is the index of the colony to check.
	 * Returns:
	 * true if the bit of the colony is set in the mask, false otherwise.
	 */
	bool containsCity(int mask, int city) {
		int cityBit = 1 << city;
		return (mask & cityBit) != 0;
	}

	public:

	/**
	 * greedyEdge()
	 * Builds a tour with the greedy edge heuristic in four stages:
	 *  1. Generates the list of edges (weight, u, v) from the upper triangle
	 *     of the matrix and sorts it by weight.
	 *  2. Traverses the sorted list once, accepting an edge only if both
	 *     endpoints have degree < 2 and belong to different fragments. A
	 *     rejected edge can never become valid later (degrees only grow and
	 *     fragments only merge), so a single pass is enough. It stops after
	 *     accepting n - 1 edges, which form a Hamiltonian path.
	 *  3. Closes the cycle by joining the two colonies with degree 1.
	 *  4. Walks the cycle from colony 0 to obtain the visiting order.
	 *
	 * Complexity:
	 *  Time: O(n^2 log n)
	 *   - Building the edge list: O(E) = O(n^2)
	 *   - Sorting the edges: O(E log E) = O(n^2 log n)  <- dominant term
	 *   - Greedy pass: O(E log n) = O(n^2 log n) in the worst case
	 *   - Finding the endpoints and walking the cycle: O(n)
	 *  Space: O(n^2) for the edge list
	 *
	 * Params:
	 * n is the number of colonies.
	 * matrix is the n x n symmetric adjacency matrix, where matrix[i][j] is
	 * the distance between colony i and colony j.
	 * Returns:
	 * a vector with the n colonies in visiting order, starting at colony 0.
	 * The return to the starting colony is implicit.
	 */
	vector<int> greedyEdge(int n, const vector<vector<int>>& matrix) {
		if (n == 1) {
			return {0};
		}

		vector<int> degree(n, 0);
		vector<int> parent(n, 0);
		iota(parent.begin(), parent.end(), 0);

		// Stage 1: the weight goes first so sort() orders the tuples by weight
		vector<tuple<int, int, int>> edges;
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				edges.emplace_back(matrix[i][j], i, j);
			}
		}
		sort(edges.begin(), edges.end());

		// Stage 2: the loop ends when the path already has n - 1 edges
		vector<vector<int>> neighbors(n);
		int accepted = 0;
		int edgeCount = edges.size();
		for (int k = 0; k < edgeCount && accepted < n - 1; k++) {
			int u = get<1>(edges[k]);
			int v = get<2>(edges[k]);
			// unite() is only reached if the degree rule holds; it checks
			// the cycle rule and merges the fragments when the edge is valid
			bool degreeAvailable = degree[u] < 2 && degree[v] < 2;
			if (degreeAvailable && unite(parent, u, v)) {
				degree[u]++;
				degree[v]++;
				neighbors[u].push_back(v);
				neighbors[v].push_back(u);
				accepted++;
			}
		}

		// Stage 3: the endpoints of the path are the only colonies with degree 1
		int firstEnd = -1;
		int secondEnd = -1;
		for (int i = 0; i < n; i++) {
			if (degree[i] == 1) {
				if (firstEnd == -1) {
					firstEnd = i;
				} else {
					secondEnd = i;
				}
			}
		}
		neighbors[firstEnd].push_back(secondEnd);
		neighbors[secondEnd].push_back(firstEnd);

		// Stage 4: every colony has two neighbors, so the next one is always
		// the neighbor we did not come from
		vector<int> tour;
		int previous = -1;
		int current = 0;
		for (int k = 0; k < n; k++) {
			tour.push_back(current);
			int next = neighbors[current][0] != previous ? neighbors[current][0] : neighbors[current][1];
			previous = current;
			current = next;
		}

		return tour;
	}

	/**
	 * heldKarp()
	 * Finds the optimal tour with dynamic programming over subsets.
	 *
	 * Key idea:
	 * For the rest of the tour, only two things matter about a partial path:
	 * which colonies were visited and the colony where it ends. Among all
	 * partial paths with the same set and the same last colony, only the
	 * cheapest one needs to be kept.
	 *
	 * State:
	 * dp[mask][i] = minimum cost of starting at colony 0, visiting exactly
	 * the colonies in mask, and ending at colony i.
	 * mask is an integer used as a set: bit j is 1 if colony j was visited.
	 *   Example with n = 4: mask = 1101 = 13 -> visited {0, 2, 3}.
	 *
	 * Recurrence:
	 *   Base:       dp[{0}][0] = 0   (mask = 1, standing at the start)
	 *   Transition: dp[mask | {j}][j] = min(dp[mask | {j}][j],
	 *                                       dp[mask][i] + matrix[i][j])
	 *               for every reachable state (mask, i) and every colony j
	 *               not in mask.
	 *   Answer:     min over i of dp[all][i] + matrix[i][0]
	 *
	 * Complexity:
	 *  Time: O(n^2 * 2^n)
	 *   - Table initialization: O(n * 2^n)
	 *   - Filling the table: 2^n masks * n last colonies * n next colonies
	 *     = O(n^2 * 2^n)  <- dominant term
	 *   - Choosing the best last colony and rebuilding the path: O(n)
	 *  Space: O(n * 2^n) for the dp and from tables
	 *  In practice the limit is memory: n = 20 needs ~250 MB, so this method
	 *  is only viable up to n ~ 20-22.
	 *
	 * Params:
	 * n is the number of colonies.
	 * matrix is the n x n adjacency matrix, where matrix[i][j] is the
	 * distance from colony i to colony j. It also works for asymmetric
	 * matrices.
	 * Returns:
	 * a vector with the n colonies of the optimal tour in visiting order,
	 * starting at colony 0. The return to the starting colony is implicit.
	 */
	vector<int> heldKarp(int n, const vector<vector<int>>& matrix) {
		if (n == 1) {
			return {0};
		}

		// Stage 1: table initialization. INF is halved so INF + distance
		// cannot overflow.
		const long long INF = LLONG_MAX / 2;
		const int FULL = 1 << n;

		// dp[mask][i]: best cost for the state (visited set, last colony)
		vector<vector<long long>> dp(FULL, vector<long long>(n, INF));
		// from[mask][i]: colony visited right before i in that best path,
		// used at the end to rebuild the tour
		vector<vector<int>> from(FULL, vector<int>(n, -1));
		dp[1][0] = 0;

		// Stage 2: every valid path starts at colony 0, so only odd masks
		// (bit 0 set) are processed. Adding a colony always produces a larger
		// mask, so each state is complete before it is extended.
		for (int mask = 1; mask < FULL; mask += 2) {
			for (int i = 0; i < n; i++) {
				// Unreachable states, such as returning to colony 0 in the
				// middle of the tour, keep INF and are never extended
				if (containsCity(mask, i) && dp[mask][i] != INF) {
					for (int j = 0; j < n; j++) {
						if (!containsCity(mask, j)) {
							int nextMask = mask | 1 << j;
							long long cost = dp[mask][i] + matrix[i][j];
							if (cost < dp[nextMask][j]) {
								dp[nextMask][j] = cost;
								from[nextMask][j] = i;
							}
						}
					}
				}
			}
		}

		// Stage 3: the return to colony 0 is added here because the cheapest
		// partial cost is not always the cheapest complete cycle
		int last = -1;
		long long best = INF;
		for (int i = 1; i < n; i++) {
			long long cost = dp[FULL - 1][i] + matrix[i][0];
			if (cost < best) {
				best = cost;
				last = i;
			}
		}

		// Stage 4: walk backwards with from, removing each colony from the
		// set, until reaching colony 0 (whose from value is -1)
		vector<int> path;
		int mask = FULL - 1;
		int current = last;
		while (current != -1) {
			path.push_back(current);
			int previous = from[mask][current];
			mask ^= 1 << current;
			current = previous;
		}

		reverse(path.begin(), path.end());
		return path;
	}
};

#endif