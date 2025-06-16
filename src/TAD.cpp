#include "../include/ordenador.hpp"
#include <iostream>
#include <iomanip>

OrdenadorUniversal::OrdenadorUniversal(double a, double b, double c, double limiar) {
    coefComp = a;
    coefMov = b;
    coefCall = c;
    limiarCusto = limiar;
    
}

void OrdenadorUniversal::ordenar(int* vetor, int tam, int mTP, int LQ) {
    minTamParticao = mTP;
    limiarQuebras = LQ;

    int quebras = contaQuebras(vetor, tam);
    //std::cout << "Quebras: " << quebras << std::endl;

    if (quebras < limiarQuebras) {
        insercao(vetor, 0, tam-1);
    } else {
        if (tam > minTamParticao) {
            quickSortRecursivo(vetor, 0, tam - 1);
        } else {
            insercao(vetor, 0, tam-1);
        }
    }
    
    estatisticas.calcularCusto(coefComp, coefMov, coefCall);
}

int OrdenadorUniversal::contaQuebras(int* vetor, int tam) {
    int quebras = 0;
    for (int i = 1; i < tam; i++) {
        //estatisticas.comp++;
        if (vetor[i] < vetor[i-1]) {
            quebras++;
        }
    }
    
    return quebras;
}

void OrdenadorUniversal::insercao(int* vetor, int esq, int dir) {
    estatisticas.call++;
    for (int i = esq + 1; i < dir; i++) {
        int chave = vetor[i];
        estatisticas.mov++;
        
        int j = i - 1;
        
        while (j >= esq) {
            estatisticas.comp++;
            if (vetor[j] > chave) {
                vetor[j + 1] = vetor[j];
                estatisticas.mov++;
                j--;
            } else {
                break;
            }
        }
        
        vetor[j + 1] = chave;
        estatisticas.mov++;
    }
}

void OrdenadorUniversal::quickSort(int* vetor, int tam) {
    //estatisticas.call++;
    quickSortRecursivo(vetor, 0, tam - 1);
    return;
}

void OrdenadorUniversal::quickSortRecursivo(int* vetor, int esq, int dir) {;
    //estatisticas.call++;
    // Verifica se o tamanho da partição é maior que minTamParticao
    
    if (dir - esq + 1 < minTamParticao) {
        insercao(vetor, esq, dir);
        //estatisticas.call--;
        return;
    }
    
    else{
        if (esq < dir) {
            int p = particiona(vetor, esq, dir);
            quickSortRecursivo(vetor, esq, p);
            quickSortRecursivo(vetor, p + 1, dir);
            return;
    }
}
}

int OrdenadorUniversal::medianaDeTres(int* vetor, int esq, int dir) {
    estatisticas.call++;
    // Certifica-se de que há pelo menos 3 elementos
    if (dir - esq < 2) return esq; // Não há necessidade de mediana se não houver 3 elementos
    
    //int meio = ((dir - esq + 1) / 2) + esq;
    int meio = dir - 1;
    trocar(vetor[dir - 2], vetor[esq]);
    //std::cout << "medianaDeTres esq " << esq << " meio " << meio << " dir " << dir << std::endl;
    // Ordena os três elementos (esq, meio, dir)
    estatisticas.comp++;
    if (vetor[meio] < vetor[esq]) {
        trocar(vetor[meio], vetor[esq]);
        estatisticas.mov += 3;
    }
    
    estatisticas.comp++;
    if (vetor[dir] < vetor[esq]) {
        trocar(vetor[dir], vetor[esq]);
        estatisticas.mov += 3;
    }
    
    estatisticas.comp++;
    if (vetor[dir] < vetor[meio]) {
        trocar(vetor[dir], vetor[meio]);
        estatisticas.mov += 3;
    }

    //trocar(vetor[meio], vetor[dir - 1]);
    //estatisticas.mov += 3;

    return  vetor[meio];
}

int OrdenadorUniversal::particiona(int* vetor, int esq, int dir) {
    // Verifica se há elementos suficientes para particionar
    if (dir <= esq) return esq;
    //estatisticas.call++;
    if (dir - esq < 2) return esq; // Não há necessidade de particionar um ou dois elementos
    int pivo = medianaDeTres(vetor, esq, dir);
    if (dir - esq <=3) {
        //estatisticas.call--;
        return dir - 1;
    }
    int i = esq + 1;
    int j = dir - 1;
    
    while (i <= j){
        while (vetor[i] < pivo) {
            estatisticas.comp++;
            i++;
        }
        while (vetor[j] > pivo) {
            estatisticas.comp++;
            j--;
        }
        estatisticas.comp++;
        if (i <= j){
            trocar(vetor[i], vetor[j]);
            i++;
            j--;
            estatisticas.mov += 3;
        }
    }
    return j;
}

