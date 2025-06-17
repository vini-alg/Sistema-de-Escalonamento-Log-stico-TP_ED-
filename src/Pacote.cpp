#include "../include/Pacote.hpp"

Pacote::Pacote(int id, int tempo_postagem, int origem, int destino)
    : id(id), tempo_postagem(tempo_postagem), armazem_origem(origem), armazem_destino(destino),
      estado_atual(EstadoPacote::NAO_POSTADO), proximo_armazem_index(0),
      tempo_total_armazenado(0.0), tempo_total_em_transporte(0.0) {}

void Pacote::definir_rota(const VetorDinamico<int>& nova_rota) {
    rota = nova_rota;
}

int Pacote::obter_proximo_destino() {
    if (proximo_armazem_index < rota.tamanho()) {
        return rota[proximo_armazem_index];
    }
    return -1; // Indica que chegou ao destino final ou não há mais rota
}

void Pacote::avancar_rota() {
    if (proximo_armazem_index < rota.tamanho()) {
        proximo_armazem_index++;
    }
}

void Pacote::atualizar_estado(EstadoPacote novo_estado) {
    estado_atual = novo_estado;
}
