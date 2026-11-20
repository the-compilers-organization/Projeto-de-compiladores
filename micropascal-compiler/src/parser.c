#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/tabela_simbolos.h"
#include "../include/semantico.h"

// ETAPA 2: regras de compatibilidade de tipos e verificacoes semanticas
#include "../include/semantico.h"


static void avancar(Parser *p) { 
    p->token_atual = lexer_proximo_token(p->lexer); 
}


static void erro(Parser *p) {
    fprintf(stderr, "Erro de sintaxe no token [%s]\n", p->token_atual.lexema);
    exit(EXIT_FAILURE);
}


// ETAPA 2: apresenta a linha e a descricao de um erro semantico
static void erro_semantico(int linha, const char *mensagem, const char *nome) {
    fprintf(stderr, "Erro semantico na linha %d: %s [%s]\n", linha, mensagem, nome);
    exit(EXIT_FAILURE);
}


static void esperar(Parser *p, TipoToken tipo) {
    if (p->token_atual.tipo != tipo) erro(p);
    avancar(p);
}


static int em(TipoToken t, const TipoToken *ts, size_t n) {
    for (size_t i = 0; i < n; ++i) if (ts[i] == t) return 1;
    return 0;
}


static const TipoToken inicio[] = {
    TOKEN_BEGIN,TOKEN_IDENTIFICADOR,TOKEN_WHILE,TOKEN_REPEAT,TOKEN_IF,TOKEN_WRITE
};


static const TipoToken logicos[] = {
    TOKEN_OR,TOKEN_AND
};


static const TipoToken relacionais[] = {
    TOKEN_IGUAL,TOKEN_DIFERENTE,TOKEN_MENOR,TOKEN_MAIOR,TOKEN_MENOR_IGUAL,TOKEN_MAIOR_IGUAL
};


static const TipoToken aditivos[] = {
    TOKEN_MAIS,TOKEN_MENOS
};


static const TipoToken multiplicativos[] = {
    TOKEN_MULT,TOKEN_DIV_REAL,TOKEN_DIV_INT
};


/*
 * TODO:
 * Alterar a declaracao de expressao() para TipoDado
 * quando a analise semantica for implementada.
 *
 * Atualizar tambem as chamadas correspondentes.
 */
static TipoDado expressao(Parser *p);
static void comando(Parser *p);


static void bloco(Parser *p);


// ETAPA 2: consulta a tabela hash e rejeita identificadores nao declarados
static TipoDado variavel_tipo(Parser *p) {
    const Simbolo *s = tabela_buscar(&p->tabela, p->token_atual.lexema);
    if (!s) erro_semantico(p->token_atual.linha, "variavel nao declarada", p->token_atual.lexema);
    return s->tipo;
}


// ETAPA 2: aplica a regra de tipos do operador e sinaliza incompatibilidades
static TipoDado aplicar_binaria(TipoToken op, TipoDado a, TipoDado b, int linha) {
    TipoDado r = semantico_binaria(op, a, b);
    if (r == TIPO_INVALIDO) erro_semantico(linha, "tipos incompativeis no operador", "binario");
    return r;
}


// ETAPA 2: identifica tipos de literais/variaveis, parenteses e operadores unarios
static TipoDado fator(Parser *p) {
    TipoToken t = p->token_atual.tipo;
    if (t == TOKEN_ABRE_PAREN) {
        avancar(p);
        TipoDado r = expressao(p);
        esperar(p, TOKEN_FECHA_PAREN);
        return r;
    }

    if (t == TOKEN_NOT || t == TOKEN_MENOS) {
        int linha = p->token_atual.linha;
        avancar(p);
        TipoDado r = semantico_unaria(t, fator(p));
        if (r == TIPO_INVALIDO) erro_semantico(linha, "operando invalido para operador unario", t == TOKEN_NOT ? "not" : "-");
        return r;
    }

    TipoDado r;
    switch (t) {
        case TOKEN_INTEIRO_LITERAL: r = TIPO_INTEIRO; break;
        case TOKEN_REAL_LITERAL: r = TIPO_REAL; break;
        case TOKEN_CHAR_LITERAL: r = TIPO_CHAR; break;
        case TOKEN_IDENTIFICADOR: r = variavel_tipo(p); break;
        default: erro(p); return TIPO_INVALIDO;
    }

    avancar(p);

    return r;
}


