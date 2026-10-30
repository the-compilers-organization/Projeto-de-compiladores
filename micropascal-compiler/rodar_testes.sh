#!/bin/sh
# Compila o micro-Pascal e roda todos os testes da pasta testes/
# Uso: dentro da pasta micropascal-compiler, execute  sh rodar_testes.sh
cd "$(dirname "$0")" || exit 1

echo "Compilando..."
gcc -Wall -Wextra -std=c11 -Iinclude src/*.c -o micropascal || { echo "Falha na compilacao."; exit 1; }
echo

teste() {
    echo "=================================================="
    echo "  $1"
    echo "  Arquivo: $2"
    echo "=================================================="
    cat "$2"
    echo
    echo "---- Saida do compilador:"
    if ./micropascal "$2"; then
        echo "[nenhuma mensagem: programa aceito]"
    fi
    echo
}

teste "Exemplo 1 do enunciado: TestaParidade" testes/exemplo1.pas
teste "Exemplo 2 do enunciado: SomaImpares" testes/exemplo2.pas
teste "Teste do lexer: todos os tipos de token" testes/teste_lexer.pas
teste "Erro lexico: caractere que nao existe na linguagem" testes/e1.pas
teste "Erro lexico: char literal com dois caracteres" testes/e2.pas
teste "Erro de sintaxe: falta o ponto e virgula" testes/e3.pas
teste "Erro de sintaxe: Begin com B maiusculo" testes/e5.pas
