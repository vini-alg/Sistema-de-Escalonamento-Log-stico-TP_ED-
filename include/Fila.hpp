#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>

template <typename T>
struct NoFila {
    T dado;
    NoFila* proximo;
};

template <typename T>
class Fila {
private:
    NoFila<T>* frente;
    NoFila<T>* tras;
    int tamanho_atual;

public:
    Fila();
    ~Fila();

    void enfileirar(const T& elemento);
    T desenfileirar();
    bool esta_vazia() const;
    int tamanho() const;
};

template <typename T>
Fila<T>::Fila() : frente(nullptr), tras(nullptr), tamanho_atual(0) {}

template <typename T>
Fila<T>::~Fila() {
    while (!esta_vazia()) {
        desenfileirar();
    }
}

template <typename T>
void Fila<T>::enfileirar(const T& elemento) {
    NoFila<T>* novo_no = new NoFila<T>{elemento, nullptr};
    if (esta_vazia()) {
        frente = tras = novo_no;
    } else {
        tras->proximo = novo_no;
        tras = novo_no;
    }
    tamanho_atual++;
}

template <typename T>
T Fila<T>::desenfileirar() {
    if (esta_vazia()) {
        throw std::out_of_range("A fila está vazia.");
    }
    NoFila<T>* no_a_remover = frente;
    T dado_removido = no_a_remover->dado;
    frente = frente->proximo;
    if (frente == nullptr) {
        tras = nullptr;
    }
    delete no_a_remover;
    tamanho_atual--;
    return dado_removido;
}

template <typename T>
bool Fila<T>::esta_vazia() const {
    return frente == nullptr;
}

template <typename T>
int Fila<T>::tamanho() const {
    return tamanho_atual;
}

#endif // FILA_HPP
