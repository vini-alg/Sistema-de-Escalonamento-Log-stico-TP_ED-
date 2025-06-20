#include "../include/Simulacao.hpp"
#include "../include/Fila.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Para std::reverse
#include <iomanip>   // Para std::setw e std::setfill

// Construtor: inicializa a simulação, carrega dados e agenda eventos iniciais.
Simulacao::Simulacao(const std::string& nome_arquivo) : tempo_atual(0), transporte_config(nullptr), num_armazens(0), matriz_adjacencia(nullptr) {
    escalonador = new Escalonador();
    carregar_dados(nome_arquivo);
    agendar_eventos_iniciais();
}

// Destrutor: libera toda a memória alocada dinamicamente.
Simulacao::~Simulacao() {
    limpar_memoria();
}

// Libera recursos: transporte, escalonador, armazéns, pacotes e matriz de adjacência.
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

// Carrega os dados de configuração a partir de um arquivo de entrada.
void Simulacao::carregar_dados(const std::string& nome_arquivo) {
    std::ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de entrada." << std::endl;
        return;
    }

    // Lê as configurações de transporte.
    int capacidade, latencia, intervalo, custo_remocao;
    arquivo >> capacidade >> latencia >> intervalo >> custo_remocao;
    transporte_config = new Transporte(capacidade, latencia, intervalo, custo_remocao);

    // Lê a matriz de adjacência que define as rotas.
    arquivo >> num_armazens;
    matriz_adjacencia = new bool*[num_armazens];
    for (int i = 0; i < num_armazens; ++i) {
        matriz_adjacencia[i] = new bool[num_armazens];
        for (int j = 0; j < num_armazens; ++j) {
            arquivo >> matriz_adjacencia[i][j];
        }
    }

    // Inicializa os armazéns.
    for (int i = 0; i < num_armazens; ++i) {
        armazens.adicionar(new Armazem(i, num_armazens));
    }

    // Lê e cria os pacotes da simulação.
    int num_pacotes;
    arquivo >> num_pacotes;
    std::string linha, str_pac, str_org, str_dst;
    int tempo, id, origem, destino;
    for (int i = 0; i < num_pacotes; ++i) {
        arquivo >> tempo >> str_pac >> id >> str_org >> origem >> str_dst >> destino;
        pacotes.adicionar(new Pacote(id, tempo, origem, destino));
    }
}

// Calcula a rota mais curta entre dois pontos usando BFS.
VetorDinamico<int> Simulacao::calcular_rota_bfs(int origem, int destino) {
    Fila<int> fila; // Fila para o BFS.
    VetorDinamico<int> antecessor(num_armazens); // Armazena o caminho.
    VetorDinamico<bool> visitado(num_armazens); // Marca nós visitados.
    for(int i = 0; i < num_armazens; ++i) {
        antecessor.adicionar(-1);
        visitado.adicionar(false);
    }

    fila.enfileirar(origem);
    visitado[origem] = true;

    // Loop principal do BFS.
    while (!fila.esta_vazia()) {
        int u = fila.desenfileirar();
        if (u == destino) break; // Chegou ao destino.

        for (int v = 0; v < num_armazens; ++v) {
            if (matriz_adjacencia[u][v] && !visitado[v]) {
                visitado[v] = true;
                antecessor[v] = u;
                fila.enfileirar(v);
            }
        }
    }

    // Reconstrói a rota a partir do destino usando o vetor de antecessores.
    VetorDinamico<int> rota;
    int atual = destino;
    while (atual != -1) {
        rota.adicionar(atual);
        atual = antecessor[atual];
    }

    // A rota foi construída de trás para frente, então precisa ser invertida.
    VetorDinamico<int> rota_invertida;
    for(int i = rota.tamanho() - 1; i >= 0; --i) {
        rota_invertida.adicionar(rota[i]);
    }
    return rota_invertida;
}

// Agenda os eventos que dão início à simulação.
void Simulacao::agendar_eventos_iniciais() {
    // Agenda a chegada inicial de cada pacote em seu armazém de origem.
    for (int i = 0; i < pacotes.tamanho(); ++i) {
        Pacote* p = pacotes[i];
        VetorDinamico<int> rota = calcular_rota_bfs(p->armazem_origem, p->armazem_destino);
        p->definir_rota(rota);
        escalonador->insere_evento(new EventoChegada(p->tempo_postagem, p->id, p->armazem_origem));
    }

    // Agenda o primeiro evento de transporte para cada rota bidirecional.
    for (int i = 0; i < num_armazens; ++i) {
        for (int j = i + 1; j < num_armazens; ++j) { // j = i + 1 para evitar duplicatas.
            if (matriz_adjacencia[i][j]) {
                escalonador->insere_evento(new EventoTransporte(transporte_config->intervalo, i, j));
                escalonador->insere_evento(new EventoTransporte(transporte_config->intervalo, j, i));
            }
        }
    }
}

// Loop principal que executa a simulação.
void Simulacao::executar() {
    // Loop continua enquanto houver eventos na fila de prioridade.
    while (!escalonador->vazio()) {
        Evento* evento = escalonador->retira_proximo_evento();
        tempo_atual = evento->tempo; // Avança o relógio da simulação.

        // Direciona o evento para a função de processamento correta.
        if (evento->tipo == TipoEvento::CHEGADA_PACOTE) {
            processar_evento_chegada(static_cast<EventoChegada*>(evento));
        } else if (evento->tipo == TipoEvento::TRANSPORTE_PACOTES) {
            processar_evento_transporte(static_cast<EventoTransporte*>(evento));
        }
        delete evento; // Libera a memória do evento processado.
    }
}

