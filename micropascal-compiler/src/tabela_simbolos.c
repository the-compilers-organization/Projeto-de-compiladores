
#include <stdlib.h>
#include <string.h>
#include "../include/tabela_simbolos.h"


static unsigned int hash_nome(const char *nome) {
    unsigned int h = 0;

    for (const char *c = nome; *c != '\0'; ++c)
        h = h * 31u + (unsigned char)*c;

    return h % TAM_TABELA;
}


void tabela_inicializar(TabelaSimbolos *t) {
    if (t == NULL)
        return;

    for (int i = 0; i < TAM_TABELA; ++i)
        t->vetor[i] = NULL;
}


const Simbolo *tabela_buscar(const TabelaSimbolos *t, const char *nome) {
    if (t == NULL || nome == NULL)
        return NULL;

    unsigned int indice = hash_nome(nome);

    for (const Simbolo *s = t->vetor[indice]; s != NULL; s = s->prox) {
        if (strcmp(s->nome, nome) == 0)
            return s;
    }

    return NULL;
}


int tabela_inserir(TabelaSimbolos *t, const char *nome,
                   TipoDado tipo, int linha) {
    /* 1. Validacao dos parametros. */
    if (t == NULL || nome == NULL)
        return -1;

    size_t tamanho = strlen(nome);

    if (tamanho == 0 || tamanho >= TAM_NOME)
        return -1;

    if (tipo != TIPO_INTEIRO && tipo != TIPO_REAL && tipo != TIPO_CHAR)
        return -1;

    /* 2. Duplicidade: somente nomes iguais (colisao nao conta). */
    if (tabela_buscar(t, nome) != NULL)
        return 0;

    /* 3. Alocacao do novo simbolo. */
    Simbolo *novo = (Simbolo *)malloc(sizeof(Simbolo));

    if (novo == NULL)
        return -1;

    /* 4. Preenchimento (tamanho < TAM_NOME garante espaco para '\0'). */
    memcpy(novo->nome, nome, tamanho + 1);
    novo->tipo  = tipo;
    novo->linha = linha;

    /* 5. Insercao no inicio da lista encadeada do indice hash. */
    unsigned int indice = hash_nome(nome);

    novo->prox = t->vetor[indice];
    t->vetor[indice] = novo;

    return 1;
}


void tabela_liberar(TabelaSimbolos *t) {
 if (t == NULL)
        return;

    for (size_t i = 0; i < TAM_TABELA; ++i) {
        Simbolo *atual = t->vetor[i];
        while (atual != NULL) {
            Simbolo *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
        t->vetor[i] = NULL;
    }
}


const char *tipo_nome(TipoDado tipo) {
    switch (tipo) {
        case TIPO_INTEIRO:  return "integer";
        case TIPO_REAL:     return "real";
        case TIPO_CHAR:     return "char";
        case TIPO_BOOLEANO: return "boolean";
        case TIPO_INVALIDO: return "invalido";
        default:            return "desconhecido";
    }
        
}