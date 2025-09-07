#ifndef STACK_H
#define STACK_H

#include "Vector.h"

template <typename T>
class Stack {
private:
    Vector<T> datos;

public:
    Stack() {}

    bool empty() const {
        return datos.empty();
    }

    int size() const {
        return datos.size();
    }

    void push(const T& valor) {
        datos.push_back(valor);
    }

    void pop() {
        if (!empty()) {
            datos.pop_back();
        }
    }

    T& top() {
        return datos[datos.size() - 1];
    }

    const T& top() const {
        return datos[datos.size() - 1];
    }

    void clear() {
        datos.clear();
    }
};

#endif
