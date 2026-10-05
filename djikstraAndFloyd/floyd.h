/* 
 * Title: floyd-warshall.h
 * 
 * Description:
 * Implementation of Floyd's algorithm to obtain the minimum cost from every
 * node to every other node of a weighted directed graph.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortés - A01713854
 * Date: October 3rd, 2026
 */

#ifndef FLOYD_H
#define FLOYD_H

#include <iostream>
#include <vector>

using namespace std;

class Floyd {
    public:
        /*
        * floyd()
        * Default constructor of the class Floyd
        */
        Floyd(){};

        /*
        * pathFinding()
        * Runs Floyd's algorithm to find the minimum cost from every node to
        * every other node in a weighted directed graph.
        *
        * Complexity:
        *   Time:  O(n^3)
        *     Three nested loops, each iterating over all nodes.
        *   Space: O(n^2)
        *     The matrix is received by reference and modified in place.
        *
        * Params:
        *   n: The number of nodes in the graph.
        *   matrix: n x n adjacency matrix representing the graph, where -1
        *           indicates no edge.
        *
        * Returns: Resulting matrix
        */
        vector<vector<int>> pathFinding(int n, vector<vector<int>>& matrix){
            vector<vector<int>> dist = matrix;

            for (int k = 0; k < n; k++) {
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        if (dist[i][k] != -1 && dist[k][j] != -1) {
                            int newDist = dist[i][k] + dist[k][j];
                            if (dist[i][j] == -1 || newDist < dist[i][j]) {
                                dist[i][j] = newDist;
                            }
                        }
                    }
                }
            }

            return dist;
        }
};

#endif // FLOYD_H