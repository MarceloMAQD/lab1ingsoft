#include "Grafogrande.h"
#include "../Estructuras/Vector.h"
#include <fstream>

GrafoGrande::GrafoGrande() {
    numNodos = 0;
    numAristas = 0;
}

void GrafoGrande::inicializar(int n) {
    adyacencia.clear();
    for (int i = 0; i < n; ++i) {
        Vector<Arista> lista;
        adyacencia.push_back(lista);
    }
    numNodos = n;
    numAristas = 0;
}

void GrafoGrande::agregarArista(int u, int v, int peso) {
    if (u >= numNodos || v >= numNodos) return;

    adyacencia[u].push_back(Arista(v, peso));
    adyacencia[v].push_back(Arista(u, peso)); // No dirigido
    numAristas++;
}

int GrafoGrande::cantidadNodos() const {
    return numNodos;
}

int GrafoGrande::cantidadAristas() const {
    return numAristas;
}

const Vector<Arista>& GrafoGrande::vecinosDe(int nodo) const {
    return adyacencia[nodo];
}

void GrafoGrande::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream fin(nombreArchivo);
    if (!fin.is_open()) {
        std::cout << "❌ Error al abrir " << nombreArchivo << "\n";
        return;
    }

    int n, m;
    fin >> n >> m;
    inicializar(n);

    int u, v, peso;
    while (fin >> u >> v >> peso) {
        agregarArista(u, v, peso);
    }

    fin.close();
}
