/*
 * Title: test_edmonds_karp.cpp
 *
 * Description:
 * Unit tests for edmondsKarp(). Each case checks the returned flow and that
 * the input matrix is not modified. Covers the assignment example, an
 * unreachable sink, a single colony, a total that exceeds int, a case that
 * needs a reverse residual edge, and invalid inputs.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#include <climits>
#include <iostream>
#include <vector>
#include "edmonds_karp.h"

using namespace std;

int failures = 0;

/**
 * checkFlow()
 * Runs edmondsKarp() on one case, prints OK or FAIL with the obtained and
 * expected values, and counts the failures.
 *
 * Params:
 * name is the description of the case.
 * capacities is the capacity matrix of the case.
 * source is the index of the origin colony.
 * sink is the index of the destination colony.
 * expected is the correct maximum flow (-1 for invalid inputs).
 */
void checkFlow(const char* name, const vector<vector<int>>& capacities,
			   int source, int sink, long long expected) {
	vector<vector<int>> original = capacities;
	long long result = edmondsKarp(capacities, source, sink);
	bool passed = result == expected && capacities == original;

	cout << name << ": ";
	if (passed) {
		cout << "OK";
	} else {
		cout << "FAIL";
		failures++;
	}
	cout << " (" << result << ", expected " << expected << ")\n";
}

/**
 * main()
 * Runs every test case.
 *
 * Returns:
 * 0 if every case passed, 1 if at least one failed.
 */
int main() {
	checkFlow("Example with opposite capacities", {
		{0, 48, 12, 18}, {52, 0, 42, 32}, {18, 46, 0, 56}, {24, 36, 52, 0}
	}, 0, 3, 78);
	checkFlow("Original input", {
		{0, 10, 5, 0}, {0, 0, 15, 10}, {0, 0, 0, 10}, {0, 0, 0, 0}
	}, 0, 3, 15);
	checkFlow("Unreachable destination", {{0, 5, 0}, {0, 0, 0}, {0, 0, 0}}, 0, 2, 0);
	checkFlow("Single colony", {{0}}, 0, 0, 0);
	checkFlow("Large total", {
		{0, INT_MAX, INT_MAX, 0}, {0, 0, 0, INT_MAX},
		{0, 0, 0, INT_MAX}, {0, 0, 0, 0}
	}, 0, 3, 2LL * INT_MAX);

	// The second augmenting path must cancel the previous flow on 1 -> 3
	checkFlow("Reverse residual edge required", {
		{0, 1, 1, 0, 0, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 1}, {0, 0, 0, 0, 0, 0}
	}, 0, 5, 2);

	checkFlow("Negative capacity", {{0, -1}, {0, 0}}, 0, 1, -1);
	checkFlow("Invalid source", {{0, 1}, {0, 0}}, -1, 1, -1);
	checkFlow("Non-square matrix", {{0, 1}, {0}}, 0, 1, -1);

	return failures == 0 ? 0 : 1;
}