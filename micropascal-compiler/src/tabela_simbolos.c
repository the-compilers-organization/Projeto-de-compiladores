
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
    /* TODO: liberar todos os simbolos das listas encadeadas. */

    /*
     * Libera a memoria dos simbolos armazenados
     * na tabela hash.
     *
     * PARAMETRO:
     *   t - tabela a ser esvaziada.
     *
     * ROTEIRO:
     *   1. Verificar se t e diferente de NULL.
     *   2. Percorrer todas as posicoes do vetor.
     *   3. Para cada posicao, obter o primeiro
     *      simbolo da lista encadeada.
     *   4. Enquanto existir um simbolo:
     *      a) Guardar o endereco do proximo.
     *      b) Liberar o simbolo atual com free.
     *      c) Avancar para o proximo simbolo.
     *   5. Atribuir NULL a posicao do vetor
     *      depois de liberar toda a lista.
     *
     * IMPORTANTE:
     *   Guardar o proximo elemento antes de
     *   liberar o simbolo atual.
     *
     *   Depois de chamar free, nao acessar
     *   os campos do simbolo liberado.
     *
     *   Nao chamar free(t) quando a propria
     *   tabela nao foi alocada com malloc.
     *
     *   Esta funcao deve ser chamada quando
     *   a analise do programa terminar.
     */
}


const char *tipo_nome(TipoDado tipo) {
    /* TODO: retornar a descricao textual do tipo informado. */

    /*
     * Converte um TipoDado para um texto legivel,
     * utilizado principalmente em mensagens de erro.
     *
     * PARAMETRO:
     *   tipo - valor do enum TipoDado.
     *
     * RETORNO:
     *   String constante correspondente ao tipo.
     *
     * ROTEIRO:
     *   1. Utilizar switch para identificar o tipo.
     *   2. Retornar o texto correspondente:
     *
     *      TIPO_INTEIRO  -> "integer"
     *      TIPO_REAL     -> "real"
     *      TIPO_CHAR     -> "char"
     *      TIPO_BOOLEANO -> "boolean"
     *      TIPO_INVALIDO -> "invalido"
     *
     *   3. Prever um retorno padrao para
     *      valores desconhecidos.
     *
     * IMPORTANTE:
     *   A string retornada e constante.
     *   Nao deve ser modificada nem liberada.
     *
     *   Esta funcao nao realiza verificacoes
     *   semanticas. Apenas converte o tipo
     *   para uma representacao textual.
     */
}
