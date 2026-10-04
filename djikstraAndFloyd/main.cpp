/**
 * Title: main.cpp
 *
 * Description:
 * Reads an n x n adjacency matrix of a weighted directed graph, where -1
 * means there is no edge, and prints the minimum cost from every node to
 * every other node by running Dijkstra's algorithm from each node as well
 * as the minimum cost to all nodes from all nodes using Floyd's algorithm.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 &
 * Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: October 3rd, 2026
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "djikstra.h"

using namespace std;


/*
For complexity analysis take into consideration:
    n = number of nodes (the adjacency matrix is n x n).
*/

/**
 * printMatrix()
 * Prints the matrix row by row, space-separated, under the given name.
 *
 * Complexity:
 *   Time:  O(n^2)
 *   Space: O(1) auxiliary.
 *
 * Params:
 *   matrix: n x n adjacency matrix to print.
 *   name: title printed before the matrix.
 * Returns: none
 */
void printMatrix(const vector<vector<int>>& matrix, const string& name) {

     cout << endl;
     cout << name << endl;

     if (matrix[0][0] == -1) {
          cout << "NO VALID MATRIX" << endl;
          return;
     }

     for (size_t i = 0; i < matrix.size(); i++) {
          for (size_t j = 0; j < matrix[0].size(); j++) {
               cout << matrix[i][j] << " ";
          }
          cout << endl;
     }
}


/**
 * printDjikstraResults()
 * Prints the minimum cost from node to every other node, using 1-indexed
 * node numbers (e.g. "node 1 to node 2: 4").
 *
 * Complexity:
 *   Time:  O(n)
 *   Space: O(n), because distances is received by value (a copy).
 *
 * Params:
 *   node: source node (0-indexed) the distances were computed from.
 *   distances: vector of size n returned by Djikstra::pathFinding.
 * Returns: none
 */
void printDjikstraResults(int node, vector<int> distances){
     cout <<""<<endl;
     for(int i = 0; i<distances.size(); i++){
          if(!(i == node)){
               cout<<"node "<<node+1<< " to node "<<i+1<<": "<<distances[i]<<endl;
          }
     }
}


/**
 * main()
 * Reads n followed by the n x n adjacency matrix from standard input, prints
 * it and ...
 *
 * Complexity:
 *   Time: Pending Floyd
 *   Space:  Pending Floyd
 *
 * Parameters: none
 * Returns:
 *   0 on successful execution.
 *   1 if the size is invalid or a matrix value cannot be read.
 */
int main() {

     int elements;
     cin>> elements;

     // Validate matrix dimensions
     if (elements < 0) {
          cout << "The matrix to receive is empty. Therefore it does not contains any path." << endl;
          return 1;
     }

     vector<vector<int>> matrix(elements, vector<int>(elements));

     // Read matrix
     for (int i = 0; i < elements; i++) {
          for (int j = 0; j < elements; j++) {

               if (!(cin >> matrix[i][j])) {
                    cout << "ERROR: matrix values must be 0 or 1." << endl;
                    return 1;
               }
          }
     }
     
     // Print original matrix
     printMatrix(matrix, "Original matrix");

     // Run Dijkstra from every node to obtain all-pairs minimum costs
     Djikstra djk;
     for(int i = 0; i < elements; i++){
          vector<int> points = djk.pathFinding(i,matrix);
          printDjikstraResults(i, points);
     }
     


     return 0;
}
