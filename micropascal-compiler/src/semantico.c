
#include "../include/semantico.h"


static int numerico(TipoDado t) {
    
    
    if (t == TIPO_INTEIRO || t == TIPO_REAL) {
        return 1;
    }
    return 0;

    /* TODO: verificar se t corresponde a TIPO_INTEIRO ou TIPO_REAL. */

    /*
     * Verifica se um tipo pertence ao conjunto dos tipos numericos.
     *
     * PARAMETRO:
     *   t - tipo que sera analisado.
     *
     * RETORNO:
     *   1 se t for TIPO_INTEIRO ou TIPO_REAL;
     *   0 nos demais casos.
     *
     * Esta funcao auxilia as verificacoes das operacoes
     * aritmeticas, comparacoes relacionais e menos unario.
     */
}


TipoDado semantico_binaria(TipoToken op, TipoDado a, TipoDado b) {
   switch (op) {
        case TOKEN_MAIS:
        case TOKEN_MENOS:
            if (numerico(a) && a == b) return a;
            if (a == TIPO_CHAR && b == TIPO_INTEIRO) return TIPO_CHAR;
            return TIPO_INVALIDO;

        case TOKEN_MULT:
            if (numerico(a) && a == b) return a;
            return TIPO_INVALIDO;

        case TOKEN_DIV_INT:
            if (a == TIPO_INTEIRO && b == TIPO_INTEIRO) return TIPO_INTEIRO;
            return TIPO_INVALIDO;

        case TOKEN_DIV_REAL:
            if (a == TIPO_REAL && b == TIPO_REAL) return TIPO_REAL;
            return TIPO_INVALIDO;

        case TOKEN_MENOR:
        case TOKEN_MENOR_IGUAL:
        case TOKEN_MAIOR:
        case TOKEN_MAIOR_IGUAL:
            if (numerico(a) && a == b) return TIPO_BOOLEANO;
            return TIPO_INVALIDO;

        case TOKEN_IGUAL:
        case TOKEN_DIFERENTE:
            if (a != TIPO_INVALIDO && a == b) return TIPO_BOOLEANO;
            return TIPO_INVALIDO;

        case TOKEN_AND:
        case TOKEN_OR:
            if (a == TIPO_BOOLEANO && b == TIPO_BOOLEANO) return TIPO_BOOLEANO;
            return TIPO_INVALIDO;

        default:
            return TIPO_INVALIDO;
    }

     /* TODO: implementar as regras de tipos das operacoes binarias. */

    /*
     * Determina o tipo resultante de uma operacao binaria.
     *
     * PARAMETROS:
     *   op - operador recebido do Parser (TipoToken).
     *   a  - tipo da expressao a esquerda.
     *   b  - tipo da expressao a direita.
     *
     * RETORNO:
     *   TipoDado resultante, ou TIPO_INVALIDO se os
     *   tipos forem incompativeis.
     *
     * REGRAS DO ENUNCIADO:
     *
     * 1. ADICAO E SUBTRACAO (+ e -):
     *    integer + integer -> integer
     *    integer - integer -> integer
     *    real + real       -> real
     *    real - real       -> real
     *
     *    Tambem sao permitidas:
     *    char + integer    -> char
     *    char - integer    -> char
     *
     *    ATENCAO:
     *    A ordem dos operandos deve ser respeitada.
     *    integer + char e integer - char sao invalidos.
     *
     * 2. MULTIPLICACAO (*):
     *    integer * integer -> integer
     *    real * real       -> real
     *
     *    Nao permite operandos de tipos diferentes.
     *
     * 3. DIVISAO INTEIRA (div):
     *    integer div integer -> integer
     *
     * 4. DIVISAO REAL (/):
     *    real / real -> real
     *
     *    Nao realizar conversao implicita de tipos.
     *
     * 5. COMPARACOES RELACIONAIS (<, <=, >, >=):
     *    integer com integer -> booleano
     *    real com real       -> booleano
     *
     *    Nao permite comparar tipos diferentes
     *    nem utilizar char ou booleano.
     *
     * 6. IGUALDADE E DIFERENCA (= e <>):
     *    integer com integer -> booleano
     *    real com real       -> booleano
     *    char com char       -> booleano
     *    booleano com booleano -> booleano
     *
     *    Os operandos devem possuir o mesmo tipo.
     *
     * 7. OPERACOES LOGICAS (and e or):
     *    booleano com booleano -> booleano
     *
     *    Qualquer outra combinacao e invalida.
     *
     * ROTEIRO:
     *   1. Identificar o operador utilizando switch ou if.
     *   2. Verificar os tipos dos operandos a e b.
     *   3. Aplicar a regra correspondente ao operador.
     *   4. Retornar o tipo resultante se a operacao for valida.
     *   5. Retornar TIPO_INVALIDO caso contrario.
     *
     * IMPORTANTE:
     *   Esta funcao verifica apenas os TIPOS.
     *   Nao deve realizar calculos com os valores.
     */

}