int OrdenadorUniversal::determinaLimiarParticao(int* vetor, int tam, int quebras, int seed) {
    
    // Certifica-se de que estamos trabalhando com tamanhos válidos
    int minMPS = 2;
    int maxMPS = tam > 10 ? tam : 10; // Mínimo de 10 elementos
    
    // Garante que o passo seja pelo menos 1 para evitar loops infinitos
    int passoMPS = (maxMPS - minMPS) / 5;
    if (passoMPS < 1) passoMPS = 1;
    
    int numMPS = 0; // Número de pontos de teste
    
    // Aloca memória para os custos com tamanho fixo
    Estatisticas custos[6]; // Máximo de 6 pontos de teste
    
    double diffCusto = 1.0e30; // Valor grande para substituir max()
    int limParticao = minMPS;
    
    std::cout << "size " << tam << " seed " << seed << " breaks " << quebras << std::endl;

    int iteracao = 0;
    
    // Clone o vetor original para não alterá-lo
    int* vetorClone = new int[tam];
    if (!vetorClone) return minMPS; // Falha na alocação de memória
    
    // Gera um vetor com alguma desordem para tornar os testes mais realistas
    for (int i = 0; i < tam; i++) {
        vetorClone[i] = i;
    }
    
    while ((diffCusto > limiarCusto) && (numMPS >= 0)) {
        std::cout << "\niter " << iteracao << std::endl;
        numMPS = 0;
        double biggestCost = 0.0; // Valor pequeno para substituir min()

        for (int t = minMPS;  t < maxMPS && numMPS < 6; t += passoMPS) {
            // Cria uma cópia do vetor para cada teste
            int* copiaVetor = new int[tam];
            if (!copiaVetor) continue; // Falha na alocação de memória
            
            // Copia manual em vez de std::copy
            for (int i = 0; i < tam; i++) {
                copiaVetor[i] = vetor[i];
            }
            
            // Reset estatísticas
            estatisticas.comp = 0;
            estatisticas.mov = 0;
            estatisticas.call = 0;
            
            // Define o tamanho mínimo da partição para este teste
            estatisticas.mps = t;

            // Execute o algoritmo de ordenação com limiarQuebras alto para forçar o uso de quicksort
;
            ordenar(copiaVetor, tam, t, 1);
            
            if (estatisticas.custo > biggestCost) {
                biggestCost = estatisticas.custo;
            }

            // Limpa a memória da cópia para cada teste
            delete[] copiaVetor;
            
            // Registra estatísticas
            custos[numMPS] = estatisticas;
            

            // Imprime estatísticas
            std::cout << "mps " << custos[numMPS].mps << " cost " << std::fixed << std::setprecision(9) 
                      << custos[numMPS].custo << " cmp " << custos[numMPS].comp 
                      << " move " << custos[numMPS].mov << " calls " << custos[numMPS].call << std::endl;
            
            numMPS++;
        }
        
        if (numMPS < 2) break; // Não há pontos de teste suficientes
        
        // Encontra partição com menor custo
        int idxMenorCusto = menorCusto(custos, numMPS);
        
        // Calcula nova faixa
        calculaNovaFaixa(idxMenorCusto, minMPS, maxMPS, passoMPS, numMPS);
        // Calcula diferença de custo
        diffCusto = valorAbsoluto(custos[idxMenorCusto].custo - biggestCost);
        int limParticao = custos[idxMenorCusto].mps;
        std::cout << "nummps " << numMPS << " limParticao " << limParticao << " mpsdiff " << std::fixed << std::setprecision(9)  << diffCusto << "\n" << std::endl;
        
        iteracao++;
        
        // Evita loops infinitos
        if (iteracao > 10 || passoMPS == 0) break;
    }
    
    delete[] vetorClone;
    return limParticao;
}

