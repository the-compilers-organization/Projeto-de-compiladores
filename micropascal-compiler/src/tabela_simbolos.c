
#include <stdlib.h>
#include <string.h>
#include "../include/tabela_simbolos.h"


static unsigned int hash_nome(const char *nome) {
    /* TODO: calcular e retornar o indice hash do identificador. */

    /*
     * Calcula o indice da tabela hash a partir do nome
     * de um identificador.
     *
     * PARAMETRO:
     *   nome - string terminada em '\0' com o nome da variavel.
     *
     * RETORNO:
     *   Indice inteiro entre 0 e TAM_TABELA - 1.
     *
     * ROTEIRO:
     *   1. Criar um acumulador do tipo unsigned int,
     *      inicializado com zero.
     *   2. Percorrer os caracteres do nome ate '\0'.
     *   3. Para cada caractere, multiplicar o acumulador
     *      por 31 e somar o codigo do caractere,
     *      convertido para unsigned char.
     *   4. Aplicar % TAM_TABELA ao acumulador.
     *   5. Retornar o indice calculado.
     *
     * IMPORTANTE:
     *   Nomes diferentes podem gerar o mesmo indice.
     *   Isso e chamado de colisao e nao representa
     *   um erro semantico.
     *
     *   As colisoes serao tratadas por listas encadeadas
     *   em cada posicao do vetor da tabela.
     *
     *   A funcao deve receber um nome valido.
     *   As funcoes que a chamam devem verificar
     *   previamente se nome nao e NULL.
     */
}


void tabela_inicializar(TabelaSimbolos *t) {
    /* TODO: inicializar todas as posicoes da tabela com NULL. */

    /*
     * Inicializa a tabela de simbolos antes de analisar
     * um programa MicroPascal.
     *
     * PARAMETRO:
     *   t - ponteiro para a tabela hash.
     *
     * ROTEIRO:
     *   1. Verificar se t e diferente de NULL.
     *   2. Percorrer as TAM_TABELA posicoes do vetor.
     *   3. Atribuir NULL a cada posicao.
     *
     * IMPORTANTE:
     *   NULL indica que a lista daquela posicao
     *   ainda nao possui simbolos.
     *
     *   A tabela deve ser inicializada antes
     *   de qualquer insercao ou busca.
     *
     *   Esta funcao nao deve alocar memoria
     *   para os simbolos.
     */
}


const Simbolo *tabela_buscar(const TabelaSimbolos *t, const char *nome) {
    /* TODO: localizar o identificador na lista da posicao hash. */

    /*
     * Procura um identificador registrado na tabela.
     *
     * PARAMETROS:
     *   t    - tabela de simbolos a consultar.
     *   nome - identificador procurado.
     *
     * RETORNO:
     *   Ponteiro const para o Simbolo encontrado,
     *   ou NULL se o identificador nao existir.
     *
     *   O const impede modificar o simbolo
     *   por meio do ponteiro retornado.
     *
     * ROTEIRO:
     *   1. Verificar se t e nome sao diferentes de NULL.
     *   2. Calcular o indice usando hash_nome(nome).
     *   3. Obter o primeiro simbolo da lista
     *      armazenada naquela posicao do vetor.
     *   4. Percorrer a lista utilizando o campo prox.
     *   5. Comparar cada nome usando strcmp.
     *   6. Se encontrar um nome igual, retornar
     *      o ponteiro para o simbolo correspondente.
     *   7. Se chegar ao final sem encontrar,
     *      retornar NULL.
     *
     * TRATAMENTO DE COLISOES:
     *   Dois identificadores diferentes podem
     *   produzir o mesmo indice hash.
     *
     *   Por isso, nao basta verificar apenas
     *   a posicao do vetor.
     *
     *   E necessario percorrer a lista encadeada
     *   e comparar os nomes completos.
     *
     * RESPONSABILIDADE SEMANTICA:
     *   Esta funcao apenas procura o identificador.
     *   Ela nao deve emitir mensagens de erro.
     *
     *   O Parser utiliza a busca para:
     *
     *   - Detectar declaracoes duplicadas.
     *   - Identificar variaveis nao declaradas.
     *   - Recuperar o tipo de uma variavel
     *     para verificar expressoes e atribuicoes.
     *
     *   Se uma variavel for utilizada e a busca
     *   retornar NULL, o Parser deve informar
     *   um erro semantico de variavel nao declarada.
     */
}


