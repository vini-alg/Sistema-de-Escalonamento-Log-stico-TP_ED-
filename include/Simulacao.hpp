#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include <string>
#include "Transporte.hpp"
#include "Armazem.hpp"
#include "Pacote.hpp"
#include "Escalonador.hpp"
#include "VetorDinamico.hpp"

class Simulacao {
private:
    double tempo_atual;
    Transporte* transporte_config;
    int num_armazens;
    VetorDinamico<Armazem*> armazens;
    VetorDinamico<Pacote*> pacotes;
    bool** matriz_adjacencia;
    Escalonador* escalonador;

    void carregar_dados(const std::string& nome_arquivo);
    void agendar_eventos_iniciais();
    VetorDinamico<int> calcular_rota_bfs(int origem, int destino);

    void processar_evento_chegada(EventoChegada* evento);
    void processar_evento_transporte(EventoTransporte* evento);

    void limpar_memoria();

public:
    Simulacao(const std::string& nome_arquivo);
    ~Simulacao();

    void executar();
};

#endif // SIMULACAO_HPP
