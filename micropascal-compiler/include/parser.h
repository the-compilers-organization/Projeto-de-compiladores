#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer *lexer;
    Token token_atual;
} Parser;

void parser_inicializar(Parser *parser, Lexer *lexer);
/* A primeira parte continua disponivel e descarta a AST construida. */
void parser_analisar_programa(Parser *parser);
/* Segunda parte: retorna a arvore, que deve ser liberada com ast_liberar. */
NoAST *parser_construir_ast(Parser *parser);
#endif
