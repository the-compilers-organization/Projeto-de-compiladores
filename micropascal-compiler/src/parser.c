#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"

/*
 * ROTEIRO PARA A EQUIPE - SEGUNDA ETAPA
 * Este arquivo ainda faz APENAS analise sintatica.
 * TODO: integrar tabela_simbolos.h e semantico.h (diretamente ou via parser.h),
 * adaptar retornos das expressoes para TipoDado e acrescentar verificacoes
 * semanticas. Os comentarios abaixo nao implementam essas mudancas.
 * Nao utilizar AST.
 */

/*
 * Avanca um token. Preservar a leitura lexico-sintatica atual.
 */
static void avancar(Parser *p) { p->token_atual = lexer_proximo_token(p->lexer); }
/*
 * Erro sintatico. TODO: definir rotina separada para erros semanticos (nome, linha, motivo), sem confundir os dois tipos de erro.
 */
static void erro(Parser *p) {
    if (p->token_atual.tipo != TOKEN_ERRO)
        fprintf(stderr, "Erro de sintaxe no token [%s]\n", p->token_atual.lexema);
    exit(EXIT_FAILURE);
}
/*
 * Consome o token esperado. Preservar a validacao sintatica.
 */
static void esperar(Parser *p, TipoToken tipo) {
    if (p->token_atual.tipo != tipo) erro(p);
    avancar(p);
}
/*
 * Verifica se um token pertence a um conjunto; util para a precedencia.
 */
static int em(TipoToken t, const TipoToken *ts, size_t n) {
    for (size_t i = 0; i < n; ++i) if (ts[i] == t) return 1;
    return 0;
}
/* Conjuntos de tokens para reconhecer comandos e operadores. */
static const TipoToken inicio[] = {TOKEN_BEGIN,TOKEN_IDENTIFICADOR,TOKEN_WHILE,TOKEN_REPEAT,TOKEN_IF,TOKEN_WRITE};
static const TipoToken logicos[] = {TOKEN_OR,TOKEN_AND};
static const TipoToken relacionais[] = {TOKEN_IGUAL,TOKEN_DIFERENTE,TOKEN_MENOR,TOKEN_MAIOR,TOKEN_MENOR_IGUAL,TOKEN_MAIOR_IGUAL};
static const TipoToken aditivos[] = {TOKEN_MAIS,TOKEN_MENOS};
static const TipoToken multiplicativos[] = {TOKEN_MULT,TOKEN_DIV_REAL,TOKEN_DIV_INT};

static void expressao(Parser *p);
static void comando(Parser *p);
static void bloco(Parser *p);

/*
 * TODO: alterar retorno de void para TipoDado. Literais retornam seus tipos; identificadores exigem consulta a tabela hash e erro se nao declarados. Parenteses propagam o tipo da expressao. O operador not deve exigir booleano; acrescentar menos unario, permitido apenas para integer/real. Consultar semantico_unaria().
 */
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
/*
 * TODO: retornar TipoDado. Guardar tipo acumulado do primeiro fator; para *, / e div, obter tipo do fator seguinte e combinar com semantico_binaria(). Preservar associatividade a esquerda.
 */
static void termo(Parser *p) {
    fator(p);
    while (em(p->token_atual.tipo,multiplicativos,3)) { avancar(p); fator(p); }
}
/*
 * TODO: retornar TipoDado. Aplicar semantico_binaria() a + e - com os tipos retornados por termo(). Atentar para char +/- integer => char, e demais regras do enunciado.
 */
static void soma(Parser *p) {
    termo(p);
    while (em(p->token_atual.tipo,aditivos,2)) { avancar(p); termo(p); }
}
/*
 * TODO: retornar TipoDado. Para comparacoes, verificar os tipos dos operandos via semantico_binaria(); resultado valido sera booleano. Preservar a precedencia atual.
 */
static void relacional(Parser *p) {
    soma(p);
    while (em(p->token_atual.tipo,relacionais,6)) { avancar(p); soma(p); }
}
/*
 * TODO: retornar TipoDado. Combinar os resultados de relacional() com and/or, que exigem booleanos. Nao construir AST: propagar somente os tipos durante a descida recursiva.
 */
static void expressao(Parser *p) {
    relacional(p);
    while (em(p->token_atual.tipo,logicos,2)) { avancar(p); relacional(p); }
}
/*
 * TODO: registrar variaveis na tabela hash. A gramatica permite varios nomes ANTES do tipo (ex.: a, b : integer;). Guardar temporariamente os identificadores e suas linhas; apos ler o tipo, inserir cada um. Rejeitar nomes duplicados, inclusive repetidos na mesma declaracao. Nunca permitir declarar booleano.
 */
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
/*
 * Bloco begin/end. A gramatica atual declara variaveis apenas na secao var do programa: nao criar automaticamente novo escopo em cada begin/end sem exigencia do professor.
 */
static void bloco(Parser *p) {
    esperar(p,TOKEN_BEGIN);
    while (em(p->token_atual.tipo,inicio,6)) comando(p);
    esperar(p,TOKEN_END);
}
/*
 * TODO: verificar semantica dos comandos. Atribuicao: buscar o identificador e exigir tipo identico ao tipo da expressao. While, repeat e if: exigir condicao booleana. Write: reconhecer e validar tipos dos identificadores da expressao; o enunciado nao impõe tipo especifico ao write. Preservar as regras sintaticas e pontuacao.
 */
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
/*
 * TODO: inicializar a tabela hash (se ela for campo de Parser) e manter a inicializacao do Lexer/token atual. Atualizar parser.h de modo consistente.
 */
void parser_inicializar(Parser *p, Lexer *lexer) {
    p->lexer = lexer;
    avancar(p);
}
/*
 * TODO: preservar a gramatica program IDENT ; var ... begin ... end. O nome do programa nao deve ser confundido com declaracao de variavel. As declaracoes populam a tabela; os comandos consultam seus tipos. Conferir EOF ao final. Definir API de finalizacao para liberar a tabela.
 */
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
