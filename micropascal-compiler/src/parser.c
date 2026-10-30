#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"

static void parser_avancar(Parser *parser);
static void parser_erro(Parser *parser);
static void parser_esperar(Parser *parser, TipoToken tipo);
static int token_em(TipoToken tipo, const TipoToken *conjunto, int n);

static void parser_secao_var(Parser *parser);
static void parser_decl_var(Parser *parser);
static void parser_tipo(Parser *parser);
static void parser_bloco(Parser *parser);
static void parser_lista_comandos(Parser *parser);
static void parser_comando(Parser *parser);
static void parser_atribuicao(Parser *parser);
static void parser_iteracao(Parser *parser);
static void parser_decisao(Parser *parser);
static void parser_escrita(Parser *parser);

static void parser_expressao(Parser *parser);
static void parser_relacional(Parser *parser);
static void parser_soma(Parser *parser);
static void parser_termo(Parser *parser);
static void parser_fator(Parser *parser);

static const TipoToken INICIO_COMANDO[] = {
    TOKEN_BEGIN, TOKEN_IDENTIFICADOR, TOKEN_WHILE, TOKEN_REPEAT, TOKEN_IF, TOKEN_WRITE
};
static const TipoToken OP_OR_AND[] = { TOKEN_OR, TOKEN_AND };
static const TipoToken OP_RELACIONAL[] = {
    TOKEN_IGUAL, TOKEN_DIFERENTE, TOKEN_MENOR, TOKEN_MAIOR, TOKEN_MENOR_IGUAL, TOKEN_MAIOR_IGUAL
};
static const TipoToken OP_SOMA[] = { TOKEN_MAIS, TOKEN_MENOS };
static const TipoToken OP_TERMO[] = { TOKEN_MULT, TOKEN_DIV_REAL, TOKEN_DIV_INT };

static int token_em(TipoToken tipo, const TipoToken *conjunto, int n) {
    for (int i = 0; i < n; i++) {
        if (conjunto[i] == tipo) return 1;
    }
    return 0;
}

static void parser_avancar(Parser *parser) {
    parser->token_atual = lexer_proximo_token(parser->lexer);
}

static void parser_erro(Parser *parser) {
     if (parser->token_atual.tipo != TOKEN_ERRO) {
        fprintf(stderr, "Erro de sintaxe no token [%s]\n", parser->token_atual.lexema);
    }
    exit(1);
}

static void parser_esperar(Parser *parser, TipoToken tipo) {
    if (parser->token_atual.tipo != tipo) parser_erro(parser);
    parser_avancar(parser);
}

void parser_inicializar(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser_avancar(parser);
}

void parser_analisar_programa(Parser *parser) {
    parser_esperar(parser, TOKEN_PROGRAM);
    parser_esperar(parser, TOKEN_IDENTIFICADOR);
    parser_esperar(parser, TOKEN_PONTOVIRG);
    parser_secao_var(parser);
    parser_bloco(parser);
    parser_esperar(parser, TOKEN_PONTO);
    if (parser->token_atual.tipo != TOKEN_EOF) parser_erro(parser);
}

static void parser_secao_var(Parser *parser) {
    parser_esperar(parser, TOKEN_VAR);
    while (parser->token_atual.tipo == TOKEN_IDENTIFICADOR) parser_decl_var(parser);
}

static void parser_decl_var(Parser *parser) {
    parser_esperar(parser, TOKEN_IDENTIFICADOR);
    while (parser->token_atual.tipo == TOKEN_VIRGULA) {
        parser_avancar(parser);
        parser_esperar(parser, TOKEN_IDENTIFICADOR);
    }
    parser_esperar(parser, TOKEN_DOISPONTOS);
    parser_tipo(parser);
    parser_esperar(parser, TOKEN_PONTOVIRG);
}

static void parser_tipo(Parser *parser) {
    TipoToken t = parser->token_atual.tipo;
    if (t != TOKEN_INTEGER && t != TOKEN_REAL && t != TOKEN_CHAR) parser_erro(parser);
    parser_avancar(parser);
}

