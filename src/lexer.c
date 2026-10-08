#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "define.h"


int palavra_reservada(char palavra[]){

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



void erro_lexico(char caractere){
    printf("Erro lexico no caracter [%c]\n",caractere);
    exit(1);
}



void obtenha_simbolo(void){
    int i;

    while(entrada[posicao] == ' ' || entrada[posicao] == '\n' || 
        entrada[posicao] == '\t' || entrada[posicao] == '\r'){
        posicao += 1;
    }

    if(entrada[posicao] == '\0'){
        strcpy(lexema, "FIM");
        simbolo_lido = FIM;
        return;
    }

    if(isalpha((unsigned char)entrada[posicao]) || entrada[posicao] == '_'){
        i = 0;

        while(isalnum((unsigned char)entrada[posicao]) || entrada[posicao] == '_'){
            if(i < TAM_LEXEMA - 1){
                lexema[i] =entrada[posicao];
                i += 1;
            }
            posicao += 1;
        }
        lexema[i] = '\0';
        simbolo_lido = palavra_reservada(lexema);
        return;
    }

    if(isdigit((unsigned char)entrada[posicao])){
        i = 0;

        while(isdigit((unsigned char)entrada[posicao])){
            if(i < TAM_LEXEMA - 1){
                lexema[i] = entrada[posicao];
                i += 1;
            }
            posicao += 1;
        }

        if(entrada[posicao] == '.' && isdigit((unsigned char)entrada[posicao + 1])){

            if(i < TAM_LEXEMA - 1){
                lexema[i] = '.';
                i += 1;
            }

            posicao += 1;

            while(isdigit((unsigned char)entrada[posicao])){
                if(i < TAM_LEXEMA - 1){
                    lexema[i] = entrada[posicao];
                    i += 1;
                }
                posicao += 1;
            }

            lexema[i] = '\0';
            simbolo_lido = REAL_LITERAL;

        }else{
            lexema[i] = '\0';
            simbolo_lido = INTEIRO_LITERAL;
        }

        return;
    }

    if(entrada[posicao] == '.' && //Reconhecimento de número real começando por ponto
        isdigit((unsigned char)entrada[posicao + 1])){
        i = 0;
        lexema[i] = '.';
        i += 1;
        posicao += 1;

        while(isdigit((unsigned char)entrada[posicao])){

            if(i < TAM_LEXEMA - 1){
                lexema[i] = entrada[posicao];
                i += 1;
            }
            posicao += 1;
        }

        lexema[i] = '\0';
        simbolo_lido = REAL_LITERAL;

        return;
    }

    if(entrada[posicao] == '\''){
        i = 0;
        lexema[i] = entrada[posicao];
        i += 1;
        posicao += 1;

        if(entrada[posicao] == '\\'){
            lexema[i] = entrada[posicao];
            i += 1;
            posicao += 1;

            if(entrada[posicao] != 'n' && entrada[posicao] != 't'){
                erro_lexico(entrada[posicao]);
            }

            lexema[i] = entrada[posicao];
            i += 1;
            posicao += 1;

        }else{
            //Caso normal: deve existir uma letra ou um dígito entre as aspas.
            if(!isalpha((unsigned char)entrada[posicao]) && !isdigit((unsigned char)entrada[posicao])){
                erro_lexico(entrada[posicao]);
            }
            lexema[i] = entrada[posicao];
            i += 1;
            posicao += 1;
        }

        /*
         * Depois do caractere é obrigatório
         * encontrar a aspa de fechamento.
         */
        if(entrada[posicao] != '\''){
            erro_lexico(entrada[posicao]);
        }

        lexema[i] = entrada[posicao];
        i += 1;
        posicao += 1;
        lexema[i] = '\0';

        simbolo_lido = CHAR_LITERAL;

        return;
    }


    // ================== A partir daqui são reconhecidos operadores e símbolos especiais

    if(entrada[posicao] == '+'){
        strcpy(lexema, "+");
        simbolo_lido = MAIS;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == '-'){
        strcpy(lexema, "-");
        simbolo_lido = MENOS;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == '*'){
        strcpy(lexema, "*");
        simbolo_lido = MULT;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == '/'){
        strcpy(lexema, "/");
        simbolo_lido = DIV_REAL;
        posicao += 1;
        return;
    }

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
            simbolo_lido = MENOR_IGUAL;
            posicao += 2;
        }
        else if(entrada[posicao + 1] == '>'){
            strcpy(lexema, "<>");
            simbolo_lido = DIFERENTE;
            posicao += 2;
        } else {
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
            simbolo_lido = MAIOR_IGUAL;
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
            simbolo_lido = ATRIBUICAO;
            posicao += 2;
        }else{
            strcpy(lexema, ":");
            simbolo_lido = DOIS_PONTOS;
            posicao += 1;
        }
        return;
    }

    if(entrada[posicao] == '('){
        strcpy(lexema, "(");
        simbolo_lido = ABRE_PAR;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == ')'){
        strcpy(lexema, ")");
        simbolo_lido = FECHA_PAR;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == ','){
        strcpy(lexema, ",");
        simbolo_lido = VIRGULA;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == ';'){
        strcpy(lexema, ";");
        simbolo_lido = PONTO_VIRGULA;
        posicao += 1;
        return;
    }

    if(entrada[posicao] == '.'){
        strcpy(lexema, ".");
        simbolo_lido = PONTO;
        posicao += 1;
        return;
    }


    //Se chegou até aqui, o caractere atual não pertence a nenhum token conhecido.
    erro_lexico(entrada[posicao]);
}