int tabela_inserir(TabelaSimbolos *t, const char *nome,
                   TipoDado tipo, int linha) {
    /* TODO: validar, impedir duplicidade, alocar e encadear o simbolo. */

    /*
     * Insere uma nova declaracao de variavel
     * na tabela de simbolos.
     *
     * PARAMETROS:
     *   t     - tabela onde o simbolo sera registrado.
     *   nome  - nome do identificador.
     *   tipo  - tipo declarado (integer, real ou char).
     *   linha - linha da declaracao.
     *
     * RETORNO:
     *    1 - simbolo inserido com sucesso.
     *    0 - identificador ja declarado.
     *   -1 - dados invalidos ou falha de memoria.
     *
     * VALIDACOES:
     *   1. Verificar se t e nome sao diferentes de NULL.
     *   2. Verificar se nome nao esta vazio.
     *   3. Verificar se o tamanho do nome e menor
     *      que TAM_NOME, incluindo espaco para '\0'.
     *   4. Verificar se o tipo declarado e permitido:
     *      TIPO_INTEIRO, TIPO_REAL ou TIPO_CHAR.
     *   5. Nao permitir TIPO_BOOLEANO ou TIPO_INVALIDO
     *      como tipo de variavel declarada.
     *
     * ROTEIRO:
     *   1. Realizar as validacoes dos parametros.
     *   2. Consultar tabela_buscar para verificar
     *      se o identificador ja foi declarado.
     *   3. Se ja existir, retornar 0.
     *   4. Alocar um novo Simbolo utilizando malloc.
     *   5. Verificar se a alocacao foi realizada.
     *      Se falhar, retornar -1.
     *   6. Copiar o nome para o novo simbolo.
     *   7. Armazenar o tipo e a linha da declaracao.
     *   8. Calcular o indice usando hash_nome(nome).
     *   9. Inserir o novo simbolo na lista encadeada
     *      correspondente ao indice calculado.
     *  10. Retornar 1 indicando sucesso.
     *
     * TRATAMENTO DE COLISOES:
     *   Cada posicao do vetor aponta para o
     *   primeiro elemento de uma lista encadeada.
     *
     *   Se dois nomes diferentes produzirem o
     *   mesmo indice, ambos devem ser armazenados
     *   na mesma lista.
     *
     *   Para inserir no inicio da lista:
     *
     *   1. Fazer o campo prox do novo simbolo
     *      apontar para o primeiro elemento atual.
     *   2. Atualizar a posicao do vetor para apontar
     *      para o novo simbolo.
     *
     *   Se a lista estiver vazia, seu primeiro
     *   elemento sera NULL.
     *
     *   A colisao nao representa duplicidade.
     *   Somente identificadores com nomes iguais
     *   devem ser considerados duplicados.
     *
     * REGRAS SEMANTICAS:
     *   - Nao permitir duas declaracoes com o mesmo nome.
     *   - Armazenar a associacao entre nome e tipo.
     *   - Nao permitir declaracoes do tipo booleano.
     *
     * RESPONSABILIDADE DO PARSER:
     *   O Parser deve verificar o retorno da insercao.
     *
     *   Retorno 1: declaracao registrada.
     *   Retorno 0: informar declaracao duplicada.
     *   Retorno -1: tratar dados invalidos
     *               ou falha de memoria.
     *
     *   Esta funcao nao deve imprimir diretamente
     *   mensagens de erro semantico.
     */
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
