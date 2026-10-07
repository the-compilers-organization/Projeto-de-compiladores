#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 *      código-fonte -> main.c -> lexer.c -> tokens -> parser.c -> válido ou erro
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
     * Armazena temporariamente cada linha
     * digitada pelo usuário.
     */
    char linha[500];


    /*
     * Controla a posição utilizada para preencher
     * o vetor entrada.
     */
    int i = 0;


    /*
     * Controla a posição utilizada para percorrer
     * cada linha recebida.
     */
    int j;


    /*
     * Armazena o tamanho da linha digitada.
     */
    int tamanho_linha;


    /*
     * Indica se foi encontrado "end." no final
     * da linha.
     *
     * 0 -> ainda não terminou
     * 1 -> terminou
     */
    int fim = 0;


    printf(
        "Digite o programa MicroPascal:\n\n"
    );


    /*
     * LEITURA DO CODIGO-FONTE
     *
     * O programa pode possuir várias linhas.
     *
     * Por isso, cada linha é lida utilizando fgets().
     *
     * A leitura termina quando uma linha possuir
     * "end." no final.
     *
     * Dessa forma, não é necessário utilizar
     * Ctrl + Z para indicar EOF.
     */
    while(
        fim == 0 &&
        fgets(
            linha,
            sizeof(linha),
            stdin
        ) != NULL
    ){

        /*
         * Copia a linha recebida para o vetor
         * que armazena todo o código-fonte.
         */
        j = 0;

        while(
            linha[j] != '\0' &&
            i < TAM - 1
        ){

            entrada[i] =
                linha[j];

            i += 1;

            j += 1;
        }


        /*
         * Obtém o tamanho da linha recebida.
         */
        tamanho_linha =
            strlen(linha);


        /*
         * fgets() normalmente mantém o '\n'
         * gerado pelo Enter.
         *
         * No Windows também pode existir '\r'.
         *
         * Para verificar se a linha termina em
         * "end.", esses caracteres são ignorados
         * apenas durante esta verificação.
         */
        while(
            tamanho_linha > 0 &&
            (
                linha[tamanho_linha - 1] == '\n' ||
                linha[tamanho_linha - 1] == '\r'
            )
        ){

            tamanho_linha -= 1;
        }


        /*
         * Verifica se os quatro últimos caracteres
         * da linha são:
         *
         *      end.
         */
        if(tamanho_linha >= 4){

            if(
                linha[tamanho_linha - 4] == 'e' &&
                linha[tamanho_linha - 3] == 'n' &&
                linha[tamanho_linha - 2] == 'd' &&
                linha[tamanho_linha - 1] == '.'
            ){

                fim = 1;
            }
        }
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