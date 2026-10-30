#!/bin/sh
set -eu
cd "$(dirname "$0")/../.."
echo 'Compilando MicroPascal...'
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
echo 'Compilando executor de testes...'
gcc -std=c11 -Wall -Wextra -Wpedantic testes/test/executar_testes.c -o executar_testes
echo 'Executando os 203 testes...'
./executar_testes ./micropascal
