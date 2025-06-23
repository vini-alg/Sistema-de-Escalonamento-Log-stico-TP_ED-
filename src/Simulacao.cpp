#include "../include/Simulacao.hpp"
#include "../include/Fila.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Para std::reverse
#include <iomanip>   // Para std::setw e std::setfill

/**
 * @brief Constrói um novo objeto Simulacao.
 * 
 * Inicializa o estado da simulação, cria o escalonador de eventos, carrega os dados
 * de configuração do arquivo especificado e agenda os eventos iniciais.
 * 
 * @param nome_arquivo O caminho para o arquivo de entrada contendo os dados da simulação.
 */
Simulacao::Simulacao(const std::string& nome_arquivo) : tempo_atual(0), transporte_config(nullptr), num_armazens(0), matriz_adjacencia(nullptr) {
    escalonador = new Escalonador();
    carregar_dados(nome_arquivo);
    agendar_eventos_iniciais();
}

/**
 * @brief Destrói o objeto Simulacao.
 * 
 * Invoca o método `limpar_memoria` para liberar todos os recursos alocados dinamicamente
 * durante a simulação, como armazéns, pacotes e configurações.
 */
Simulacao::~Simulacao() {
    limpar_memoria();
}

/**
 * @brief Libera toda a memória alocada dinamicamente pela simulação.
 * 
 * Garante que não haja vazamentos de memória ao deletar o `transporte_config`,
 * o `escalonador`, todos os `armazens` e `pacotes`, e a `matriz_adjacencia`.
 */
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

/**
 * @brief Carrega os dados da simulação a partir de um arquivo de entrada.
 * 
 * Lê as configurações de transporte, a matriz de adjacência dos armazéns e os detalhes
 * dos pacotes a serem simulados. Inicializa os objetos correspondentes.
 * 
 * @param nome_arquivo O caminho para o arquivo de configuração.
 */
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
        id--;
        pacotes.adicionar(new Pacote(id, tempo, origem, destino));
    }
}

/**
 * @brief Calcula a rota mais curta entre dois armazéns usando o algoritmo Breadth-First Search (BFS).
 * 
 * Percorre o grafo de armazéns para encontrar o caminho com o menor número de saltos.
 * 
 * @param origem O ID do armazém de partida.
 * @param destino O ID do armazém de chegada.
 * @return Um `VetorDinamico<int>` contendo a sequência de IDs de armazéns que formam a rota.
 */
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

/**
 * @brief Agenda os eventos iniciais da simulação.
 * 
 * Para cada pacote, calcula sua rota e agenda um `EventoChegada` no armazém de origem.
 * Também agenda os `EventoTransporte` iniciais para todas as rotas de transporte ativas.
 */
void Simulacao::agendar_eventos_iniciais() {
    // Agenda a chegada inicial de cada pacote em seu armazém de origem.
    for (int i = 0; i < pacotes.tamanho(); ++i) {
        Pacote* p = pacotes[i];
        VetorDinamico<int> rota = calcular_rota_bfs(p->armazem_origem, p->armazem_destino);
        p->definir_rota(rota);
        escalonador->insere_evento(new EventoChegada(p->tempo_postagem, p->id, p->armazem_origem));
    }

    // Agenda o primeiro evento de transporte para cada rota bidirecional.
    if (!transporte_config) return;
    for (int i = 0; i < num_armazens; ++i) {
        for (int j = i + 1; j < num_armazens; ++j) { // j = i + 1 para evitar duplicatas.
            if (matriz_adjacencia[i][j]) {
                escalonador->insere_evento(new EventoTransporte(transporte_config->intervalo, i, j));
                escalonador->insere_evento(new EventoTransporte(transporte_config->intervalo, j, i));
            }
        }
    }
}

/**
 * @brief Executa o loop principal da simulação.
 * 
 * Processa eventos do escalonador em ordem cronológica até que a fila de eventos esteja vazia.
 * O tempo da simulação avança conforme os eventos são processados.
 */
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

/**
 * @brief Processa um evento de chegada de um pacote a um armazém.
 * 
 * Atualiza o estado do pacote e o armazena na seção correta. Se o pacote chegou
 * ao seu destino final, ele é marcado como ENTREGUE. Caso contrário, ele é armazenado
 * para futuro transporte.
 * 
 * @param evento O ponteiro para o `EventoChegada` a ser processado.
 */
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
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_atual << " pacote " << std::setw(3) << std::setfill('0') << pacote->display_id << " entregue em " << std::setw(3) << std::setfill('0') << evento->id_armazem << std::endl;
    } else {
        // Se não for o destino final, armazena o pacote na seção correta.
        armazens[evento->id_armazem]->armazenar_pacote(pacote);
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_atual << " pacote " << std::setw(3) << std::setfill('0') << pacote->display_id << " armazenado em " << std::setw(3) << std::setfill('0') << evento->id_armazem << " na secao " << std::setw(3) << std::setfill('0') << pacote->obter_proximo_destino() << std::endl;
    }
}

/**
 * @brief Verifica se todos os pacotes da simulação foram entregues.
 * 
 * Itera sobre todos os pacotes e verifica se o estado de cada um é `ENTREGUE`.
 * 
 * @return `true` se todos os pacotes foram entregues, `false` caso contrário.
 */