TipoDado semantico_unaria(TipoToken op, TipoDado a) {

    if(op == TOKEN_MENOS){
        if(a == TIPO_INTEIRO || a == TIPO_REAL){
            return a;
        }
        return TIPO_INVALIDO;
    }

    if(op == TOKEN_NOT){
        if(a == TIPO_BOOLEANO){
            return TIPO_BOOLEANO;
        }
        return TIPO_INVALIDO;
    }

    return TIPO_INVALIDO;

    /* TODO: verificar o operador e validar o tipo do operando. */

    /*
     * Determina o tipo resultante de uma operacao unaria.
     *
     * PARAMETROS:
     *   op - operador unario recebido do Parser.
     *   a  - tipo do operando.
     *
     * RETORNO:
     *   Tipo resultante ou TIPO_INVALIDO.
     *
     * REGRAS:
     *
     * 1. MENOS UNARIO (-):
     *    -integer -> integer
     *    -real    -> real
     *
     *    Nao permite char nem booleano.
     *
     * 2. OPERADOR NOT:
     *    booleano -> booleano
     *
     *    OBSERVACAO:
     *    O operador not existe na gramatica do projeto,
     *    mas sua regra de tipo nao foi especificada
     *    explicitamente no item 2.2 do enunciado.
     *    A regra booleano -> booleano foi adotada
     *    por consistencia com os operadores logicos.
     *
     * ROTEIRO:
     *   1. Identificar o operador.
     *   2. Verificar se o tipo do operando e permitido.
     *   3. Retornar o tipo resultante.
     *   4. Retornar TIPO_INVALIDO se nao for permitido.
     *
     * ATENCAO:
     *   Diferenciar TOKEN_MENOS unario da
     *   subtracao binaria.
     */
}


int semantico_atribuicao(TipoDado destino, TipoDado origem) {

    if(destino == TIPO_INVALIDO || origem == TIPO_INVALIDO){
        return 0;
    }

    if(destino == origem){
        return 1;
    }

    return 0;

    /* TODO: verificar se os tipos sao iguais e validos. */

    /*
     * Verifica se uma atribuicao respeita as regras
     * de tipos definidas no enunciado.
     *
     * PARAMETROS:
     *   destino - tipo da variavel a esquerda de :=.
     *   origem  - tipo da expressao a direita de :=.
     *
     * RETORNO:
     *   1 se os tipos forem iguais e validos;
     *   0 se a atribuicao for incompativel.
     *
     * EXEMPLOS:
     *   integer := integer -> valido
     *   real := real       -> valido
     *   char := char       -> valido
     *
     *   integer := real    -> invalido
     *   real := integer    -> invalido
     *   char := integer    -> invalido
     *
     * ROTEIRO:
     *   1. Verificar se destino e origem sao validos.
     *   2. Verificar se os tipos sao iguais.
     *   3. Retornar 1 se forem compativeis.
     *   4. Retornar 0 caso contrario.
     *
     * IMPORTANTE:
     *   Nao permitir conversao implicita de tipos.
     *   O tipo booleano e interno e nao pode ser
     *   declarado como tipo de variavel.
     */
}


int semantico_condicao(TipoDado tipo) {

    if(tipo == TIPO_BOOLEANO){
        return 1;
    }

    return 0;

    /* TODO: verificar se o tipo corresponde a TIPO_BOOLEANO. */

    /*
     * Verifica se uma expressao pode ser utilizada
     * como condicao de um comando.
     *
     * PARAMETRO:
     *   tipo - tipo resultante da expressao.
     *
     * RETORNO:
     *   1 se tipo for TIPO_BOOLEANO;
     *   0 caso contrario.
     *
     * APLICACAO:
     *   if
     *   while
     *   repeat/until
     *
     * EXEMPLOS:
     *   if x > 10 then ...   -> valido
     *   while x < 5 do ...   -> valido
     *
     *   if x then ...        -> invalido se x
     *                          for integer, real ou char.
     *
     * ROTEIRO:
     *   1. Receber o tipo calculado pelo Parser.
     *   2. Verificar se e TIPO_BOOLEANO.
     *   3. Retornar 1 se for booleano.
     *   4. Retornar 0 caso contrario.
     *
     * IMPORTANTE:
     *   O booleano e utilizado internamente pelo
     *   compilador para resultados de expressoes.
     *   Nao pode ser declarado na secao var.
     */
}