int OrdenadorUniversal::determinaLimiarQuebras(int* vetor, int tam, int quebras, int seed) {
    
    // Define o tamanho mínimo da partição para este teste usando o valor já calibrado
    // Assumimos que determinaLimiarParticao já foi chamado
    
    int minLQ = 1;
    int maxLQ = tam / 2; // No máximo metade do vetor como limiar de quebras
    if (maxLQ < 5) maxLQ = 5; // Mínimo de 5 elementos para teste
    
    int passoLQ = (maxLQ - minLQ) / 5;
    if (passoLQ < 1) passoLQ = 1; // Garante pelo menos um passo
    
    int numLQ = 0; // Número de pontos de teste
    
    Estatisticas custosQS[6]; // Quicksort - máximo de 6 pontos
    Estatisticas custosIN[6]; // Inserção - máximo de 6 pontos
    
    double diffCusto = 1.0e30; // Valor grande para substituir max()
    int limQuebras = minLQ;

    int iteracao = 0;
    
    // Gera um vetor de teste com diferentes quebras
    int* vetorTeste = new int[tam];
    if (!vetorTeste) return minLQ; // Falha na alocação
    
    // Gera um vetor ordenado primeiro
    for (int i = 0; i < tam; i++) {
        vetorTeste[i] = i;
    }
    
    while ((diffCusto > limiarCusto) && (numLQ >= 0)) {
        std::cout << "iter " << iteracao << std::endl;
        numLQ = 0;
        
        for (int lq = minLQ; lq <= maxLQ && numLQ < 6; lq += passoLQ) {
            // Testa com Quicksort
            int* copiaVetor = new int[tam];
            if (!copiaVetor) continue; // Falha na alocação
            
            // Copia manual em vez de std::copy
            for (int i = 0; i < tam; i++) {
                copiaVetor[i] = vetorTeste[i];
            }
            
            for (int i = 0; i < lq*2; i+=2) {
                trocar(copiaVetor[i], copiaVetor[i + 1]);
            }

            estatisticas.comp = 0;
            estatisticas.mov = 0;
            estatisticas.call = 0;
            ordenar(copiaVetor, tam, 0, 0);
            
            custosQS[numLQ] = estatisticas;
            
            std::cout << "qs lq " << lq << " cost " << std::fixed << std::setprecision(9) 
                      << estatisticas.custo << " cmp " << estatisticas.comp 
                      << " move " << estatisticas.mov << " calls " << estatisticas.call << std::endl;
            
            delete[] copiaVetor; // Libera a memória da cópia
            
            // Testa com Inserção
            copiaVetor = new int[tam];
            if (!copiaVetor) continue; // Falha na alocação
            
            // Copia manual em vez de std::copy
            for (int i = 0; i < tam; i++) {
                copiaVetor[i] = vetorTeste[i];
            }
            
            for (int i = 0; i < lq*2; i+=2) {
                trocar(copiaVetor[i], copiaVetor[i + 1]);
            }

            estatisticas.comp = 0;
            estatisticas.mov = 0;
            estatisticas.call = 0;
            limiarQuebras = tam; // Forçar uso de inserção
            ordenar(copiaVetor, tam, 0, tam);
            
            custosIN[numLQ] = estatisticas;
            
            std::cout << "in lq " << lq << " cost " << std::fixed << std::setprecision(9) 
                      << estatisticas.custo << " cmp " << estatisticas.comp 
                      << " move " << estatisticas.mov << " calls " << estatisticas.call << std::endl;
            
            delete[] copiaVetor; // Libera a memória da cópia
            
            numLQ++;
        }
        
        if (numLQ < 2) break; // Não há pontos de teste suficientes
        
        // Compara custos para encontrar o limiar ideal
        int idxMelhorLQ = 0;
        for (int i = 0; i < numLQ; i++) {
            if (custosQS[i].custo > custosIN[i].custo) {
                idxMelhorLQ = i;
            } else {
                break; // Se quicksort já é melhor, não precisa procurar mais
            }
        }
        
        limQuebras = minLQ + idxMelhorLQ * passoLQ;
        
        // Calcula nova faixa
        calculaNovaFaixa(idxMelhorLQ, minLQ, maxLQ, passoLQ, numLQ);
        
        // Calcula diferença de custo
        diffCusto = valorAbsoluto(custosQS[0].custo - custosQS[numLQ-1].custo);
        
        std::cout << "numlq " << numLQ << " limQuebras " << limQuebras << " lqdiff " << diffCusto << "\n" << std::endl;
        
        iteracao++;
        
        // Evita loops infinitos
        if (iteracao > 10 || passoLQ == 0) break;
    }
    
    delete[] vetorTeste;
    return limQuebras;
}

int OrdenadorUniversal::menorCusto(Estatisticas* custos, int numCustos) {
    int idxMenor = 0;
    double menorCusto = custos[0].custo;
    
    for (int i = 1; i < numCustos; i++) {
        if (custos[i].custo < menorCusto) {
            menorCusto = custos[i].custo;
            idxMenor = i;
        }
    }
    
    return idxMenor;
}

void OrdenadorUniversal::calculaNovaFaixa(int limIndice, int& min, int& max, int& passo, int numPontos) {
    int newMin, newMax;
    
    if (limIndice == 0) {
        newMin = 0;
        newMax = 2;
    } else if (limIndice == numPontos - 1) {
        newMin = numPontos - 3;
        newMax = numPontos - 1;
    } else {
        newMin = limIndice - 1;
        newMax = limIndice + 1;
    }
    
    // Converte índices para valores reais
    int oldMin = min;
    
    min = oldMin + newMin * passo;
    max = oldMin + newMax * passo;
    
    passo = (max - min) / 5;
    if (passo == 0) passo = 1;
}

void OrdenadorUniversal::registraEstatisticas(Estatisticas& custo) {
    custo = estatisticas;
}

void OrdenadorUniversal::imprimeEstatisticas(const Estatisticas& custo, int limiar) {
    std::cout << "mps " << limiar << " cost " << std::fixed << std::setprecision(9) 
              << custo.custo << " cmp " << custo.comp 
              << " move " << custo.mov << " calls " << custo.call << std::endl;
}
