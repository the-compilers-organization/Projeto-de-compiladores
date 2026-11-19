#!/bin/sh
set -eu
# Executa a partir da pasta micropascal-compiler/, independentemente do diretorio inicial.
cd "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
echo 'Compilando MicroPascal...'
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
echo 'Compilando executor de testes...'
gcc -std=c11 -Wall -Wextra -Wpedantic test/executar_testes.c -o executar_testes
echo 'Executando os testes do manifesto...'
./executar_testes ./micropascal
