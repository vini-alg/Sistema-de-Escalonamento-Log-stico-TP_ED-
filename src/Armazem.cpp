#include "../include/Armazem.hpp"

Armazem::Armazem(int id, int total_armazens) : id(id), num_secoes(total_armazens) {
    secoes = new Pilha<Pacote*>[total_armazens];
}

Armazem::~Armazem() {
    delete[] secoes;
}

void Armazem::armazenar_pacote(Pacote* pacote) {
    int proximo_destino = pacote->obter_proximo_destino();
    if (proximo_destino != -1 && proximo_destino < num_secoes) {
        secoes[proximo_destino].empilha(pacote);
        pacote->atualizar_estado(EstadoPacote::ARMAZENADO);
    }
}

Pilha<Pacote*>& Armazem::obter_secao(int id_armazem_destino) {
    if (id_armazem_destino >= 0 && id_armazem_destino < num_secoes) {
        return secoes[id_armazem_destino];
    }
    throw std::out_of_range("ID de armazém de destino inválido.");
}
