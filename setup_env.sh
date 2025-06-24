#!/bin/bash

# Verifica se o ambiente virtual já existe
if [ -d ".venv" ]; then
    echo "O ambiente virtual já existe. Pulando criação."
else
    # Cria o ambiente virtual chamado "venv"
    python3 -m venv .venv
    echo "Ambiente virtual criado com sucesso."
fi

# Ativa o ambiente virtual
source .venv/bin/activate

# Instala as bibliotecas listadas no requirements.txt
if [ -f "requirements.txt" ]; then
    pip install -r requirements.txt
    echo "Bibliotecas instaladas com sucesso."
else
    echo "Arquivo requirements.txt não encontrado."
fi
