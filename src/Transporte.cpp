#include "../include/Transporte.hpp"

/**
 * @brief Constrói um novo objeto Transporte.
 * 
 * @param cap A capacidade de transporte.
 * @param lat A latência de transporte.
 * @param interv O intervalo entre transportes.
 * @param custo O custo de remoção de pacotes.
 */
Transporte::Transporte(int cap, int lat, int interv, int custo)
    : capacidade(cap), latencia(lat), intervalo(interv), custo_remocao(custo) {}
