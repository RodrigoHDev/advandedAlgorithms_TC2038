/*
 * Title: branchBound.h
 * Description: Branch & Bound solver for a maze of 0s (walls) and 1s (free cells).
 */

#ifndef BRANCHBOUND_H
#define BRANCHBOUND_H

#include <limits>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class branchBound {
private:
    int bestSteps;
    stack<pair<int, int>> way;
    stack<pair<int, int>> bestWay;
    vector<vector<bool>> visited;

    /**
     * Explores simple paths and prunes a path whose cost cannot improve bestSteps.
     * Advance order: down, up, left, right.
     * Complexity: O(3^(M*N)) time in the worst case and O(M*N) auxiliary space.
     */
    void findWay(const vector<vector<int>>& maze, int row, int col, int cost) {
        const int rows = static_cast<int>(maze.size());
        const int columns = static_cast<int>(maze[0].size());

        if (cost >= bestSteps) {
            return;
        }

        way.push({row, col});
        visited[row][col] = true;

        if (row == rows - 1 && col == columns - 1) {
            bestSteps = cost;
            bestWay = way;
        } else {
            const int rowDirections[] = {1, -1, 0, 0};
            const int columnDirections[] = {0, 0, -1, 1};

            for (int direction = 0; direction < 4; direction++) {
                const int nextRow = row + rowDirections[direction];
                const int nextColumn = col + columnDirections[direction];

                if (nextRow >= 0 && nextRow < rows &&
                    nextColumn >= 0 && nextColumn < columns &&
                    maze[nextRow][nextColumn] == 1 &&
                    !visited[nextRow][nextColumn]) {
                    findWay(maze, nextRow, nextColumn, cost + 1);
                }
            }
        }

        way.pop();
        visited[row][col] = false;
    }

    /**
     * Builds the output matrix from the best path.
     * Complexity: O(M*N) time and O(M*N) space for the returned matrix.
     */
    vector<vector<int>> assemblePath(int rows, int columns) const {
        if (bestWay.empty()) {
            return {{-1}};
        }

        vector<vector<int>> result(rows, vector<int>(columns, 0));
        stack<pair<int, int>> path = bestWay;

        while (!path.empty()) {
            const pair<int, int> tile = path.top();
            result[tile.first][tile.second] = 1;
            path.pop();
        }

        return result;
    }

public:
    /**
     * Finds the shortest path using Branch & Bound.
     * Complexity: O(3^(M*N)) time in the worst case and O(M*N) auxiliary space.
     */
    vector<vector<int>> travelMaze(
        const vector<vector<int>>& maze, int rows, int columns) {
        if (maze[0][0] == 0 || maze[rows - 1][columns - 1] == 0) {
            return {{-1}};
        }

        bestSteps = numeric_limits<int>::max();
        while (!way.empty()) way.pop();
        while (!bestWay.empty()) bestWay.pop();
        visited.assign(rows, vector<bool>(columns, false));

        findWay(maze, 0, 0, 0);
        return assemblePath(rows, columns);
    }
};

#endif
