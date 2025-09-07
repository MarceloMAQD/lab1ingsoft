#include "CargadorGrafo.h"
#include <fstream>
#include <iostream>

bool CargadorGrafo::cargarGrafo(GrafoGrande& grafo, const std::string& archivoGrafo) {
    std::ifstream fin(archivoGrafo);
    if (!fin.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de grafo: " << archivoGrafo << std::endl;
        return false;
    }

    int n, m;
    fin >> n >> m;
    grafo.inicializar(n);

    int u, v, peso;
    while (fin >> u >> v >> peso) {
        grafo.agregarArista(u, v, peso);
    }
    fin.close();
    std::cout << "Grafo cargado con " << n << " nodos y " << m << " aristas." << std::endl;
    return true;
}

bool CargadorGrafo::cargarPosiciones(Vector<std::pair<float, float>>& posiciones, int numNodos, const std::string& archivoPosiciones) {
    std::ifstream fin(archivoPosiciones);
    if (!fin.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo de posiciones: " << archivoPosiciones << std::endl;
        return false;
    }

    posiciones.inicializar(numNodos);
    for(int i=0; i<numNodos; ++i) {
        posiciones.push_back({0.0f, 0.0f}); // Inicializar con valores por defecto
    }

    int id;
    float lon, lat;
    while (fin >> id >> lon >> lat) {
        if (id >= 0 && id < numNodos) {
            posiciones[id] = {lon, lat};
        }
    }
    fin.close();
    std::cout << "Posiciones de nodos cargadas." << std::endl;
    return true;
}