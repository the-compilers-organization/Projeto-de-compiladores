#!/usr/bin/env bash
set -e

# Obtém o diretório onde este script está localizado
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Se test/manifesto.csv não estiver na pasta atual, tenta ir para a pasta do script
if [ ! -f "test/manifesto.csv" ]; then
    cd "$SCRIPT_DIR"
fi

echo "Compilando MicroPascal..."
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal

echo "Compilando executor de testes..."
gcc -std=c11 -Wall -Wextra -Wpedantic test/executar_testes.c -o executar_testes

echo "Executando os testes do manifesto..."
./executar_testes ./micropascal