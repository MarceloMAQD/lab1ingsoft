#ifndef MINHEAP_H
#define MINHEAP_H

template <typename T>
class MinHeap {
private:
    T* datos;
    int capacidad;
    int tam;

    void subir(int);
    void bajar(int);
    void intercambiar(int, int);
    void redimensionar();

public:
    MinHeap();
    ~MinHeap();

    void push(const T& valor);
    void pop();
    T& top();
    bool empty() const;
    int size() const;
};

template <typename T>
MinHeap<T>::MinHeap() {
    capacidad = 8;
    tam = 0;
    datos = new T[capacidad];
}

template <typename T>
MinHeap<T>::~MinHeap() {
    delete[] datos;
}

template <typename T>
void MinHeap<T>::redimensionar() {
    capacidad *= 2;
    T* nuevo = new T[capacidad];
    for (int i = 0; i < tam; ++i)
        nuevo[i] = datos[i];
    delete[] datos;
    datos = nuevo;
}

template <typename T>
void MinHeap<T>::intercambiar(int i, int j) {
    T tmp = datos[i];
    datos[i] = datos[j];
    datos[j] = tmp;
}

template <typename T>
void MinHeap<T>::subir(int i) {
    while (i > 0 && datos[i] < datos[(i - 1) / 2]) {
        intercambiar(i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

template <typename T>
void MinHeap<T>::bajar(int i) {
    int menor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;
    if (izq < tam && datos[izq] < datos[menor]) menor = izq;
    if (der < tam && datos[der] < datos[menor]) menor = der;
    if (menor != i) {
        intercambiar(i, menor);
        bajar(menor);
    }
}

template <typename T>
void MinHeap<T>::push(const T& valor) {
    if (tam == capacidad)
        redimensionar();
    datos[tam] = valor;
    subir(tam);
    tam++;
}

template <typename T>
void MinHeap<T>::pop() {
    if (tam == 0) return;
    datos[0] = datos[--tam];
    bajar(0);
}

template <typename T>
T& MinHeap<T>::top() {
    return datos[0];
}

template <typename T>
bool MinHeap<T>::empty() const {
    return tam == 0;
}

template <typename T>
int MinHeap<T>::size() const {
    return tam;
}

#endif

