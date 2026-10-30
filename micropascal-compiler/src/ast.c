#include <stdio.h>
#include <stdlib.h>
#include "../include/ast.h"

NoAST *ast_criar(TipoNoAST tipo, Token token) {
    NoAST *no = calloc(1, sizeof *no);
    if (!no) { perror("Memoria insuficiente para AST"); exit(EXIT_FAILURE); }
    no->tipo = tipo;
    no->token = token;
    return no;
}
void ast_adicionar_filho(NoAST *pai, NoAST *filho) {
    if (!pai || !filho) return;
    if (!pai->filho) { pai->filho = filho; return; }
    NoAST *atual = pai->filho;
    while (atual->proximo) atual = atual->proximo;
    atual->proximo = filho;
}
static const char *nome_tipo(TipoNoAST tipo) {
    switch(tipo) {
        case AST_PROGRAMA: return "PROGRAMA";
        case AST_DECLARACAO: return "DECLARACAO";
        case AST_BLOCO: return "BLOCO";
        case AST_ATRIBUICAO: return "ATRIBUICAO";
        case AST_WHILE: return "WHILE";
        case AST_REPEAT: return "REPEAT";
        case AST_IF: return "IF";
        case AST_WRITE: return "WRITE";
        case AST_BINARIO: return "OPERADOR_BINARIO";
        case AST_UNARIO: return "OPERADOR_UNARIO";
        case AST_IDENTIFICADOR: return "IDENTIFICADOR";
        case AST_LITERAL: return "LITERAL";
    }
    return "DESCONHECIDO";
}
static void imprimir_rec(const NoAST *no, FILE *saida, int nivel) {
    for (; no; no = no->proximo) {
        for (int i = 0; i < nivel; ++i) fputs("  ", saida);
        fprintf(saida, "%s", nome_tipo(no->tipo));
        if (no->token.lexema[0]) fprintf(saida, " [%s]", no->token.lexema);
        fputc('\n', saida);
        imprimir_rec(no->filho, saida, nivel + 1);
    }
}
void ast_imprimir(const NoAST *no, FILE *saida) { imprimir_rec(no, saida, 0); }
void ast_liberar(NoAST *no) {
    while (no) {
        NoAST *proximo = no->proximo;
        ast_liberar(no->filho);
        free(no);
        no = proximo;
    }
}
