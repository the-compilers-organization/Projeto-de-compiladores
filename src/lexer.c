#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "define.h"


/*
 * Responsabilidade:
 *
 * Este arquivo implementa o ANALISADOR LEXICO (Lexer).
 *
 * O Lexer percorre o código-fonte armazenado em entrada
 * caractere por caractere e agrupa esses caracteres em
 * lexemas.
 *
 * Para cada lexema encontrado, determina qual token ele
 * representa.
 *
 *
 * Tokens:
 *
 *      IDENTIFICADOR
 *      ATRIBUICAO
 *      INTEIRO_LITERAL
 *      PONTO_VIRGULA
 *
 * Portanto: caracteres -> Lexer -> tokens
 */


/*
 * FUNCAO: palavra_reservada
 *
 * Responsabilidade:
 *
 * Verificar se uma sequência de letras reconhecida pelo
 * Lexer corresponde a uma palavra reservada da linguagem.
 *
 * Exemplos:
 *
 *      "program" -> PROGRAM
 *      "while"   -> WHILE
 *      "integer" -> INTEGER
 *      "div"     -> DIV
 *
 * Se a palavra não for reservada:
 *
 *      "idade" -> IDENTIFICADOR
 */
int palavra_reservada(
        char palavra[]){

    if(strcmp(palavra, "program") == 0){
        return PROGRAM;
    }

    if(strcmp(palavra, "if") == 0){
        return IF;
    }

    if(strcmp(palavra, "then") == 0){
        return THEN;
    }

    if(strcmp(palavra, "else") == 0){
        return ELSE;
    }

    if(strcmp(palavra, "while") == 0){
        return WHILE;
    }

    if(strcmp(palavra, "do") == 0){
        return DO;
    }

    if(strcmp(palavra, "repeat") == 0){
        return REPEAT;
    }

    if(strcmp(palavra, "until") == 0){
        return UNTIL;
    }

    if(strcmp(palavra, "integer") == 0){
        return INTEGER;
    }

    if(strcmp(palavra, "real") == 0){
        return REAL;
    }

    if(strcmp(palavra, "char") == 0){
        return CHAR;
    }

    if(strcmp(palavra, "begin") == 0){
        return BEGIN_TOKEN;
    }

    if(strcmp(palavra, "end") == 0){
        return END_TOKEN;
    }

    if(strcmp(palavra, "write") == 0){
        return WRITE;
    }

    if(strcmp(palavra, "var") == 0){
        return VAR;
    }

    if(strcmp(palavra, "div") == 0){
        return DIV;
    }

    if(strcmp(palavra, "and") == 0){
        return AND;
    }

    if(strcmp(palavra, "or") == 0){
        return OR;
    }

    if(strcmp(palavra, "not") == 0){
        return NOT;
    }

    return IDENTIFICADOR;
}


/*
 * ============================================================
 * FUNCAO: erro_lexico
 * ============================================================
 *
 * Responsabilidade:
 *
 * Informar que foi encontrado um caractere que não pode
 * iniciar ou formar um token válido da linguagem.
 *
 * Após mostrar a mensagem, a execução é encerrada porque
 * não é possível continuar a análise corretamente.
 */
void erro_lexico(
        char caractere){

    printf(
        "Erro lexico no caracter [%c]\n",
        caractere
    );

    exit(1);
}


/*
 * FUNCAO: obtenha_simbolo
 *
 * Responsabilidade:
 *
 * Esta é a principal função do Lexer.
 *
 * Ela começa na posição atual da entrada, identifica o
 * próximo lexema, determina seu token e atualiza:
 *
 *      lexema
 *      simbolo_lido
 *      posicao
 *
 * A função reconhece:
 *
 * - identificadores;
 * - palavras reservadas;
 * - números inteiros;
 * - números reais;
 * - caracteres literais;
 * - operadores;
 * - símbolos especiais;
 * - fim da entrada.
 *
 * Espaços, tabulações e quebras de linha são ignorados.
 */
