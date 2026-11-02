/*
 * Title: kruskal.h
 *
 * Description:
 * Computes the optimal fiber optic cabling between colonies as the minimum
 * spanning tree of the distance graph, using Kruskal's algorithm with a
 * disjoint set union to detect cycles. The function only computes the tree;
 * displaying the result is handled by the main program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <algorithm>
#include <numeric>
#include <utility>
#include <vector>

/*
In order to analyze complexity:
n = number of colonies (nodes).
E = number of possible connections; with a matrix E = O(n^2).
*/

// Connection between two colonies and its distance in km
struct Edge {
	int u = 0;
	int v = 0;
	int weight = 0;
};

// Result of the minimum spanning tree
struct MstResult {
	std::vector<Edge> selected;
	int totalWeight = 0;
	bool connected = false;
};

class DisjointSet {
	private:
	std::vector<int> parent;
	std::vector<int> treeRank;

	public:

	/**
	 * DisjointSet()
	 * Creates n separate groups, one per colony.
	 *
	 * Complexity:
	 *  Time: O(n)
	 *  Space: O(n)
	 *
	 * Params:
	 * n is the number of colonies.
	 */
	DisjointSet(int n) : parent(n, 0), treeRank(n, 0) {
		std::iota(parent.begin(), parent.end(), 0);
	}

	/**
	 * find()
	 * Returns the representative of the group that contains x and points
	 * every visited node directly to it (path compression).
	 *
	 * Complexity:
	 *  Time: O(alpha(n)) amortized, practically constant
	 *  Space: O(log n) for the recursion stack
	 *
	 * Params:
	 * x is the colony whose group is searched.
	 * Returns:
	 * the index of the representative of the group.
	 */
	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]);
		}
		return parent[x];
	}

	/**
	 * unite()
	 * Merges the groups of a and b, hanging the shorter tree from the taller
	 * one (union by rank) so the trees stay shallow.
	 *
	 * Complexity:
	 *  Time: O(alpha(n)) amortized, practically constant
	 *  Space: O(1)
	 *
	 * Params:
	 * a and b are the colonies at the ends of a connection.
	 * Returns:
	 * true if the groups were merged, false if a and b were already in the
	 * same group (the connection would close a cycle).
	 */
	bool unite(int a, int b) {
		int rootA = find(a);
		int rootB = find(b);
		bool merged = rootA != rootB;

		if (merged) {
			if (treeRank[rootA] < treeRank[rootB]) {
				std::swap(rootA, rootB);
			}
			parent[rootB] = rootA;
			if (treeRank[rootA] == treeRank[rootB]) {
				treeRank[rootA]++;
			}
		}

		return merged;
	}
};

/**
 * kruskal()
 * Builds the minimum spanning tree: sorts the connections by distance and
 * accepts each one only if it joins two different groups. A connection
 * inside the same group would close a cycle.
 *
 * Complexity:
 *  Time: O(n^2 + E log E)
 *   - Building the edge list from the matrix: O(n^2)
 *   - Sorting the edges: O(E log E)  <- dominant term
 *   - Selecting edges: O(E * alpha(n))
 *  Space: O(E) for the edge list
 *
 * Params:
 * distances is the n x n matrix of distances in km; 0 means there is no
 * connection between the two colonies.
 * n is the number of colonies.
 * Returns:
 * an MstResult with the selected connections, their total distance, and
 * whether all colonies could be connected.
 */
inline MstResult kruskal(const std::vector<std::vector<int>>& distances, int n) {
	std::vector<Edge> edges;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (distances[i][j] > 0) {
				edges.push_back({i, j, distances[i][j]});
			}
		}
	}

	std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
		return a.weight < b.weight;
	});

	DisjointSet dsu(n);
	MstResult result;
	int selectedEdges = 0;
	int edgeCount = edges.size();

	// A tree with n colonies has exactly n - 1 connections
	for (int k = 0; k < edgeCount && selectedEdges < n - 1; k++) {
		const Edge& edge = edges[k];
		if (dsu.unite(edge.u, edge.v)) {
			result.selected.push_back(edge);
			result.totalWeight += edge.weight;
			selectedEdges++;
		}
	}

	result.connected = selectedEdges == n - 1;
	return result;
}

#endif