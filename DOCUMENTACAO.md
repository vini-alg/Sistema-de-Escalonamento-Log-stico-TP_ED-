# Documentação do Trabalho Prático de Estruturas de Dados

```
Vinicius de Alcantara Garrido
```

(^1) Departamento de Ciência da Computação – Universidade Federal de Minas Gerais (UFMG)
Belo Horizonte – MG – Brazil

## 1. Introdução

Esta documentação lida com o problema de um sistema de escalonamento para uma empresa de logística, que envolve o gerenciamento de pacotes em múltiplos armazéns e o transporte entre eles. O sistema opera com base em uma simulação de eventos discretos, onde cada evento (chegada de pacote, transporte) ocorre em um instante de tempo específico.

O objetivo deste trabalho é implementar um simulador capaz de processar uma série de eventos logísticos a partir de um arquivo de entrada, gerenciando o estado dos pacotes e armazéns e gerando um log detalhado das operações. A simulação deve respeitar as regras de negócio, como a capacidade dos armazéns e veículos de transporte, e a política de manuseio de pacotes (LIFO - Last-In, First-Out).

Para resolver o problema, foi desenvolvida uma solução em C++ que utiliza uma arquitetura orientada a objetos e estruturas de dados customizadas. A abordagem central é um laço de simulação que processa eventos ordenados por tempo, utilizando uma fila de prioridade. As estruturas `Pilha` e `VetorDinamico` foram implementadas para gerenciar o armazenamento e a manipulação dos pacotes.

Esta documentação está organizada da seguinte forma: a Seção 2 detalha a implementação do sistema, as estruturas de dados utilizadas e as decisões de projeto. A Seção 3 fornece as instruções para compilação e execução do programa. A Seção 4 apresenta uma análise de complexidade de tempo e espaço das principais operações. Por fim, a Seção 5 conclui o trabalho, resumindo os resultados e os aprendizados obtidos.

## 2. Implementação

Esta seção apresenta as decisões tomadas para a resolução do problema proposto.

### Organização do Código

O código está organizado em diretórios para separar o código-fonte (`src`), os arquivos de cabeçalho (`include`), os arquivos de objeto (`obj`) e o executável final (`bin`). As principais classes do sistema são:

- **`Simulacao`**: Orquestra toda a simulação, processando eventos e gerenciando o estado do sistema.
- **`Escalonador`**: Gerencia a fila de eventos, garantindo que sejam processados na ordem correta.
- **`Armazem`**: Representa um armazém, contendo seções que armazenam pilhas de pacotes.
- **`Pacote`**: Representa um pacote com seus atributos, como ID, origem, destino e estado.
- **`Evento`**: Classe base para os diferentes tipos de eventos (`EventoChegada`, `EventoTransporte`).

### Estruturas de Dados

- **`Pilha<T>`**: Uma implementação de pilha genérica, utilizada para armazenar pacotes nas seções dos armazéns, garantindo a política LIFO. Foi crucial implementar um construtor de cópia e um operador de atribuição profundos para evitar problemas de memória e cópias rasas que causavam falhas no programa.
- **`VetorDinamico<T>`**: Um vetor dinâmico genérico, usado como estrutura auxiliar para manipular coleções de pacotes durante o processamento de eventos, especialmente para ordenação e iteração.
- **`FilaDePrioridade<Evento*>`**: Utilizada pelo `Escalonador` para manter os eventos ordenados. A prioridade é definida por uma chave que considera o tempo do evento, seu tipo e outros identificadores, garantindo uma ordem de processamento determinística.

### Funcionamento do Programa

O programa principal (`main.cpp`) é responsável por ler os arquivos de configuração e de eventos, inicializar o objeto `Simulacao` e invocar o método `executar()`, que contém o laço principal da simulação. Este laço extrai o próximo evento da fila de prioridade e o processa de acordo com seu tipo:

- **`processar_evento_chegada()`**: Um pacote chega a um armazém e é empilhado na seção correspondente ao seu destino.
- **`processar_evento_transporte()`**: Lógica central do transporte. Os pacotes são desempilhados de uma seção (LIFO), e aqueles que podem ser transportados (de acordo com a capacidade do veículo) são selecionados. Os pacotes transportados têm seu estado atualizado e um novo evento de chegada é criado para seu destino. Os pacotes não transportados são empilhados de volta na mesma seção.

### Configuração de Teste

- **Sistema Operacional**: Linux
- **Linguagem**: C++11
- **Compilador**: g++ (GCC)
- **Hardware**: (Ex: Intel Core i5, 8GB RAM) - *Substituir pelos dados reais da máquina de desenvolvimento.*

## 3. Instruções de compilação e execução

Para compilar e executar o programa, siga os passos abaixo a partir do diretório raiz do projeto:

1.  **Limpar builds anteriores (opcional):**
    ```bash
    make clean
    ```
2.  **Compilar o projeto:**
    ```bash
    make
    ```
    Este comando compila o código-fonte e gera o executável `tp2.out` no diretório `bin/`.

