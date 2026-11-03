#include "../include/semantico.h"


static int numerico(TipoDado t) {
    /* TODO: comparar t com os dois tipos numericos e retornar 1 ou 0. */

    /*
    * Verifica se um tipo pertence ao conjunto dos tipos numericos.
    *
    * PARAMETRO:
    *   t - tipo que sera analisado.
    *
    * RETORNO:
    *   1 se t for TIPO_INTEIRO ou TIPO_REAL; 0 nos demais casos.
    *
    * Esta funcao auxilia as verificacoes das operacoes aritmeticas,
    * comparacoes de ordem e menos unario.
    */
}


TipoDado semantico_binaria(TipoToken op, TipoDado a, TipoDado b) {
    /* TODO: implementar as regras de tipos das operacoes binarias. */

    /*
    * Determina o tipo resultante de uma operacao binaria.
    *
    * PARAMETROS:
    *   op - operador recebido do parser (TipoToken).
    *   a  - tipo da expressao a esquerda.
    *   b  - tipo da expressao a direita.
    *
    * RETORNO:
    *   TipoDado resultante, ou TIPO_INVALIDO se os tipos forem incompativeis.
    *
    * REGRAS DO ENUNCIADO:
    *   + e - : integer/integer -> integer; real/real -> real;
    *           char/integer ou integer/char -> char.
    *   *     : integer/integer -> integer; real/real -> real.
    *   div   : integer/integer -> integer.
    *   /     : real/real -> real.
    *   <, <=, >, >= : operandos numericos do mesmo tipo -> booleano.
    *   =, <> : operandos do mesmo tipo, inclusive char e booleano -> booleano.
    *   and, or : booleano/booleano -> booleano.
    *   Qualquer outra combinacao -> TIPO_INVALIDO.
    *
    * ROTEIRO:
    *   1. Identificar op (por exemplo, com switch).
    *   2. Verificar a combinacao de a e b permitida para esse operador.
    *   3. Retornar o tipo resultante ou TIPO_INVALIDO.
    *   4. Nao realizar calculos de valores: aqui se verificam apenas TIPOS.
    */
}


TipoDado semantico_unaria(TipoToken op, TipoDado a) {
    /* TODO: verificar o operador, validar a e retornar o tipo. */

    /*
    * Determina o tipo resultante de uma operacao unaria.
    *
    * PARAMETROS:
    *   op - operador unario.
    *   a  - tipo do operando.
    *
    * RETORNO:
    *   Tipo resultante, ou TIPO_INVALIDO se o operando nao for aceito.
    *
    * REGRAS:
    *   - unario: aceita integer e real, preservando o tipo do operando.
    *   not: o parser ja reconhece esse operador; adotar booleano -> booleano
    *        se essa for a regra confirmada pelo professor.
    *   Demais combinacoes: TIPO_INVALIDO.
    *
    * ATENCAO: distinguir TOKEN_MENOS unario de subtracao binaria.
    */
}


int semantico_atribuicao(TipoDado destino, TipoDado origem) {
    /* TODO: conferir igualdade e rejeitar TIPO_INVALIDO. */

    /*
    * Verifica se uma atribuicao respeita a regra de tipos do enunciado.
    *
    * PARAMETROS:
    *   destino - tipo da variavel a esquerda de :=.
    *   origem  - tipo da expressao a direita de :=.
    *
    * RETORNO:
    *   1 para tipos iguais e validos; 0 para atribuicao incompativel.
    *
    * Exemplo: integer := integer e valido; integer := real e invalido.
    * Nao permitir conversao implicita, pois o enunciado exige o mesmo tipo.
    */
}


int semantico_condicao(TipoDado tipo) {
    /* TODO: verificar se tipo corresponde a TIPO_BOOLEANO. */

    /*
    * Verifica se uma expressao pode ser usada como condicao.
    *
    * PARAMETRO:
    *   tipo - tipo resultante da expressao analisada pelo parser.
    *
    * RETORNO:
    *   1 se for TIPO_BOOLEANO; 0 caso contrario.
    *
    * Aplicar em if, while e repeat/until.
    * O tipo booleano e interno ao compilador: nao pode ser declarado em var.
    */
}
