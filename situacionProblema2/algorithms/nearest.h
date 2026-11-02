/*
 * Title: nearest.h
 *
 * Description:
 * CentralLocator class to decide which existing central is geographically
 * closest to a new service contract, using the Euclidean distance between
 * points on a plane. The class only compares the coordinates it receives;
 * reading the input and displaying the results are handled by the main
 * program.
 *
 * Implementation for the subject- Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortes - A01713854 & Ricardo
 * Date: Noviembre 1, 2026
 */

#ifndef NEAREST_H
#define NEAREST_H

#include <cmath>
#include <utility>
#include <vector>

using namespace std;

/*
In order to analyze complexity:
C = number of existing centrals.
*/

class CentralLocator {
	public:

	/**
	 * nearestCentral()
	 * Finds the central closest to a new contract by computing the Euclidean
	 * distance from the new location to every central and keeping the
	 * minimum. For a single query every central must be checked once, so a
	 * linear search is already optimal.
	 *
	 * Complexity:
	 *  Time: O(C)
	 *  Space: O(1)
	 *
	 * Params:
	 * centrals is the list of (x, y) locations of the existing centrals.
	 * newPoint is the (x, y) location of the new contract.
	 * Returns:
	 * a pair with the index of the closest central and its distance.
	 * Returns {-1, -1.0} if there are no centrals.
	 */
	pair<int, double> nearestCentral(const vector<pair<double, double>>& centrals,
									 const pair<double, double>& newPoint) {
		int best = -1;
		double bestDistance = -1.0;
		int centralCount = centrals.size();

		for (int i = 0; i < centralCount; i++) {
			double distance = hypot(centrals[i].first - newPoint.first,
									centrals[i].second - newPoint.second);
			if (best == -1 || distance < bestDistance) {
				bestDistance = distance;
				best = i;
			}
		}

		return {best, bestDistance};
	}
};

#endif