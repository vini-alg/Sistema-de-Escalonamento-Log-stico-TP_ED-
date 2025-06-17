#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "Evento.hpp"

// Implementação de um Min-Heap para a fila de prioridade de eventos
class Escalonador {
private:
    Evento** heap;
    int capacidade;
    int tamanho;

    void subir(int i);
    void descer(int i);
    int pai(int i) { return (i - 1) / 2; }
    int esquerda(int i) { return 2 * i + 1; }
    int direita(int i) { return 2 * i + 2; }
    void trocar(int i, int j);
    void redimensionar();

public:
    Escalonador(int capacidade_inicial = 100);
    ~Escalonador();

    void insere_evento(Evento* evento);
    Evento* retira_proximo_evento();
    bool vazio() const;
};

#endif // ESCALONADOR_HPP
