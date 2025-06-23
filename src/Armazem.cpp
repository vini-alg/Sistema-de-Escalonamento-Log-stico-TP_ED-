#include "../include/Armazem.hpp"

/**
 * @brief Constrói um novo objeto Armazem.
 *
 * @param id O identificador para o armazém.
 * @param total_armazens O número total de armazéns na simulação, usado para alocar as seções.
 */
Armazem::Armazem(int id, int total_armazens) : id(id), num_secoes(total_armazens) {
    secoes = new Pilha<Pacote*>[total_armazens];
}

/**
 * @brief Destrói o objeto Armazem, liberando os recursos alocados.
 */
Armazem::~Armazem() {
    delete[] secoes;
}

/**
 * @brief Adiciona um pacote à seção de armazenamento apropriada.
 *
 * O pacote é colocado na pilha correspondente ao seu próximo destino na rota.
 * @param pacote O pacote a ser armazenado.
 */
void Armazem::armazenar_pacote(Pacote* pacote) {
    int proximo_destino = pacote->obter_proximo_destino();
    if (proximo_destino != -1 && proximo_destino < num_secoes) {
        secoes[proximo_destino].empilha(pacote);
        pacote->atualizar_estado(EstadoPacote::ARMAZENADO);
    }
}

/**
 * @brief Recupera a seção de empilhamento para um armazém de destino específico.
 *
 * @param id_armazem_destino O ID do armazém de destino.
 * @return Uma referência para a pilha de pacotes da seção solicitada.
 * @throws std::out_of_range Se o ID do armazém de destino for inválido.
 */
Pilha<Pacote*>& Armazem::obter_secao(int id_armazem_destino) {
    if (id_armazem_destino >= 0 && id_armazem_destino < num_secoes) {
        return secoes[id_armazem_destino];
    }
    throw std::out_of_range("ID de armazém de destino inválido.");
}
