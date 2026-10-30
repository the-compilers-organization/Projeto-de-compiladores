#ifndef AST_H
#define AST_H
#include "token.h"
#include <stdio.h>

typedef enum {
    AST_PROGRAMA, AST_DECLARACAO, AST_BLOCO, AST_ATRIBUICAO,
    AST_WHILE, AST_REPEAT, AST_IF, AST_WRITE,
    AST_BINARIO, AST_UNARIO, AST_IDENTIFICADOR, AST_LITERAL
} TipoNoAST;

typedef struct NoAST {
    TipoNoAST tipo;
    Token token;
    struct NoAST *filho;
    struct NoAST *proximo;
} NoAST;

NoAST *ast_criar(TipoNoAST tipo, Token token);
void ast_adicionar_filho(NoAST *pai, NoAST *filho);
void ast_imprimir(const NoAST *no, FILE *saida);
void ast_liberar(NoAST *no);
#endif
