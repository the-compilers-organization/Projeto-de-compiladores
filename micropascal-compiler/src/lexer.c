#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../include/lexer.h"

void lexer_inicializar(Lexer *lexer, FILE *arquivo) {
    lexer->arquivo = arquivo;
    lexer->linha_atual = 1;
    lexer->caractere_atual = fgetc(arquivo);
}

void lexer_finalizar(Lexer *lexer) {
    (void)lexer;
}

#define TAM_MAX_LEXEMA 255

static void erro_lexico(int c) {
    if (c == EOF) {
        fprintf(stderr, "Erro léxico: fim de arquivo inesperado\n");
    } else {
        fprintf(stderr, "Erro léxico no caracter [%c]\n", c);
    }
    exit(1);
}

static int avancar(Lexer *lexer) {
    int c = fgetc(lexer->arquivo);
    lexer->caractere_atual = c;
    return c;
}

static void pular_comentario(Lexer *lexer) {

    while (lexer->caractere_atual != '\n' && lexer->caractere_atual != EOF) {
        avancar(lexer);
    }
}

static void pular_brancos(Lexer *lexer) {
    while (1) {
        if (lexer->caractere_atual == ' '  || lexer->caractere_atual == '\n' ||
            lexer->caractere_atual == '\t' || lexer->caractere_atual == '\r') {
            if (lexer->caractere_atual == '\n') {
                lexer->linha_atual++;
            }
            avancar(lexer);
        } else if (lexer->caractere_atual == '/') {
            int proximo = fgetc(lexer->arquivo);
            if (proximo == '/') {
                pular_comentario(lexer);
            } else {
                ungetc(proximo, lexer->arquivo);
                return;
            }
        } else {
            return;
        }
    }
}

static TipoToken verificar_palavra_reservada(const char *lexema) {
    if (strcmp(lexema, "program") == 0) return TOKEN_PROGRAM;
    if (strcmp(lexema, "if") == 0)      return TOKEN_IF;
    if (strcmp(lexema, "then") == 0)    return TOKEN_THEN;
    if (strcmp(lexema, "else") == 0)    return TOKEN_ELSE;
    if (strcmp(lexema, "while") == 0)   return TOKEN_WHILE;
    if (strcmp(lexema, "do") == 0)      return TOKEN_DO;
    if (strcmp(lexema, "repeat") == 0)  return TOKEN_REPEAT;
    if (strcmp(lexema, "until") == 0)   return TOKEN_UNTIL;
    if (strcmp(lexema, "integer") == 0) return TOKEN_INTEGER;
    if (strcmp(lexema, "real") == 0)    return TOKEN_REAL;
    if (strcmp(lexema, "char") == 0)    return TOKEN_CHAR;
    if (strcmp(lexema, "begin") == 0)   return TOKEN_BEGIN;
    if (strcmp(lexema, "end") == 0)     return TOKEN_END;
    if (strcmp(lexema, "write") == 0)   return TOKEN_WRITE;
    if (strcmp(lexema, "var") == 0)     return TOKEN_VAR;
    if (strcmp(lexema, "div") == 0)     return TOKEN_DIV_INT;
    if (strcmp(lexema, "and") == 0)     return TOKEN_AND;
    if (strcmp(lexema, "or") == 0)      return TOKEN_OR;
    if (strcmp(lexema, "not") == 0)     return TOKEN_NOT;

    return TOKEN_IDENTIFICADOR;
}

static Token ler_identificador(Lexer *lexer) {
    Token token;
    int i = 0;

    while (isalnum(lexer->caractere_atual) || lexer->caractere_atual == '_') {
        if (i >= TAM_MAX_LEXEMA) erro_lexico(lexer->caractere_atual);
        token.lexema[i++] = (char)lexer->caractere_atual;
        avancar(lexer);
    }
    token.lexema[i] = '\0';

    token.tipo = verificar_palavra_reservada(token.lexema);
    token.linha = lexer->linha_atual;
    return token;
}

static Token ler_numero(Lexer *lexer) {
    Token token;
    int i = 0;

    while (isdigit(lexer->caractere_atual)) {
        if (i >= TAM_MAX_LEXEMA) erro_lexico(lexer->caractere_atual);
        token.lexema[i++] = (char)lexer->caractere_atual;
        avancar(lexer);
    }

    if (lexer->caractere_atual == '.') {
        if (i >= TAM_MAX_LEXEMA) erro_lexico('.');
        token.lexema[i++] = '.';
        avancar(lexer);

        if (!isdigit(lexer->caractere_atual)) erro_lexico(lexer->caractere_atual);
        while (isdigit(lexer->caractere_atual)) {
            if (i >= TAM_MAX_LEXEMA) erro_lexico(lexer->caractere_atual);
            token.lexema[i++] = (char)lexer->caractere_atual;
            avancar(lexer);
        }
        token.tipo = TOKEN_REAL_LITERAL;
    } else {
        token.tipo = TOKEN_INTEIRO_LITERAL;
    }

    token.lexema[i] = '\0';
    token.linha = lexer->linha_atual;
    return token;
}

