#ifndef EDMONDS_KARP_H
#define EDMONDS_KARP_H

#include <queue>
#include <vector>

using namespace std;

// Calcula el flujo maximo de informacion entre dos colonias.
// source es el origen y sink es el destino.
long long edmondsKarp(const vector<vector<int>>& capacities, int source, int sink) {
    // Revisar que el origen y el destino existan.
    int n = capacities.size();
    if (source < 0 || sink < 0 || source >= n || sink >= n)
        return -1;

    // Copiar las capacidades y revisar que sean validas.
    vector<vector<long long>> residual(n, vector<long long>(n));
    for (int u = 0; u < n; ++u) {

        if (capacities[u].size() != capacities.size()) return -1;

        for (int v = 0; v < n; ++v) {
            if (capacities[u][v] < 0) return -1;
            residual[u][v] = capacities[u][v];
        }
    }

    // No hay envio si el origen y el destino son iguales.
    if (source == sink) return 0;

    // Guardar el flujo total y la colonia anterior de cada paso.
    long long maxFlow = 0;
    vector<int> parent(n);

    
    while (true) {
        // Limpiar las visitas para buscar otro camino.
        for (int i = 0; i < n; i++) {
            parent[i] = -1;
        }
        parent[source] = source; // Marcar el origen como visitado.
        queue<int> pending;
        pending.push(source); // La cola empieza con el origen.

        // Buscar con BFS un camino que tenga capacidad disponible.
        while (!pending.empty() && parent[sink] == -1) {
            const int u = pending.front();
            pending.pop();
            for (int v = 0; v < n; ++v) {
                if (parent[v] == -1 && residual[u][v] > 0) {
                    parent[v] = u;
                    pending.push(v);
                    if (v == sink) break; // Ya se encontro un camino al destino.
                }
            }
        }
        // Terminar si ya no hay un camino disponible.
        if (parent[sink] == -1) break;

        // Buscar cuanto flujo permite la conexion mas limitada del camino.
        long long increment = residual[parent[sink]][sink];
        for (int v = sink; v != source; v = parent[v]) {
            if (residual[parent[v]][v] < increment) {
                increment = residual[parent[v]][v];
            }
        }

        // Actualizar las capacidades del camino.
        for (int v = sink; v != source; v = parent[v]) {
            const int u = parent[v];
            residual[u][v] -= increment;
            // La capacidad inversa permite corregir un envio anterior.
            residual[v][u] += increment;
        }
        // Sumar lo enviado al flujo total.
        maxFlow += increment;
    }
    // Devolver el flujo maximo.
    return maxFlow;
}

#endif
