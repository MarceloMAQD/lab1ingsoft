#include "Core/Grafogrande.h"
#include "Core/Algoritmos.h"
#include "Core/CargadorGrafo.h"
#include "Estructuras/Vector.h"
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

void guardarCaminoEnArchivo(const Vector<int>& camino, const string& nombreArchivo);
// Modificada para devolver el origen y destino seleccionados
// Retorna {-1, -1} si la selección es cancelada o hay un error
pair<int, int> ejecutarVisualizador(const string& extras = "", bool interactive = false);
void agregarObstaculos(const string& archivoOriginal, const string& archivoDestino, const string& archivoObstaculos, float porcentaje);

int main() {
    GrafoGrande grafo;
    Vector<pair<float, float>> posiciones;

    //Cargar el grafo y las posiciones de Arequipa
    cout << "--- Cargando Mapa de Arequipa ---" << endl;
    if (!CargadorGrafo::cargarGrafo(grafo, "arequipa_grafo.txt")) return 1;
    if (!CargadorGrafo::cargarPosiciones(posiciones, grafo.cantidadNodos(), "arequipa_posiciones.txt")) return 1;

    //Pedir origen y destino - Ahora con opción de selección gráfica
    int origen, destino;
    cout << "\nSeleccione el metodo para ingresar origen y destino:" << endl;
    cout << "1. Ingreso por consola (IDs de nodo)" << endl;
    cout << "2. Seleccion grafica en el mapa" << endl;
    int opcion_ingreso;
    cin >> opcion_ingreso;

    if (opcion_ingreso == 2) {
        cout << "\nIniciando seleccion grafica. Por favor, haga clic en el mapa para seleccionar el origen y luego el destino." << endl;
        pair<int, int> seleccion = ejecutarVisualizador("", true); // Llamar en modo interactivo
        origen = seleccion.first;
        destino = seleccion.second;

        if (origen == -1 || destino == -1) {
            cout << "Seleccion grafica cancelada o incompleta. Saliendo." << endl;
            return 1;
        }
        cout << "Origen seleccionado: " << origen << ", Destino seleccionado: " << destino << endl;

    } else { // Por defecto, entrada por consola
        cout << "\nEl mapa tiene " << grafo.cantidadNodos() << " nodos (de 0 a " << grafo.cantidadNodos() - 1 << ")." << endl;
        cout << "Ingrese el nodo de origen: ";
        cin >> origen;
        cout << "Ingrese el nodo de destino: ";
        cin >> destino;
    }

    // Validación de entrada para origen y destino
    if (origen < 0 || origen >= grafo.cantidadNodos() || destino < 0 || destino >= grafo.cantidadNodos()) {
        cout << "Error: Origen o destino fuera de rango." << endl;
        return 1;
    }


    //Pedir el algoritmo a usar
    cout << "\nSeleccione el algoritmo de busqueda:" << endl;
    cout << "1. Dijkstra" << endl;
    cout << "2. A* (A-Star)" << endl;
    cout << "3. BFS (Busqueda en Anchura)" << endl;
    cout << "4. DFS (Busqueda en Profundidad)" << endl;
    cout << "5. Best First Search" << endl;
    int opcion_algo;
    cin >> opcion_algo;

    Vector<int> camino;
    int distancia = 0;
    bool encontrado = false;

    switch (opcion_algo) {
        case 1:
            Algoritmos::dijkstra(grafo, origen, destino, camino, distancia);
            break;
        case 2:
            Algoritmos::a_estrella(grafo, origen, destino, posiciones, camino, distancia);
            break;
        case 3:
            Algoritmos::bfs(grafo, origen, destino, camino, encontrado);
            break;
        case 4:
            Algoritmos::dfs(grafo, origen, destino, camino, encontrado);
            break;
        case 5:
            Algoritmos::best_first_search(grafo, origen, destino, posiciones, camino, encontrado);
            break;
        default:
            cout << "Opcion invalida." << endl;
            return 1;
    }

    if (camino.empty()) {
        cout << "\nNo se encontro un camino entre " << origen << " y " << destino << "." << endl;
    } else {
        cout << "\nCamino encontrado! Tiene " << camino.size() << " nodos." << endl;
        guardarCaminoEnArchivo(camino, "camino_calculado.txt");

        //Mostrar el mapa con la ruta inicial
        cout << "Mostrando visualizacion del mapa y la ruta..." << endl;
        ejecutarVisualizador(); //Llamar en modo no interactivo
    }

    //Preguntar si se desean agregar obstáculos
    cout << "\nDesea agregar obstaculos aleatorios y recalcular la ruta? (1=Si, 0=No): ";
    int opcion_obs;
    cin >> opcion_obs;

    if (opcion_obs == 1) {
        float porcentaje;
        cout << "Que porcentaje de calles desea bloquear? (ej. 0.1 para 10%): ";
        cin >> porcentaje;

        // Genera un nuevo grafo con aristas bloqueadas y un archivo de obstáculos
        agregarObstaculos("arequipa_grafo.txt", "grafo_con_obstaculos.txt", "obstaculos.txt", porcentaje);

        // Recargamos el grafo con los obstáculos
        GrafoGrande grafo_con_obstaculos;
        CargadorGrafo::cargarGrafo(grafo_con_obstaculos, "grafo_con_obstaculos.txt");

        camino.clear();
        distancia = 0;
        encontrado = false;

        cout << "\nRecalculando la ruta con los nuevos obstaculos..." << endl;
        
        switch (opcion_algo) {
            case 1: Algoritmos::dijkstra(grafo_con_obstaculos, origen, destino, camino, distancia); break;
            case 2: Algoritmos::a_estrella(grafo_con_obstaculos, origen, destino, posiciones, camino, distancia); break;
            case 3: Algoritmos::bfs(grafo_con_obstaculos, origen, destino, camino, encontrado); break;
            case 4: Algoritmos::dfs(grafo_con_obstaculos, origen, destino, camino, encontrado); break;
            case 5: Algoritmos::best_first_search(grafo_con_obstaculos, origen, destino, posiciones, camino, encontrado); break;
        }

        if (camino.empty()) {
            cout << "\nNo se encontro una ruta con los obstaculos actuales." << endl;
        } else {
            cout << "\nNueva ruta encontrada con obstaculos!" << endl;
            guardarCaminoEnArchivo(camino, "camino_calculado.txt");
            
            // 6. Mostrar el mapa con la nueva ruta y los obstáculos
            cout << "Mostrando nueva visualizacion..." << endl;
            ejecutarVisualizador("con_obstaculos");
        }
    }

    cout << "\n--- Fin del Programa ---" << endl;
    return 0;
}

