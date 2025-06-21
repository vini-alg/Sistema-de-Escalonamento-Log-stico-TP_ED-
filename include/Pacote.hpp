#ifndef PACOTE_HPP
#define PACOTE_HPP

#include "VetorDinamico.hpp"

// Enumeração para os estados do pacote
enum class EstadoPacote {
    NAO_POSTADO,
    CHEGADA_ESCALONADA,
    ARMAZENADO,
    REMOVIDO_PARA_TRANSPORTE,
    ENTREGUE
};

class Pacote {
public:
    int id;
    int display_id = id % 100;
    int tempo_postagem;
    int armazem_origem;
    int armazem_destino;
    EstadoPacote estado_atual;
    VetorDinamico<int> rota;
    int proximo_armazem_index;

    // Estatísticas
    double tempo_total_armazenado;
    double tempo_total_em_transporte;

    Pacote(int id, int tempo_postagem, int origem, int destino);
    void definir_rota(const VetorDinamico<int>& nova_rota);
    int obter_proximo_destino();
    void avancar_rota();
    void atualizar_estado(EstadoPacote novo_estado);
};

#endif // PACOTE_HPP
