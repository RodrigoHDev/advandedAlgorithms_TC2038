/* 
 * Title: djikstra.h
 * 
 * Description:
 * Implementation of Dijkstra's algorithm to obtain the minimum cost from one
 * source node to every other node of a weighted directed graph.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortés - A01713854
 * Date: October 3rd, 2026
 */

#ifndef DJIKSTRA_H
#define DJIKSTRA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

/*
For complexity analysis take into consideration:
    n = number of nodes (the adjacency matrix is n x n).
*/
class Djikstra{
     private:
     // Represents an "infinite" cost, used for unreachable nodes and missing edges
     int MAX_INT = 1e8;

	public:
     /*
     * Djikstra()
     * Default constructor of the class Djikstra
     * 
     * Complexity: O(1) time and O(1) space.
     * Return: none.
     */
     Djikstra(){};

     /*
     * pathFinding()
     * Runs Dijkstra's algorithm from a single source node. 
     * costs[x] stores the minimum known cost from start_node to node x.
     * The priority queue stores (cost, node) pairs ordered by the lowest cost.
     * When a cheaper path to a node is found, a new pair is pushed instead of
     * updating the old one, so outdated pairs are skipped when popped
     * (lazy deletion).
     *
     * Complexity:
     *   Time:  O(n^2 log n)
     *     Each node is expanded only once (with its final cost) and each
     *     expansion scans its whole row of the matrix: n * n = O(n^2) edge checks.
     *     Every successful check pushes into the queue, so it holds up to O(n^2)
     *     pairs, and each push/pop costs O(log n^2) = O(log n).
     *   Space: O(n^2)
     *     The queue may hold up to O(n^2) pairs and the matrix is received by
     *     value (an O(n^2) copy). The costs vector is O(n).
     *
     * Params:
     *   start_node: source node, must be within the matrix limits 0 to n-1.
     *   matrix: n x n adjacency matrix, -1 means there is no edge.
     * Returns: a vector of size n with the minimum cost from start_node to
     *   every node (MAX_INT if the node cannot be reached).
     */
     vector<int> pathFinding(int start_node, vector <vector<int>> matrix){
          int current_cost = 0;
          vector<int> costs(matrix.size(), MAX_INT);
          priority_queue<tuple<int, int>, vector<tuple<int, int>>, greater<tuple<int, int>>> pq;

          // The source node costs 0 to reach itself
          costs[start_node] = 0;
          pq.emplace(tuple{0,start_node});

          while(! pq.empty()){
               // Take the node with the lowest known cost
               auto top = pq.top();
               pq.pop();

               int cost_top = get<0>(top);
               int node_top = get<1>(top);

               // Outdated pair: a cheaper path to this node was already found
               if(cost_top > costs[node_top]){
                    continue;
               }

               // Check every possible neighbor of the current node
               for(int i = 0; i < matrix[node_top].size(); i++){
                    // A missing edge (-1) is treated as an infinite cost
                    int cost_inner = (matrix[node_top][i] == -1) ? MAX_INT : matrix[node_top][i];
                    int node_inner = i;

                    // If going through the current node is cheaper, update the cost
                    // and push the neighbor with its new cost
                    if(costs[node_top] + cost_inner < costs[node_inner]){
                         costs[node_inner] = costs[node_top] + cost_inner;
                         pq.emplace(tuple{costs[node_inner], node_inner});
                    }
               }
          }
          return costs;
     };
};

#endif