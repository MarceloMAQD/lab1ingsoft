#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector {
private:
    T* datos;
    int capacidad;
    int tam;

    void redimensionar();

public:
    Vector();
    ~Vector();
    Vector(const Vector<T>& otro);
    Vector<T>& operator=(const Vector<T>& otro);
    void push_back(const T& valor);
    void pop_back();
    T& operator[](int index);
    const T& operator[](int index) const;

    int size() const;
    bool empty() const;
    void clear();
    void inicializar(int n);
};

template <typename T>
Vector<T>::Vector() {
    capacidad = 4;
    tam = 0;
    datos = new T[capacidad];
}

template <typename T>
Vector<T>::~Vector() {
    delete[] datos;
}

template <typename T>
void Vector<T>::redimensionar() {
    int nuevaCapacidad = capacidad * 2;
    T* nuevo = new T[nuevaCapacidad];
    for (int i = 0; i < tam; ++i) {
        nuevo[i] = datos[i];
    }
    delete[] datos;
    datos = nuevo;
    capacidad = nuevaCapacidad;
}

template <typename T>
void Vector<T>::push_back(const T& valor) {
    if (tam == capacidad)
        redimensionar();
    datos[tam++] = valor;
}

template <typename T>
void Vector<T>::pop_back() {
    if (tam > 0)
        tam--;
}

template <typename T>
T& Vector<T>::operator[](int index) {
    return datos[index];
}

template <typename T>
const T& Vector<T>::operator[](int index) const {
    return datos[index];
}

template <typename T>
int Vector<T>::size() const {
    return tam;
}

template <typename T>
bool Vector<T>::empty() const {
    return tam == 0;
}

template <typename T>
void Vector<T>::clear() {
    tam = 0;
}

// Inicializa el vector con una capacidad minima de n elementos
template <typename T>
void Vector<T>::inicializar(int n) {
    if (capacidad < n) {
        delete[] datos;
        capacidad = n;
        datos = new T[capacidad];
    }
    tam = 0;
}

// Constructor de copia para facilitar la copia de vectores
template <typename T>
Vector<T>::Vector(const Vector<T>& otro) {
    capacidad = otro.capacidad;
    tam = otro.tam;
    datos = new T[capacidad];
    for (int i = 0; i < tam; ++i) {
        datos[i] = otro.datos[i];
    }
}

// Operador de asignacion para facilitar la copia de vectores
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& otro) {
    if (this != &otro) {
        delete[] datos;
        capacidad = otro.capacidad;
        tam = otro.tam;
        datos = new T[capacidad];
        for (int i = 0; i < tam; ++i) {
            datos[i] = otro.datos[i];
        }
    }
    return *this;
}
#endif