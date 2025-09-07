#ifndef CARGADOR_GRAFO_H
#define CARGADOR_GRAFO_H

#include "Grafogrande.h"
#include "../Estructuras/Vector.h"
#include <string>

class CargadorGrafo {
public:
    static bool cargarGrafo(GrafoGrande& grafo, const std::string& archivoGrafo);
    static bool cargarPosiciones(Vector<std::pair<float, float>>& posiciones, int numNodos, const std::string& archivoPosiciones);
};

#endif