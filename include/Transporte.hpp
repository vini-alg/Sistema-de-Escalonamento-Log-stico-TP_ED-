#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

/**
 * @class Transporte
 * @brief Armazena as configurações globais para os eventos de transporte na simulação.
 *
 * Esta classe encapsula os parâmetros que definem o comportamento dos transportes
 * entre os armazéns, como capacidade do veículo, latência da rota, intervalo entre
 * transportes e o custo (em tempo) para remover pacotes.
 */
class Transporte {
public:
    int capacidade;     ///< A capacidade máxima de pacotes que um veículo pode transportar.
    int latencia;       ///< O tempo (em unidades de simulação) que um transporte leva entre armazéns.
    int intervalo;      ///< O intervalo de tempo entre a partida de transportes consecutivos de um armazém.
    int custo_remocao;  ///< O custo de tempo para remover um pacote de uma pilha em um armazém.

    /**
     * @brief Constrói um novo objeto Transporte com as configurações especificadas.
     * @param cap A capacidade do veículo.
     * @param lat A latência da rota.
     * @param interv O intervalo entre transportes.
     * @param custo O custo de tempo para remoção de pacotes.
     */
    Transporte(int cap = 0, int lat = 0, int interv = 0, int custo = 0);
};

#endif // TRANSPORTE_HPP
