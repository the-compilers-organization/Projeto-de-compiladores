#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "tabela_simbolos.h"

typedef struct {
    Lexer *lexer;
    Token token_atual;
    TabelaSimbolos tabela;
} Parser;

void parser_inicializar(Parser *parser, Lexer *lexer);
void parser_analisar_programa(Parser *parser);

#endif
