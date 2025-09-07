#include "Grafogrande.h"
#include "Generadorgrafo.h"
#include "Estructuras/Vector.h"
#include "funciones_aux.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    GrafoGrande grafo;
    Vector<pair<float, float>> posiciones;
    int modo = -1;
    
    cout << "=== PROYECTO: RUTAS OPTIMAS ===\n";
    cout << "1. Modo Visual (con graficos)\n";
    cout << "2. Modo Solo Tiempos (consola)\n";
    cout << "Elige un modo: ";
    cin >> modo;

    cout << "1. Generar grafo tipo rejilla\n";
    cout << "2. Generar grafo aleatorio general\n";
    cout << "Elige una opcion: ";
    int opcion;
    cin >> opcion;

    if (opcion == 1) {
        int filas, columnas;
        cout << "Filas: "; cin >> filas;
        cout << "Columnas: "; cin >> columnas;
        GeneradorGrafo::generarRejilla(grafo, posiciones, filas, columnas, 100, 1000);
    } else if (opcion == 2) {
        int totalNodos, maxConexiones;
        cout << "Cantidad total de nodos: "; cin >> totalNodos;
        cout << "Maximo conexiones por nodo: "; cin >> maxConexiones;
        GeneradorGrafo::generarGrafoAleatorio(grafo, posiciones, totalNodos, 100, 1000, maxConexiones);
    } else {
        cout << "Opcion invalida.\n";
        return 0;
    }

    guardarGrafoEnArchivo(grafo, "grafo.txt");
    remove("posiciones.txt");

    // Lógica principal en un bucle
    bool continuar = true;
    while (continuar) {
        int origen, destino;
        cout << "\n--- Nuevo Calculo de Ruta ---\n";
        cout << "Nodo origen: "; cin >> origen;
        cout << "Nodo destino: "; cin >> destino;

        GrafoGrande grafoOriginal;
        grafoOriginal.cargarDesdeArchivo("grafo.txt");

        if (modo == 1) {
            // Lógica para el modo visual
            remove("obstaculos.txt");
            remove("camino_dijkstra.txt");
            remove("camino_astar.txt");
            remove("camino_dfs.txt");
            remove("camino_bfs.txt");
            remove("camino_best.txt");

            cout << "Ejecutando los 5 algoritmos...\n";
            ejecutarTodosLosAlgoritmos(grafoOriginal, posiciones, origen, destino, true);

            cout << " Mostrando las 5 rutas encontradas...\n";
            system("python visualizar_5.py");

            cout << "\nDeseas agregar obstaculos y recalcular? (1=Si / 0=No): ";
            int opcionObs;
            cin >> opcionObs;

            if (opcionObs == 1) {
                float porcentaje;
                cout << "Porcentaje de aristas a bloquear (0.0 a 1.0): ";
                cin >> porcentaje;

                agregarObstaculosDesdeArchivo("grafo.txt", "obstaculos.txt", porcentaje);
                grafoOriginal.cargarDesdeArchivo("grafo.txt");

                cout << "Recalculando rutas considerando obstaculos...\n";
                ejecutarTodosLosAlgoritmos(grafoOriginal, posiciones, origen, destino, true);

                cout << " Mostrando las 5 rutas CON obstaculos...\n";
                system("python visualizar_5.py");
            }

        } else if (modo == 2) {
            // Lógica para el modo de solo tiempos
            cout << "\n=== REPORTE DE ALGORITMOS ===\n";
            cout << "1. Solo tiempos (con hilos)\n";
            cout << "2. Tiempos y memoria (secuencial)\n";
            int submodo;
            cin >> submodo;

            auto inicioGrafo = chrono::high_resolution_clock::now();
            grafoOriginal.cargarDesdeArchivo("grafo.txt");
            auto finGrafo = chrono::high_resolution_clock::now();
            double tiempoGrafoMs = chrono::duration<double, milli>(finGrafo - inicioGrafo).count();

            cout << "Tiempo de construccion del grafo: " << tiempoGrafoMs << " ms\n";

            if (submodo == 1) {
                compararAlgoritmosConHilos(grafoOriginal, posiciones, origen, destino);
            } else if (submodo == 2) {
                medirTodosLosAlgoritmos(grafoOriginal, posiciones, origen, destino);
            }

            cout << "\nDeseas agregar obstáculos y recalcular? (1=Si / 0=No): ";
            int opcionObs;
            cin >> opcionObs;

            if (opcionObs == 1) {
                float porcentaje;
                cout << "Porcentaje de aristas a bloquear (0.0 a 1.0): ";
                cin >> porcentaje;

                agregarObstaculosDesdeArchivo("grafo.txt", "obstaculos.txt", porcentaje);
                grafoOriginal.cargarDesdeArchivo("grafo.txt");

                cout << "\n--- Comparando con obstaculos ---\n";
                if (submodo == 1) {
                    compararAlgoritmosConHilos(grafoOriginal, posiciones, origen, destino);
                } else {
                    medirTodosLosAlgoritmos(grafoOriginal, posiciones, origen, destino);
                }
            }
        }
        
        cout << "\n¿Deseas calcular otra ruta en el mismo grafo? (1=Si / 0=No): ";
        int opcionContinuar;
        cin >> opcionContinuar;
        if (opcionContinuar == 0) {
            continuar = false;
        }
    }

    cout << "=== FIN DEL PROGRAMA ===\n";
    return 0;
}