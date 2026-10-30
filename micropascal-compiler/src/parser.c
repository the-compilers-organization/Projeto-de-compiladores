#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"

static void avancar(Parser *p) { p->token_atual = lexer_proximo_token(p->lexer); }
static void erro(Parser *p) {
    if (p->token_atual.tipo != TOKEN_ERRO)
        fprintf(stderr, "Erro de sintaxe no token [%s]\n", p->token_atual.lexema);
    exit(EXIT_FAILURE);
}
static Token esperar(Parser *p, TipoToken t) {
    if (p->token_atual.tipo != t) erro(p);
    Token token = p->token_atual;
    avancar(p);
    return token;
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

static NoAST *expressao(Parser *p);
static NoAST *comando(Parser *p);
static NoAST *bloco(Parser *p);

static NoAST *fator(Parser *p) {
    Token t = p->token_atual;
    switch(t.tipo) {
        case TOKEN_ABRE_PAREN: {
            avancar(p);
            NoAST *n = expressao(p);
            esperar(p,TOKEN_FECHA_PAREN);
            return n;
        }
        case TOKEN_NOT: {
            avancar(p);
            NoAST *n = ast_criar(AST_UNARIO,t);
            ast_adicionar_filho(n,fator(p));
            return n;
        }
        case TOKEN_INTEIRO_LITERAL: case TOKEN_REAL_LITERAL: case TOKEN_CHAR_LITERAL:
            avancar(p); return ast_criar(AST_LITERAL,t);
        case TOKEN_IDENTIFICADOR:
            avancar(p); return ast_criar(AST_IDENTIFICADOR,t);
        default: erro(p); return NULL;
    }
}
static NoAST *binario(Parser *p, NoAST *(*nivel)(Parser *), const TipoToken *ops, size_t nops) {
    NoAST *esquerda = nivel(p);
    while (em(p->token_atual.tipo,ops,nops)) {
        Token op = p->token_atual;
        avancar(p);
        NoAST *direita = nivel(p);
        NoAST *no = ast_criar(AST_BINARIO,op);
        ast_adicionar_filho(no,esquerda);
        ast_adicionar_filho(no,direita);
        esquerda = no;
    }
    return esquerda;
}
static NoAST *termo(Parser *p) { return binario(p,fator,multiplicativos,3); }
static NoAST *soma(Parser *p) { return binario(p,termo,aditivos,2); }
static NoAST *relacional(Parser *p) { return binario(p,soma,relacionais,6); }
static NoAST *expressao(Parser *p) { return binario(p,relacional,logicos,2); }

static NoAST *declaracao(Parser *p) {
    Token id = esperar(p,TOKEN_IDENTIFICADOR);
    NoAST *decl = ast_criar(AST_DECLARACAO,id);
    ast_adicionar_filho(decl,ast_criar(AST_IDENTIFICADOR,id));
    while (p->token_atual.tipo == TOKEN_VIRGULA) {
        avancar(p);
        id = esperar(p,TOKEN_IDENTIFICADOR);
        ast_adicionar_filho(decl,ast_criar(AST_IDENTIFICADOR,id));
    }
    esperar(p,TOKEN_DOISPONTOS);
    Token tipo = p->token_atual;
    if (tipo.tipo != TOKEN_INTEGER && tipo.tipo != TOKEN_REAL && tipo.tipo != TOKEN_CHAR) erro(p);
    avancar(p);
    /* O ultimo filho registra o tipo declarado. */
    ast_adicionar_filho(decl,ast_criar(AST_LITERAL,tipo));
    esperar(p,TOKEN_PONTOVIRG);
    return decl;
}
static NoAST *bloco(Parser *p) {
    Token t = esperar(p,TOKEN_BEGIN);
    NoAST *no = ast_criar(AST_BLOCO,t);
    while (em(p->token_atual.tipo,inicio,6)) ast_adicionar_filho(no,comando(p));
    esperar(p,TOKEN_END);
    return no;
}
static NoAST *comando(Parser *p) {
    Token t = p->token_atual;
    NoAST *no;
    switch(t.tipo) {
        case TOKEN_BEGIN:
            no = bloco(p);
            esperar(p,TOKEN_PONTOVIRG);
            return no;
        case TOKEN_IDENTIFICADOR:
            avancar(p);
            no = ast_criar(AST_ATRIBUICAO,t);
            ast_adicionar_filho(no,ast_criar(AST_IDENTIFICADOR,t));
            esperar(p,TOKEN_ATRIBUICAO);
            ast_adicionar_filho(no,expressao(p));
            esperar(p,TOKEN_PONTOVIRG);
            return no;
        case TOKEN_WHILE:
            avancar(p);
            no = ast_criar(AST_WHILE,t);
            ast_adicionar_filho(no,expressao(p));
            esperar(p,TOKEN_DO);
            ast_adicionar_filho(no,comando(p));
            return no;
        case TOKEN_REPEAT:
            avancar(p);
            no = ast_criar(AST_REPEAT,t);
            ast_adicionar_filho(no,comando(p));
            esperar(p,TOKEN_UNTIL);
            ast_adicionar_filho(no,expressao(p));
            esperar(p,TOKEN_PONTOVIRG);
            return no;
        case TOKEN_IF:
            avancar(p);
            no = ast_criar(AST_IF,t);
            ast_adicionar_filho(no,expressao(p));
            esperar(p,TOKEN_THEN);
            ast_adicionar_filho(no,comando(p));
            if (p->token_atual.tipo == TOKEN_ELSE) {
                avancar(p);
                ast_adicionar_filho(no,comando(p));
            }
            return no;
        case TOKEN_WRITE:
            avancar(p);
            no = ast_criar(AST_WRITE,t);
            esperar(p,TOKEN_ABRE_PAREN);
            ast_adicionar_filho(no,expressao(p));
            esperar(p,TOKEN_FECHA_PAREN);
            esperar(p,TOKEN_PONTOVIRG);
            return no;
        default: erro(p); return NULL;
    }
}
void parser_inicializar(Parser *p, Lexer *lexer) { p->lexer=lexer; avancar(p); }
NoAST *parser_construir_ast(Parser *p) {
    esperar(p,TOKEN_PROGRAM);
    Token nome = esperar(p,TOKEN_IDENTIFICADOR);
    NoAST *programa = ast_criar(AST_PROGRAMA,nome);
    esperar(p,TOKEN_PONTOVIRG);
    esperar(p,TOKEN_VAR);
    while (p->token_atual.tipo == TOKEN_IDENTIFICADOR)
        ast_adicionar_filho(programa,declaracao(p));
    ast_adicionar_filho(programa,bloco(p));
    esperar(p,TOKEN_PONTO);
    if (p->token_atual.tipo != TOKEN_EOF) erro(p);
    return programa;
}
void parser_analisar_programa(Parser *p) {
    NoAST *arvore = parser_construir_ast(p);
    ast_liberar(arvore);
}
