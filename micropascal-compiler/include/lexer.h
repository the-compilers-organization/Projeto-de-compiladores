
#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "token.h"


typedef struct {
    FILE *arquivo;
    int linha_atual;
    int caractere_atual;
} Lexer;

void lexer_inicializar(Lexer *lexer, FILE *arquivo);

Token lexer_proximo_token(Lexer *lexer);

void lexer_finalizar(Lexer *lexer);

#endif 
