#include <stdlib.h>
#include <string.h>
#include "../include/tabela_simbolos.h"



static unsigned int hash_nome(const char *nome) {
    // TODO: calcular e retornar o indice hash do identificador

    //  Calcula o indice da tabela hash a partir do nome de um identificador.
    //  PARAMETRO:
    //  nome - string terminada em '\0' com o nome da variavel.
    //  RETORNO:
    //  Indice inteiro entre 0 e TAM_TABELA - 1.
    //  ROTEIRO:
    //  1. Percorrer os caracteres do nome.
    //  2. Combinar seus codigos em um acumulador unsigned int.
    //  Uma possibilidade e multiplicar o acumulador por 31 e somar
    //  o valor do caractere (convertido para unsigned char).
    //  3. Aplicar o operador % TAM_TABELA ao resultado.
    //  Nomes diferentes podem gerar o mesmo indice (colisao).
    //  Essa situacao sera tratada pela lista encadeada em cada posicao.

}


void tabela_inicializar(TabelaSimbolos *t) {
    /* TODO: inicializar todas as posicoes da tabela com NULL. */

    /*
    * Inicializa a tabela de simbolos antes de analisar um programa.
    *
    * PARAMETRO:
    *   t - ponteiro para a tabela hash.
    *
    * ROTEIRO:
    *   Percorrer todas as TAM_TABELA posicoes do vetor da tabela
    *   e atribuir NULL a cada uma, indicando que ainda nao ha simbolos.
    */
}


const Simbolo *tabela_buscar(const TabelaSimbolos *t, const char *nome) {
    /* TODO: percorrer a lista da posicao hash e localizar o nome. */

    /*
    * Procura um identificador ja registrado na tabela.
    *
    * PARAMETROS:
    *   t    - tabela de simbolos a consultar.
    *   nome - identificador procurado.
    *
    * RETORNO:
    *   Ponteiro const para o Simbolo encontrado, ou NULL se nao existir.
    *   O const impede alterar o registro por meio do ponteiro retornado.
    *
    * ROTEIRO:
    *   1. Calcular a posicao usando hash_nome(nome).
    *   2. Percorrer a lista encadeada dessa posicao.
    *   3. Comparar os nomes usando strcmp.
    *   4. Retornar o simbolo correspondente ou NULL.
    *
    * Essa busca sera usada para detectar declaracoes duplicadas e
    * identificar variaveis nao declaradas durante a analise semantica.
    */
}


int tabela_inserir(TabelaSimbolos *t, const char *nome, TipoDado tipo, int linha) {
    /* TODO: validar, impedir duplicidade, alocar e encadear o simbolo. */

    /*
    * Insere uma nova declaracao de variavel na tabela de simbolos.
    *
    * PARAMETROS:
    *   t     - tabela onde o simbolo sera registrado.
    *   nome  - nome do identificador.
    *   tipo  - tipo declarado (integer, real ou char).
    *   linha - linha da declaracao, util para mensagens de erro.
    *
    * RETORNO PROPOSTO:
    *   1 se a insercao for realizada; 0 se nao for possivel inserir.
    *
    * ROTEIRO:
    *   1. Validar o tamanho de nome para nao ultrapassar o campo da struct.
    *   2. Consultar tabela_buscar para impedir nomes duplicados.
    *   3. Alocar um novo Simbolo com malloc e conferir o resultado.
    *   4. Copiar o nome, guardar tipo e linha.
    *   5. Calcular o indice com hash_nome.
    *   6. Encadear o novo simbolo na lista dessa posicao.
    *   7. Retornar sucesso.
    *
    * IMPORTANTE:
    *   Nao permitir a declaracao de variaveis do tipo booleano.
    *   O retorno 0 pode indicar duplicidade ou falha de memoria; se o
    *   parser precisar diferenciar esses erros, consultar antes de inserir
    *   ou definir codigos de retorno mais especificos no cabecalho.
    */
}


void tabela_liberar(TabelaSimbolos *t) {
    /* TODO: percorrer as listas, liberar os nos e limpar o vetor. */

    /*
    * Libera a memoria de todos os simbolos armazenados.
    *
    * PARAMETRO:
    *   t - tabela a ser esvaziada.
    *
    * ROTEIRO:
    *   1. Percorrer cada posicao do vetor hash.
    *   2. Para cada lista, guardar o proximo antes de chamar free.
    *   3. Liberar cada Simbolo e avancar ate NULL.
    *   4. Atribuir NULL a posicao depois de liberar a lista.
    *
    * Nao chamar free(t) quando a propria tabela nao foi alocada com malloc.
    */
}


const char *tipo_nome(TipoDado tipo) {
    /* TODO: retornar a descricao textual do tipo informado. */

    /*
    * Converte um TipoDado para um texto legivel em mensagens de erro.
    *
    * PARAMETRO:
    *   tipo - valor do enum TipoDado.
    *
    * RETORNO:
    *   String constante correspondente ao tipo.
    *
    * ROTEIRO:
    *   Usar switch para mapear TIPO_INTEIRO, TIPO_REAL, TIPO_CHAR,
    *   TIPO_BOOLEANO e TIPO_INVALIDO para seus nomes textuais.
    *   Prever um retorno padrao para valores desconhecidos.
    *
    * A string retornada nao deve ser liberada nem modificada.
    */
}
