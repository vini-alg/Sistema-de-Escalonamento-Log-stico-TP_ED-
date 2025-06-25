#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

/**
 * @enum TipoEvento
 * @brief Enumeração para os diferentes tipos de eventos na simulação.
 */
enum class TipoEvento {
    CHEGADA_PACOTE,
    TRANSPORTE_PACOTES
};

/**
 * @class Evento
 * @brief Classe base abstrata para todos os eventos na simulação.
 *
 * Define a interface comum para eventos, incluindo tempo e tipo,
 * e um método virtual puro para obter uma chave de prioridade.
 */
class Evento {
public:
    double tempo;       ///< O tempo de simulação em que o evento ocorre.
    TipoEvento tipo;    ///< O tipo do evento.

    /**
     * @brief Constrói um novo objeto Evento.
     * @param tempo O tempo em que o evento ocorre.
     * @param tipo O tipo do evento.
     */
    Evento(double tempo, TipoEvento tipo) : tempo(tempo), tipo(tipo) {}

    /**
     * @brief Destrutor virtual padrão.
     */
    virtual ~Evento() = default;

    /**
     * @brief Método virtual puro para obter a chave de prioridade do evento.
     *
     * A chave de prioridade é usada para ordenar eventos na fila de prioridade.
     * Eventos com chaves menores têm prioridade mais alta.
     * @return Uma chave de prioridade `long long`.
     */
    virtual long long obter_chave_prioridade() const = 0;
};

/**
 * @class EventoChegada
 * @brief Representa um evento de chegada de pacote em um armazém.
 */
class EventoChegada : public Evento {
public:
    int id_pacote;      ///< O ID do pacote que está chegando.
    int id_armazem;     ///< O ID do armazém onde o pacote está chegando.

    /**
     * @brief Constrói um novo objeto EventoChegada.
     * @param tempo O tempo de chegada.
     * @param id_pacote O ID do pacote.
     * @param id_armazem O ID do armazém.
     */
    EventoChegada(double tempo, int id_pacote, int id_armazem)
        : Evento(tempo, TipoEvento::CHEGADA_PACOTE), id_pacote(id_pacote), id_armazem(id_armazem) {}

    /**
     * @brief Gera uma chave de prioridade para o evento de chegada.
     *
     * A chave é composta pelo tempo, ID do pacote e um tipo de desempate.
     * @return A chave de prioridade calculada.
     */
    long long obter_chave_prioridade() const override {
        // Chave: Tempo (6) | ID Pacote (6) | Tipo (1) = 13 dígitos
        long long chave = (long long)tempo * 10000000LL + id_pacote * 10LL + 1;
        return chave;
    }
};

/**
 * @class EventoTransporte
 * @brief Representa um evento de transporte de pacotes de um armazém para outro.
 */
class EventoTransporte : public Evento {
public:
    int id_armazem_origem;    ///< O ID do armazém de origem.
    int id_armazem_destino;   ///< O ID do armazém de destino.

    /**
     * @brief Constrói um novo objeto EventoTransporte.
     * @param tempo O tempo do evento de transporte.
     * @param origem O ID do armazém de origem.
     * @param destino O ID do armazém de destino.
     */
    EventoTransporte(double tempo, int origem, int destino)
        : Evento(tempo, TipoEvento::TRANSPORTE_PACOTES), id_armazem_origem(origem), id_armazem_destino(destino) {}

    /**
     * @brief Gera uma chave de prioridade para o evento de transporte.
     *
     * A chave é composta pelo tempo, IDs de origem e destino, e um tipo de desempate.
     * @return A chave de prioridade calculada.
     */
    long long obter_chave_prioridade() const override {
        // Chave: Tempo (6) | Origem (3) | Destino (3) | Tipo (1) = 13 dígitos
        long long chave = (long long)tempo * 10000000LL + id_armazem_origem * 10000LL + id_armazem_destino * 10LL + 2;
        return chave;
    }
};

/**
 * @struct ComparadorEventos
 * @brief Functor de comparação para ordenar eventos em uma fila de prioridade.
 *
 * Compara dois ponteiros de Evento com base em suas chaves de prioridade.
 * Aquele com a menor chave de prioridade é considerado "maior" para
 * uso em uma fila de prioridade de mínimo (min-heap).
 */
struct ComparadorEventos {
    bool operator()(const Evento* a, const Evento* b) const {
        return a->obter_chave_prioridade() > b->obter_chave_prioridade();
    }
};

#endif // EVENTO_HPP
