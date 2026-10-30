#ifndef TOKEN_H
#define TOKEN_H

/*
 * token.h
 *
 * Definições compartilhadas de tokens para o compilador micro-Pascal.
 * Usado tanto pelo lexer (que produz tokens) quanto pelo parser
 * (que consome tokens).
 */

typedef enum {
    /* Fim de arquivo / erro */
    TOKEN_EOF,
    TOKEN_ERRO,

    /* Identificador */
    TOKEN_IDENTIFICADOR,

    /* Literais */
    TOKEN_INTEIRO_LITERAL,
    TOKEN_REAL_LITERAL,
    TOKEN_CHAR_LITERAL,

    /* Palavras reservadas */
    TOKEN_PROGRAM,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_DO,
    TOKEN_REPEAT,
    TOKEN_UNTIL,
    TOKEN_INTEGER,
    TOKEN_REAL,
    TOKEN_CHAR,
    TOKEN_BEGIN,
    TOKEN_END,
    TOKEN_WRITE,
    TOKEN_VAR,

    /* Operadores lógico-aritméticos (também reservados: div, and, or, not) */
    TOKEN_MAIS,        /* +   */
    TOKEN_MENOS,       /* -   */
    TOKEN_MULT,        /* *   */
    TOKEN_DIV_REAL,    /* /   */
    TOKEN_DIV_INT,     /* div */
    TOKEN_AND,         /* and */
    TOKEN_OR,          /* or  */
    TOKEN_NOT,         /* not */

    /* Operadores relacionais */
    TOKEN_MENOR,       /* <  */
    TOKEN_MAIOR,       /* >  */
    TOKEN_MENOR_IGUAL, /* <= */
    TOKEN_MAIOR_IGUAL, /* >= */
    TOKEN_IGUAL,       /* =  */
    TOKEN_DIFERENTE,   /* <> */

    /* Atribuição */
    TOKEN_ATRIBUICAO,  /* := */

    /* Símbolos especiais */
    TOKEN_ABRE_PAREN,  /* ( */
    TOKEN_FECHA_PAREN, /* ) */
    TOKEN_VIRGULA,     /* , */
    TOKEN_PONTOVIRG,   /* ; */
    TOKEN_PONTO,       /* . */
    TOKEN_DOISPONTOS   /* : */
} TipoToken;

typedef struct {
    TipoToken tipo;
    char lexema[256];   /* lexema textual do token */
    int linha;          /* linha onde o token foi encontrado (útil para erros) */
} Token;

#endif /* TOKEN_H */
