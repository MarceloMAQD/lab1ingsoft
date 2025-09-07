#include "Algoritmos.h"
#include "../Estructuras/MinHeap.h"
#include "../Estructuras/Vector.h"
#include "../Estructuras/Stack.h"
#include "../Estructuras/Queue.h"
#include <math.h>

struct Par {
    int distancia;
    int nodo;

    bool operator<(const Par& otro) const {
        return distancia < otro.distancia;
    }
};

//usa minheap
void Algoritmos::dijkstra(const GrafoGrande& grafo, int origen, int destino, Vector<int>& camino, int& distanciaTotal) {
    int n = grafo.cantidadNodos();
    Vector<int> distancias;
    Vector<int> previo;

    distancias.inicializar(n);
    previo.inicializar(n);
    for (int i = 0; i < n; ++i) {
        distancias.push_back(1e9);  // INF
        previo.push_back(-1);
    }

    distancias[origen] = 0;
    MinHeap<Par> heap;
    heap.push({0, origen});

    while (!heap.empty()) {
        Par actual = heap.top();
        heap.pop();
        int u = actual.nodo;

        if (u == destino)
            break;

        const Vector<Arista>& vecinos = grafo.vecinosDe(u);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            int peso = vecinos[i].peso;
            if (distancias[u] + peso < distancias[v]) {
                distancias[v] = distancias[u] + peso;
                previo[v] = u;
                heap.push({distancias[v], v});
            }
        }
    }

    // Reconstruir camino
    camino.clear();
    if (distancias[destino] == 1e9) {
        distanciaTotal = -1;
        return; // No hay camino
    }

    for (int u = destino; u != -1; u = previo[u])
        camino.push_back(u);

    // Invertir camino manualmente (ya que no tenemos reverse)
    for (int i = 0, j = camino.size() - 1; i < j; ++i, --j) {
        int tmp = camino[i];
        camino[i] = camino[j];
        camino[j] = tmp;
    }

    distanciaTotal = distancias[destino];
}

//usa minheap
void Algoritmos::a_estrella(const GrafoGrande& grafo, int origen, int destino, const Vector<std::pair<float, float>>& posiciones, Vector<int>& camino, int& distanciaTotal) {
    const int n = grafo.cantidadNodos();

    Vector<int> previo;
    previo.inicializar(n);
    for (int i = 0; i < n; ++i)
        previo[i] = -1;

    Vector<int> distancia;
    distancia.inicializar(n);
    for (int i = 0; i < n; ++i)
        distancia[i] = 1e9;  // INF

    Vector<bool> procesado;
    procesado.inicializar(n);
    for (int i = 0; i < n; ++i)
        procesado[i] = false;

    distancia[origen] = 0;

    //Manhattan (rejillas)
    auto heuristica = [&](int nodo) -> float {
        auto [x1, y1] = posiciones[nodo];
        auto [x2, y2] = posiciones[destino];
        float dx = x1 - x2;
        float dy = y1 - y2;
        return abs(x1 - x2) + abs(y1 - y2);
    };

    using Par = std::pair<float, int>;  // (f(n), nodo)
    MinHeap<Par> heap;
    heap.push({heuristica(origen), origen});

    while (!heap.empty()) {
        int u = heap.top().second;
        heap.pop();

        if (procesado[u])
            continue;

        procesado[u] = true;

        if (u == destino)
            break;

        const Vector<Arista>& vecinos = grafo.vecinosDe(u);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            int peso = vecinos[i].peso;

            if (distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                previo[v] = u;
                float prioridad = distancia[v] + heuristica(v);
                heap.push({prioridad, v});
            }
        }
    }

    // Reconstruir camino
    camino.clear();
    distanciaTotal = distancia[destino];

    if (distancia[destino] >= 1e9)
        return;  // No hay camino

    for (int u = destino; u != -1; u = previo[u])
        camino.push_back(u);

    for (int i = 0, j = camino.size() - 1; i < j; ++i, --j)
        std::swap(camino[i], camino[j]);
}

