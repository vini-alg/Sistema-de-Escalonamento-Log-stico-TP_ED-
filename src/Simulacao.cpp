#include "../include/Simulacao.hpp"
#include "../include/Fila.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Para std::reverse

Simulacao::Simulacao(const std::string& nome_arquivo) : tempo_atual(0), transporte_config(nullptr), num_armazens(0), matriz_adjacencia(nullptr) {
    escalonador = new Escalonador();
    carregar_dados(nome_arquivo);
    agendar_eventos_iniciais();
}

Simulacao::~Simulacao() {
    limpar_memoria();
}

void Simulacao::limpar_memoria() {
    delete transporte_config;
    delete escalonador;

    for (int i = 0; i < armazens.tamanho(); ++i) delete armazens[i];
    for (int i = 0; i < pacotes.tamanho(); ++i) delete pacotes[i];

    if (matriz_adjacencia) {
        for (int i = 0; i < num_armazens; ++i) delete[] matriz_adjacencia[i];
        delete[] matriz_adjacencia;
    }
}

void Simulacao::carregar_dados(const std::string& nome_arquivo) {
    std::ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de entrada." << std::endl;
        return;
    }

    int capacidade, latencia, intervalo, custo_remocao;
    arquivo >> capacidade >> latencia >> intervalo >> custo_remocao;
    transporte_config = new Transporte(capacidade, latencia, intervalo, custo_remocao);

    arquivo >> num_armazens;
    matriz_adjacencia = new bool*[num_armazens];
    for (int i = 0; i < num_armazens; ++i) {
        matriz_adjacencia[i] = new bool[num_armazens];
        for (int j = 0; j < num_armazens; ++j) {
            arquivo >> matriz_adjacencia[i][j];
        }
    }

    for (int i = 0; i < num_armazens; ++i) {
        armazens.adicionar(new Armazem(i, num_armazens));
    }

    int num_pacotes;
    arquivo >> num_pacotes;
    std::string linha, str_pac, str_org, str_dst;
    int tempo, id, origem, destino;
    for (int i = 0; i < num_pacotes; ++i) {
        arquivo >> tempo >> str_pac >> id >> str_org >> origem >> str_dst >> destino;
        pacotes.adicionar(new Pacote(id, tempo, origem, destino));
    }
}

VetorDinamico<int> Simulacao::calcular_rota_bfs(int origem, int destino) {
    Fila<int> fila;
    VetorDinamico<int> antecessor(num_armazens);
    VetorDinamico<bool> visitado(num_armazens);
    for(int i = 0; i < num_armazens; ++i) {
        antecessor.adicionar(-1);
        visitado.adicionar(false);
    }

    fila.enfileirar(origem);
    visitado[origem] = true;

    while (!fila.esta_vazia()) {
        int u = fila.desenfileirar();
        if (u == destino) break;

        for (int v = 0; v < num_armazens; ++v) {
            if (matriz_adjacencia[u][v] && !visitado[v]) {
                visitado[v] = true;
                antecessor[v] = u;
                fila.enfileirar(v);
            }
        }
    }

    VetorDinamico<int> rota;
    int atual = destino;
    while (atual != -1) {
        rota.adicionar(atual);
        atual = antecessor[atual];
    }

    // A rota foi construída do destino para a origem, precisamos inverter.
    VetorDinamico<int> rota_invertida;
    for(int i = rota.tamanho() - 1; i >= 0; --i) {
        rota_invertida.adicionar(rota[i]);
    }
    return rota_invertida;
}

void Simulacao::agendar_eventos_iniciais() {
    // Agendar chegada inicial dos pacotes
    for (int i = 0; i < pacotes.tamanho(); ++i) {
        Pacote* p = pacotes[i];
        VetorDinamico<int> rota = calcular_rota_bfs(p->armazem_origem, p->armazem_destino);
        p->definir_rota(rota);
        escalonador->insere_evento(new EventoChegada(p->tempo_postagem, p->id, p->armazem_origem));
    }

    // Agendar primeiro transporte para cada par de armazéns conectados
    for (int i = 0; i < num_armazens; ++i) {
        for (int j = i + 1; j < num_armazens; ++j) { // j = i + 1 para evitar duplicatas e self-loops
            if (matriz_adjacencia[i][j]) {
                escalonador->insere_evento(new EventoTransporte(transporte_config->intervalo, i, j));
                escalonador->insere_evento(new EventoTransporte(transporte_config->intervalo, j, i));
            }
        }
    }
}