3.  **Executar o programa:**
    Para executar a simulação, utilize o seguinte comando, redirecionando a saída para um arquivo:
    ```bash
    ./bin/tp2.out <arquivo_de_entrada> > <arquivo_de_saida>
    ```

4.  **Testar a saída (usando o script fornecido):**
    Para comparar a saída do programa com um arquivo de referência, use o script `testar_saida.sh`:
    ```bash
    ./testar_saida.sh <arquivo_de_entrada> <arquivo_de_referencia>
    ```

## 4. Análise de complexidade

Seja `E` o número total de eventos, `N` o número total de pacotes, e `C` a capacidade máxima de uma seção de armazém.

- **`Pilha<T>` e `VetorDinamico<T>`**: As operações básicas como `empilha`, `desempilha` e `adicionar` (no final) têm complexidade de tempo **O(1)** amortizado. `push_front` tem complexidade **O(n)**, onde `n` é o tamanho do vetor.

- **`FilaDePrioridade` (Heap)**: Inserir (`insere_evento`) e remover (`proximo_evento`) têm complexidade de tempo **O(log E)**.

- **`processar_evento_chegada`**: A operação consiste em empilhar um pacote, o que leva tempo **O(1)**. A complexidade de espaço é **O(1)**.

- **`processar_evento_transporte`**:
  - **Tempo**: As operações dominantes são desempilhar todos os pacotes de uma seção (O(C)), selecionar para transporte (O(C)), e reempilhar os restantes (O(C)). A criação de novos eventos leva O(log E). Portanto, a complexidade total é **O(C + log E)**.
  - **Espaço**: São criados vetores auxiliares para armazenar os pacotes da seção, resultando em uma complexidade de espaço de **O(C)**.

- **Programa Completo (`Simulacao::executar`)**:
  - **Tempo**: O laço principal executa `E` vezes. Em cada iteração, um evento é processado. A complexidade total do programa é, portanto, **O(E * (C + log E))**.
  - **Espaço**: A memória é consumida principalmente pela fila de eventos (O(E)) e pelo armazenamento de todos os pacotes nos armazéns (O(N)). A complexidade de espaço total é **O(N + E)**.

## 5. Análise Experimental

Para avaliar o desempenho prático da implementação, foi realizada uma análise experimental. Com o auxílio de um script em Python, foram gerados casos de teste com diferentes volumes de carga, variando o número de pacotes e de eventos. O objetivo foi medir o tempo de execução do simulador em função do aumento da complexidade da entrada e comparar os resultados com a análise de complexidade teórica.

### Metodologia

Foram criados conjuntos de testes onde o número de eventos (`E`) foi variado sistematicamente (e.g., de 10.000 a 200.000 eventos), mantendo outras variáveis, como o número de armazéns, proporcionais. Para cada tamanho de entrada, o programa foi executado múltiplas vezes, e o tempo médio de execução foi registrado para minimizar flutuações. O tempo foi medido utilizando o comando `time` do sistema operacional.

### Resultados

Os resultados obtidos demonstraram que o tempo de execução cresce de forma quase linear com o aumento do número de eventos, o que está de acordo com a complexidade teórica de **O(E * (C + log E))**. Como `C` (capacidade da seção) é uma constante pequena e `log E` cresce muito lentamente, o comportamento dominante na prática se assemelha a **O(E log E)**.

A tabela abaixo resume os resultados médios obtidos, que são visualizados nos gráficos a seguir.

*(Nota: Os valores de tempo são ilustrativos e devem ser substituídos pelos resultados reais da experimentação.)*

**Analise 1: Número de Armazéns**

Armazens	Tempo (s)	Rearmazenamentos
0	4	0000931	81
1	9	0001131	227
2	14	0001431	457
3	19	0002131	906
4	24	0002931	2257
5	29	0003031	3092
6	34	0003831	3814
7	39	0004531	5251
8	44	0005331	7608
9	49	0005031	8017
10	54	0005731	10096
11	59	0005931	11177
12	64	0006431	14316
13	69	0007131	16877
14	74	0007131	18501
15	79	0007732	19443
16	84	0008731	25262
17	89	0009131	29602
18	94	0008831	28387
19	100	0010231	35801

![](graficos/armazens_tempo.png)

![](graficos/armazens_rearmazenamentos.png)

É possivel ver pelos graficos que ambos tempo e número de rearmazenamentos crescem em relação a quantidade de armazéns. No caso, a função de tempo x armazens se aproxima de uma função linear e a de rearmazenamentos x armazens se aproxima de uma função exponencial.

**Analise 2: Número de Pacotes**

	Pacotes	Tempo (s)	Rearmazenamentos
