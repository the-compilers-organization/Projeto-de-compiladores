#include "tokens.h"
// ------------------------------------------- VARIAVEIS GLOBAIS COMPARTILHADAS: 

extern char entrada[TAM];
extern char lexema[TAM_LEXEMA];
extern int posicao; //Indica a posição atual do Lexer dentro do vetor entrada.
extern int simbolo_lido; //Armazena o token atualmente reconhecido pelo Lexer.


// #ifndef GLOBAIS_H
// #define GLOBAIS_H

// #include "tokens.h"

// // VARIAVEIS GLOBAIS COMPARTILHADAS

// extern char entrada[TAM];

// extern char lexema[TAM_LEXEMA];

// extern int posicao;

// extern int simbolo_lido;

// #endif