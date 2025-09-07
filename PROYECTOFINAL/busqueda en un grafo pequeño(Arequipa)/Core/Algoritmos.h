#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "Grafogrande.h"
#include "../Estructuras/Vector.h"

class Algoritmos {
public:
    static void dijkstra(const GrafoGrande& grafo, int origen, int destino, Vector<int>& camino, int& distanciaTotal);
    static void a_estrella(const GrafoGrande& grafo, int origen, int destino, const Vector<std::pair<float, float>>& posiciones, Vector<int>& camino, int& distanciaTotal);
    static void dfs(const GrafoGrande& grafo, int origen, int destino, Vector<int>& camino, bool& encontrado);
    static void bfs(const GrafoGrande& grafo, int origen, int destino, Vector<int>& camino, bool& encontrado);
    static void best_first_search(const GrafoGrande& grafo, int origen, int destino, const Vector<std::pair<float, float>>& posiciones, Vector<int>& camino, bool& encontrado);

};

#endif
