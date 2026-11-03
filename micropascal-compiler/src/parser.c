#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"

static void avancar(Parser *p) { p->token_atual = lexer_proximo_token(p->lexer); }
static void erro(Parser *p) {
    if (p->token_atual.tipo != TOKEN_ERRO)
        fprintf(stderr, "Erro de sintaxe no token [%s]\n", p->token_atual.lexema);
    exit(EXIT_FAILURE);
}
static void esperar(Parser *p, TipoToken tipo) {
    if (p->token_atual.tipo != tipo) erro(p);
    avancar(p);
}
static int em(TipoToken t, const TipoToken *ts, size_t n) {
    for (size_t i = 0; i < n; ++i) if (ts[i] == t) return 1;
    return 0;
}
static const TipoToken inicio[] = {TOKEN_BEGIN,TOKEN_IDENTIFICADOR,TOKEN_WHILE,TOKEN_REPEAT,TOKEN_IF,TOKEN_WRITE};
static const TipoToken logicos[] = {TOKEN_OR,TOKEN_AND};
static const TipoToken relacionais[] = {TOKEN_IGUAL,TOKEN_DIFERENTE,TOKEN_MENOR,TOKEN_MAIOR,TOKEN_MENOR_IGUAL,TOKEN_MAIOR_IGUAL};
static const TipoToken aditivos[] = {TOKEN_MAIS,TOKEN_MENOS};
static const TipoToken multiplicativos[] = {TOKEN_MULT,TOKEN_DIV_REAL,TOKEN_DIV_INT};

static void expressao(Parser *p);
static void comando(Parser *p);
static void bloco(Parser *p);

static void fator(Parser *p) {
    switch (p->token_atual.tipo) {
        case TOKEN_ABRE_PAREN:
            avancar(p);
            expressao(p);
            esperar(p,TOKEN_FECHA_PAREN);
            return;
        case TOKEN_NOT:
            avancar(p);
            fator(p);
            return;
        case TOKEN_INTEIRO_LITERAL:
        case TOKEN_REAL_LITERAL:
        case TOKEN_CHAR_LITERAL:
        case TOKEN_IDENTIFICADOR:
            avancar(p);
            return;
        default: erro(p);
    }
}
static void termo(Parser *p) {
    fator(p);
    while (em(p->token_atual.tipo,multiplicativos,3)) { avancar(p); fator(p); }
}
static void soma(Parser *p) {
    termo(p);
    while (em(p->token_atual.tipo,aditivos,2)) { avancar(p); termo(p); }
}
static void relacional(Parser *p) {
    soma(p);
    while (em(p->token_atual.tipo,relacionais,6)) { avancar(p); soma(p); }
}
static void expressao(Parser *p) {
    relacional(p);
    while (em(p->token_atual.tipo,logicos,2)) { avancar(p); relacional(p); }
}
static void declaracao(Parser *p) {
    esperar(p,TOKEN_IDENTIFICADOR);
    while (p->token_atual.tipo == TOKEN_VIRGULA) {
        avancar(p);
        esperar(p,TOKEN_IDENTIFICADOR);
    }
    esperar(p,TOKEN_DOISPONTOS);
    TipoToken tipo = p->token_atual.tipo;
    if (tipo != TOKEN_INTEGER && tipo != TOKEN_REAL && tipo != TOKEN_CHAR) erro(p);
    avancar(p);
    esperar(p,TOKEN_PONTOVIRG);
}
static void bloco(Parser *p) {
    esperar(p,TOKEN_BEGIN);
    while (em(p->token_atual.tipo,inicio,6)) comando(p);
    esperar(p,TOKEN_END);
}
static void comando(Parser *p) {
    switch (p->token_atual.tipo) {
        case TOKEN_BEGIN:
            bloco(p);
            esperar(p,TOKEN_PONTOVIRG);
            return;
        case TOKEN_IDENTIFICADOR:
            avancar(p);
            esperar(p,TOKEN_ATRIBUICAO);
            expressao(p);
            esperar(p,TOKEN_PONTOVIRG);
            return;
        case TOKEN_WHILE:
            avancar(p);
            expressao(p);
            esperar(p,TOKEN_DO);
            comando(p);
            return;
        case TOKEN_REPEAT:
            avancar(p);
            comando(p);
            esperar(p,TOKEN_UNTIL);
            expressao(p);
            esperar(p,TOKEN_PONTOVIRG);
            return;
        case TOKEN_IF:
            avancar(p);
            expressao(p);
            esperar(p,TOKEN_THEN);
            comando(p);
            if (p->token_atual.tipo == TOKEN_ELSE) { avancar(p); comando(p); }
            return;
        case TOKEN_WRITE:
            avancar(p);
            esperar(p,TOKEN_ABRE_PAREN);
            expressao(p);
            esperar(p,TOKEN_FECHA_PAREN);
            esperar(p,TOKEN_PONTOVIRG);
            return;
        default: erro(p);
    }
}
void parser_inicializar(Parser *p, Lexer *lexer) {
    p->lexer = lexer;
    avancar(p);
}
void parser_analisar_programa(Parser *p) {
    esperar(p,TOKEN_PROGRAM);
    esperar(p,TOKEN_IDENTIFICADOR);
    esperar(p,TOKEN_PONTOVIRG);
    esperar(p,TOKEN_VAR);
    while (p->token_atual.tipo == TOKEN_IDENTIFICADOR) declaracao(p);
    bloco(p);
    esperar(p,TOKEN_PONTO);
    if (p->token_atual.tipo != TOKEN_EOF) erro(p);
}