// ETAPA 2: propaga tipos em multiplicacao e divisoes
static TipoDado termo(Parser *p) {
    TipoDado r = fator(p);
    while (em(p->token_atual.tipo, multiplicativos, 3)) {
        TipoToken op = p->token_atual.tipo;
        int linha = p->token_atual.linha;
        avancar(p);
        r = aplicar_binaria(op, r, fator(p), linha);
    }

    return r;
}


// ETAPA 2: propaga tipos em adicao e subtracao
static TipoDado soma(Parser *p) {
    TipoDado r = termo(p);
    while (em(p->token_atual.tipo, aditivos, 2)) {
        TipoToken op = p->token_atual.tipo;
        int linha = p->token_atual.linha;
        avancar(p);
        r = aplicar_binaria(op, r, termo(p), linha);
    }

    return r;
}


// ETAPA 2: valida operadores relacionais e retorna o tipo resultante
static TipoDado relacional(Parser *p) {
    TipoDado r = soma(p);
    while (em(p->token_atual.tipo, relacionais, 6)) {
        TipoToken op = p->token_atual.tipo;
        int linha = p->token_atual.linha;
        avancar(p);
        r = aplicar_binaria(op, r, soma(p), linha);
    }

    return r;
}


// ETAPA 2: combina expressoes relacionais com operadores logicos
static TipoDado expressao(Parser *p) {
    TipoDado r = relacional(p);
    while (em(p->token_atual.tipo, logicos, 2)) {
        TipoToken op = p->token_atual.tipo;
        int linha = p->token_atual.linha;
        avancar(p);
        r = aplicar_binaria(op, r, relacional(p), linha);
    }

    return r;
}


static void declaracao(Parser *p) {
    typedef struct {
        char nome[TAM_NOME];
        int linha;
    } IdentificadorPendente;

    size_t quantidade = 0;
    size_t capacidade = 4;
    IdentificadorPendente *ids = malloc(capacidade * sizeof *ids);
    if (ids == NULL) {
        fprintf(stderr, "Erro semantico: memoria insuficiente ao ler declaracao.\n");
        tabela_liberar(&p->tabela);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        if (p->token_atual.tipo != TOKEN_IDENTIFICADOR) {
            free(ids);
            erro(p);
        }

        if (quantidade == capacidade) {
            capacidade *= 2;
            IdentificadorPendente *novo = realloc(ids, capacidade * sizeof *ids);
            if (novo == NULL) {
                fprintf(stderr, "Erro semantico: memoria insuficiente ao ler declaracao.\n");
                free(ids);
                tabela_liberar(&p->tabela);
                exit(EXIT_FAILURE);
            }
            ids = novo;
        }

        memcpy(ids[quantidade].nome, p->token_atual.lexema, TAM_NOME);
        ids[quantidade].nome[TAM_NOME - 1] = '\0';
        ids[quantidade].linha = p->token_atual.linha;
        ++quantidade;
        avancar(p);

        if (p->token_atual.tipo != TOKEN_VIRGULA)
            break;
        avancar(p);
    }

    esperar(p, TOKEN_DOISPONTOS);
    TipoDado tipo;

    switch (p->token_atual.tipo) {
        case TOKEN_INTEGER: tipo = TIPO_INTEIRO; break;
        case TOKEN_REAL:    tipo = TIPO_REAL;    break;
        case TOKEN_CHAR:    tipo = TIPO_CHAR;    break;
        default:
            free(ids);
            erro(p);
            return;
    }
    avancar(p);
    esperar(p, TOKEN_PONTOVIRG);

    for (size_t i = 0; i < quantidade; ++i) {
        int resultado = tabela_inserir(&p->tabela, ids[i].nome, tipo, ids[i].linha);
        if (resultado != 1) {
            if (resultado == 0) {
                fprintf(stderr,
                        "Erro semantico na linha %d: identificador '%s' ja declarado.\n",
                        ids[i].linha, ids[i].nome);
            } else {
                fprintf(stderr,
                        "Erro semantico na linha %d: nao foi possivel registrar '%s'.\n",
                        ids[i].linha, ids[i].nome);
            }
            free(ids);
            tabela_liberar(&p->tabela);
            exit(EXIT_FAILURE);
        }
    }

    free(ids);
}