bool Simulacao::todos_pacotes_entregues() const {
    for (int i = 0; i < pacotes.tamanho(); ++i) {
        if (pacotes[i]->estado_atual != EstadoPacote::ENTREGUE) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Processa um evento de transporte de pacotes entre dois armazéns.
 * 
 * Seleciona pacotes de uma seção de um armazém para transporte, com base na capacidade
 * do veículo e na ordem de postagem. Agenda novos eventos de chegada para os pacotes
 * transportados e reagenda um novo evento de transporte para o futuro.
 * 
 * @param evento O ponteiro para o `EventoTransporte` a ser processado.
 */
void Simulacao::processar_evento_transporte(EventoTransporte* evento) {
    if (todos_pacotes_entregues()) {
        return;
    }

    double tempo_atual = evento->tempo;
    if (!this->transporte_config) return;

    Pilha<Pacote*>& secao = armazens[evento->id_armazem_origem]->obter_secao(evento->id_armazem_destino);
    
    if (secao.esta_vazia()) {
        if (!todos_pacotes_entregues()) {
            escalonador->insere_evento(new EventoTransporte(tempo_atual + this->transporte_config->intervalo, evento->id_armazem_origem, evento->id_armazem_destino));
        }
        return;
    }

    VetorDinamico<Pacote*> pacotes_na_pilha;
    while (!secao.esta_vazia()) {
        pacotes_na_pilha.adicionar(secao.desempilha());
    }

    VetorDinamico<Pacote*> pacotes_ordenados = pacotes_na_pilha;
    
    for (int i = 0; i < pacotes_ordenados.tamanho() - 1; i++) {
        for (int j = 0; j < pacotes_ordenados.tamanho() - i - 1; j++) {
            if (pacotes_ordenados[j]->tempo_postagem > pacotes_ordenados[j + 1]->tempo_postagem) {
                Pacote* temp = pacotes_ordenados[j];
                pacotes_ordenados[j] = pacotes_ordenados[j + 1];
                pacotes_ordenados[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < 10; ++i){
        //std::cout << pacotes_ordenados[i]->display_id << " ";
    }

    VetorDinamico<Pacote*> para_transportar;
    for (int i = 0; i < pacotes_ordenados.tamanho() && para_transportar.tamanho() < this->transporte_config->capacidade; ++i) {
        para_transportar.adicionar(pacotes_ordenados[i]);
    }

    // Calcula o tempo final da operação de remoção de todos os pacotes da pilha.
    // Este tempo será usado para todos os logs de remoção e trânsito, garantindo consistência.
    double custo_total_remocao = pacotes_na_pilha.tamanho() * this->transporte_config->custo_remocao;
    // Adiciona um log de depuração para verificar os valores de tempo e latência
    std::cout << "DEBUG: tempo_atual=" << tempo_atual << ", latencia=" << this->transporte_config->latencia << ", custo_remocao=" << custo_total_remocao << std::endl;

    // Reverte para a fórmula que estava mais próxima, para continuar a investigação.
    double tempo_final_operacao = tempo_atual + this->transporte_config->capacidade + custo_total_remocao;

    VetorDinamico<Pacote*> pacotes_a_reempilhar;
    VetorDinamico<double> tempos_reempilhar;
    double tempo_op_intermediario = tempo_atual; // Tempo para logs de re-armazenamento

    // Processa todos os pacotes que estavam na pilha
    for (int i = 0; i < pacotes_na_pilha.tamanho(); i++) {
        Pacote* p = pacotes_na_pilha[i];
        tempo_op_intermediario += this->transporte_config->custo_remocao;

        // Log de remoção com o timestamp final
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_final_operacao << " pacote " << std::setw(3) << std::setfill('0') << p->display_id << " removido de " << std::setw(3) << std::setfill('0') << evento->id_armazem_origem << " na secao " << std::setw(3) << std::setfill('0') << evento->id_armazem_destino << std::endl;

        // Verifica se o pacote será transportado ou precisa ser reempilhado
        bool sera_transportado = false;
        for (int j = 0; j < para_transportar.tamanho(); ++j) {
            if (p->id == para_transportar[j]->id) {
                sera_transportado = true;
                break;
            }
        }

        if (sera_transportado) {
            p->atualizar_estado(EstadoPacote::REMOVIDO_PARA_TRANSPORTE);
        } else {
            pacotes_a_reempilhar.adicionar(p);
            tempos_reempilhar.adicionar(tempo_op_intermediario); // Usa o tempo incremental para reempilhar
        }
    }

    // Processa os pacotes que serão transportados
    for (int i = 0; i < para_transportar.tamanho(); ++i) {
        Pacote* p = para_transportar[i];
        // Log de trânsito com o timestamp final
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_final_operacao << " pacote " << std::setw(3) << std::setfill('0') << p->display_id << " em transito de " << std::setw(3) << std::setfill('0') << evento->id_armazem_origem << " para " << std::setw(3) << std::setfill('0') << evento->id_armazem_destino << std::endl;
        
        // Agenda o evento de chegada no destino
        escalonador->insere_evento(new EventoChegada(tempo_final_operacao + this->transporte_config->latencia, p->id, evento->id_armazem_destino));
    }

    for (int i = pacotes_a_reempilhar.tamanho() - 1; i >= 0; i--) {
        Pacote* p = pacotes_a_reempilhar[i];
        double tempo_rearmazenamento = tempos_reempilhar[i];
        secao.empilha(p);
        std::cout << std::setw(7) << std::setfill('0') << (int)tempo_rearmazenamento << " pacote " << std::setw(3) << std::setfill('0') << p->display_id << " rearmazenado em " << std::setw(3) << std::setfill('0') << evento->id_armazem_origem << " na secao " << std::setw(3) << std::setfill('0') << evento->id_armazem_destino << std::endl;
    }

    if (!todos_pacotes_entregues()) {
        escalonador->insere_evento(new EventoTransporte(tempo_atual + this->transporte_config->intervalo, evento->id_armazem_origem, evento->id_armazem_destino));
    }
}