void Simulacao::executar() {
    std::cout << "Iniciando a simulação..." << std::endl;
    if (escalonador->vazio()) {
        std::cout << "ALERTA: O escalonador está vazio no início da execução." << std::endl;
    }

    while (!escalonador->vazio()) {
        Evento* evento = escalonador->retira_proximo_evento();
        tempo_atual = evento->tempo;

        // std::cout << "Processando evento no tempo: " << tempo_atual << std::endl;

        if (evento->tipo == TipoEvento::CHEGADA_PACOTE) {
            processar_evento_chegada(static_cast<EventoChegada*>(evento));
        } else if (evento->tipo == TipoEvento::TRANSPORTE_PACOTES) {
            processar_evento_transporte(static_cast<EventoTransporte*>(evento));
        }
        delete evento;
    }
    std::cout << "Simulação concluída." << std::endl;
}

void Simulacao::processar_evento_chegada(EventoChegada* evento) {
    Pacote* pacote = nullptr;
    for (int i = 0; i < pacotes.tamanho(); ++i) {
        if (pacotes[i]->id == evento->id_pacote) {
            pacote = pacotes[i];
            break;
        }
    }

    if (!pacote) return; // Pacote não encontrado

    // Avança a rota para o armazém atual
    if (pacote->obter_proximo_destino() == evento->id_armazem) {
        pacote->avancar_rota();
    }

    if (evento->id_armazem == pacote->armazem_destino) {
        pacote->atualizar_estado(EstadoPacote::ENTREGUE);
        printf("%07d pacote %03d entregue em %03d\n", (int)tempo_atual, pacote->id, evento->id_armazem);
    } else {
        Armazem* armazem = armazens[evento->id_armazem];
        armazem->armazenar_pacote(pacote);
        printf("%07d pacote %03d armazenado em %03d na secao %03d\n", (int)tempo_atual, pacote->id, evento->id_armazem, pacote->obter_proximo_destino());
    }
}

void Simulacao::processar_evento_transporte(EventoTransporte* evento) {
    Armazem* armazem_origem = armazens[evento->id_armazem_origem];
    Pilha<Pacote*>& secao = armazem_origem->obter_secao(evento->id_armazem_destino);

    if (secao.esta_vazia()) {
        escalonador->insere_evento(new EventoTransporte(tempo_atual + transporte_config->intervalo, evento->id_armazem_origem, evento->id_armazem_destino));
        return;
    }

    VetorDinamico<Pacote*> pacotes_movidos;
    while (!secao.esta_vazia()) {
        pacotes_movidos.adicionar(secao.desempilha());
    }

    // Ordena por tempo de postagem para encontrar os mais antigos
    for (int i = 0; i < pacotes_movidos.tamanho() - 1; i++) {
        for (int j = 0; j < pacotes_movidos.tamanho() - i - 1; j++) {
            if (pacotes_movidos[j]->tempo_postagem > pacotes_movidos[j + 1]->tempo_postagem) {
                Pacote* temp = pacotes_movidos[j];
                pacotes_movidos[j] = pacotes_movidos[j+1];
                pacotes_movidos[j+1] = temp;
            }
        }
    }

    VetorDinamico<Pacote*> para_transportar;
    int transportados_count = 0;
    for (int i = 0; i < pacotes_movidos.tamanho() && transportados_count < transporte_config->capacidade; ++i) {
        para_transportar.adicionar(pacotes_movidos[i]);
        transportados_count++;
    }

    double tempo_op = tempo_atual;
    // Simula remoção e rearmazenamento
    for (int i = pacotes_movidos.tamanho() - 1; i >= 0; i--) {
        Pacote* p = pacotes_movidos[i];
        bool sera_transportado = false;
        for (int j = 0; j < para_transportar.tamanho(); ++j) {
            if (p->id == para_transportar[j]->id) {
                sera_transportado = true;
                break;
            }
        }

        tempo_op += transporte_config->custo_remocao;
        printf("%07d pacote %03d removido de %03d na secao %03d\n", (int)tempo_op, p->id, evento->id_armazem_origem, evento->id_armazem_destino);

        if (sera_transportado) {
            p->atualizar_estado(EstadoPacote::REMOVIDO_PARA_TRANSPORTE);
        } else {
            secao.empilha(p); // Rearmazena imediatamente
            printf("%07d pacote %03d rearmazenado em %03d na secao %03d\n", (int)tempo_op, p->id, evento->id_armazem_origem, evento->id_armazem_destino);
        }
    }

    // Agenda a chegada dos pacotes transportados
    for (int i = 0; i < para_transportar.tamanho(); ++i) {
        Pacote* p = para_transportar[i];
        printf("%07d pacote %03d em transito de %03d para %03d\n", (int)tempo_op, p->id, evento->id_armazem_origem, evento->id_armazem_destino);
        escalonador->insere_evento(new EventoChegada(tempo_op + transporte_config->latencia, p->id, evento->id_armazem_destino));
    }

    // Agenda o próximo evento de transporte
    escalonador->insere_evento(new EventoTransporte(tempo_atual + transporte_config->intervalo, evento->id_armazem_origem, evento->id_armazem_destino));
}