static void bloco(Parser *p) {
    esperar(p, TOKEN_BEGIN);
    while (em(p->token_atual.tipo, inicio, 6)) comando(p);
    esperar(p, TOKEN_END);
}


// ETAPA 2: exige tipo booleano nas condicoes de controle de fluxo
static void verificar_condicao(TipoDado t, int linha) {
    if (!semantico_condicao(t)) erro_semantico(linha, "condicao deve ser booleana", "condicao");
}


// ETAPA 2: verifica tipos em atribuicoes, condicoes e expressoes de write
static void comando(Parser *p) {

    switch (p->token_atual.tipo) {
        case TOKEN_BEGIN:
            bloco(p);
            esperar(p, TOKEN_PONTOVIRG);
            return;
        case TOKEN_IDENTIFICADOR: {
            TipoDado destino = variavel_tipo(p);
            int linha = p->token_atual.linha;
            avancar(p);
            esperar(p, TOKEN_ATRIBUICAO);
            TipoDado origem = expressao(p);
            if (!semantico_atribuicao(destino, origem)) erro_semantico(linha, "tipos incompativeis na atribuicao", ":=");
            esperar(p, TOKEN_PONTOVIRG);
            return;
        }
        case TOKEN_WHILE: {
            int linha = p->token_atual.linha;
            avancar(p);
            verificar_condicao(expressao(p), linha);
            esperar(p, TOKEN_DO);
            comando(p);
            return;
        }
        case TOKEN_REPEAT: {
            int linha = p->token_atual.linha;
            avancar(p);
            comando(p);
            esperar(p, TOKEN_UNTIL);
            verificar_condicao(expressao(p), linha);
            esperar(p, TOKEN_PONTOVIRG);
            return;
        }
        case TOKEN_IF: {
            int linha = p->token_atual.linha;
            avancar(p);
            verificar_condicao(expressao(p), linha);
            esperar(p, TOKEN_THEN);
            comando(p);
            if (p->token_atual.tipo == TOKEN_ELSE) { avancar(p); comando(p); }
            return;
        }
        case TOKEN_WRITE:
            avancar(p);
            esperar(p, TOKEN_ABRE_PAREN);
            (void)expressao(p);
            esperar(p, TOKEN_FECHA_PAREN);
            esperar(p, TOKEN_PONTOVIRG);
            return;
        default: erro(p);
    }
}


// ETAPA 2: inicializa a tabela de simbolos junto com o Parse
void parser_inicializar(Parser *p, Lexer *lexer) {
    p->lexer = lexer;
    tabela_inicializar(&p->tabela);
    avancar(p);
}


void parser_analisar_programa(Parser *p) {
    esperar(p, TOKEN_PROGRAM);
    esperar(p, TOKEN_IDENTIFICADOR);
    esperar(p, TOKEN_PONTOVIRG);
    esperar(p, TOKEN_VAR);

    while (p->token_atual.tipo == TOKEN_IDENTIFICADOR) declaracao(p);
    
    bloco(p);
    esperar(p, TOKEN_PONTO);

    if (p->token_atual.tipo != TOKEN_EOF)
        erro(p);

        tabela_liberar(&p->tabela);
}
