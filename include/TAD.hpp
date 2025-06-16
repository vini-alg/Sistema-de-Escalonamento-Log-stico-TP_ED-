#ifndef ORDENADOR_H
#define ORDENADOR_H

// Implementações próprias de funções utilitárias

// Minha função aleatoria
inline int meuRand(int& seed) {
    // Gerador linear congruencial simples
    const int a = 1664525;
    const int c = 1013904223;
    const int m = 2147483647; // 2^31 - 1
    
    int f_seed = (a * seed + c) % m;
    return f_seed;
}

// Troca dois elementos
inline void trocar(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Modulo de um double
inline double valorAbsoluto(double valor) {
    return valor < 0 ? -valor : valor;
}

struct Estatisticas {
    int mps;
    long long comp;
    long long mov;
    long long call;
    double custo;

    Estatisticas() : comp(0), mov(0), call(0), custo(0.0) {}

    void calcularCusto(double a, double b, double c) {
        custo = a * comp + b * mov + c * call;
    }
};

class OrdenadorUniversal {
private:
    double coefComp;  // a
    double coefMov; // b
    double coefCall;      // c
    double limiarCusto;       // limiar de convergência
    
    int minTamParticao;       // tamanho mínimo de partição para quicksort
    int limiarQuebras;        // número máximo de quebras para usar inserção
    
    Estatisticas estatisticas;

    // Métodos de ordenação
    void insercao(int* vetor, int esq, int dir);
    void quickSort(int* vetor, int tam);
    void quickSortRecursivo(int* vetor, int esq, int dir);
    int particiona(int* vetor, int esq, int dir);
    
    // Métodos auxiliares
    int medianaDeTres(int* vetor, int esq, int dir);
    int menorCusto(Estatisticas* custos, int numCustos);
    void calculaNovaFaixa(int limParticao, int& minMPS, int& maxMPS, int& passoMPS, int numMPS);
    
public:
    int contaQuebras(int* vetor, int tam);
    OrdenadorUniversal(double a, double b, double c, double limiar);
    
    // Getters e setters
    int getMinTamParticao() const { return minTamParticao; }
    void setMinTamParticao(int valor) { minTamParticao = valor; }
    
    int getLimiarQuebras() const { return limiarQuebras; }
    void setLimiarQuebras(int valor) { limiarQuebras = valor; }
    
    Estatisticas getEstatisticas() const { return estatisticas; }
    void resetEstatisticas() { estatisticas = Estatisticas(); }
    
    // Método principal de ordenação
    void ordenar(int* vetor, int tam, int minTamParticao, int limiarQuebras);
    
    // Métodos para calibração do TAD
    int determinaLimiarParticao(int* vetor, int tam, int quebras, int seed);
    int determinaLimiarQuebras(int* vetor, int tam, int quebras, int seed);
    
    // Método para registrar estatísticas
    void registraEstatisticas(Estatisticas& custo);
    
    // Método para imprimir estatísticas
    void imprimeEstatisticas(const Estatisticas& custo, int limiar);
};

#endif // ORDENADOR_H
