#ifndef PILHA_HPP
#define PILHA_HPP

#include <stdexcept>

// Nó da pilha
template <typename T>
struct NoPilha {
    T dado;
    NoPilha* proximo;
};

// A classe Pilha (LIFO)
template <typename T>
class Pilha {
private:
    NoPilha<T>* topo;
    int tamanho;

public:
    Pilha();
    ~Pilha();

    void empilha(T elemento); // push
    T desempilha();           // pop
    T ver_topo() const;       // peek
    bool esta_vazia() const;
    int obter_tamanho() const;
    void limpar();
};

// Como Pilha é uma classe de template, a implementação precisa estar no cabeçalho
template <typename T>
Pilha<T>::Pilha() : topo(nullptr), tamanho(0) {}

template <typename T>
Pilha<T>::~Pilha() {
    limpar();
}

template <typename T>
void Pilha<T>::empilha(T elemento) {
    NoPilha<T>* novo_no = new NoPilha<T>{elemento, topo};
    topo = novo_no;
    tamanho++;
}

template <typename T>
T Pilha<T>::desempilha() {
    if (esta_vazia()) {
        throw std::out_of_range("A pilha está vazia.");
    }
    NoPilha<T>* no_a_remover = topo;
    T dado_removido = no_a_remover->dado;
    topo = topo->proximo;
    delete no_a_remover;
    tamanho--;
    return dado_removido;
}

template <typename T>
T Pilha<T>::ver_topo() const {
    if (esta_vazia()) {
        throw std::out_of_range("A pilha está vazia.");
    }
    return topo->dado;
}

template <typename T>
bool Pilha<T>::esta_vazia() const {
    return topo == nullptr;
}

template <typename T>
int Pilha<T>::obter_tamanho() const {
    return tamanho;
}

template <typename T>
void Pilha<T>::limpar() {
    while (!esta_vazia()) {
        desempilha();
    }
}

#endif // PILHA_HPP
