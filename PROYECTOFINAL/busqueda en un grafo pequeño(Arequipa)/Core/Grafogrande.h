#ifndef GRAFOGRANDE_H
#define GRAFOGRANDE_H

#include "../Estructuras/Vector.h"

struct Arista {
    int destino;
    int peso;

    Arista() {}
    Arista(int d, int p) : destino(d), peso(p) {}
};

class GrafoGrande {
private:
    Vector<Vector<Arista>> adyacencia;
    int numNodos;
    int numAristas;

public:
    GrafoGrande();
    void inicializar(int n); // reserva espacio para n nodos
    void agregarArista(int u, int v, int peso);
    int cantidadNodos() const;
    int cantidadAristas() const;
    void cargarDesdeArchivo(const std::string& nombreArchivo);

    const Vector<Arista>& vecinosDe(int nodo) const;

};

#endif