// Processa a chegada de um pacote a um armazém.
void Simulacao::processar_evento_chegada(EventoChegada* evento) {
    Pacote* pacote = nullptr;
    // Encontra o pacote correspondente ao evento.
    for (int i = 0; i < pacotes.tamanho(); ++i) {
        if (pacotes[i]->id == evento->id_pacote) {
            pacote = pacotes[i];
            break;
        }
    }

    if (!pacote) return; // Segurança: se o pacote não for encontrado, ignora.

    // Garante que o pacote avance em sua rota planejada.
    if (pacote->obter_proximo_destino() == evento->id_armazem) {
        pacote->avancar_rota();
    }

    // Verifica se o armazém atual é o destino final do pacote.
    if (evento->id_armazem == pacote->armazem_destino) {
        pacote->atualizar_estado(EstadoPacote::ENTREGUE);
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_atual << " pacote " << std::setw(3) << std::setfill('0') << pacote->id << " entregue em " << std::setw(3) << std::setfill('0') << evento->id_armazem << std::endl;
    } else {
        // Se não for o destino final, armazena o pacote na seção correta.
        Armazem* armazem = armazens[evento->id_armazem];
        armazem->armazenar_pacote(pacote);
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_atual << " pacote " << std::setw(3) << std::setfill('0') << pacote->id << " armazenado em " << std::setw(3) << std::setfill('0') << evento->id_armazem << " na secao " << std::setw(3) << std::setfill('0') << pacote->obter_proximo_destino() << std::endl;
    }
}

// Processa um evento de transporte de pacotes.
void Simulacao::processar_evento_transporte(EventoTransporte* evento) {
    Armazem* armazem_origem = armazens[evento->id_armazem_origem];
    Pilha<Pacote*>& secao = armazem_origem->obter_secao(evento->id_armazem_destino);

    // Se a seção de destino estiver vazia, apenas reagenda o transporte.
    if (secao.esta_vazia()) {
        escalonador->insere_evento(new EventoTransporte(tempo_atual + transporte_config->intervalo, evento->id_armazem_origem, evento->id_armazem_destino));
        return;
    }

    // Desempilha todos os pacotes da seção para um vetor temporário para poder ordená-los.
    VetorDinamico<Pacote*> pacotes_movidos;
    while (!secao.esta_vazia()) {
        pacotes_movidos.adicionar(secao.desempilha());
    }

    // Ordena os pacotes por tempo de postagem (Bubble Sort) para priorizar os mais antigos.
    for (int i = 0; i < pacotes_movidos.tamanho() - 1; i++) {
        for (int j = 0; j < pacotes_movidos.tamanho() - i - 1; j++) {
            if (pacotes_movidos[j]->tempo_postagem > pacotes_movidos[j + 1]->tempo_postagem) {
                Pacote* temp = pacotes_movidos[j];
                pacotes_movidos[j] = pacotes_movidos[j+1];
                pacotes_movidos[j+1] = temp;
            }
        }
    }

    // Seleciona os pacotes a serem transportados, respeitando a capacidade do veículo.
    VetorDinamico<Pacote*> para_transportar;
    int transportados_count = 0;
    for (int i = 0; i < pacotes_movidos.tamanho() && transportados_count < transporte_config->capacidade; ++i) {
        para_transportar.adicionar(pacotes_movidos[i]);
        transportados_count++;
    }

    double tempo_op = tempo_atual;
    // Simula a remoção de pacotes da pilha, um a um, aplicando o custo de remoção.
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
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_op << " pacote " << std::setw(3) << std::setfill('0') << p->id << " removido de " << std::setw(3) << std::setfill('0') << evento->id_armazem_origem << " na secao " << std::setw(3) << std::setfill('0') << evento->id_armazem_destino << std::endl;

        if (sera_transportado) {
            p->atualizar_estado(EstadoPacote::REMOVIDO_PARA_TRANSPORTE);
        } else {
            // Pacotes não transportados são rearmazenados na seção.
            secao.empilha(p);
            std::cout << std::setw(7) << std::setfill('0') << (int)tempo_op << " pacote " << std::setw(3) << std::setfill('0') << p->id << " rearmazenado em " << std::setw(3) << std::setfill('0') << evento->id_armazem_origem << " na secao " << std::setw(3) << std::setfill('0') << evento->id_armazem_destino << std::endl;
        }
    }

    // Agenda a chegada futura dos pacotes que foram transportados.
    for (int i = 0; i < para_transportar.tamanho(); ++i) {
        Pacote* p = para_transportar[i];
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_op << " pacote " << std::setw(3) << std::setfill('0') << p->id << " em transito de " << std::setw(3) << std::setfill('0') << evento->id_armazem_origem << " para " << std::setw(3) << std::setfill('0') << evento->id_armazem_destino << std::endl;
        escalonador->insere_evento(new EventoChegada(tempo_op + transporte_config->latencia, p->id, evento->id_armazem_destino));
    }

    // Agenda o próximo evento de transporte para esta mesma rota.
    escalonador->insere_evento(new EventoTransporte(tempo_atual + transporte_config->intervalo, evento->id_armazem_origem, evento->id_armazem_destino));
}
