#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include <string>
#include "Transporte.hpp"
#include "Armazem.hpp"
#include "Pacote.hpp"
#include "Escalonador.hpp"
#include "VetorDinamico.hpp"

/**
 * @brief Orquestra a simulação de logística, gerenciando o tempo, eventos e estado do sistema.
 */
class Simulacao {
private:
    double tempo_atual; ///< Relógio da simulação, avança a cada evento.
    Transporte* transporte_config; ///< Configurações globais de transporte (capacidade, latência, etc.).
    int num_armazens; ///< Número total de armazéns na simulação.
    VetorDinamico<Armazem*> armazens; ///< Vetor dinâmico com ponteiros para todos os armazéns.
    VetorDinamico<Pacote*> pacotes; ///< Vetor dinâmico com ponteiros para todos os pacotes da simulação.
    bool** matriz_adjacencia; ///< Matriz que representa as rotas de transporte entre os armazéns.
    Escalonador* escalonador; ///< Fila de prioridade (min-heap) que gerencia os eventos futuros.
    std::string last_line_buffer; ///< Buffer para a última linha de log para evitar nova linha no final.

    /**
     * @brief Lê o arquivo de entrada e inicializa o estado da simulação.
     * @param nome_arquivo Caminho para o arquivo de configuração.
     */
    void carregar_dados(const std::string& nome_arquivo);

    /**
     * @brief Cria e agenda os primeiros eventos (chegada de pacotes e transportes).
     */
    void agendar_eventos_iniciais();

    /**
     * @brief Calcula a rota mais curta entre dois armazéns usando BFS.
     * @param origem ID do armazém de origem.
     * @param destino ID do armazém de destino.
     * @return VetorDinamico<int> contendo a sequência de IDs de armazéns da rota.
     */
    VetorDinamico<int> calcular_rota_bfs(int origem, int destino);

    /**
     * @brief Processa um evento de chegada de pacote a um armazém.
     * @param evento Ponteiro para o evento de chegada.
     */
    void processar_evento_chegada(EventoChegada* evento);

    /**
     * @brief Processa um evento de transporte de pacotes entre armazéns.
     * @param evento Ponteiro para o evento de transporte.
     */
    void processar_evento_transporte(EventoTransporte* evento);

    /**
     * @brief Libera toda a memória alocada dinamicamente durante a simulação.
     */
    void limpar_memoria();

    /**
     * @brief Verifica se todos os pacotes já foram entregues.
     * @return True se todos os pacotes foram entregues, False caso contrário.
     */
    bool todos_pacotes_entregues() const;

    /**
     * @brief Imprime uma linha de log, gerenciando o buffer para a última linha.
     * @param line A linha a ser impressa.
     */
    void print_log_line(const std::string& line);

public:
    /**
     * @brief Construtor da classe Simulacao.
     * @param nome_arquivo Caminho para o arquivo de configuração inicial.
     */
    Simulacao(const std::string& nome_arquivo);

    /**
     * @brief Destrutor da classe Simulacao, responsável por chamar a limpeza de memória.
     */
    ~Simulacao();

    /**
     * @brief Inicia e executa o loop principal da simulação até que não hajam mais eventos.
     */
    void executar();
};

#endif // SIMULACAO_HPP
