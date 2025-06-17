#ifndef ARMAZEM_HPP
#define ARMAZEM_HPP

#include "Pilha.hpp"
#include "Pacote.hpp"

class Armazem {
private:
    int id;
    // Cada índice do array corresponde a uma seção para um armazém adjacente.
    Pilha<Pacote*>* secoes;
    int num_secoes; // igual ao número total de armazéns na simulação

public:
    Armazem(int id, int total_armazens);
    ~Armazem();

    void armazenar_pacote(Pacote* pacote);
    
    // Retorna a pilha de uma seção específica para manipulação
    Pilha<Pacote*>& obter_secao(int id_armazem_destino);
};

#endif // ARMAZEM_HPP
