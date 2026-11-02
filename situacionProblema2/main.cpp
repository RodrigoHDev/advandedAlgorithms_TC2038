/*
 * Title: main.cpp
 *
 * Description:
 * Main program of the problem situation. Reads the input file (number of
 * colonies, distance matrix, capacity matrix, centrals and new contract),
 * and solves the four requested problems:
 *   1. Optimal fiber optic cabling (Kruskal).
 *   2. Shortest mail route that visits every colony (Held-Karp).
 *   3. Maximum information flow from the first to the last colony
 *      (Edmonds-Karp).
 *   4. Closest central to the new contract (Euclidean distance).
 * An optional argument selects the input file; input/input.txt is the
 * default.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "algorithms/edmonds_karp.h"
#include "algorithms/kruskal.h"
#include "algorithms/nearest.h"
#include "algorithms/tsp.h"

using namespace std;

// Data read from the input file
struct InputData {
	int numColonies = 0;
	vector<vector<int>> distances;
	vector<vector<int>> capacities;
	vector<pair<double, double>> centers;
	pair<double, double> newLocation = {0.0, 0.0};
};

/**
 * readMatrix()
 * Reads a square matrix of integers from the input file.
 *
 * Params:
 * file is the open input file.
 * size is the number of rows and columns.
 * Returns:
 * the matrix read from the file.
 */
vector<vector<int>> readMatrix(ifstream& file, int size) {
	vector<vector<int>> matrix(size, vector<int>(size, 0));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			file >> matrix[i][j];
		}
	}
	return matrix;
}

/**
 * readInput()
 * Reads and validates every block of the input file. Errors are reported
 * on the standard error output.
 *
 * Params:
 * inputPath is the path of the input file.
 * data is the structure where the values read are stored.
 * Returns:
 * true if the whole file was read correctly, false otherwise.
 */
bool readInput(const char* inputPath, InputData& data) {
	ifstream file(inputPath);
	if (!file.is_open()) {
		cerr << "Error: Could not open " << inputPath << endl;
		return false;
	}

	if (!(file >> data.numColonies) || data.numColonies <= 0) {
		cerr << "Error: Invalid number of colonies" << endl;
		return false;
	}

	data.distances = readMatrix(file, data.numColonies);
	data.capacities = readMatrix(file, data.numColonies);

	int numCenters = 0;
	if (!(file >> numCenters) || numCenters <= 0) {
		cerr << "Error: Invalid number of centers" << endl;
		return false;
	}

	// Coordinates are read as "x y" pairs
	data.centers.assign(numCenters, {0.0, 0.0});
	for (int i = 0; i < numCenters; i++) {
		file >> data.centers[i].first >> data.centers[i].second;
	}
	file >> data.newLocation.first >> data.newLocation.second;

	// A failure in any previous read leaves the stream in a failed state
	if (file.fail()) {
		cerr << "Error: Invalid or incomplete input file" << endl;
		return false;
	}

	return true;
}

/**
 * printMatrix()
 * Displays a matrix with a title, one row per line.
 *
 * Params:
 * title is the text shown before the matrix.
 * matrix is the matrix to display.
 */
void printMatrix(const string& title, const vector<vector<int>>& matrix) {
	cout << "\n" << title << ":\n";
	for (const vector<int>& row : matrix) {
		for (int value : row) {
			cout << value << " ";
		}
		cout << endl;
	}
}

/**
 * printInput()
 * Displays the data read from the input file to verify it.
 *
 * Params:
 * data is the structure with the values read.
 */
void printInput(const InputData& data) {
	cout << "Number of colonies: " << data.numColonies << endl;
	printMatrix("Distance matrix", data.distances);
	printMatrix("Capacity matrix", data.capacities);

	cout << "\nNumber of centers: " << data.centers.size() << endl;
	cout << "\nExisting centers:\n";
	for (size_t i = 0; i < data.centers.size(); i++) {
		cout << "Center " << i + 1 << ": (" << data.centers[i].first << ", "
			 << data.centers[i].second << ")" << endl;
	}

	cout << "\nNew customer location: (" << data.newLocation.first << ", "
		 << data.newLocation.second << ")" << endl;
}

