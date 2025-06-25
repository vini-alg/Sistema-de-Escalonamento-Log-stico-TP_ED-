#!/bin/bash

# Verifica se o número de argumentos está correto
if [ "$#" -ne 2 ]; then
    echo "Uso: $0 <arquivo_entrada> <arquivo_referencia>"
    echo "Exemplo: $0 e1/ex1 e1/sched.log"
    exit 1
fi

ARQUIVO_ENTRADA="$1"
ARQUIVO_REFERENCIA="$2"
ARQUIVO_SAIDA="/tmp/saida_$(date +%s).log"

# Verifica se o arquivo de entrada existe
if [ ! -f "$ARQUIVO_ENTRADA" ]; then
    echo "Erro: Arquivo de entrada '$ARQUIVO_ENTRADA' não encontrado."
    exit 1
fi

# Verifica se o arquivo de referência existe
if [ ! -f "$ARQUIVO_REFERENCIA" ]; then
    echo "Erro: Arquivo de referência '$ARQUIVO_REFERENCIA' não encontrado."
    exit 1
fi

echo "=== Iniciando teste ==="
echo "Arquivo de entrada: $ARQUIVO_ENTRADA"
echo "Arquivo de referência: $ARQUIVO_REFERENCIA"

# Compila o projeto (opcional, descomente se necessário)
# echo "Compilando o projeto..."
# make clean
# make all

# Executa o programa e salva a saída
echo "Executando o programa com entrada: $ARQUIVO_ENTRADA"
./bin/tp2.out "$ARQUIVO_ENTRADA" > "$ARQUIVO_SAIDA" 2>&1

# Verifica se a execução foi bem-sucedida
if [ $? -ne 0 ]; then
    echo "Erro: Falha ao executar o programa."
    rm -f "$ARQUIVO_SAIDA"
    exit 1
fi

# Compara as saídas
echo "\nComparando saída com a referência..."
if diff -q "$ARQUIVO_SAIDA" "$ARQUIVO_REFERENCIA" > /dev/null; then
    echo "Teste PASSOU! As saídas são idênticas."
    rm -f "$ARQUIVO_SAIDA"
    exit 0
else
    echo "Teste FALHOU! As saídas são diferentes."
    echo "\nDiferenças encontradas (saída | referência):"
    echo "----------------------------------------"
    diff -y "$ARQUIVO_SAIDA" "$ARQUIVO_REFERENCIA" | head -n 20
    
    # Mostra o total de linhas diferentes
    DIFERENCAS=$(diff -y --suppress-common-lines "$ARQUIVO_SAIDA" "$ARQUIVO_REFERENCIA" | wc -l)
    echo "\nTotal de linhas diferentes: $DIFERENCAS"
    
    # Opção para ver as diferenças completas
    echo "\nDeseja ver todas as diferenças? [s/N]"
    read -r RESPOSTA
    if [[ "$RESPOSTA" =~ ^[Ss]$ ]]; then
        diff -y "$ARQUIVO_SAIDA" "$ARQUIVO_REFERENCIA" | less
    fi
    
    # Salva a saída para análise posterior
    echo "\nSaída do programa salva em: $ARQUIVO_SAIDA"
    exit 1
fi
