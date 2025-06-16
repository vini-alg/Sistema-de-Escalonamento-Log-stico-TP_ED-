#include <iostream>
#include <fstream>
#include "../include/ordenador.hpp"

// Função para gerar vetores com diferentes configurações
void gerarVetor(int* vetor, int tam, int seed, int grauDesordem = -1) {
    
    // Inicializa vetor ordenado
    for (int i = 0; i < tam; i++) {
        vetor[i] = i;
    }
    
    // Se grauDesordem == -1, cria um vetor totalmente aleatório
    // Caso contrário, cria um vetor com aproximadamente grauDesordem quebras
    if (grauDesordem == -1) {
        // Embaralha completamente
        for (int i = tam-1; i > 0; i--) {
            int j = meuRand(seed) % (i+1);
            trocar(vetor[i], vetor[j]);
        }
    } else {
        // Cria quebras específicas
        int quebrasDesejadas = grauDesordem;
        int quebrasAtuais = 0;
        
        while (quebrasAtuais < quebrasDesejadas && quebrasAtuais < tam-1) {
            int i = meuRand(seed) % (tam-1);
            
            // Troca apenas se isso vai criar uma quebra
            if (vetor[i] < vetor[i+1]) {
                trocar(vetor[i], vetor[i+1]);
                quebrasAtuais++;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    try {

        std::ifstream arquivo(argv[1]);
        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir o arquivo.\n";
        return 1;
        }

        // Lê os parâmetros de entrada
        double limiarCusto, a, b, c;
        int tam, seed;
        
        arquivo >> seed >> limiarCusto >> a >> b >> c >> tam;
        
        // Cria o ordenador universal com os parâmetros lidos
        OrdenadorUniversal ordenador(a, b, c, limiarCusto);
        
        // Determina o limiar de partição com um vetor de teste
        int* vetorParticao = new int[tam];
        if (!vetorParticao) {
            std::cout << "Erro na alocação de memória" << std::endl;
            return 2;
        }
        
        // Inicializa o vetor
        int i = 0;
        while (arquivo >> vetorParticao[i]) {
            i++;
            if (i >= tam) break;
        }
        arquivo.close();

        int quebras = ordenador.contaQuebras(vetorParticao, tam);
        // Introduz algumas trocas aleatórias para ter um vetor parcialmente ordenado
        for (int i = 0; i < quebras; i++) {
            int pos1 = meuRand(seed) % tam;
            int pos2 = meuRand(seed) % tam;
            trocar(vetorParticao[pos1], vetorParticao[pos2]);
        }
        
        // Determina o limiar de partição
        int limiarParticao = ordenador.determinaLimiarParticao(vetorParticao, tam, quebras, seed);
        
        // Define o limiar de partição encontrado
        ordenador.setMinTamParticao(limiarParticao);
        
        // Limpa a memória do vetor de partição antes de prosseguir
        delete[] vetorParticao;
        
        // Cria um novo vetor para determinar o limiar de quebras
        int* vetorQuebras = new int[tam];
        if (!vetorQuebras) {
            std::cout << "Erro na alocação de memória" << std::endl;
            return 2;
        }
        
        // Inicializa o vetor
        for (int i = 0; i < tam; i++) {
            vetorQuebras[i] = i;
        }
        
        // Introduz algumas quebras específicas
        seed = 1; // Reinicia a semente
        for (int i = 0; i < tam/4; i++) {
            int pos = meuRand(seed) % (tam - 1);
            trocar(vetorQuebras[pos], vetorQuebras[pos + 1]);
        }
        
        // Determina o limiar de quebras
        //int limiarQuebras = ordenador.determinaLimiarQuebras(vetorQuebras, tam, quebras, seed);
        
        // Define o limiar de quebras encontrado
        //ordenador.setLimiarQuebras(limiarQuebras);
        
        // Apresenta os resultados finais
        std::cout << "\nResultados finais:\n";
        std::cout << "Limiar de partição: " << limiarParticao << std::endl;
        //std::cout << "Limiar de quebras: " << limiarQuebras << std::endl;
        
        // Limpa a memória
        delete[] vetorQuebras;
        
        return 0;
    }
    catch (...) {
        std::cout << "Erro inesperado durante a execução" << std::endl;
        return 99;
    }
}
