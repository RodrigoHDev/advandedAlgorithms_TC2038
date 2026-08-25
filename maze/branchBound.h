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




    vector<vector<int>> findWay(vector<vector<int>>& maze, pair<int,int> tile, int cost){
        //Si no esta mi celda en el stack, la meto.
        
        if(cost>=bestSteps){
            return {};
        }
        
        if((way.empty() || way.top() != tile) && visited[tile.first][tile.second] == false){
            way.push(tile);
            visited[tile.first][tile.second] = true;
        }

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
        1 = [1,0] = a la derecha
        0 = [-1,0] = a la izquierda
        2 = [0, -1] = abajo
        3 = [0, 1] = arriba
        */

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




    vector<vector<int>> assemblePath(int rows, int colms, stack<pair<int,int>>& path){
        
        vector<vector<int>> final(rows, vector<int>(colms, 0));
        int counter = 0;

        while(!bestWay.empty()){
            pair<int,int> tile = bestWay.top();
            final[tile.first][tile.second] = 1;
            bestWay.pop();
            counter += 1;
        }

        if(counter == 0){
            return {{-1}};
        }

        return final;
    }
};

#endif