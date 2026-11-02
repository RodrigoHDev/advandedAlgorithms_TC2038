#include <iostream>
#include <fstream>
#include <vector>
#include "algorithms/kruskal.h"
#include "algorithms/edmonds_karp.h"
#include <utility>
#include <iomanip>
#include "algorithms/tsp.h"
#include "algorithms/nearest.h" 

using namespace std;

int main(int argc, char* argv[]) {
    // An optional path selects a test input; input.txt remains the default.
    const char* inputPath = argc > 1 ? argv[1] : "input/input.txt";
    ifstream file(inputPath);

    if (!file.is_open()) {
        cerr << "Error: Could not open " << inputPath << endl;
        return 1;
    }

    int N;

    // 1. Read number of colonies
    if (!(file >> N) || N <= 0) {
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
    if (!(file >> K) || K <= 0) {
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



    // Section 2: shortest route visiting every colony and returning to the start.
    // Held-Karp is exact but exponential, so the greedy heuristic is used
    // for inputs too large to fit in memory.
    tsp solver;
    vector<int> route = solver.held_karp(N, distances);

    long long routeCost = 0;
    for (int i = 0; i < N; i++)
        routeCost += distances[route[i]][route[(i + 1) % N]];

    cout << "\n=== SHORTEST ROUTE ===\n";
    cout << "Route: ";
    for (int i = 0; i < N; i++)
        cout << "Colony " << route[i] + 1 << " -> ";
    cout << "Colony " << route[0] + 1 << '\n';
    cout << "Total distance: " << routeCost << " km\n";


    // Seccion 3: flujo maximo desde la primera hasta la ultima colonia.
    long long maxFlow = edmondsKarp(capacities, 0, N - 1);
    if (maxFlow == -1) {
        cerr << "Error: Invalid capacity matrix or colony indices\n";
        return 1;
    }
    cout << "\n=== MAXIMUM FLOW (EDMONDS-KARP) ===\n";
    cout << "Maximum information flow (first to last colony): "
        << maxFlow << '\n';


     // Section 4: closest existing center to the new customer.
    nearest near;
    pair<int, double> closest = near.nearest_central(centers, newLocation);

    cout << "\n=== CLOSEST CENTER ===\n";
    cout << "Closest center: Center " << closest.first + 1 << " ("
        << centers[closest.first].first << ", "
        << centers[closest.first].second << ")\n";
    cout << fixed << setprecision(2)
        << "Distance: " << closest.second << '\n';

    return 0;
}
