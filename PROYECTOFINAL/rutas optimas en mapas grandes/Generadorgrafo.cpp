#include "Generadorgrafo.h" 
#include <ctime>  
#include <fstream>
using namespace std;

void GeneradorGrafo::generarRejilla(GrafoGrande& grafo, Vector<pair<float, float>>& posiciones,int rows, int cols, int pesoMin, int pesoMax) {
    int totalNodos = rows * cols;
    grafo.inicializar(totalNodos);
    posiciones.inicializar(totalNodos);

    srand(time(0));

    
    for (int fila = 0; fila < rows; ++fila) {
        for (int col = 0; col < cols; ++col) {
            int nodoActual = fila * cols + col;
            //coordenadas (x,y)
            float x = static_cast<float>(col);
            float y = static_cast<float>(fila);

            posiciones[nodoActual] = {x, y};

            //Conectar con el vecino de la derecha
            if (col + 1 < cols) {
                int vecino = fila * cols + (col + 1);
                int peso = pesoMin + rand() % (pesoMax - pesoMin + 1);
                grafo.agregarArista(nodoActual, vecino, peso);
            }
            //Conectar con el vecino de abajo
            if (fila + 1 < rows) {
                int vecino = (fila + 1) * cols + col;
                int peso = pesoMin + rand() % (pesoMax - pesoMin + 1);
                grafo.agregarArista(nodoActual, vecino, peso);
            }
        }
    }
}

//conexion al azar
void GeneradorGrafo::generarGrafoAleatorio(GrafoGrande& grafo, Vector<pair<float, float>>& posiciones, int totalNodos, int pesoMin, int pesoMax, int maxConexiones) {
    grafo.inicializar(totalNodos);
    posiciones.inicializar(totalNodos);

    srand(time(0));

    // Asignar posiciones aleatorias (coordenadas espaciales)
    for (int i = 0; i < totalNodos; ++i) {
        float x = static_cast<float>(rand() % 1000);
        float y = static_cast<float>(rand() % 1000);
        posiciones[i] = {x, y};
    }

    // Conectar los nodos entre si
    for (int u = 0; u < totalNodos; ++u) {
        //conexiones de cada nodo 'u'
        int conexiones = rand() % (maxConexiones + 1);

        for (int c = 0; c < conexiones; ++c) {
            //Eligir un nodo v para conectar con u 
            int v = rand() % totalNodos;
            if (v != u) {
                int peso = pesoMin + rand() % (pesoMax - pesoMin + 1);
                grafo.agregarArista(u, v, peso);
            }
        }
    }
}

