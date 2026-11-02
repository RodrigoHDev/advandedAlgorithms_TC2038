/**
 * nearest()
 * Finds the central closest to a new contract by computing the Euclidean
 * distance from the new location to every central and keeping the minimum.
 *
 * Complexity:
 *  Time: O(C), where C is the number of centrals
 *  Space: O(1)
 *
 * Params:
 * centrals is the list of (x, y) locations of the existing centrals.
 * newPoint is the (x, y) location of the new contract.
 * Returns:
 * a pair with the index of the closest central and its distance.
 * Returns {-1, -1} if there are no centrals.
 */

#ifndef NEAREST_H
#define NEAREST_H


#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>

using namespace std;

class nearest {
public:
     pair<int, double> nearest_central(const vector<pair<double,double>>& centrals,
                              pair<double,double> newPoint) {
          if (centrals.empty()) return {-1, -1};

          int best = 0;
          double bestDist = hypot(centrals[0].first - newPoint.first,
                              centrals[0].second - newPoint.second);

          for (int i = 1; i < centrals.size(); i++) {
               double d = hypot(centrals[i].first - newPoint.first,
                              centrals[i].second - newPoint.second);
               if (d < bestDist) {
                    bestDist = d;
                    best = i;
               }
          }
          return {best, bestDist};
     }
};

#endif