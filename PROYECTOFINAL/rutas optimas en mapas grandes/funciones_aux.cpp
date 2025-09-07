#include "funciones_aux.h"
#include "Medidor.hpp"
void agregarObstaculosDesdeArchivo(const std::string& archivoGrafo, const std::string& archivoObstaculos, float porcentajeBloqueo) {
    srand(time(0));
    std::ifstream fin(archivoGrafo);
    std::ofstream fout("grafo_temp.txt");
    std::ofstream foutObstaculos(archivoObstaculos);

    if (!fin.is_open() || !fout.is_open() || !foutObstaculos.is_open()) {
        std::cout << "Error al abrir archivos.\n";
        return;
    }

    int n, m;
    fin >> n >> m;
    fout << n << " " << m << "\n";

    std::string linea;
    std::getline(fin, linea); // Consumir fin de linea despues de m

    while (std::getline(fin, linea)) {
        if (linea.empty()) continue;

        // Decidir aleatoriamente si esta arista sera un obstaculo
        if ((rand() % 100) < (porcentajeBloqueo * 100)) {
            // Es un obstaculo, guardala en obstaculos.txt
            foutObstaculos << linea << "\n";
        } else {
            // No es un obstaculo, mantén la arista en el nuevo grafo
            fout << linea << "\n";
        }
    }

    fin.close();
    fout.close();
    foutObstaculos.close();

    // Sobreescribe el archivo grafo.txt con el nuevo grafo reducido
    std::remove(archivoGrafo.c_str());
    std::rename("grafo_temp.txt", archivoGrafo.c_str());
    std::cout << "Obstaculos generados y grafo reducido.\n";
}

void guardarGrafoEnArchivo(const GrafoGrande& grafo, const std::string& nombreArchivo) {
    std::ofstream fout(nombreArchivo);

    int totalAristas = 0;
    for (int u = 0; u < grafo.cantidadNodos(); ++u) {
        const Vector<Arista>& vecinos = grafo.vecinosDe(u);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            if (u < v)
                totalAristas++;
        }
    }

    fout << grafo.cantidadNodos() << " " << totalAristas << "\n";

    for (int u = 0; u < grafo.cantidadNodos(); ++u) {
        const Vector<Arista>& vecinos = grafo.vecinosDe(u);
        for (int i = 0; i < vecinos.size(); ++i) {
            int v = vecinos[i].destino;
            int peso = vecinos[i].peso;

            if (u < v)
                fout << u << " " << v << " " << peso << "\n";
        }
    }

    fout.close();
}

//almacenar un camino en un archivo
void guardarCaminoEnArchivo(const Vector<int>& camino, const std::string& nombreArchivo) {
    std::ofstream fout(nombreArchivo);
    if (!fout.is_open()) {
        std::cout << "Error al abrir " << nombreArchivo << " para escritura.\n";
        return;
    }

    for (int i = 0; i < camino.size(); ++i) {
        fout << camino[i] << "\n";
    }

    fout.close();
    std::cout << "Camino guardado en " << nombreArchivo << "\n";
}

//Ejecucion secuencial y medicion del tiempo de c/u
void ejecutarTodosLosAlgoritmos(const GrafoGrande& grafoOriginal, const Vector<std::pair<float, float>>& posiciones, int origen, int destino, bool mostrarGraficos) {
    GrafoGrande grafo;
    Vector<int> camino;
    int distancia;

    auto ejecutar = [&](const std::string& nombre, int tipo) {
        grafo = grafoOriginal;
        camino.clear();
        distancia = -1;

        auto inicio = std::chrono::high_resolution_clock::now();

        if (tipo == 1)
            Algoritmos::dijkstra(grafo, origen, destino, camino, distancia);
        else if (tipo == 2)
            Algoritmos::a_estrella(grafo, origen, destino, posiciones, camino, distancia);
        else if (tipo == 3) {
            bool encontrado = false;
            Algoritmos::dfs(grafo, origen, destino, camino, encontrado);
        }
        else if (tipo == 4) {
            bool encontrado = false;
            Algoritmos::bfs(grafo, origen, destino, camino, encontrado);
        }
        else if (tipo == 5) {
            bool encontrado = false;
            Algoritmos::best_first_search(grafo, origen, destino, posiciones, camino, encontrado);
        }

        auto fin = std::chrono::high_resolution_clock::now();
        double tiempoMs = std::chrono::duration<double, std::milli>(fin - inicio).count();

        std::cout << "Tiempo" << nombre << ": " << tiempoMs << " ms" << std::endl;

        if (mostrarGraficos) {
            std::string archivo;
            if (tipo == 1) archivo = "camino_dijkstra.txt";
            else if (tipo == 2) archivo = "camino_astar.txt";
            else if (tipo == 3) archivo = "camino_dfs.txt";
            else if (tipo == 4) archivo = "camino_bfs.txt";
            else if (tipo == 5) archivo = "camino_best.txt";

            guardarCaminoEnArchivo(camino, archivo);
        }
    };

    ejecutar("Dijkstra", 1);
    ejecutar("A*", 2);
    ejecutar("DFS", 3);
    ejecutar("BFS", 4);
    ejecutar("Best First", 5);
}

