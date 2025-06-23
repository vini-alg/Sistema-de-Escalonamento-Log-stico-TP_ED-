#ifndef FILA_HPP
#define FILA_HPP

#include <stdexcept>

/**
 * @struct NoFila
 * @brief Representa um nó em uma estrutura de dados de fila.
 * @tparam T O tipo de dado armazenado no nó.
 */
template <typename T>
struct NoFila {
    T dado;             ///< O dado armazenado no nó.
    NoFila* proximo;    ///< Ponteiro para o próximo nó na fila.
};

/**
 * @class Fila
 * @brief Implementa uma estrutura de dados de fila genérica (FIFO).
 * @tparam T O tipo de elementos a serem armazenados na fila.
 */
template <typename T>
class Fila {
private:
    NoFila<T>* frente;      ///< Ponteiro para o primeiro elemento da fila.
    NoFila<T>* tras;        ///< Ponteiro para o último elemento da fila.
    int tamanho_atual;      ///< O número de elementos atualmente na fila.

public:
    /** @brief Constrói uma nova fila vazia. */
    Fila();

    /** @brief Destrói a fila, liberando todos os seus elementos. */
    ~Fila();

    /**
     * @brief Adiciona um elemento ao final da fila.
     * @param elemento O elemento a ser adicionado.
     */
    void enfileirar(const T& elemento);

    /**
     * @brief Remove e retorna o elemento no início da fila.
     * @return O elemento removido.
     * @throws std::out_of_range se a fila estiver vazia.
     */
    T desenfileirar();

    /**
     * @brief Verifica se a fila está vazia.
     * @return Verdadeiro se a fila estiver vazia, falso caso contrário.
     */
    bool esta_vazia() const;

    /**
     * @brief Retorna o número de elementos na fila.
     * @return O tamanho da fila.
     */
    int tamanho() const;
};

/**
 * @brief Construtor da classe Fila. Inicializa uma fila vazia.
 */
template <typename T>
Fila<T>::Fila() : frente(nullptr), tras(nullptr), tamanho_atual(0) {}

/**
 * @brief Destrutor da classe Fila. Libera toda a memória alocada para os nós da fila.
 */
template <typename T>
Fila<T>::~Fila() {
    while (!esta_vazia()) {
        desenfileirar();
    }
}

/**
 * @brief Adiciona um elemento ao final da fila (enfileirar).
 * @param elemento O elemento a ser adicionado à fila.
 */
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

/**
 * @brief Remove e retorna o elemento da frente da fila (desenfileirar).
 * @return O elemento que estava na frente da fila.
 * @throws std::out_of_range se a fila estiver vazia.
 */
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

/**
 * @brief Verifica se a fila está vazia.
 * @return `true` se a fila estiver vazia, `false` caso contrário.
 */
template <typename T>
bool Fila<T>::esta_vazia() const {
    return frente == nullptr;
}

/**
 * @brief Retorna o número de elementos atualmente na fila.
 * @return O tamanho da fila.
 */
template <typename T>
int Fila<T>::tamanho() const {
    return tamanho_atual;
}

#endif // FILA_HPP
