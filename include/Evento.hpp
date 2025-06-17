#ifndef EVENTO_HPP
#define EVENTO_HPP

#include <string>
#include <iomanip>
#include <sstream>

// Enumeração para os tipos de evento
enum class TipoEvento {
    CHEGADA_PACOTE,
    TRANSPORTE_PACOTES
};

class Evento {
public:
    double tempo; // Tempo de ocorrência do evento
    TipoEvento tipo;

    Evento(double tempo, TipoEvento tipo) : tempo(tempo), tipo(tipo) {}
    virtual ~Evento() = default;

    // Método virtual para obter a chave de prioridade como uma string longa
    virtual long long obter_chave_prioridade() const = 0;
};

class EventoChegada : public Evento {
public:
    int id_pacote;
    int id_armazem;

    EventoChegada(double tempo, int id_pacote, int id_armazem)
        : Evento(tempo, TipoEvento::CHEGADA_PACOTE), id_pacote(id_pacote), id_armazem(id_armazem) {}

    long long obter_chave_prioridade() const override {
        // Chave: Tempo (6) | ID Pacote (6) | Tipo (1) = 13 dígitos
        long long chave = (long long)tempo * 10000000LL + id_pacote * 10LL + 1;
        return chave;
    }
};

class EventoTransporte : public Evento {
public:
    int id_armazem_origem;
    int id_armazem_destino;

    EventoTransporte(double tempo, int origem, int destino)
        : Evento(tempo, TipoEvento::TRANSPORTE_PACOTES), id_armazem_origem(origem), id_armazem_destino(destino) {}

    long long obter_chave_prioridade() const override {
        // Chave: Tempo (6) | Origem (3) | Destino (3) | Tipo (1) = 13 dígitos
        long long chave = (long long)tempo * 10000000LL + id_armazem_origem * 10000LL + id_armazem_destino * 10LL + 2;
        return chave;
    }
};

#endif // EVENTO_HPP
