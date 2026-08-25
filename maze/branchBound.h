/* 
 * Title: branchBound.h
 * 
 * Description: 
 * implementation of the branch & bound algorithm to traverse a maze
 * of 0 and 1 where:
 * - 0 = wall
 * - 1 = free
 * Looking to go from origin [0][0] to exit [columns-1][rows-1].
 * The algorithm looks for the bestWay to traverse the maze and discards 
 * certain paths depending on a cost variable compared to the temporal 
 * best option.
 * In case the cost is not equal or exceded, then the path continues to be
 * explored, until all paths have been either discarded or explored.
 * 
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortés - A01713854
 * Date: August 25, 2026
 */

#ifndef BRANCHBOUND_H
#define BRANCHBOUND_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <utility>

using namespace std;


class branchBound{
	public:

    int bestSteps;
    stack<pair<int,int>> way;
    stack<pair<int,int>> bestWay;
    vector<vector<bool>> visited;

    /*
    branchBound()
    Class constructor without parameters
    */
	branchBound(){};


    /*
    travelMaze()
    Function responsible for triggering the Branch & Bound algorithm to try to find the
    best way to traverse the maze of fize rows x colms.
    This function calls the recursive function findWay().
    Afterwards assembles a vector<vector<int>> based on the bestWay.

    Consider complexity over the next functions over:
    M = number of rows
    N = number of columns

    maze: original maze to traverse provided by main.cpp.
    rows: number of rows of the maze.
    colms: number of columns of the maze.
    return: the vector containing the path to traverse the maze or a {{-1}} 
    if there is no available path.
    */
    vector<vector<int>> travelMaze(vector<vector<int>>& maze, int rows, int colms){
        bestSteps = rows * colms;
        
        visited.assign(rows, vector<bool>(colms, false));

        pair<int,int> origen{0,0};
        findWay(maze, origen, 0);
        vector<vector<int>> finishPath = assemblePath(rows, colms, bestWay);
        return finishPath;
    }



    /*
    findWay()
    Recursive function that applied the Branch & Bound algorithm to find the best path
    though a given maze of 0s and 1s.
    This function is composed of the following parts:
        Validation with cost (this cuts the search through certain sections, only once a 
        valid path has been found, over which the cost is declared).

        Validation over the cell visited. If the cell is not in the stack and has not been 
        visited (stored in a matrix of bool), add it to the stack.

        If arrived at the end of the maze [N-1][M-1], verify cost and if better declare this
        as the new standard for comparison. Copy the path into stack bestPath.

        Over a series of directions [right,left,down,up] if the next cell is a 1 and is 
        within the limits of the maze, sends a recursive call to iself from this new tile
        and increases cost, creating an exploration over all possible paths unless cost cuts
        it first.

        Once the four explorations are done, the tile is popped out of the stack and declared
        as unvisited to be able to explore a different path using the same tile.

    Complexity:
        Time: O(3^{NxM}) at the worst case.
            
        Taking into consideration the worst base for a backtracking is b^{d} where:
                b = number of possible branches per new recursive call (3 as the previous one
                is already visited).
                d = Greatest path without repeating elements (which is the whole maze).
            
            Although this algorithm is not backtracking, it requires of a finished path in order
            to create the cost validation, which in the worst case will be the this time consuming.
            This is the worst case as if found easlier one of the best paths, the branch cutting will
            be implemented earlier, additionally, the vector visited helps to validate which cells 
            have been already checked in the recursive call avoiding repeaing the same path twice.
        
        Space: O(N x M) 
            There are 4 data structures used in this algorithm.
            - Original maze O(N x M)
            - Visited O(N x M)
            - Stack way. At worst O(N x M)
            - Stack bestWay. At worst O(N x M)

            However, all of these form O(4 (N x M)); taking into consideration the low impact of this
            multiplication, generally, the overall spacial complexity is declared as O(N x M).

    */
    vector<vector<int>> findWay(vector<vector<int>>& maze, pair<int,int> tile, int cost){
        //Cutting branches
        if(cost>=bestSteps){
            return {};
        }
        
        //Adding tiles
        if((way.empty() || way.top() != tile) && visited[tile.first][tile.second] == false){
            way.push(tile);
            visited[tile.first][tile.second] = true;
        }

        //Finding end of path
        if(tile.first == maze.size()-1 && tile.second == maze[0].size()-1){
            if(cost < bestSteps){
                bestSteps = cost;
                bestWay = way;
            }
            else{
                return {};
            }
        }

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, -1, 1};
        /*
        1 = [1,0] = right
        0 = [-1,0] = left
        2 = [0, -1] = down
        3 = [0, 1] = up
        */

        //Traversing more options in the maze if available in 3 directions
        for(int i = 0; i < 4; i++){
            int newX = tile.first + dx[i];
            int newY = tile.second + dy[i];

            if(newX >= 0 && newX <= maze.size()-1 && newY >= 0 && newY <= maze[0].size()-1){
                if(maze[newX][newY]==1 && visited[newX][newY] == false){
                    pair<int,int> next{newX,newY};
                    findWay(maze, next, cost+1);
                }
            }
        }

        way.pop();
        visited[tile.first][tile.second] = false;
        return {};
    }



    /*
    assemblePath()
    Auxiliar function responsible for assembling the final path based on the given 
    stack of bestWay. If not a single element is at the end of the maze, then the
    path is declared as -1 which is invalid.

    rows: Number of rows in the original maze.
    colms: Number of columns in the original maze.
    path: Stack of ints that store the best path found.
    return: vector<vector<int>> with the final path
    */
    vector<vector<int>> assemblePath(int rows, int colms, stack<pair<int,int>>& path){
        
        vector<vector<int>> final(rows, vector<int>(colms, 0));
        int counter = 0;

        while(!bestWay.empty()){
            pair<int,int> tile = bestWay.top();
            final[tile.first][tile.second] = 1;
            bestWay.pop();

            //Validation for invalid paths
            if(tile.first == rows-1 && tile.second == colms-1)
                counter += 1;
        }

        if(counter == 0){
            return {{-1}};
        }

        return final;
    }
};

#endif