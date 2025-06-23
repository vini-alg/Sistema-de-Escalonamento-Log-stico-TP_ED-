#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "Evento.hpp"

/**
 * @class Escalonador
 * @brief Gerencia uma fila de prioridade de eventos usando um min-heap.
 *
 * Esta classe é responsável por agendar e recuperar eventos com base em seu tempo,
 * garantindo que o evento com o menor tempo seja sempre processado primeiro.
 */
class Escalonador {
private:
    Evento** heap;      ///< Ponteiro para o array que armazena os elementos do heap.
    int capacidade;     ///< A capacidade atual do array do heap.
    int tamanho;        ///< O número atual de elementos no heap.

    /** @brief Move um elemento para cima no heap para manter a propriedade do heap. */
    void subir(int i);
    /** @brief Move um elemento para baixo no heap para manter a propriedade do heap. */
    void descer(int i);
    /** @brief Retorna o índice do nó pai. */
    int pai(int i) { return (i - 1) / 2; }
    /** @brief Retorna o índice do filho esquerdo. */
    int esquerda(int i) { return 2 * i + 1; }
    /** @brief Retorna o índice do filho direito. */
    int direita(int i) { return 2 * i + 2; }
    /** @brief Troca dois elementos no heap. */
    void trocar(int i, int j);
    /** @brief Redimensiona o array do heap quando a capacidade é excedida. */
    void redimensionar();

public:
    /**
     * @brief Constrói um novo Escalonador com uma capacidade inicial.
     * @param capacidade_inicial A capacidade inicial da fila de eventos.
     */
    Escalonador(int capacidade_inicial = 100);

    /**
     * @brief Destrói o Escalonador, liberando a memória alocada.
     */
    ~Escalonador();

    /**
     * @brief Insere um novo evento na fila de prioridade.
     * @param evento O evento a ser adicionado.
     */
    void insere_evento(Evento* evento);

    /**
     * @brief Remove e retorna o evento com o menor tempo (a raiz do heap).
     * @return O próximo evento a ser processado, ou nullptr se a fila estiver vazia.
     */
    Evento* retira_proximo_evento();

    /**
     * @brief Verifica se a fila de eventos está vazia.
     * @return Verdadeiro se a fila estiver vazia, falso caso contrário.
     */
    bool vazio() const;
};

#endif // ESCALONADOR_HPP