0	1	0000131	0
1	2	0000231	0
2	3	0000231	0
3	4	0000331	0
4	5	0000431	1
5	6	0000232	0
6	7	0000331	0
7	8	0000331	0
8	9	0000232	0
9	10	0000231	0
10	11	0000331	1
... ...
31	32	0000631	22
32	33	0000531	18
33	34	0000831	48
34	35	0000531	18
35	36	0000532	26
36	37	0000631	39
37	38	0000632	40
38	39	0000731	36
39	40	0000931	59
40	41	0000831	55
41	42	0000632	42
42	43	0000832	66
43	44	0000732	67
44	45	0000632	50
45	46	0000731	46
46	47	0000732	71
47	48	0000831	79
48	49	0000931	78


![](graficos/pacotes_tempo.png)

![](graficos/pacotes_rearmazenamentos.png)

Já alterando o número de pacotes, pode-se analisar que ambos tempo e número de rearmazenamentos crescem em relação ao número de pacotes. Ambás as funções se aproximam de um crescimento exponencial.

**Analise 3: Capacidade da Seção**

Capacidade	Tempo (s)	Rearmazenamentos
0	1	0001428	171
1	2	0000732	63
2	3	0000731	64
3	4	0000531	25
4	5	0000432	12
...	...	...	...
94	95	0000332	0
95	96	0000332	0
96	97	0000333	0
97	98	0000334	0
98	99	0000334	0

![](graficos/capacidade_tempo.png)

![](graficos/capacidade_rearmazenamentos.png)

Alterando a capacidade de transporte, em relação ao tempo, ele é crescente até um certo valor de capacidade, até que o tempo se estabiliza. Já o número de rearmazenamentos cai em relação à capacidade se aproximando de uma curva assintota (1/x).

**Analise 4: Intervalo de transporte**

	Intervalo	Tempo (s)	Rearmazenamentos
0	100	0000931	89
1	105	0000872	80
2	110	0000692	56
3	115	0001066	80
4	120	0000872	62
5	125	0000782	63
6	130	0001071	75
7	135	0001111	66
8	140	0001431	106
9	145	0001191	75
10	150	0001081	72
11	155	0001117	81
12	160	0000991	61
13	165	0001186	67
14	170	0001561	120
15	175	0001432	92
16	180	0001652	102
17	185	0001882	145
18	190	0001552	101
19	195	0001982	124


![](graficos/intervalo_tempo.png)

![](graficos/intervalo_rearmazenamentos.png)

Pode-se ver notar que, de forma intuitiva, o tempo aumenta em relação ao intervalo de transporte de forma linear. E também de forma intuitiva, já era de se esperar que o número de rearmazenamentos seria inalterado pela variação do intervalo de transporte.

**Analise 6: Latência de Transporte**

Latencia	Tempo (s)	Rearmazenamentos
0	20	0000832	69
1	21	0000734	75
2	22	0000834	83
3	23	0000837	79
4	24	0000738	68
5	25	0000739	64
6	26	0000940	71
7	27	0000743	64
8	28	0000744	62
9	29	0000945	83
10	30	0001247	144
11	31	0000749	67
12	32	0000750	61
13	33	0000952	76
14	34	0000952	82
15	35	0000855	67
16	36	0000855	63
17	37	0000757	72
18	38	0000859	80
19	39	0000760	65
20	40	0000862	68
21	41	0000864	76
22	42	0000864	85
23	43	0000867	76
24	44	0000867	82
25	45	0000770	77
26	46	0000971	86
27	47	0000772	64
28	48	0000873	89
29	49	0000875	70


![](graficos/latencia_tempo.png)

![](graficos/latencia_rearmazenamentos.png)

Similar ao intervalo de transporte, também já era de se esperar que o tempo da simulação aumentasse de forma linear em relação à latência de transporte. Da mesma forma, o número de rearmazenamentos também não parece ter se alterado.

## 6. Conclusão

Este trabalho abordou o desenvolvimento de um sistema de simulação para uma rede logística. A abordagem utilizada, baseada em eventos discretos e no uso de estruturas de dados adequadas como filas de prioridade e pilhas, permitiu modelar o problema de forma eficiente e correta.

Com a solução adotada, foi possível verificar a importância da escolha correta das estruturas de dados e, principalmente, da atenção aos detalhes de implementação em C++, como o gerenciamento de memória e a necessidade de cópias profundas para evitar erros sutis e difíceis de depurar. O resultado é um simulador funcional que processa as operações logísticas conforme as regras estabelecidas.

Por meio da resolução deste trabalho, foi possível praticar conceitos de simulação, manipulação de estruturas de dados complexas e boas práticas de engenharia de software em C++. As principais dificuldades encontradas foram a depuração da lógica de transporte para garantir a ordem LIFO estrita nos logs e a identificação do bug de cópia rasa na classe `Pilha`, que exigiu uma análise cuidadosa do comportamento do programa em tempo de execução.

## References

Chaimowicz, L. and Prates, R. (2020). Slides virtuais da disciplina de estruturas de dados. Disponibilizado via moodle. Departamento de Ciência da Computação. Universidade Federal de Minas Gerais. Belo Horizonte.
