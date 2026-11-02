#include <iostream>
#include <climits>
#include "edmonds_karp.h"

using namespace std;

int failures = 0;

void check(const char* name, const vector<vector<int>>& capacities,
           int source, int sink, long long expected) {
    vector<vector<int>> original = capacities;
    long long result = edmondsKarp(capacities, source, sink);
    bool ok = result == expected && capacities == original;
    cout << name << ": ";
    if (ok) cout << "OK";
    else {
        cout << "FAIL";
        failures++;
    }
    cout << " (" << result << ", expected " << expected << ")\n";
}

int main() {
    check("Example with opposite capacities", {
        {0,48,12,18}, {52,0,42,32}, {18,46,0,56}, {24,36,52,0}
    }, 0, 3, 78);
    check("Original input", {
        {0,10,5,0}, {0,0,15,10}, {0,0,0,10}, {0,0,0,0}
    }, 0, 3, 15);
    check("Unreachable destination", {{0,5,0},{0,0,0},{0,0,0}}, 0, 2, 0);
    check("Single colony", {{0}}, 0, 0, 0);
    check("Large total", {
        {0,INT_MAX,INT_MAX,0}, {0,0,0,INT_MAX},
        {0,0,0,INT_MAX}, {0,0,0,0}
    }, 0, 3, 2LL * INT_MAX);
    // The second augmenting path must cancel the previous flow on 1 -> 3.
    check("Reverse residual edge required", {
        {0,1,1,0,0,0}, {0,0,0,1,1,0}, {0,0,0,1,0,0},
        {0,0,0,0,0,1}, {0,0,0,0,0,1}, {0,0,0,0,0,0}
    }, 0, 5, 2);
    check("Negative capacity", {{0,-1},{0,0}}, 0, 1, -1);
    check("Invalid source", {{0,1},{0,0}}, -1, 1, -1);
    check("Non-square matrix", {{0,1},{0}}, 0, 1, -1);
    return failures == 0 ? 0 : 1;
}
