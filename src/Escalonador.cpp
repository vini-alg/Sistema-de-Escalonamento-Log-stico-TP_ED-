#include "../include/Escalonador.hpp"
#include <stdexcept>

/**
 * @brief Constrói um novo Escalonador com uma capacidade inicial.
 * @param capacidade_inicial A capacidade inicial da fila de eventos.
 */
Escalonador::Escalonador(int capacidade_inicial) : capacidade(capacidade_inicial), tamanho(0) {
    heap = new Evento*[capacidade];
}

/**
 * @brief Destrói o Escalonador, liberando a memória alocada para cada evento e para o heap.
 */
Escalonador::~Escalonador() {
    for (int i = 0; i < tamanho; ++i) {
        delete heap[i];
    }
    delete[] heap;
}

/**
 * @brief Troca dois elementos no heap.
 * @param i Índice do primeiro elemento.
 * @param j Índice do segundo elemento.
 */
void Escalonador::trocar(int i, int j) {
    Evento* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

/**
 * @brief Dobra a capacidade do heap quando ele está cheio.
 */
void Escalonador::redimensionar() {
    capacidade *= 2;
    Evento** novo_heap = new Evento*[capacidade];
    for (int i = 0; i < tamanho; ++i) {
        novo_heap[i] = heap[i];
    }
    delete[] heap;
    heap = novo_heap;
}

/**
 * @brief Move um nó para cima na árvore para manter a propriedade do min-heap.
 * @param i O índice do nó a ser movido para cima.
 */
void Escalonador::subir(int i) {
    while (i > 0 && heap[pai(i)]->obter_chave_prioridade() > heap[i]->obter_chave_prioridade()) {
        trocar(i, pai(i));
        i = pai(i);
    }
}

/**
 * @brief Move um nó para baixo na árvore para manter a propriedade do min-heap.
 * @param i O índice do nó a ser movido para baixo.
 */
void Escalonador::descer(int i) {
    int min_index = i;
    int l = esquerda(i);
    if (l < tamanho && heap[l]->obter_chave_prioridade() < heap[min_index]->obter_chave_prioridade()) {
        min_index = l;
    }
    int r = direita(i);
    if (r < tamanho && heap[r]->obter_chave_prioridade() < heap[min_index]->obter_chave_prioridade()) {
        min_index = r;
    }
    if (i != min_index) {
        trocar(i, min_index);
        descer(min_index);
    }
}

/**
 * @brief Insere um evento na fila de prioridade.
 * @param evento O ponteiro para o evento a ser inserido.
 */
void Escalonador::insere_evento(Evento* evento) {
    if (tamanho == capacidade) {
        redimensionar();
    }
    heap[tamanho] = evento;
    tamanho++;
    subir(tamanho - 1);
}

/**
 * @brief Remove e retorna o evento com a maior prioridade (menor tempo).
 * @return Um ponteiro para o próximo evento, ou nullptr se a fila estiver vazia.
 */
Evento* Escalonador::retira_proximo_evento() {
    if (vazio()) {
        return nullptr;
    }
    Evento* raiz = heap[0];
    heap[0] = heap[tamanho - 1];
    tamanho--;
    if (tamanho > 0) {
        descer(0);
    }
    return raiz;
}

/**
 * @brief Verifica se a fila de prioridade de eventos está vazia.
 * @return Verdadeiro se a fila estiver vazia, falso caso contrário.
 */
bool Escalonador::vazio() const {
    return tamanho == 0;
}