/**
 * printMst()
 * Displays the connections of the optimal cabling, named with letters,
 * and the total length of cable.
 *
 * Params:
 * mst is the result returned by kruskal().
 */
void printMst(const MstResult& mst) {
	cout << "\n=== MINIMUM SPANNING TREE (KRUSKAL) ===\n";
	for (const Edge& edge : mst.selected) {
		char u = static_cast<char>('A' + edge.u);
		char v = static_cast<char>('A' + edge.v);
		cout << "(" << u << ", " << v << ") - " << edge.weight << " km\n";
	}

	if (mst.connected) {
		cout << "Total cable required: " << mst.totalWeight << " km\n";
	} else {
		cout << "Error: The graph is disconnected. "
			 << "It is impossible to connect all colonies.\n";
	}
}

/**
 * calculateRouteCost()
 * Adds the distances of a closed route, including the return from the
 * last colony to the first one.
 *
 * Params:
 * route is the list of colonies in visiting order.
 * distances is the distance matrix.
 * Returns:
 * the total distance of the route in km.
 */
long long calculateRouteCost(const vector<int>& route, const vector<vector<int>>& distances) {
	long long cost = 0;
	int length = route.size();
	for (int i = 0; i < length; i++) {
		cost += distances[route[i]][route[(i + 1) % length]];
	}
	return cost;
}

/**
 * printRoute()
 * Displays the mail route, including the return to the starting colony,
 * and its total distance.
 *
 * Params:
 * route is the list of colonies in visiting order.
 * distances is the distance matrix.
 */
void printRoute(const vector<int>& route, const vector<vector<int>>& distances) {
	cout << "\n=== SHORTEST ROUTE ===\n";
	cout << "Route: ";
	for (int colony : route) {
		cout << "Colony " << colony + 1 << " -> ";
	}
	cout << "Colony " << route[0] + 1 << '\n';
	cout << "Total distance: " << calculateRouteCost(route, distances) << " km\n";
}

/**
 * printMaxFlow()
 * Displays the maximum information flow between the first and the last
 * colony.
 *
 * Params:
 * maxFlow is the value returned by edmondsKarp().
 */
void printMaxFlow(long long maxFlow) {
	cout << "\n=== MAXIMUM FLOW (EDMONDS-KARP) ===\n";
	cout << "Maximum information flow (first to last colony): " << maxFlow << '\n';
}

/**
 * printClosestCenter()
 * Displays the closest central to the new contract and its distance with
 * two decimals.
 *
 * Params:
 * closest is the pair (index, distance) returned by nearestCentral().
 * centers is the list of central coordinates.
 */
void printClosestCenter(const pair<int, double>& closest, const vector<pair<double, double>>& centers) {
	cout << "\n=== CLOSEST CENTER ===\n";
	cout << "Closest center: Center " << closest.first + 1 << " ("
		 << centers[closest.first].first << ", "
		 << centers[closest.first].second << ")\n";
	cout << fixed << setprecision(2) << "Distance: " << closest.second << '\n';
}

/**
 * main()
 * Reads the input and runs the four solutions in order.
 *
 * Params:
 * argc is the number of command line arguments.
 * argv contains the arguments; argv[1] is the optional input path.
 * Returns:
 * 0 if every section ran correctly, 1 if the input was invalid.
 */
int main(int argc, char* argv[]) {
	const char* inputPath = argc > 1 ? argv[1] : "input/input.txt";
	InputData data;
	if (!readInput(inputPath, data)) {
		return 1;
	}
	printInput(data);

	// Section 1: optimal cabling between colonies
	printMst(kruskal(data.distances, data.numColonies));

	// Section 2: exact shortest route; Held-Karp is viable for small inputs
	TspSolver solver;
	printRoute(solver.heldKarp(data.numColonies, data.distances), data.distances);

	// Section 3: maximum flow from the first to the last colony
	long long maxFlow = edmondsKarp(data.capacities, 0, data.numColonies - 1);
	if (maxFlow == -1) {
		cerr << "Error: Invalid capacity matrix or colony indices\n";
		return 1;
	}
	printMaxFlow(maxFlow);

	// Section 4: closest existing central to the new contract
	CentralLocator locator;
	printClosestCenter(locator.nearestCentral(data.centers, data.newLocation), data.centers);

	return 0;
}