void obtenha_simbolo(void){

    int i;


    /*
     * Ignora caracteres que servem apenas para separar
     * os lexemas.
     */
    while(
        entrada[posicao] == ' ' ||
        entrada[posicao] == '\n' ||
        entrada[posicao] == '\t' ||
        entrada[posicao] == '\r'
    ){

        posicao += 1;
    }


    /*
     * Se encontrar '\0', chegamos ao final do
     * código-fonte.
     */
    if(entrada[posicao] == '\0'){

        strcpy(
            lexema,
            "FIM"
        );

        simbolo_lido = FIM;

        return;
    }


    /*
     * Reconhecimento de identificadores e palavras
     * reservadas.
     *
     * Primeiro o Lexer lê a palavra completa.
     * Depois palavra_reservada() determina se ela é
     * reservada ou um IDENTIFICADOR.
     */
    if(
        isalpha(
            (unsigned char)
            entrada[posicao]
        ) ||
        entrada[posicao] == '_'
    ){

        i = 0;

        while(
            isalnum(
                (unsigned char)
                entrada[posicao]
            ) ||
            entrada[posicao] == '_'
        ){

            if(i < TAM_LEXEMA - 1){

                lexema[i] =
                    entrada[posicao];

                i += 1;
            }

            posicao += 1;
        }

        lexema[i] = '\0';

        simbolo_lido =
            palavra_reservada(
                lexema
            );

        return;
    }


    /*
     * Reconhecimento de números.
     *
     * Exemplos:
     *
     *      25    -> INTEIRO_LITERAL
     *      25.5  -> REAL_LITERAL
     */
    if(isdigit(
        (unsigned char)
        entrada[posicao]
    )){

        i = 0;

        while(isdigit(
            (unsigned char)
            entrada[posicao]
        )){

            if(i < TAM_LEXEMA - 1){

                lexema[i] =
                    entrada[posicao];

                i += 1;
            }

            posicao += 1;
        }


        /*
         * Se após os dígitos existir um ponto seguido
         * por outro dígito, temos um REAL_LITERAL.
         */
        if(
            entrada[posicao] == '.' &&
            isdigit(
                (unsigned char)
                entrada[posicao + 1]
            )
        ){

            if(i < TAM_LEXEMA - 1){

                lexema[i] = '.';
                i += 1;
            }

            posicao += 1;

            while(isdigit(
                (unsigned char)
                entrada[posicao]
            )){

                if(i < TAM_LEXEMA - 1){

                    lexema[i] =
                        entrada[posicao];

                    i += 1;
                }

                posicao += 1;
            }

            lexema[i] = '\0';

            simbolo_lido =
                REAL_LITERAL;

        }else{

            lexema[i] = '\0';

            simbolo_lido =
                INTEIRO_LITERAL;
        }

        return;
    }


    /*
     * Reconhecimento de número real começando por ponto.
     */
    if(
        entrada[posicao] == '.' &&
        isdigit(
            (unsigned char)
            entrada[posicao + 1]
        )
    ){

        i = 0;

        lexema[i] = '.';
        i += 1;
        posicao += 1;

        while(isdigit(
            (unsigned char)
            entrada[posicao]
        )){

            if(i < TAM_LEXEMA - 1){

                lexema[i] =
                    entrada[posicao];

                i += 1;
            }

            posicao += 1;
        }

        lexema[i] = '\0';

        simbolo_lido =
            REAL_LITERAL;

        return;
    }


    /*
     * Reconhecimento de CHAR_LITERAL.
     *
     * Exemplos:
     *
     *      'a'
     *      '5'
     *      '\n'
     *      '\t'
     */
    if(entrada[posicao] == '\''){

        i = 0;

        lexema[i] =
            entrada[posicao];

        i += 1;
        posicao += 1;


        /*
         * Verifica os caracteres especiais
         * \n e \t.
         */
        if(entrada[posicao] == '\\'){

            lexema[i] =
                entrada[posicao];

            i += 1;
            posicao += 1;

            if(
                entrada[posicao] != 'n' &&
                entrada[posicao] != 't'
            ){

                erro_lexico(
                    entrada[posicao]
                );
            }

            lexema[i] =
                entrada[posicao];

            i += 1;
            posicao += 1;

        }else{

            /*
             * Caso normal: deve existir uma letra
             * ou um dígito entre as aspas.
             */
            if(
                !isalpha(
                    (unsigned char)
                    entrada[posicao]
                ) &&
                !isdigit(
                    (unsigned char)
                    entrada[posicao]
                )
            ){

                erro_lexico(
                    entrada[posicao]
                );
            }

            lexema[i] =
                entrada[posicao];

            i += 1;
            posicao += 1;
        }


        /*
         * Depois do caractere é obrigatório
         * encontrar a aspa de fechamento.
         */
        if(entrada[posicao] != '\''){

            erro_lexico(
                entrada[posicao]
            );
        }

        lexema[i] =
            entrada[posicao];

        i += 1;
        posicao += 1;

        lexema[i] = '\0';

        simbolo_lido =
            CHAR_LITERAL;

        return;
    }


    /*
     * A partir daqui são reconhecidos operadores
     * e símbolos especiais.
     */


    /* Operador + */
    if(entrada[posicao] == '+'){

        strcpy(lexema, "+");

        simbolo_lido = MAIS;

        posicao += 1;

        return;
    }


    /* Operador - */
    if(entrada[posicao] == '-'){

        strcpy(lexema, "-");

        simbolo_lido = MENOS;

        posicao += 1;

        return;
    }


    /* Operador * */
    if(entrada[posicao] == '*'){

        strcpy(lexema, "*");

        simbolo_lido = MULT;

        posicao += 1;

        return;
    }


    /* Operador / */
    if(entrada[posicao] == '/'){

        strcpy(lexema, "/");

        simbolo_lido = DIV_REAL;

        posicao += 1;

        return;
    }


    /* Operador relacional = */
    if(entrada[posicao] == '='){

        strcpy(lexema, "=");

        simbolo_lido = IGUAL;

        posicao += 1;

        return;
    }


    /*
     * Operadores iniciados por <.
     *
     * O Lexer precisa olhar o próximo caractere
     * para diferenciar:
     *
     *      <
     *      <=
     *      <>
     */
    if(entrada[posicao] == '<'){

        if(entrada[posicao + 1] == '='){

            strcpy(lexema, "<=");

            simbolo_lido =
                MENOR_IGUAL;

            posicao += 2;

        }else if(
            entrada[posicao + 1] == '>'
        ){

            strcpy(lexema, "<>");

            simbolo_lido =
                DIFERENTE;

            posicao += 2;

        }else{

            strcpy(lexema, "<");

            simbolo_lido = MENOR;

            posicao += 1;
        }

        return;
    }


    /*
     * Operadores iniciados por >.
     *
     * Pode ser:
     *
     *      >
     *      >=
     */
    if(entrada[posicao] == '>'){

        if(entrada[posicao + 1] == '='){

            strcpy(lexema, ">=");

            simbolo_lido =
                MAIOR_IGUAL;

            posicao += 2;

        }else{

            strcpy(lexema, ">");

            simbolo_lido = MAIOR;

            posicao += 1;
        }

        return;
    }


    /*
     * O caractere ':' exige lookahead porque pode
     * representar:
     *
     *      :    -> DOIS_PONTOS
     *      :=   -> ATRIBUICAO
     */
    if(entrada[posicao] == ':'){

        if(entrada[posicao + 1] == '='){

            strcpy(lexema, ":=");

            simbolo_lido =
                ATRIBUICAO;

            posicao += 2;

        }else{

            strcpy(lexema, ":");

            simbolo_lido =
                DOIS_PONTOS;

            posicao += 1;
        }

        return;
    }


    /* Abre parêntese */
    if(entrada[posicao] == '('){

        strcpy(lexema, "(");

        simbolo_lido =
            ABRE_PAR;

        posicao += 1;

        return;
    }


    /* Fecha parêntese */
    if(entrada[posicao] == ')'){

        strcpy(lexema, ")");

        simbolo_lido =
            FECHA_PAR;

        posicao += 1;

        return;
    }


    /* Vírgula */
    if(entrada[posicao] == ','){

        strcpy(lexema, ",");

        simbolo_lido =
            VIRGULA;

        posicao += 1;

        return;
    }


    /* Ponto e vírgula */
    if(entrada[posicao] == ';'){

        strcpy(lexema, ";");

        simbolo_lido =
            PONTO_VIRGULA;

        posicao += 1;

        return;
    }


    /* Ponto */
    if(entrada[posicao] == '.'){

        strcpy(lexema, ".");

        simbolo_lido =
            PONTO;

        posicao += 1;

        return;
    }


    /*
     * Se chegou até aqui, o caractere atual não
     * pertence a nenhum token conhecido.
     */
    erro_lexico(
        entrada[posicao]
    );
}