#include <stdio.h>
#include <stdlib.h>

#include "define.h"


/*
 * Responsabilidade:
 *
 * Este arquivo controla a execução geral do analisador.
 *
 * Ele:
 *
 * 1. recebe o código-fonte;
 * 2. armazena o código no vetor entrada;
 * 3. solicita ao Lexer o primeiro token;
 * 4. inicia o Parser pela função programa();
 * 5. verifica se toda a entrada foi consumida;
 * 6. informa se o programa é sintaticamente válido.
 *
 * Fluxo:
 *      código-fonte -> main.c ->  lexer.c -> tokens -> parser.c -> válido ou erro
 */


/*
 * VARIAVEIS GLOBAIS
 */

/*
 * Armazena todo o código-fonte recebido.
 */
char entrada[TAM];


/*
 * Armazena o texto correspondente ao token atual.
 *
 * Exemplo:
 *
 * se o Lexer reconhecer a variável "idade":
 *
 *      lexema = "idade"
 *      simbolo_lido = IDENTIFICADOR
 */
char lexema[TAM_LEXEMA];


/*
 * Indica a posição atual dentro de entrada.
 *
 * O Lexer utiliza essa variável para saber de qual
 * caractere deve continuar a análise.
 */
int posicao = 0;


/*
 * Armazena o token atual reconhecido pelo Lexer.
 *
 * Exemplos:
 *
 *      PROGRAM
 *      IDENTIFICADOR
 *      MAIS
 *      INTEIRO_LITERAL
 */
int simbolo_lido;


/*
 * FUNCAO: main
 *
 * Responsabilidade:
 *
 * Iniciar e coordenar todo o processo de análise.
 */
int main(){

    /*
     * Armazena temporariamente cada caractere
     * recebido por getchar().
     *
     * É int porque getchar() também precisa representar
     * o valor especial EOF.
     */
    int caractere;


    /*
     * Controla a posição utilizada para preencher
     * o vetor entrada.
     */
    int i = 0;


    printf(
        "Digite o programa MicroPascal:\n\n"
    );


    /*
     * LEITURA DO CODIGO-FONTE
     *
     * O programa pode possuir várias linhas.
     *
     * Por isso, os caracteres são lidos um por um até
     * encontrar EOF ou atingir o limite do vetor.
     */
    while(
        (caractere = getchar()) != EOF &&
        i < TAM - 1
    ){

        entrada[i] =
            caractere;

        i += 1;
    }


    /*
     * Coloca '\0' no final para transformar entrada
     * em uma string válida em C.
     */
    entrada[i] = '\0';


    /*
     * INICIO DA ANALISE LEXICA
     *
     * Solicita ao Lexer o primeiro token.
     *
     * A partir daqui:
     *
     *      simbolo_lido
     *
     * contém o token atual.
     */
    obtenha_simbolo();


    /*
     * INICIO DA ANALISE SINTATICA
     *
     * programa() é o símbolo inicial da gramática.
     *
     * A partir dela, o Parser chama as demais funções
     * necessárias.
     */
    programa();


    /*
     * VERIFICACAO FINAL
     *
     * Se o Parser reconheceu o programa corretamente,
     * depois do ponto final devemos encontrar FIM.
     *
     * Isso garante que não existem tokens extras após
     * o programa.
     */
    if(simbolo_lido == FIM){

        printf(
            "\nPrograma sintaticamente valido.\n"
        );

    }else{

        erro_sintatico();
    }


    return 0;
}