#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <utility>

using namespace std;

// Represents an edge between two colonies
struct Edge {
    int u, v;
    int weight;
};

// Disjoint Set Union (DSU)
struct DSU {
    vector<int> parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        iota(parent.begin(), parent.end(), 0);
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
            swap(a, b);

        parent[b] = a;

        if (rank[a] == rank[b])
            rank[a]++;

        return true;
    }
};

// Kruskal's Minimum Spanning Tree
void kruskal(const vector<vector<int>>& distances, int n) {

    vector<Edge> edges;

    // 1. Build the edge list from the adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {

            if (distances[i][j] > 0) {
                edges.push_back({i, j, distances[i][j]});
            }
        }
    }

    // 2. Sort edges by weight
    sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });

    // 3. Initialize DSU
    DSU dsu(n);

    int totalWeight = 0;
    int selectedEdges = 0;

    cout << "\n=== MINIMUM SPANNING TREE (KRUSKAL) ===\n";

    // 4. Select edges without creating cycles
    for (const Edge& edge : edges) {

        if (dsu.unite(edge.u, edge.v)) {

            char u = 'A' + edge.u;
            char v = 'A' + edge.v;

            cout << "(" << u << ", " << v << ") - "
                 << edge.weight << " km\n";

            totalWeight += edge.weight;
            selectedEdges++;

            if (selectedEdges == n - 1)
                break;
        }
    }

    // 5. Display result
    if (selectedEdges != n - 1) {
        cout << "Error: The graph is disconnected. "
             << "It is impossible to connect all colonies.\n";
        return;
    }

    cout << "Total cable required: "
         << totalWeight << " km\n";
}

int main() {
    ifstream file("input.txt");

    if (!file.is_open()) {
        cerr << "Error: Could not open input.txt" << endl;
        return 1;
    }

    int N;

    // 1. Read number of colonies
    file >> N;

    if (N <= 0) {
        cerr << "Error: Invalid number of colonies" << endl;
        return 1;
    }

    // 2. Read distance matrix
    vector<vector<int>> distances(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file >> distances[i][j];
        }
    }

    // 3. Read capacity matrix
    vector<vector<int>> capacities(N, vector<int>(N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            file >> capacities[i][j];
        }
    }

    // 4. Read number of existing centers
    int K;
    file >> K;

    if (K <= 0) {
        cerr << "Error: Invalid number of centers" << endl;
        return 1;
    }

    // 5. Read center coordinates
    vector<pair<double, double>> centers(K);

    for (int i = 0; i < K; i++) {
        file >> centers[i].first >> centers[i].second;
    }

    // 6. Read new customer coordinates
    pair<double, double> newLocation;
    file >> newLocation.first >> newLocation.second;

    if (file.fail()) {
        cerr << "Error: Invalid or incomplete input file" << endl;
        return 1;
    }

    file.close();

    // 7. Display input (for testing)
    cout << "Number of colonies: " << N << endl;

    cout << "\nDistance matrix:\n";
    for (const auto& row : distances) {
        for (int value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << "\nCapacity matrix:\n";
    for (const auto& row : capacities) {
        for (int value : row) {
            cout << value << " ";
        }
        cout << endl;
    }

    cout << "\nNumber of centers: " << K << endl;

    cout << "\nExisting centers:\n";
    for (int i = 0; i < K; i++) {
        cout << "Center " << i + 1 << ": ("
             << centers[i].first << ", "
             << centers[i].second << ")" << endl;
    }

    cout << "\nNew customer location: ("
         << newLocation.first << ", "
         << newLocation.second << ")" << endl;

    kruskal(distances, N);

    return 0;
}
