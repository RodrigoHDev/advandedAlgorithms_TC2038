/**
 * Title: main.cpp
 *
 * Description:
 * Reads a maze of 0s and 1s and solves it using
 * Backtracking and Branch & Bound.
 *
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 *
 * Author: Alexis Yaocalli Berthou Haas - A01713458 &
 * Rodrigo Alejandro Hurtado Cortes - A01713854
 * Date: August 24, 2026
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "backtracking.h"
#include "branchBound.h"

using namespace std;


/**
 * printMaze()
 * Prints a maze or its solution.
 */
void printMaze(vector<vector<int>> maze, string name) {

    cout << endl;
    cout << name << endl;

    if (maze[0][0] == -1) {
        cout << "NO VALID SOLUTION" << endl;
        return;
    }

    for (int i = 0; i < maze.size(); i++) {

        for (int j = 0; j < maze[0].size(); j++) {
            cout << maze[i][j] << " ";
        }

        cout << endl;
    }
}


/**
 * main()
 * Reads the maze and solves it using
 * Backtracking and Branch & Bound.
 */
int main() {

    int rows;
    int columns;

    cin >> rows >> columns;

    // Validate maze dimensions
    if (rows <= 0 || columns <= 0) {
        cout << "ERROR: rows and columns must be positive integers." << endl;
        return 1;
    }

    vector<vector<int>> maze(rows, vector<int>(columns));

    // Read maze
    for (int i = 0; i < rows; i++) {

        for (int j = 0; j < columns; j++) {

            cin >> maze[i][j];

            // Only 0 and 1 are valid
            if (maze[i][j] != 0 && maze[i][j] != 1) {
                cout << "ERROR: maze values must be 0 or 1." << endl;
                return 1;
            }
        }
    }

    // Print original maze
    printMaze(maze, "Original Maze");

    // Backtracking
    Backtracking backtracking;

    vector<vector<int>> backtrackingResult =
        backtracking.travelMaze(maze, rows, columns);

    printMaze(backtrackingResult, "Backtracking");

    // Branch & Bound
    branchBound branch;

    vector<vector<int>> branchResult =
        branch.travelMaze(maze, rows, columns);

    printMaze(branchResult, "Branch & Bound");

    return 0;
}