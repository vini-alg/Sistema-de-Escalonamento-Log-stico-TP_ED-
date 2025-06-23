#include "../include/Pacote.hpp"

/**
 * @brief Constrói um novo objeto Pacote.
 * @param id O ID único do pacote.
 * @param tempo_postagem O tempo de postagem do pacote.
 * @param origem O armazém de origem.
 * @param destino O armazém de destino.
 */
Pacote::Pacote(int id, int tempo_postagem, int origem, int destino)
    : id(id), display_id(id % 100), tempo_postagem(tempo_postagem), armazem_origem(origem), armazem_destino(destino),
      estado_atual(EstadoPacote::NAO_POSTADO), proximo_armazem_index(0),
      tempo_total_armazenado(0.0), tempo_total_em_transporte(0.0) {}

/**
 * @brief Define a rota para o pacote.
 * @param nova_rota A rota a ser seguida pelo pacote.
 */
void Pacote::definir_rota(const VetorDinamico<int>& nova_rota) {
    rota = nova_rota;
}

/**
 * @brief Obtém o próximo armazém de destino na rota do pacote.
 * @return O ID do próximo armazém, ou -1 se a rota estiver completa.
 */
int Pacote::obter_proximo_destino() {
    if (proximo_armazem_index < rota.tamanho()) {
        return rota[proximo_armazem_index];
    }
    return -1; // Indica que chegou ao destino final ou não há mais rota
}

/**
 * @brief Avança o pacote para o próximo armazém em sua rota.
 */
void Pacote::avancar_rota() {
    if (proximo_armazem_index < rota.tamanho()) {
        proximo_armazem_index++;
    }
}

/**
 * @brief Atualiza o estado atual do pacote.
 * @param novo_estado O novo estado do pacote.
 */
void Pacote::atualizar_estado(EstadoPacote novo_estado) {
    estado_atual = novo_estado;
}
