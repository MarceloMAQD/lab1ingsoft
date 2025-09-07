#ifndef FUNCIONES_AUX_H
#define FUNCIONES_AUX_H

#include "Grafogrande.h"
#include "Algoritmos.h"
#include "Estructuras/Vector.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

struct Estadisticas {
    std::string nombre;
    double tiempoConstruccion = 0;
    double tiempoBusqueda = 0;
    int longitudCamino = 0;
    size_t memoriaUsadaBytes = 0;
};


void agregarObstaculosDesdeArchivo(const std::string& archivoGrafo, const std::string& archivoObstaculos, float porcentajeBloqueo);
void guardarGrafoEnArchivo(const GrafoGrande& grafo, const std::string& nombreArchivo);
void guardarCaminoEnArchivo(const Vector<int>& camino, const std::string& nombreArchivo);
void ejecutarTodosLosAlgoritmos(const GrafoGrande& grafoOriginal, const Vector<std::pair<float, float>>& posiciones, int origen, int destino, bool mostrarGraficos);
void medirAlgoritmoHilo(int tipo, const GrafoGrande& grafoOriginal, const Vector<std::pair<float, float>>& posiciones, int origen, int destino, Estadisticas& stats);
void compararAlgoritmosConHilos(const GrafoGrande& grafoOriginal, const Vector<std::pair<float, float>>& posiciones, int origen, int destino);
void medirTodosLosAlgoritmos(const GrafoGrande& grafoOriginal, const Vector<std::pair<float, float>>& posiciones, int origen, int destino);


#endif
