/* 
 * Title: backtracking.h
 * 
 * Description: 
 * 
 * Implementation of the backtracking algorithm to solve a maze of 0s and 1s, where: 
 * - 0 = wall 
 * - 1 = free space 
 * 
 * The algorithm searches for a path from [0][0] to [rows-1][columns-1]. 
 * If a path fails, it goes back and tries another direction until a solution is found.
 * 
 * Implementation for the subject - Analysis and Design of Advanced
 * Algorithms
 * 
 * Author: Alexis Yaocalli Berthou Haas - A01713458 & Rodrigo Alejandro Hurtado Cortés - A01713854
 * Date: August 25, 2026
 */


#ifndef BACKTRACKING_H
#define BACKTRACKING_H

#include <vector>

using namespace std;

class Backtracking {

private:

    vector<vector<bool>> visited;
    vector<vector<int>> solution;

    /**
     * Busca un camino desde la posición actual hasta la salida.
     *
     * Criterio de avance: abajo -> derecha -> izquierda -> arriba.
     * Complejidad: tiempo O(3^(M*N)) en el peor caso y espacio O(M*N),
     * considerando visited, solution y la pila de recursion.
     */
    bool findWay(const vector<vector<int>>& maze, int row, int col) {

        int rows = maze.size();
        int cols = maze[0].size();

        // Verificar que la posición sea válida
        if (row < 0 || row >= rows ||
            col < 0 || col >= cols ||
            maze[row][col] == 0 ||
            visited[row][col]) {

            return false;
        }

        // Marcar la casilla como visitada
        visited[row][col] = true;
        solution[row][col] = 1;

        // Si llegamos a la salida, terminamos
        if (row == rows - 1 && col == cols - 1) {
            return true;
        }

        // Intentar avanzar en las cuatro direcciones

        // Abajo
        if (findWay(maze, row + 1, col)) {
            return true;
        }

        // Derecha
        if (findWay(maze, row, col + 1)) {
            return true;
        }

        // Izquierda
        if (findWay(maze, row, col - 1)) {
            return true;
        }


        // Arriba
        if (findWay(maze, row - 1, col)) {
            return true;
        }

        // Backtracking:
        // Si ninguna dirección funciona,
        // quitamos esta casilla del camino.
        solution[row][col] = 0;

        return false;
    }


public:
    /**
     * Encuentra un camino desde la entrada hasta la salida del laberinto.
     *
     * @param maze El laberinto representado como una matriz de 0s y 1s.
     * @param rows Número de filas del laberinto.
     * @param cols Número de columnas del laberinto.
     * @return Una matriz que representa el camino encontrado, o {{-1}} si no hay camino.
     * Complejidad: tiempo O(3^(M*N)) y espacio O(M*N).
     */

    vector<vector<int>> travelMaze(
        const vector<vector<int>>& maze,
        int rows,
        int cols
    ) {

        // Validar que inicio y salida sean transitables
        if (maze[0][0] == 0 ||
            maze[rows - 1][cols - 1] == 0) {

            return {{-1}};
        }

        visited.assign(rows, vector<bool>(cols, false));
        solution.assign(rows, vector<int>(cols, 0));

        // Comenzar desde (0,0)
        bool found = findWay(maze, 0, 0);

        if (!found) {
            return {{-1}};
        }

        return solution;
    }
};

#endif