//medir el rendimiento (funcion enviada para el hilo)
void medirAlgoritmoHilo(int tipo, const GrafoGrande& grafoOriginal, 
                    const Vector<std::pair<float, float>>& posiciones, 
                    int origen, int destino, Estadisticas& stats) {
    GrafoGrande grafo = grafoOriginal;
    Vector<int> camino;
    int distancia = -1;

    auto inicioBusqueda = std::chrono::high_resolution_clock::now();

    if (tipo == 1) {
        stats.nombre = "Dijkstra";
        Algoritmos::dijkstra(grafo, origen, destino, camino, distancia);
    } else if (tipo == 2) {
        stats.nombre = "A*";
        Algoritmos::a_estrella(grafo, origen, destino, posiciones, camino, distancia);
    } else if (tipo == 3) {
        stats.nombre = "DFS";
        bool encontrado = false;
        Algoritmos::dfs(grafo, origen, destino, camino, encontrado);
    } else if (tipo == 4) {
        stats.nombre = "BFS";
        bool encontrado = false;
        Algoritmos::bfs(grafo, origen, destino, camino, encontrado);
    } else if (tipo == 5) {
        stats.nombre = "Best First";
        bool encontrado = false;
        Algoritmos::best_first_search(grafo, origen, destino, posiciones, camino, encontrado);
    }

    auto finBusqueda = std::chrono::high_resolution_clock::now();
    stats.tiempoBusqueda = std::chrono::duration<double, std::milli>(finBusqueda - inicioBusqueda).count();

    stats.longitudCamino = camino.size();
    stats.memoriaUsadaBytes = grafo.cantidadNodos() * sizeof(int);  // Aproximado
}

//uso de hilos para la evaluacion de algoritmos
void compararAlgoritmosConHilos(const GrafoGrande& grafoOriginal, 
                                 const Vector<std::pair<float, float>>& posiciones,
                                 int origen, int destino) {
    Estadisticas resultados[5];
    std::thread hilos[5];

    for (int i = 0; i < 5; ++i) {
        hilos[i] = std::thread(medirAlgoritmoHilo, i+1, std::ref(grafoOriginal),
                               std::ref(posiciones), origen, destino, std::ref(resultados[i]));
    }

    for (int i = 0; i < 5; ++i)
        hilos[i].join();

    std::cout << "\n=== REPORTE COMPARATIVO ===\n";
    for (int i = 0; i < 5; ++i) {
        auto& r = resultados[i];
        std::cout << "\nAlgoritmo: " << r.nombre << "\n";
        std::cout << "Tiempo de busqueda: " << r.tiempoBusqueda << " ms\n";
        std::cout << "Tamano del camino: " << r.longitudCamino << " nodos\n";
        std::cout << "Memoria usada estimada: " << r.memoriaUsadaBytes / 1024 << " KB\n";
    }
    std::cout << "============================\n";
}

void medirTodosLosAlgoritmos(const GrafoGrande& grafoOriginal,
                             const Vector<std::pair<float, float>>& posiciones,
                             int origen, int destino) {
    GrafoGrande grafo;
    Vector<int> camino;
    int distancia;

    auto medir = [&](const std::string& nombre, int tipo) {
        grafo = grafoOriginal;
        camino.clear();
        distancia = -1;

        // Usamos Medidor.hpp para tiempo y memoria reales
        medirAlgoritmo(nombre, [&]() {
            if (tipo == 1)
                Algoritmos::dijkstra(grafo, origen, destino, camino, distancia);
            else if (tipo == 2)
                Algoritmos::a_estrella(grafo, origen, destino, posiciones, camino, distancia);
            else if (tipo == 3) {
                bool encontrado = false;
                Algoritmos::dfs(grafo, origen, destino, camino, encontrado);
            }
            else if (tipo == 4) {
                bool encontrado = false;
                Algoritmos::bfs(grafo, origen, destino, camino, encontrado);
            }
            else if (tipo == 5) {
                bool encontrado = false;
                Algoritmos::best_first_search(grafo, origen, destino, posiciones, camino, encontrado);
            }
        });

        // Mostrar tamaño del camino encontrado
        std::cout << "  Tamano del camino: " << camino.size() << " nodos\n";

        // Guardar el camino para verificación (opcional)
        std::string archivo;
        if (tipo == 1) archivo = "camino_dijkstra.txt";
        else if (tipo == 2) archivo = "camino_astar.txt";
        else if (tipo == 3) archivo = "camino_dfs.txt";
        else if (tipo == 4) archivo = "camino_bfs.txt";
        else if (tipo == 5) archivo = "camino_best.txt";

        guardarCaminoEnArchivo(camino, archivo);
    };

    std::cout << "\n=== METRICAS TIEMPO, MEMORIA Y CAMINO ===\n";
    medir("Dijkstra", 1);
    medir("A*", 2);
    medir("DFS", 3);
    medir("BFS", 4);
    medir("Best First", 5);
    std::cout << "=========================================\n";
}