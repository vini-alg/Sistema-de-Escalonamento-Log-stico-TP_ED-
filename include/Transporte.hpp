#ifndef TRANSPORTE_HPP
#define TRANSPORTE_HPP

class Transporte {
public:
    int capacidade;
    int latencia;
    int intervalo;
    int custo_remocao;

    Transporte(int cap = 0, int lat = 0, int interv = 0, int custo = 0);
};

#endif // TRANSPORTE_HPP
