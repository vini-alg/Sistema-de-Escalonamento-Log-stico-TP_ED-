#ifndef VETOR_DINAMICO_HPP
#define VETOR_DINAMICO_HPP

#include <stdexcept>

/**
 * @class VetorDinamico
 * @brief Implementa um vetor dinâmico genérico (array redimensionável).
 * @tparam T O tipo de elementos a serem armazenados no vetor.
 */
template <typename T>
class VetorDinamico {
private:
    T* dados;           ///< Ponteiro para o array de dados alocado dinamicamente.
    int capacidade;     ///< A capacidade atual do vetor (espaço alocado).
    int tamanho_atual;  ///< O número de elementos atualmente no vetor.

    /**
     * @brief Dobra a capacidade do vetor quando ele está cheio.
     * 
     * Aloca um novo array com o dobro da capacidade, copia os elementos existentes
     * e libera a memória do array antigo.
     */
    void redimensionar();

public:
    /**
     * @brief Constrói um novo vetor dinâmico.
     * @param cap_inicial A capacidade inicial do vetor.
     */
    VetorDinamico(int cap_inicial = 10);

    /**
     * @brief Destrói o vetor dinâmico, liberando a memória alocada.
     */
    ~VetorDinamico();

    /**
     * @brief Construtor de cópia.
     * @param outro O vetor a ser copiado.
     */
    VetorDinamico(const VetorDinamico& outro);

    /**
     * @brief Operador de atribuição de cópia.
     * @param outro O vetor a ser copiado.
     * @return Uma referência para este vetor após a cópia.
     */
    VetorDinamico& operator=(const VetorDinamico& outro);

    /**
     * @brief Adiciona um elemento ao final do vetor.
     * @param elemento O elemento a ser adicionado.
     */
    void adicionar(T elemento);

    /**
     * @brief Adiciona um elemento no início do vetor.
     * @param elemento O elemento a ser adicionado.
     */
    void push_front(T elemento);

    /**
     * @brief Acessa um elemento no índice especificado.
     * @param indice O índice do elemento a ser acessado.
     * @return Uma referência ao elemento.
     * @throws std::out_of_range se o índice for inválido.
     */
    T& operator[](int indice);

    /**
     * @brief Acessa um elemento constante no índice especificado.
     * @param indice O índice do elemento a ser acessado.
     * @return Uma referência constante ao elemento.
     * @throws std::out_of_range se o índice for inválido.
     */
    const T& operator[](int indice) const;

    /**
     * @brief Retorna o número de elementos no vetor.
     * @return O tamanho do vetor.
     */
    int tamanho() const;

    /**
     * @brief Remove todos os elementos do vetor e o redefine para seu estado inicial.
     */
    void limpar();

    /**
     * @brief Ordena o vetor por ID usando o algoritmo de insertion sort.
     */
    void ordenar_por_id();
};

/**
 * @brief Construtor da classe VetorDinamico.
 * @param cap_inicial Capacidade inicial do vetor.
 */
template <typename T>
VetorDinamico<T>::VetorDinamico(int cap_inicial) : capacidade(cap_inicial), tamanho_atual(0) {
    dados = new T[capacidade];
}

/**
 * @brief Destrutor da classe VetorDinamico. Libera a memória alocada.
 */
template <typename T>
VetorDinamico<T>::~VetorDinamico() {
    delete[] dados;
}

/**
 * @brief Construtor de cópia para a classe VetorDinamico.
 * @param outro O outro VetorDinamico a ser copiado.
 */
template <typename T>
VetorDinamico<T>::VetorDinamico(const VetorDinamico& outro) {
    capacidade = outro.capacidade;
    tamanho_atual = outro.tamanho_atual;
    dados = new T[capacidade];
    for (int i = 0; i < tamanho_atual; ++i) {
        dados[i] = outro.dados[i];
    }
}

/**
 * @brief Operador de atribuição de cópia.
 * @param outro O outro VetorDinamico a ser atribuído a este.
 * @return Uma referência a este objeto.
 */
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

/**
 * @brief Redimensiona o vetor, dobrando sua capacidade.
 */
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

/**
 * @brief Adiciona um elemento ao final do vetor. Se necessário, redimensiona.
 * @param elemento O elemento a ser adicionado.
 */
template <typename T>
void VetorDinamico<T>::adicionar(T elemento) {
    if (tamanho_atual == capacidade) {
        redimensionar();
    }
    dados[tamanho_atual++] = elemento;
}

/**
 * @brief Acessa o elemento no índice especificado com verificação de limites.
 * @param indice O índice do elemento.
 * @return Uma referência ao elemento no índice.
 * @throws std::out_of_range se o índice estiver fora do intervalo.
 */
template <typename T>
T& VetorDinamico<T>::operator[](int indice) {
    if (indice < 0 || indice >= tamanho_atual) {
        throw std::out_of_range("Índice fora do intervalo.");
    }
    return dados[indice];
}

/**
 * @brief Acessa o elemento constante no índice especificado com verificação de limites.
 * @param indice O índice do elemento.
 * @return Uma referência constante ao elemento no índice.
 * @throws std::out_of_range se o índice estiver fora do intervalo.
 */
template <typename T>
const T& VetorDinamico<T>::operator[](int indice) const {
    if (indice < 0 || indice >= tamanho_atual) {
        throw std::out_of_range("Índice fora do intervalo.");
    }
    return dados[indice];
}

/**
 * @brief Retorna o número de elementos no vetor.
 * @return O tamanho atual do vetor.
 */
template <typename T>
int VetorDinamico<T>::tamanho() const {
    return tamanho_atual;
}

/**
 * @brief Limpa o vetor, removendo todos os elementos e redefinindo a capacidade.
 */
template <typename T>
void VetorDinamico<T>::limpar() {
    delete[] dados;
    capacidade = 10;
    tamanho_atual = 0;
    dados = new T[capacidade];
}

/**
 * @brief Ordena o vetor de ponteiros para objetos que possuem um membro 'display_id'.
 */
template <typename T>
void VetorDinamico<T>::ordenar_por_id() {
    for (int i = 1; i < tamanho_atual; i++) {
        T chave = dados[i];
        int j = i - 1;
        while (j >= 0 && dados[j]->id > chave->id) {
            dados[j + 1] = dados[j];
            j = j - 1;
        }
        dados[j + 1] = chave;
    }
}

/**
 * @brief Adiciona um elemento no início do vetor.
 * @tparam T O tipo de dado do vetor.
 * @param elemento O elemento a ser adicionado.
 */
template <typename T>
void VetorDinamico<T>::push_front(T elemento) {
    if (tamanho_atual == capacidade) {
        redimensionar();
    }
    for (int i = tamanho_atual; i > 0; --i) {
        dados[i] = dados[i - 1];
    }
    dados[0] = elemento;
    tamanho_atual++;
}

#endif // VETOR_DINAMICO_HPP
