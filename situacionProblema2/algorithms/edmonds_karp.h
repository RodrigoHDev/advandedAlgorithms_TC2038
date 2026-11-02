/*
 * Title: edmonds_karp.h
 *
 * Description:
 * Computes the maximum information flow between two colonies with the
 * Edmonds-Karp algorithm: it repeatedly finds, with a breadth-first search,
 * the augmenting path with the fewest connections in the residual network
 * and sends through it as much flow as its most limited connection allows.
 * The functions only process the capacity matrix they receive; reading the
 * input and displaying the results are handled by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#ifndef EDMONDS_KARP_H
#define EDMONDS_KARP_H

#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

/*
In order to analyze complexity:
n = number of colonies (nodes).
E = number of connections with capacity; with a matrix E = O(n^2).
*/

/**
 * isValidNetwork()
 * Checks that the capacity matrix is square, has no negative capacities,
 * and that the source and sink are valid colony indices.
 *
 * Complexity:
 *  Time: O(n^2)
 *  Space: O(1)
 *
 * Params:
 * capacities is the n x n matrix, where capacities[u][v] is the maximum
 * flow that can be sent from colony u to colony v.
 * source is the index of the origin colony.
 * sink is the index of the destination colony.
 * Returns:
 * true if the network can be processed, false otherwise.
 */
bool isValidNetwork(const vector<vector<int>>& capacities, int source, int sink) {
	int n = capacities.size();
	bool valid = source >= 0 && sink >= 0 && source < n && sink < n;

	for (int u = 0; u < n && valid; u++) {
		int rowSize = capacities[u].size();
		valid = rowSize == n;
		for (int v = 0; v < rowSize && valid; v++) {
			valid = capacities[u][v] >= 0;
		}
	}

	return valid;
}

/**
 * edmondsKarp()
 * Calculates the maximum flow from source to sink. Each iteration finds
 * the shortest augmenting path with BFS, sends the bottleneck amount
 * through it, and updates the residual network. Sending flow on u -> v
 * adds the same amount to v -> u, which allows a later path to cancel
 * a previous decision.
 *
 * Complexity:
 *  Time: O(n * E^2) in general; each BFS over the matrix costs O(n^2)
 *  Space: O(n^2) for the residual matrix
 *
 * Params:
 * capacities is the n x n matrix, where capacities[u][v] is the maximum
 * flow that can be sent from colony u to colony v. It is not modified.
 * source is the index of the origin colony.
 * sink is the index of the destination colony.
 * Returns:
 * the maximum flow from source to sink, 0 if source and sink are the same
 * colony, or -1 if the matrix or the indices are invalid.
 */
long long edmondsKarp(const vector<vector<int>>& capacities, int source, int sink) {
	if (!isValidNetwork(capacities, source, sink)) {
		return -1;
	}
	if (source == sink) {
		return 0;
	}

	// long long avoids overflow when several int capacities are added
	int n = capacities.size();
	vector<vector<long long>> residual(n, vector<long long>(n, 0));
	for (int u = 0; u < n; u++) {
		for (int v = 0; v < n; v++) {
			residual[u][v] = capacities[u][v];
		}
	}

	long long maxFlow = 0;
	vector<int> parent(n, -1);
	bool pathFound = true;

	while (pathFound) {
		// parent[v] stores the previous colony on the path; -1 means unvisited
		fill(parent.begin(), parent.end(), -1);
		parent[source] = source;
		queue<int> pending;
		pending.push(source);

		// BFS stops as soon as the sink is reached
		while (!pending.empty() && parent[sink] == -1) {
			int u = pending.front();
			pending.pop();
			for (int v = 0; v < n && parent[sink] == -1; v++) {
				if (parent[v] == -1 && residual[u][v] > 0) {
					parent[v] = u;
					pending.push(v);
				}
			}
		}

		pathFound = parent[sink] != -1;
		if (pathFound) {
			// The path can only carry what its most limited connection allows
			long long increment = residual[parent[sink]][sink];
			for (int v = sink; v != source; v = parent[v]) {
				if (residual[parent[v]][v] < increment) {
					increment = residual[parent[v]][v];
				}
			}

			for (int v = sink; v != source; v = parent[v]) {
				int u = parent[v];
				residual[u][v] -= increment;
				residual[v][u] += increment;
			}
			maxFlow += increment;
		}
	}

	return maxFlow;
}

#endif