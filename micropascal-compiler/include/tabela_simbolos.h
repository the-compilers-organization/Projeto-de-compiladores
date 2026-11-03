#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H

#define TAM_TABELA 211
#define TAM_NOME 256

typedef enum {
    TIPO_INTEIRO, TIPO_REAL, TIPO_CHAR, TIPO_BOOLEANO, TIPO_INVALIDO
} TipoDado;

typedef struct simbolo {
    char nome[TAM_NOME];
    TipoDado tipo;
    int linha;
    struct simbolo *prox;
} Simbolo;

typedef struct {
    Simbolo *vetor[TAM_TABELA];
} TabelaSimbolos;

void tabela_inicializar(TabelaSimbolos *tabela);

const Simbolo *tabela_buscar(const TabelaSimbolos *tabela, const char *nome);

// 1 = inserido, 0 = duplicado, -1 = falha de memoria/nome invalido 
int tabela_inserir(TabelaSimbolos *tabela, const char *nome, TipoDado tipo, int linha);

void tabela_liberar(TabelaSimbolos *tabela);

const char *tipo_nome(TipoDado tipo);
#endif
