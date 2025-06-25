#ifndef PACOTE_HPP
#define PACOTE_HPP

#include "VetorDinamico.hpp"

/**
 * @enum EstadoPacote
 * @brief Enumeração dos possíveis estados de um pacote durante a simulação.
 */
enum class EstadoPacote {
    NAO_POSTADO,
    CHEGADA_ESCALONADA,
    ARMAZENADO,
    REMOVIDO_PARA_TRANSPORTE,
    ENTREGUE
};

/**
 * @class Pacote
 * @brief Representa um pacote no sistema de logística.
 *
 * Contém informações sobre o pacote, como ID, origem, destino, rota e estado atual,
 * além de estatísticas de tempo para análise de desempenho.
 */
class Pacote {
public:
    int id;                         ///< Identificador único do pacote.
    int display_id = id % 100;      ///< ID de exibição para facilitar a leitura, derivado do ID.
    int tempo_postagem;             ///< O tempo de simulação em que o pacote foi postado.
    int armazem_origem;             ///< O armazém de origem do pacote.
    int armazem_destino;            ///< O armazém de destino final do pacote.
    EstadoPacote estado_atual;      ///< O estado atual do pacote no processo de entrega.
    VetorDinamico<int> rota;        ///< A sequência de armazéns que o pacote deve visitar.
    int proximo_armazem_index;      ///< Índice do próximo armazém na rota.
    bool first_time = true;

    // Estatísticas
    double tempo_total_armazenado;      ///< Tempo total que o pacote passou armazenado.
    double tempo_total_em_transporte;   ///< Tempo total que o pacote passou em transporte.

    /**
     * @brief Constrói um novo objeto Pacote.
     * @param id O ID único do pacote.
     * @param tempo_postagem O tempo de postagem do pacote.
     * @param origem O armazém de origem.
     * @param destino O armazém de destino.
     */
    Pacote(int id, int tempo_postagem, int origem, int destino);

    /**
     * @brief Define a rota para o pacote.
     * @param nova_rota A rota a ser seguida pelo pacote.
     */
    void definir_rota(const VetorDinamico<int>& nova_rota);

    /**
     * @brief Obtém o próximo armazém de destino na rota do pacote.
     * @return O ID do próximo armazém, ou -1 se a rota estiver completa.
     */
    int obter_proximo_destino();

    /**
     * @brief Avança o pacote para o próximo armazém em sua rota.
     */
    void avancar_rota();

    /**
     * @brief Atualiza o estado atual do pacote.
     * @param novo_estado O novo estado do pacote.
     */
    void atualizar_estado(EstadoPacote novo_estado);
};

#endif // PACOTE_HPP