static void parser_bloco(Parser *parser) {
    parser_esperar(parser, TOKEN_BEGIN);
    parser_lista_comandos(parser);
    parser_esperar(parser, TOKEN_END);
}

static void parser_lista_comandos(Parser *parser) {
    while (token_em(parser->token_atual.tipo, INICIO_COMANDO, 6)) parser_comando(parser);
}

static void parser_comando(Parser *parser) {
    switch (parser->token_atual.tipo) {
        case TOKEN_BEGIN:
            parser_bloco(parser);
            parser_esperar(parser, TOKEN_PONTOVIRG);
            return;
        case TOKEN_IDENTIFICADOR:
            parser_atribuicao(parser);
            return;
        case TOKEN_WHILE:
        case TOKEN_REPEAT:
            parser_iteracao(parser);
            return;
        case TOKEN_IF:
            parser_decisao(parser);
            return;
        case TOKEN_WRITE:
            parser_escrita(parser);
            return;
        default:
            parser_erro(parser);
    }
}

static void parser_atribuicao(Parser *parser) {
    parser_esperar(parser, TOKEN_IDENTIFICADOR);
    parser_esperar(parser, TOKEN_ATRIBUICAO);
    parser_expressao(parser);
    parser_esperar(parser, TOKEN_PONTOVIRG);
}

static void parser_iteracao(Parser *parser) {
    if (parser->token_atual.tipo == TOKEN_WHILE) {
        parser_avancar(parser);
        parser_expressao(parser);
        parser_esperar(parser, TOKEN_DO);
        parser_comando(parser);
        return;
    }
    parser_esperar(parser, TOKEN_REPEAT);
    parser_comando(parser);
    parser_esperar(parser, TOKEN_UNTIL);
    parser_expressao(parser);
    parser_esperar(parser, TOKEN_PONTOVIRG);
}

static void parser_decisao(Parser *parser) {
    parser_esperar(parser, TOKEN_IF);
    parser_expressao(parser);
    parser_esperar(parser, TOKEN_THEN);
    parser_comando(parser);
    if (parser->token_atual.tipo == TOKEN_ELSE) {
        parser_avancar(parser);
        parser_comando(parser);
    }
}

static void parser_escrita(Parser *parser) {
    parser_esperar(parser, TOKEN_WRITE);
    parser_esperar(parser, TOKEN_ABRE_PAREN);
    parser_expressao(parser);
    parser_esperar(parser, TOKEN_FECHA_PAREN);
    parser_esperar(parser, TOKEN_PONTOVIRG);
}

static void parser_expressao(Parser *parser) {
    parser_relacional(parser);
    while (token_em(parser->token_atual.tipo, OP_OR_AND, 2)) {
        parser_avancar(parser);
        parser_relacional(parser);
    }
}

static void parser_relacional(Parser *parser) {
    parser_soma(parser);
    while (token_em(parser->token_atual.tipo, OP_RELACIONAL, 6)) {
        parser_avancar(parser);
        parser_soma(parser);
    }
}

static void parser_soma(Parser *parser){
    parser_termo(parser);
    while (token_em(parser->token_atual.tipo, OP_SOMA, 2)){
        parser_avancar(parser);
        parser_termo(parser);
    }
}

static void parser_termo(Parser *parser){
    parser_fator(parser);
    while (token_em(parser->token_atual.tipo, OP_TERMO, 3)){
        parser_avancar(parser);
        parser_fator(parser);
    }
}

static void parser_fator(Parser *parser){
    switch (parser->token_atual.tipo){
        case TOKEN_ABRE_PAREN:
            parser_avancar(parser);
            parser_expressao(parser);
            parser_esperar(parser, TOKEN_FECHA_PAREN);
            return;
        case TOKEN_NOT:
            parser_avancar(parser);
            parser_fator(parser);
            return;
        case TOKEN_INTEIRO_LITERAL:
        case TOKEN_REAL_LITERAL:
        case TOKEN_CHAR_LITERAL:
        case TOKEN_IDENTIFICADOR:
            parser_avancar(parser);
            return;
        default:
            parser_erro(parser);
    }
}
