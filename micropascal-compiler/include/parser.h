#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

// ETAPA 2: disponibiliza a tabela hash e os tipos dos simbolos ao Parser
#include "tabela_simbolos.h"

typedef struct {
    Lexer *lexer;
    Token token_atual;

    // ETAPA 2: tabela de simbolos usada nas declaracoes e consultas de variaveis
    TabelaSimbolos tabela;
} Parser;

void parser_inicializar(Parser *parser, Lexer *lexer);
void parser_analisar_programa(Parser *parser);

// ETAPA 2: libera os nos alocados na tabela de simbolos
void parser_finalizar(Parser *parser);

#endif
