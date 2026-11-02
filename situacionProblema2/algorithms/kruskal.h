#ifndef KRUSKAL_H
#define KRUSKAL_H

// Seccion 1: Kruskal calcula el cableado optimo entre colonias
// mediante un arbol de expansion minima del grafo de distancias.
#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Represents an edge between two colonies
struct Edge {
    int u, v;
    int weight;
};

// Disjoint Set Union (DSU)
struct DSU {
    std::vector<int> parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);

        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (rank[a] < rank[b])
            std::swap(a, b);

        parent[b] = a;

        if (rank[a] == rank[b])
            rank[a]++;

        return true;
    }
};

// Kruskal's Minimum Spanning Tree
inline void kruskal(const std::vector<std::vector<int>>& distances, int n) {

    std::vector<Edge> edges;

    // 1. Build the edge list from the adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {

            if (distances[i][j] > 0) {
                edges.push_back({i, j, distances[i][j]});
            }
        }
    }

    // 2. Sort edges by weight
    std::sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });

    // 3. Initialize DSU
    DSU dsu(n);

    int totalWeight = 0;
    int selectedEdges = 0;

    std::cout << "\n=== MINIMUM SPANNING TREE (KRUSKAL) ===\n";

    // 4. Select edges without creating cycles
    for (const Edge& edge : edges) {

        if (dsu.unite(edge.u, edge.v)) {

            char u = 'A' + edge.u;
            char v = 'A' + edge.v;

            std::cout << "(" << u << ", " << v << ") - "
                 << edge.weight << " km\n";

            totalWeight += edge.weight;
            selectedEdges++;

            if (selectedEdges == n - 1)
                break;
        }
    }

    // 5. Display result
    if (selectedEdges != n - 1) {
        std::cout << "Error: The graph is disconnected. "
             << "It is impossible to connect all colonies.\n";
        return;
    }

    std::cout << "Total cable required: "
         << totalWeight << " km\n";
}


#endif

