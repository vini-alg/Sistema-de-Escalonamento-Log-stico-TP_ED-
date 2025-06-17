#include "../include/Escalonador.hpp"
#include <stdexcept>

Escalonador::Escalonador(int capacidade_inicial) : capacidade(capacidade_inicial), tamanho(0) {
    heap = new Evento*[capacidade];
}

Escalonador::~Escalonador() {
    for (int i = 0; i < tamanho; ++i) {
        delete heap[i];
    }
    delete[] heap;
}

void Escalonador::trocar(int i, int j) {
    Evento* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void Escalonador::redimensionar() {
    capacidade *= 2;
    Evento** novo_heap = new Evento*[capacidade];
    for (int i = 0; i < tamanho; ++i) {
        novo_heap[i] = heap[i];
    }
    delete[] heap;
    heap = novo_heap;
}

void Escalonador::subir(int i) {
    while (i > 0 && heap[pai(i)]->obter_chave_prioridade() > heap[i]->obter_chave_prioridade()) {
        trocar(i, pai(i));
        i = pai(i);
    }
}

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

void Escalonador::insere_evento(Evento* evento) {
    if (tamanho == capacidade) {
        redimensionar();
    }
    heap[tamanho] = evento;
    tamanho++;
    subir(tamanho - 1);
}

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

bool Escalonador::vazio() const {
    return tamanho == 0;
}
