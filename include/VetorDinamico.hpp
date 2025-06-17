#ifndef VETOR_DINAMICO_HPP
#define VETOR_DINAMICO_HPP

#include <stdexcept>

template <typename T>
class VetorDinamico {
private:
    T* dados;
    int capacidade;
    int tamanho_atual;

    void redimensionar();

public:
    VetorDinamico(int cap_inicial = 10);
    ~VetorDinamico();
    VetorDinamico(const VetorDinamico& outro);
    VetorDinamico& operator=(const VetorDinamico& outro);

    void adicionar(const T& elemento);
    T& operator[](int indice);
    const T& operator[](int indice) const;
    int tamanho() const;
    void limpar();
};

template <typename T>
VetorDinamico<T>::VetorDinamico(int cap_inicial) : capacidade(cap_inicial), tamanho_atual(0) {
    dados = new T[capacidade];
}

template <typename T>
VetorDinamico<T>::~VetorDinamico() {
    delete[] dados;
}

template <typename T>
VetorDinamico<T>::VetorDinamico(const VetorDinamico& outro) {
    capacidade = outro.capacidade;
    tamanho_atual = outro.tamanho_atual;
    dados = new T[capacidade];
    for (int i = 0; i < tamanho_atual; ++i) {
        dados[i] = outro.dados[i];
    }
}

template <typename T>
VetorDinamico<T>& VetorDinamico<T>::operator=(const VetorDinamico& outro) {
    if (this != &outro) {
        delete[] dados;
        capacidade = outro.capacidade;
        tamanho_atual = outro.tamanho_atual;
        dados = new T[capacidade];
        for (int i = 0; i < tamanho_atual; ++i) {
            dados[i] = outro.dados[i];
        }
    }
    return *this;
}

template <typename T>
void VetorDinamico<T>::redimensionar() {
    capacidade = (capacidade == 0) ? 1 : capacidade * 2;
    T* novos_dados = new T[capacidade];
    for (int i = 0; i < tamanho_atual; ++i) {
        novos_dados[i] = dados[i];
    }
    delete[] dados;
    dados = novos_dados;
}

template <typename T>
void VetorDinamico<T>::adicionar(const T& elemento) {
    if (tamanho_atual == capacidade) {
        redimensionar();
    }
    dados[tamanho_atual++] = elemento;
}

template <typename T>
T& VetorDinamico<T>::operator[](int indice) {
    if (indice < 0 || indice >= tamanho_atual) {
        throw std::out_of_range("Índice fora do intervalo.");
    }
    return dados[indice];
}

template <typename T>
const T& VetorDinamico<T>::operator[](int indice) const {
    if (indice < 0 || indice >= tamanho_atual) {
        throw std::out_of_range("Índice fora do intervalo.");
    }
    return dados[indice];
}

template <typename T>
int VetorDinamico<T>::tamanho() const {
    return tamanho_atual;
}

template <typename T>
void VetorDinamico<T>::limpar() {
    delete[] dados;
    capacidade = 10;
    tamanho_atual = 0;
    dados = new T[capacidade];
}

#endif // VETOR_DINAMICO_HPP
