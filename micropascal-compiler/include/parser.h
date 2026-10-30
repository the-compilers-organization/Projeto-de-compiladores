#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"

typedef struct {
    Lexer *lexer;
    Token token_atual;
} Parser;

void parser_inicializar(Parser *parser, Lexer *lexer);

void parser_analisar_programa(Parser *parser);

#endif 