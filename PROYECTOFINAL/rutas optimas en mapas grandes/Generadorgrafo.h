#ifndef GENERADORGRAFO_H
#define GENERADORGRAFO_H

#include "Grafogrande.h"

class GeneradorGrafo {
public:
    static void generarRejilla(GrafoGrande& grafo, Vector<std::pair<float, float>>& posiciones ,int rows, int cols, int pesoMin = 1, int pesoMax = 10);
    static void generarGrafoAleatorio(GrafoGrande& grafo, Vector<std::pair<float, float>>& posiciones ,int totalNodos, int pesoMin, int pesoMax, int maxConexionesPorNodo);
};

#endif