//usa stack
void Algoritmos::dfs(const GrafoGrande& grafo, int origen, int destino, Vector<int>& camino, bool& encontrado) {
    const int n = grafo.cantidadNodos();
    Vector<bool> visitado;
    visitado.inicializar(n);
    for (int i = 0; i < n; ++i)
        visitado[i] = false;

    Stack<int> pila;
    Vector<int> previo;
    previo.inicializar(n);
    for (int i = 0; i < n; ++i)
        previo[i] = -1;

    pila.push(origen);

    encontrado = false;

    while (!pila.empty()) {
        int nodo = pila.top();
        pila.pop();

        if (visitado[nodo])
            continue;
        visitado[nodo] = true;

        if (nodo == destino) {
            encontrado = true;
            break;
        }

        const Vector<Arista>& vecinos = grafo.vecinosDe(nodo);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            if (!visitado[v]) {
                pila.push(v);
                previo[v] = nodo;
            }
        }
    }

    // Reconstruir camino (si existe)
    camino.clear();
    if (encontrado) {
        for (int u = destino; u != -1; u = previo[u])
            camino.push_back(u);

        for (int i = 0, j = camino.size() - 1; i < j; ++i, --j)
            std::swap(camino[i], camino[j]);
    }
}

//usa queue
void Algoritmos::bfs(const GrafoGrande& grafo, int origen, int destino, Vector<int>& camino, bool& encontrado) {
    const int n = grafo.cantidadNodos();
    Vector<bool> visitado;
    visitado.inicializar(n);
    for (int i = 0; i < n; ++i)
        visitado[i] = false;

    Queue<int> cola;
    Vector<int> previo;
    previo.inicializar(n);
    for (int i = 0; i < n; ++i)
        previo[i] = -1;

    cola.push(origen);
    visitado[origen] = true;
    encontrado = false;

    while (!cola.empty()) {
        int nodo = cola.front();
        cola.pop();

        if (nodo == destino) {
            encontrado = true;
            break;
        }

        const Vector<Arista>& vecinos = grafo.vecinosDe(nodo);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            if (!visitado[v]) {
                visitado[v] = true;
                previo[v] = nodo;
                cola.push(v);
            }
        }
    }

    // Reconstruir camino si existe
    camino.clear();
    if (encontrado) {
        for (int u = destino; u != -1; u = previo[u])
            camino.push_back(u);

        for (int i = 0, j = camino.size() - 1; i < j; ++i, --j)
            std::swap(camino[i], camino[j]);
    }
}

//usa mminheap
void Algoritmos::best_first_search(const GrafoGrande& grafo, int origen, int destino, const Vector<std::pair<float, float>>& posiciones, Vector<int>& camino, bool& encontrado) {
    const int n = grafo.cantidadNodos();
    Vector<bool> visitado;
    visitado.inicializar(n);
    for (int i = 0; i < n; ++i)
        visitado[i] = false;

    MinHeap<std::pair<float, int>> heap; // (heurística, nodo)
    Vector<int> previo;
    previo.inicializar(n);
    for (int i = 0; i < n; ++i)
        previo[i] = -1;

    auto heuristica = [&](int nodo) {
        float dx = abs(posiciones[nodo].first  - posiciones[destino].first);
        float dy = abs(posiciones[nodo].second - posiciones[destino].second);
        return dx + dy;
    };

    heap.push({heuristica(origen), origen});
    encontrado = false;

    while (!heap.empty()) {
        int nodo = heap.top().second;
        heap.pop();

        if (visitado[nodo])
            continue;
        visitado[nodo] = true;

        if (nodo == destino) {
            encontrado = true;
            break;
        }

        const Vector<Arista>& vecinos = grafo.vecinosDe(nodo);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            if (!visitado[v]) {
                heap.push({heuristica(v), v});
                previo[v] = nodo;
            }
        }
    }

    // Reconstruir camino
    camino.clear();
    if (encontrado) {
        for (int u = destino; u != -1; u = previo[u])
            camino.push_back(u);

        for (int i = 0, j = camino.size() - 1; i < j; ++i, --j)
            std::swap(camino[i], camino[j]);
    }
}