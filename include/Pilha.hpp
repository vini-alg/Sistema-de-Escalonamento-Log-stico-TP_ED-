#ifndef PILHA_HPP
#define PILHA_HPP

#include <stdexcept>

/**
 * @struct NoPilha
 * @brief Representa um nó em uma estrutura de dados de pilha.
 * @tparam T O tipo de dado armazenado no nó.
 */
template <typename T>
struct NoPilha {
    T dado;             ///< O dado armazenado no nó.
    NoPilha* proximo;    ///< Ponteiro para o próximo nó na pilha.
};

/**
 * @class Pilha
 * @brief Implementa uma estrutura de dados de pilha genérica (LIFO).
 * @tparam T O tipo de elementos a serem armazenados na pilha.
 */
template <typename T>
class Pilha {
private:
    NoPilha<T>* topo;   ///< Ponteiro para o elemento no topo da pilha.
    int tamanho;        ///< O número de elementos atualmente na pilha.

public:
    /** @brief Constrói uma nova pilha vazia. */
    Pilha();

    /** @brief Destrói a pilha, liberando todos os seus elementos. */
    ~Pilha();

    /**
     * @brief Construtor de cópia.
     * @param outro A pilha a ser copiada.
     */
    Pilha(const Pilha<T>& outro);

    /**
     * @brief Operador de atribuição de cópia.
     * @param outro A pilha a ser copiada.
     * @return Uma referência para esta pilha após a cópia.
     */
    Pilha<T>& operator=(const Pilha<T>& outro);

    /**
     * @brief Adiciona um elemento ao topo da pilha (push).
     * @param elemento O elemento a ser adicionado.
     */
    void empilha(T elemento);

    /**
     * @brief Remove e retorna o elemento do topo da pilha (pop).
     * @return O elemento removido.
     * @throws std::out_of_range se a pilha estiver vazia.
     */
    T desempilha();

    /**
     * @brief Retorna o elemento no topo da pilha sem removê-lo (peek).
     * @return O elemento do topo.
     * @throws std::out_of_range se a pilha estiver vazia.
     */
    T ver_topo() const;

    /**
     * @brief Verifica se a pilha está vazia.
     * @return Verdadeiro se a pilha estiver vazia, falso caso contrário.
     */
    bool esta_vazia() const;

    /**
     * @brief Retorna o número de elementos na pilha.
     * @return O tamanho da pilha.
     */
    int obter_tamanho() const;

    /** @brief Remove todos os elementos da pilha. */
    void limpar();
};

/**
 * @brief Construtor da classe Pilha. Inicializa uma pilha vazia.
 */
template <typename T>
Pilha<T>::Pilha() : topo(nullptr), tamanho(0) {}

/**
 * @brief Destrutor da classe Pilha. Libera toda a memória alocada para os nós da pilha.
 */
template <typename T>
Pilha<T>::~Pilha() {
    limpar();
}

/**
 * @brief Adiciona um elemento ao topo da pilha (empilha).
 * @param elemento O elemento a ser adicionado à pilha.
 */
template <typename T>
void Pilha<T>::empilha(T elemento) {
    NoPilha<T>* novo_no = new NoPilha<T>{elemento, topo};
    topo = novo_no;
    tamanho++;
}

/**
 * @brief Remove e retorna o elemento do topo da pilha (desempilha).
 * @return O elemento que estava no topo da pilha.
 * @throws std::out_of_range se a pilha estiver vazia.
 */
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

/**
 * @brief Retorna o elemento no topo da pilha sem removê-lo.
 * @return O elemento no topo da pilha.
 * @throws std::out_of_range se a pilha estiver vazia.
 */
template <typename T>
T Pilha<T>::ver_topo() const {
    if (esta_vazia()) {
        throw std::out_of_range("A pilha está vazia.");
    }
    return topo->dado;
}

/**
 * @brief Verifica se a pilha está vazia.
 * @return `true` se a pilha estiver vazia, `false` caso contrário.
 */
template <typename T>
bool Pilha<T>::esta_vazia() const {
    return topo == nullptr;
}

/**
 * @brief Retorna o número de elementos atualmente na pilha.
 * @return O tamanho da pilha.
 */
template <typename T>
int Pilha<T>::obter_tamanho() const {
    return tamanho;
}

/**
 * @brief Remove todos os elementos da pilha, deixando-a vazia.
 */
template <typename T>
void Pilha<T>::limpar() {
    while (!esta_vazia()) {
        desempilha();
    }
}

#endif // PILHA_HPP
