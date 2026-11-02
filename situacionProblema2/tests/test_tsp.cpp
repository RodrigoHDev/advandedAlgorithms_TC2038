/*
 * test_tsp.cpp
 *
 * Pruebas rápidas para el TSP con greedy de aristas (tsp.h).
 *
 * Compilar:  g++ -std=c++17 test_tsp.cpp -o test_tsp
 * Ejecutar:  ./test_tsp
 *
 * Nota: el greedy de aristas es una heurística, así que el resultado
 * esperado NO siempre es el óptimo. En cada caso se comenta:
 *   - el tour y costo que debe dar el greedy
 *   - el tour y costo óptimo (calculado por fuerza bruta), para comparar
 *
 * El tour puede salir en sentido inverso (0 3 1 2 en vez de 0 2 1 3)
 * y sigue siendo el mismo ciclo, por eso la prueba compara el COSTO.
 */

#include <iostream>
#include <vector>
#include <string>
#include "tsp.h"

using namespace std;

// Costo total del ciclo, incluyendo el regreso al nodo inicial
long long costoTour(const vector<int>& tour, const vector<vector<int>>& m) {
     long long total = 0;
     int n = tour.size();
     for (int i = 0; i < n; i++)
          total += m[tour[i]][tour[(i + 1) % n]];
     return total;
}

// Verifica que el tour visite cada nodo exactamente una vez
bool esPermutacion(const vector<int>& tour, int n) {
     if ((int)tour.size() != n) return false;
     vector<bool> visto(n, false);
     for (int x : tour) {
          if (x < 0 || x >= n || visto[x]) return false;
          visto[x] = true;
     }
     return true;
}

void probar(const string& nombre, const vector<vector<int>>& m,
               long long esperado, const string& nombres = "") {
     int n = m.size();
     tsp solver;
     vector<int> tour = solver.held_karp(n, m);

     cout << "== " << nombre << " ==\n";
     cout << "Tour obtenido: ";
     for (int x : tour) {
          if (!nombres.empty()) cout << nombres[x] << ' ';
          else cout << x << ' ';
     }
     if (!tour.empty())
          cout << "-> " << (nombres.empty() ? to_string(tour[0]) : string(1, nombres[tour[0]]));
     cout << "\n";

     if (!esPermutacion(tour, n)) {
          cout << "FALLA: el tour no visita cada nodo exactamente una vez\n\n";
          return;
     }

     long long costo = costoTour(tour, m);
     cout << "Costo obtenido: " << costo << "   Esperado: " << esperado << "\n";
     cout << (costo == esperado ? "OK" : "FALLA") << "\n\n";
}

int main() {
     // ------------------------------------------------------------------
     // Caso 1: 4 nodos (ejemplo clásico de libro)
     //
     // Aristas ordenadas: 0-1(10), 0-2(15), 0-3(20), 1-3(25), 2-3(30), 1-2(35)
     //   0-1 se acepta
     //   0-2 se acepta           -> nodo 0 llega a grado 2
     //   0-3 se rechaza (grado de 0)
     //   1-3 se acepta           -> ya hay n-1 = 3 aristas
     //   Extremos con grado 1: 2 y 3 -> cierre con 2-3 (30)
     //
     // Greedy esperado: 0 1 3 2 -> 0   costo 10 + 25 + 30 + 15 = 80
     // Óptimo:          0 1 3 2 -> 0   costo 80  (aquí el greedy SÍ es óptimo)
     // ------------------------------------------------------------------
     vector<vector<int>> caso1 = {
          { 0, 10, 15, 20},
          {10,  0, 35, 25},
          {15, 35,  0, 30},
          {20, 25, 30,  0}
     };
     probar("Caso 1: 4 nodos", caso1, 80);

     // ------------------------------------------------------------------
     // Caso 2: el ejemplo de 6 ciudades del gráfico (A=0 ... F=5)
     //
     // Aceptadas: A-B(11), B-C(13), C-D(13), E-F(14)
     // Rechazadas: A-D(15) por ciclo; C-E, B-D, C-F, A-C, B-E, B-F por grado
     // Aceptada: D-F(29)        -> ya hay 5 aristas
     // Extremos con grado 1: A y E -> cierre con A-E (32)
     //
     // Greedy esperado: A B C D F E -> A   costo 11+13+13+29+14+32 = 112
     // Óptimo:          A B E F C D -> A   costo 11+21+14+18+13+15 = 92
     // (el greedy queda ~22% arriba del óptimo por las aristas forzadas D-F y A-E)
     // ------------------------------------------------------------------
     vector<vector<int>> caso2 = {
          { 0, 11, 20, 15, 32, 38},
          {11,  0, 13, 18, 21, 29},
          {20, 13,  0, 13, 16, 18},
          {15, 18, 13,  0, 30, 29},
          {32, 21, 16, 30,  0, 14},
          {38, 29, 18, 29, 14,  0}
     };
     probar("Caso 2: 6 ciudades (A-F)", caso2, 92);

     // ------------------------------------------------------------------
     // Caso 3: 5 nodos donde el greedy se equivoca bastante
     //
     // Aristas ordenadas (empates se rompen por (u, v)):
     //   0-1(1) acepta, 0-4(2) acepta -> 0 con grado 2
     //   1-4(2) rechaza por ciclo (0,1,4 ya están en el mismo fragmento)
     //   0-2(3) rechaza por grado, 3-4(3) acepta -> 4 con grado 2
     //   0-3(5) rechaza por grado, 2-4(5) rechaza por grado
     //   1-3(7) rechaza por ciclo
     //   1-2(8) acepta            -> ya hay 4 aristas
     //   Extremos con grado 1: 2 y 3 -> cierre con 2-3 (8)
     //
     // Greedy esperado: 0 1 2 3 4 -> 0   costo 1 + 8 + 8 + 3 + 2 = 22
     // Óptimo:          0 1 4 3 2 -> 0   costo 1 + 2 + 3 + 8 + 3 = 17
     // (el greedy queda ~29% arriba: dos aristas de peso 8 al final)
     // ------------------------------------------------------------------
     vector<vector<int>> caso3 = {
          {0, 1, 3, 5, 2},
          {1, 0, 8, 7, 2},
          {3, 8, 0, 8, 5},
          {5, 7, 8, 0, 3},
          {2, 2, 5, 3, 0}
     };
     probar("Caso 3: 5 nodos (greedy no optimo)", caso3, 17);

     return 0;
}