void guardarCaminoEnArchivo(const Vector<int>& camino, const string& nombreArchivo) {
    ofstream fout(nombreArchivo);
    for (int i = 0; i < camino.size(); ++i) {
        fout << camino[i] << "\n";
    }
    fout.close();
}

pair<int, int> ejecutarVisualizador(const string& extras, bool interactive) {
    string command = "python visualizar.py --pois arequipa_pois.txt";
    if (extras == "con_obstaculos") {
        command += " --obstaculos obstaculos.txt";
    }
    if (interactive) {
        command += " --interactive";
        
        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            cerr << "Error: No se pudo ejecutar el visualizador." << endl;
            return {-1, -1};
        }

        char buffer[128];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
        _pclose(pipe);

        size_t found = result.find("SELECTED_NODES:");
        if (found != string::npos) {
            string nodes_str = result.substr(found + 15);
            int origin_node, dest_node;
            if (sscanf(nodes_str.c_str(), "%d %d", &origin_node, &dest_node) == 2) {
                return {origin_node, dest_node};
            }
        }
        return {-1, -1};
    } else {
        system(command.c_str());
        return { -1, -1 };
    }
}

void agregarObstaculos(const string& archivoOriginal, const string& archivoDestino, const string& archivoObstaculos, float porcentaje) {
    srand(time(0));
    ifstream fin(archivoOriginal);
    ofstream fout(archivoDestino);
    ofstream foutObstaculos(archivoObstaculos);

    int n, m;
    fin >> n >> m;
    fout << n << " " << m << "\n";

    int u, v, peso;
    while (fin >> u >> v >> peso) {
        if ((rand() % 100) / 100.0f < porcentaje) {
            foutObstaculos << u << " " << v << "\n";
        } else {
            fout << u << " " << v << " " << peso << "\n";
        }
    }
    fin.close();
    fout.close();
    foutObstaculos.close();
    cout << "Obstaculos generados en '" << archivoObstaculos << "'." << endl;
}
