/*
 * Title: test_tsp.cpp
 *
 * Description:
 * Tests for the TspSolver class (tsp.h). Each case runs both methods:
 * greedyEdge() is compared with the cost its trace produces, and
 * heldKarp() with the optimal cost (verified by brute force). A tour can
 * come out in reverse order and still be the same cycle, so the tests
 * compare the cost and check that every colony appears exactly once.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#include <iostream>
#include <string>
#include <vector>
#include "tsp.h"

using namespace std;

int failures = 0;

/**
 * calculateTourCost()
 * Adds the distances of a closed tour, including the return to the start.
 *
 * Params:
 * tour is the list of colonies in visiting order.
 * matrix is the distance matrix.
 * Returns:
 * the total cost of the tour.
 */
long long calculateTourCost(const vector<int>& tour, const vector<vector<int>>& matrix) {
	long long total = 0;
	int length = tour.size();
	for (int i = 0; i < length; i++) {
		total += matrix[tour[i]][tour[(i + 1) % length]];
	}
	return total;
}

/**
 * isPermutation()
 * Checks that the tour visits every colony exactly once.
 *
 * Params:
 * tour is the list of colonies in visiting order.
 * n is the number of colonies.
 * Returns:
 * true if the tour contains each colony from 0 to n - 1 once.
 */
bool isPermutation(const vector<int>& tour, int n) {
	bool valid = static_cast<int>(tour.size()) == n;
	vector<bool> seen(n, false);
	int length = tour.size();

	for (int i = 0; i < length && valid; i++) {
		int city = tour[i];
		valid = city >= 0 && city < n && !seen[city];
		if (valid) {
			seen[city] = true;
		}
	}

	return valid;
}

/**
 * reportTour()
 * Prints a tour, its cost and whether it matches the expected cost, and
 * counts the failures.
 *
 * Params:
 * method is the name of the method that produced the tour.
 * tour is the list of colonies in visiting order.
 * matrix is the distance matrix.
 * expected is the expected cost of the tour.
 */
void reportTour(const string& method, const vector<int>& tour,
				const vector<vector<int>>& matrix, long long expected) {
	int n = matrix.size();
	cout << "  [" << method << "] Tour: ";
	for (int city : tour) {
		cout << city << " ";
	}
	cout << "-> " << tour[0] << "\n";

	if (!isPermutation(tour, n)) {
		cout << "  FAIL: the tour does not visit every colony exactly once\n";
		failures++;
	} else {
		long long cost = calculateTourCost(tour, matrix);
		bool passed = cost == expected;
		cout << "  Cost: " << cost << "   Expected: " << expected << "   "
			 << (passed ? "OK" : "FAIL") << "\n";
		if (!passed) {
			failures++;
		}
	}
}

/**
 * runCase()
 * Runs both TSP methods on one matrix and reports each result.
 *
 * Params:
 * name is the description of the case.
 * matrix is the distance matrix.
 * expectedGreedy is the cost the greedy edge heuristic must produce.
 * expectedOptimal is the optimal cost that Held-Karp must find.
 */
void runCase(const string& name, const vector<vector<int>>& matrix,
			 long long expectedGreedy, long long expectedOptimal) {
	int n = matrix.size();
	TspSolver solver;
	cout << "== " << name << " ==\n";
	reportTour("Greedy   ", solver.greedyEdge(n, matrix), matrix, expectedGreedy);
	reportTour("Held-Karp", solver.heldKarp(n, matrix), matrix, expectedOptimal);
	cout << "\n";
}

/**
 * main()
 * Runs every test case.
 *
 * Returns:
 * 0 if every case passed, 1 if at least one failed.
 */
int main() {
	// Case 1: 4 colonies, the greedy heuristic finds the optimum.
	// Sorted edges: 0-1(10), 0-2(15), 0-3(20), 1-3(25), 2-3(30), 1-2(35)
	// Greedy accepts 0-1, 0-2, rejects 0-3 (degree of 0), accepts 1-3 and
	// closes with 2-3: 0 1 3 2 -> 0, cost 10 + 25 + 30 + 15 = 80.
	vector<vector<int>> caseOne = {
		{0, 10, 15, 20},
		{10, 0, 35, 25},
		{15, 35, 0, 30},
		{20, 25, 30, 0}
	};
	runCase("Case 1: 4 colonies", caseOne, 80, 80);

	// Case 2: 6 colonies (A = 0 ... F = 5).
	// Greedy accepts A-B(11), B-C(13), C-D(13), E-F(14), rejects A-D(15) by
	// cycle and several edges by degree, accepts D-F(29) and closes with
	// A-E(32): cost 112. Optimum: A B E F C D -> A, cost 92.
	vector<vector<int>> caseTwo = {
		{0, 11, 20, 15, 32, 38},
		{11, 0, 13, 18, 21, 29},
		{20, 13, 0, 13, 16, 18},
		{15, 18, 13, 0, 30, 29},
		{32, 21, 16, 30, 0, 14},
		{38, 29, 18, 29, 14, 0}
	};
	runCase("Case 2: 6 colonies (A-F)", caseTwo, 112, 92);

	// Case 3: 5 colonies where the greedy heuristic is far from the optimum.
	// Greedy: 0 1 2 3 4 -> 0, cost 1 + 8 + 8 + 3 + 2 = 22, because two
	// edges of weight 8 are forced at the end. Optimum: 0 1 4 3 2 -> 0,
	// cost 17.
	vector<vector<int>> caseThree = {
		{0, 1, 3, 5, 2},
		{1, 0, 8, 7, 2},
		{3, 8, 0, 8, 5},
		{5, 7, 8, 0, 3},
		{2, 2, 5, 3, 0}
	};
	runCase("Case 3: 5 colonies (greedy not optimal)", caseThree, 22, 17);

	return failures == 0 ? 0 : 1;
}