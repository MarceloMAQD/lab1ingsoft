#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue {
private:
    T* datos;
    int frente, fondo, capacidad;

    void redimensionar();

public:
    Queue();
    ~Queue();

    void push(const T& valor);
    void pop();
    T& front();
    bool empty() const;
    int size() const;
};

template <typename T>
Queue<T>::Queue() {
    capacidad = 8;
    frente = fondo = 0;
    datos = new T[capacidad];
}

template <typename T>
Queue<T>::~Queue() {
    delete[] datos;
}

template <typename T>
void Queue<T>::redimensionar() {
    int nuevoTam = capacidad * 2;
    T* nuevo = new T[nuevoTam];
    int tam = size();
    for (int i = 0; i < tam; ++i)
        nuevo[i] = datos[(frente + i) % capacidad];
    delete[] datos;
    datos = nuevo;
    frente = 0;
    fondo = tam;
    capacidad = nuevoTam;
}

template <typename T>
void Queue<T>::push(const T& valor) {
    if ((fondo + 1) % capacidad == frente)
        redimensionar();
    datos[fondo] = valor;
    fondo = (fondo + 1) % capacidad;
}

template <typename T>
void Queue<T>::pop() {
    if (!empty())
        frente = (frente + 1) % capacidad;
}

template <typename T>
T& Queue<T>::front() {
    return datos[frente];
}

template <typename T>
bool Queue<T>::empty() const {
    return frente == fondo;
}

template <typename T>
int Queue<T>::size() const {
    return (fondo - frente + capacidad) % capacidad;
}

#endif
