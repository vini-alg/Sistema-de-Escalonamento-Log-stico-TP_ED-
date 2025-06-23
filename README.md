# Documentação do Sistema de Escalonamento Logístico

Este documento fornece uma visão geral detalhada da arquitetura e dos componentes do Sistema de Escalonamento Logístico. A documentação é extraída diretamente dos comentários do código-fonte.

---

## Tabela de Conteúdos
1.  [Armazem](#armazem)
2.  [Escalonador](#escalonador)
3.  [Evento](#evento)
4.  [Fila](#fila)
5.  [Pacote](#pacote)
6.  [Pilha](#pilha)
7.  [Simulacao](#simulacao)
8.  [Transporte](#transporte)
9.  [VetorDinamico](#vetordinamico)

---

## `Armazem`
**Header:** `include/Armazem.hpp`
**Source:** `src/Armazem.cpp`

> Representa uma instalação de armazenamento para pacotes. Um armazém contém múltiplas seções, cada uma sendo uma pilha de pacotes destinada a um armazém de destino específico.

### Interface Pública

#### `Armazem(int id, int total_armazens)`
> Constrói um novo objeto Armazem.
> **Parâmetros:**
> - `id`: O identificador para o armazém.
> - `total_armazens`: O número total de armazéns na simulação, usado para alocar as seções.

#### `~Armazem()`
> Destrói o objeto Armazem, liberando os recursos alocados.

#### `void armazenar_pacote(Pacote* pacote)`
> Adiciona um pacote à seção de armazenamento apropriada. O pacote é colocado na pilha correspondente ao seu próximo destino na rota.
> **Parâmetros:**
> - `pacote`: O pacote a ser armazenado.

#### `Pilha<Pacote*>& obter_secao(int id_armazem_destino)`
> Recupera a seção de empilhamento para um armazém de destino específico.
> **Parâmetros:**
> - `id_armazem_destino`: O ID do armazém de destino.
> **Retorna:** Uma referência para a pilha de pacotes da seção solicitada.

### Membros Privados
- `int id`: Identificador único do armazém.
- `Pilha<Pacote*>* secoes`: Array de pilhas, onde cada pilha representa uma seção de pacotes.
- `int num_secoes`: igual ao número total de armazéns na simulação.

---

## `Escalonador`
**Header:** `include/Escalonador.hpp`
**Source:** `src/Escalonador.cpp`

> Gerencia uma fila de prioridade de eventos usando um min-heap. Esta classe é responsável por agendar e recuperar eventos com base em seu tempo, garantindo que o evento com o menor tempo seja sempre processado primeiro.

### Interface Pública

#### `Escalonador(int capacidade_inicial = 100)`
> Constrói um novo Escalonador com uma capacidade inicial.
> **Parâmetros:**
> - `capacidade_inicial`: A capacidade inicial da fila de eventos.

#### `~Escalonador()`
> Destrói o Escalonador, liberando a memória alocada.

#### `void insere_evento(Evento* evento)`
> Insere um novo evento na fila de prioridade.
> **Parâmetros:**
> - `evento`: O evento a ser adicionado.

#### `Evento* retira_proximo_evento()`
> Remove e retorna o evento com o menor tempo (a raiz do heap).
> **Retorna:** O próximo evento a ser processado, ou nullptr se a fila estiver vazia.

#### `bool vazio() const`
> Verifica se a fila de eventos está vazia.
> **Retorna:** Verdadeiro se a fila estiver vazia, falso caso contrário.

---

## `Evento`
**Header:** `include/Evento.hpp`

> Classe base abstrata para todos os eventos na simulação. Define a interface comum para eventos, incluindo tempo e tipo, e um método virtual puro para obter uma chave de prioridade.

### Enum `TipoEvento`
- `CHEGADA_PACOTE`
- `TRANSPORTE_PACOTES`

### Subclasses

#### `EventoChegada`
> Representa um evento de chegada de pacote em um armazém.

#### `EventoTransporte`
> Representa um evento de transporte de pacotes de um armazém para outro.

---

## `Fila`
**Header:** `include/Fila.hpp`

> Implementa uma estrutura de dados de fila genérica (FIFO).

### Interface Pública

#### `Fila()`
> Constrói uma nova fila vazia.

#### `~Fila()`
> Destrói a fila, liberando todos os seus elementos.

#### `void enfileirar(const T& elemento)`
> Adiciona um elemento ao final da fila.
> **Parâmetros:**
> - `elemento`: O elemento a ser adicionado.

#### `T desenfileirar()`
> Remove e retorna o elemento no início da fila.
> **Retorna:** O elemento removido.

#### `bool esta_vazia() const`
> Verifica se a fila está vazia.
> **Retorna:** Verdadeiro se a fila estiver vazia, falso caso contrário.

#### `int tamanho() const`
> Retorna o número de elementos na fila.
> **Retorna:** O tamanho da fila.

---

## `Pacote`
**Header:** `include/Pacote.hpp`
**Source:** `src/Pacote.cpp`

> Representa um pacote no sistema de logística. Contém informações sobre o pacote, como ID, origem, destino, rota e estado atual, além de estatísticas de tempo para análise de desempenho.

### Enum `EstadoPacote`
- `NAO_POSTADO`
- `CHEGADA_ESCALONADA`
- `ARMAZENADO`
- `REMOVIDO_PARA_TRANSPORTE`
- `ENTREGUE`

### Interface Pública

#### `Pacote(int id, int tempo_postagem, int origem, int destino)`
> Constrói um novo objeto Pacote.
> **Parâmetros:**
> - `id`: O ID único do pacote.
> - `tempo_postagem`: O tempo de postagem do pacote.
> - `origem`: O armazém de origem.
> - `destino`: O armazém de destino.

#### `void definir_rota(const VetorDinamico<int>& nova_rota)`
> Define a rota para o pacote.
> **Parâmetros:**
> - `nova_rota`: A rota a ser seguida pelo pacote.

#### `int obter_proximo_destino()`
> Obtém o próximo armazém de destino na rota do pacote.
> **Retorna:** O ID do próximo armazém, ou -1 se a rota estiver completa.

#### `void avancar_rota()`
> Avança o pacote para o próximo armazém em sua rota.

#### `void atualizar_estado(EstadoPacote novo_estado)`
> Atualiza o estado atual do pacote.
> **Parâmetros:**
> - `novo_estado`: O novo estado do pacote.

---

## `Pilha`
**Header:** `include/Pilha.hpp`

> Implementa uma estrutura de dados de pilha genérica (LIFO).

### Interface Pública

#### `Pilha()`
> Constrói uma nova pilha vazia.

#### `~Pilha()`
> Destrói a pilha, liberando todos os seus elementos.

#### `void empilha(T elemento)`
> Adiciona um elemento ao topo da pilha (push).
> **Parâmetros:**
> - `elemento`: O elemento a ser adicionado.

#### `T desempilha()`
> Remove e retorna o elemento do topo da pilha (pop).
> **Retorna:** O elemento removido.

#### `T ver_topo() const`
> Retorna o elemento no topo da pilha sem removê-lo (peek).
> **Retorna:** O elemento do topo.

#### `bool esta_vazia() const`
> Verifica se a pilha está vazia.
> **Retorna:** Verdadeiro se a pilha estiver vazia, falso caso contrário.

#### `int obter_tamanho() const`
> Retorna o número de elementos na pilha.
> **Retorna:** O tamanho da pilha.

#### `void limpar()`
> Remove todos os elementos da pilha.

---

## `Simulacao`
**Header:** `include/Simulacao.hpp`
**Source:** `src/Simulacao.cpp`

> Orquestra a simulação de logística, gerenciando o tempo, eventos e estado do sistema.

### Interface Pública

#### `Simulacao(const std::string& nome_arquivo)`
> Construtor da classe Simulacao.
> **Parâmetros:**
> - `nome_arquivo`: Caminho para o arquivo de configuração inicial.

#### `~Simulacao()`
> Destrutor da classe Simulacao, responsável por chamar a limpeza de memória.

#### `void executar()`
> Inicia e executa o loop principal da simulação até que não hajam mais eventos.

---

## `Transporte`
**Header:** `include/Transporte.hpp`
**Source:** `src/Transporte.cpp`

> Armazena as configurações globais para os eventos de transporte na simulação. Esta classe encapsula os parâmetros que definem o comportamento dos transportes entre os armazéns, como capacidade do veículo, latência da rota, intervalo entre transportes e o custo (em tempo) para remover pacotes.

### Interface Pública

#### `Transporte(int cap = 0, int lat = 0, int interv = 0, int custo = 0)`
> Constrói um novo objeto Transporte com as configurações especificadas.
> **Parâmetros:**
> - `cap`: A capacidade do veículo.
> - `lat`: A latência da rota.
> - `interv`: O intervalo entre transportes.
> - `custo`: O custo de tempo para remoção de pacotes.

---

## `VetorDinamico`
**Header:** `include/VetorDinamico.hpp`

> Implementa um vetor dinâmico genérico (array redimensionável).

### Interface Pública

#### `VetorDinamico(int cap_inicial = 10)`
> Constrói um novo vetor dinâmico.
> **Parâmetros:**
> - `cap_inicial`: A capacidade inicial do vetor.

#### `~VetorDinamico()`
> Destrói o vetor dinâmico, liberando a memória alocada.

#### `VetorDinamico(const VetorDinamico& outro)`
> Construtor de cópia.
> **Parâmetros:**
> - `outro`: O vetor a ser copiado.

#### `VetorDinamico& operator=(const VetorDinamico& outro)`
> Operador de atribuição de cópia.
> **Parâmetros:**
> - `outro`: O vetor a ser copiado.
> **Retorna:** Uma referência para este vetor após a cópia.

#### `void adicionar(const T& elemento)`
> Adiciona um elemento ao final do vetor.
> **Parâmetros:**
> - `elemento`: O elemento a ser adicionado.

#### `T& operator[](int indice)`
> Acessa um elemento no índice especificado.
> **Parâmetros:**
> - `indice`: O índice do elemento a ser acessado.
> **Retorna:** Uma referência ao elemento.

#### `const T& operator[](int indice) const`
> Acessa um elemento constante no índice especificado.
> **Parâmetros:**
> - `indice`: O índice do elemento a ser acessado.
> **Retorna:** Uma referência constante ao elemento.

#### `int tamanho() const`
> Retorna o número de elementos no vetor.
> **Retorna:** O tamanho do vetor.

#### `void limpar()`
> Remove todos os elementos do vetor e o redefine para seu estado inicial.

---

## Esquema de Funcionamento do Algoritmo

Esta seção descreve o fluxo de trabalho do simulador de logística, desde a configuração inicial até o processamento de eventos e a conclusão da simulação. Usaremos um exemplo hipotético para ilustrar os principais conceitos.

### Visão Geral do Fluxo de Simulação

O sistema opera com base em uma simulação orientada a eventos, onde as ações (como a chegada de um pacote ou a partida de um transporte) são gerenciadas por um escalonador de eventos de prioridade (min-heap).

1.  **Inicialização**: A simulação começa lendo um arquivo de entrada que define a topologia da rede (armazéns e rotas), a configuração dos transportes e a lista de pacotes a serem entregues.
2.  **Cálculo de Rotas**: Para cada pacote, o algoritmo de Busca em Largura (BFS) é usado para encontrar a rota mais curta (em número de saltos) entre o armazém de origem e o de destino.
3.  **Escalonamento Inicial**: Os eventos iniciais são criados e inseridos no escalonador. Isso inclui:
    *   **Eventos de Chegada**: Para cada pacote, um evento de chegada é agendado no armazém de origem no tempo de postagem especificado.
    *   **Eventos de Transporte**: Para cada rota possível entre armazéns adjacentes, um evento de transporte inicial é agendado.
4.  **Loop de Simulação**: A simulação processa os eventos em ordem cronológica, um por um, até que o escalonador esteja vazio.
5.  **Término**: A simulação termina quando todos os pacotes foram entregues e não há mais eventos a serem processados.

### Exemplo de Simulação Passo a Passo

Vamos considerar um cenário simples para ilustrar o fluxo:

**Configuração Inicial:**

*   **Armazéns**: 3 armazéns (A0, A1, A2).
*   **Rotas**: A0 ↔ A1, A1 ↔ A2.
*   **Transporte**: Capacidade de 10 pacotes, latência de 5 unidades de tempo, intervalo de 100 unidades de tempo, custo de remoção de 1 unidade de tempo.
*   **Pacote P1**: Postado no tempo 0, origem A0, destino A2.

**Passo 1: Carregamento e Escalonamento Inicial**

1.  A `Simulacao` lê a configuração.
2.  A rota para o Pacote P1 é calculada via BFS: `A0 -> A1 -> A2`.
3.  O `Escalonador` recebe os seguintes eventos iniciais:
    *   `EventoChegada(tempo=0, pacote=P1, armazem=A0)`
    *   `EventoTransporte(tempo=100, origem=A0, destino=A1)`
    *   `EventoTransporte(tempo=100, origem=A1, destino=A0)`
    *   `EventoTransporte(tempo=100, origem=A1, destino=A2)`
    *   `EventoTransporte(tempo=100, origem=A2, destino=A1)`

**Passo 2: Loop de Simulação - Processando Eventos**

O loop principal começa, extraindo o evento com o menor tempo do escalonador.

**A. Processando `EventoChegada(tempo=0, pacote=P1, armazem=A0)`**

1.  O tempo da simulação avança para 0.
2.  O Pacote P1 chega ao Armazém A0.
3.  Como o próximo destino de P1 é A1, ele é armazenado na seção de A0 que leva a A1. Ele é colocado no topo da `Pilha` dessa seção.
4.  O estado de P1 é atualizado para `ARMAZENADO`.

**B. Processando `EventoTransporte(tempo=100, origem=A0, destino=A1)`**

1.  O tempo da simulação avança para 100.
2.  O Armazém A0 verifica sua seção de pacotes para A1.
3.  Ele encontra o Pacote P1. Como a capacidade do veículo (10) não foi excedida, P1 é preparado para transporte.
4.  O custo de remoção é aplicado. O tempo da operação é `100 + 1 = 101`.
5.  Um novo `EventoChegada` para P1 é escalonado para o Armazém A1 no futuro: `EventoChegada(tempo=101 + 5, pacote=P1, armazem=A1)`, onde 5 é a latência.
6.  Um novo `EventoTransporte` de A0 para A1 é agendado para o próximo ciclo: `EventoTransporte(tempo=100 + 100, origem=A0, destino=A1)`.

**C. Processando `EventoChegada(tempo=106, pacote=P1, armazem=A1)`**

1.  O tempo avança para 106.
2.  P1 chega ao Armazém A1. Sua rota é avançada.
3.  O próximo destino é A2. P1 é armazenado na seção de A1 para A2.

Este ciclo continua, com o transporte de A1 para A2 sendo acionado, até que P1 chegue ao seu destino final (A2), momento em que seu estado é alterado para `ENTREGUE`.

**Passo 3: Fim da Simulação**

Quando o escalonador de eventos fica vazio e todos os pacotes estão no estado `ENTREGUE`, a simulação é concluída e as estatísticas finais são apresentadas.