static Token ler_char_literal(Lexer *lexer) {
    Token token;
    int i = 0;

    token.lexema[i++] = '\'';
    avancar(lexer);

    if (lexer->caractere_atual == '\\') {
        token.lexema[i++] = '\\';
        avancar(lexer);
        if (lexer->caractere_atual == 'n' || lexer->caractere_atual == 't') {
            token.lexema[i++] = (char)lexer->caractere_atual;
            avancar(lexer);
        } else {
            erro_lexico(lexer->caractere_atual);
        }
    } else if (isalnum(lexer->caractere_atual) || lexer->caractere_atual == '_') {
        token.lexema[i++] = (char)lexer->caractere_atual;
        avancar(lexer);
    } else {
        erro_lexico(lexer->caractere_atual);
    }

    if (lexer->caractere_atual != '\'') {
        erro_lexico(lexer->caractere_atual);
    }

    token.lexema[i++] = '\'';
    avancar(lexer);
    token.lexema[i] = '\0';

    token.tipo = TOKEN_CHAR_LITERAL;
    token.linha = lexer->linha_atual;
    return token;
}

static Token ler_operador_ou_simbolo(Lexer *lexer) {
    Token token;
    token.linha = lexer->linha_atual;
    int c = lexer->caractere_atual;

    switch (c) {
        case '<':
            avancar(lexer);
            if (lexer->caractere_atual == '=') {
                strcpy(token.lexema, "<=");
                token.tipo = TOKEN_MENOR_IGUAL;
                avancar(lexer);
            } else if (lexer->caractere_atual == '>') {
                strcpy(token.lexema, "<>");
                token.tipo = TOKEN_DIFERENTE;
                avancar(lexer);
            } else {
                strcpy(token.lexema, "<");
                token.tipo = TOKEN_MENOR;
            }
            return token;

        case '>':
            avancar(lexer);
            if (lexer->caractere_atual == '=') {
                strcpy(token.lexema, ">=");
                token.tipo = TOKEN_MAIOR_IGUAL;
                avancar(lexer);
            } else {
                strcpy(token.lexema, ">");
                token.tipo = TOKEN_MAIOR;
            }
            return token;

        case ':':
            avancar(lexer);
            if (lexer->caractere_atual == '=') {
                strcpy(token.lexema, ":=");
                token.tipo = TOKEN_ATRIBUICAO;
                avancar(lexer);
            } else {
                strcpy(token.lexema, ":");
                token.tipo = TOKEN_DOISPONTOS;
            }
            return token;

        case '+': strcpy(token.lexema, "+"); token.tipo = TOKEN_MAIS;        avancar(lexer); return token;
        case '-': strcpy(token.lexema, "-"); token.tipo = TOKEN_MENOS;       avancar(lexer); return token;
        case '*': strcpy(token.lexema, "*"); token.tipo = TOKEN_MULT;        avancar(lexer); return token;
        case '/': strcpy(token.lexema, "/"); token.tipo = TOKEN_DIV_REAL;    avancar(lexer); return token;
        case '=': strcpy(token.lexema, "="); token.tipo = TOKEN_IGUAL;       avancar(lexer); return token;
        case '(': strcpy(token.lexema, "("); token.tipo = TOKEN_ABRE_PAREN;  avancar(lexer); return token;
        case ')': strcpy(token.lexema, ")"); token.tipo = TOKEN_FECHA_PAREN; avancar(lexer); return token;
        case ',': strcpy(token.lexema, ","); token.tipo = TOKEN_VIRGULA;     avancar(lexer); return token;
        case ';': strcpy(token.lexema, ";"); token.tipo = TOKEN_PONTOVIRG;   avancar(lexer); return token;
        case '.': strcpy(token.lexema, "."); token.tipo = TOKEN_PONTO;       avancar(lexer); return token;

        default:
            erro_lexico(c);
            return token;
    }
}

Token lexer_proximo_token(Lexer *lexer) {
    pular_brancos(lexer);

    if (lexer->caractere_atual == EOF) {
        Token token;
        token.tipo = TOKEN_EOF;
        token.lexema[0] = '\0';
        token.linha = lexer->linha_atual;
        return token;
    }

    if (isalpha(lexer->caractere_atual) || lexer->caractere_atual == '_') {
        return ler_identificador(lexer);
    }

    if (isdigit(lexer->caractere_atual)) {
        return ler_numero(lexer);
    }

    if (lexer->caractere_atual == '.') {
        int proximo = fgetc(lexer->arquivo);
        ungetc(proximo, lexer->arquivo);
        if (isdigit(proximo)) {
            return ler_numero(lexer);
        }
    }

    if (lexer->caractere_atual == '\'') {
        return ler_char_literal(lexer);
    }

    return ler_operador_ou_simbolo(lexer);
}
