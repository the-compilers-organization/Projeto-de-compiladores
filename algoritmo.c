#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "define.h"


int proximo_token(void){

    while(isspace(entrada[posicao])){
        posicao += 1;
    }


    if(entrada[posicao] == '\0'){

        printf("FIM\n");

        return FIM;
    }


    if(isalpha(entrada[posicao])){

        posicao += 1;

        while(isalnum(entrada[posicao])){
            posicao += 1;
        }

        printf("IDENT\n");

        return IDENT;
    }


    if(isdigit(entrada[posicao])){

        posicao += 1;

        while(isdigit(entrada[posicao])){
            posicao += 1;
        }

        printf("NUMERO\n");

        return NUMERO;
    }


    if(entrada[posicao] == '+'){

        posicao += 1;

        printf("MAIS\n");

        return MAIS;
    }


    if(entrada[posicao] == '*'){

        posicao += 1;

        if(entrada[posicao] == '*'){

            posicao += 1;

            printf("POTENCIA\n");

            return POTENCIA;
        }

        printf("MULT\n");

        return MULT;
    }


    if(entrada[posicao] == '('){

        posicao += 1;

        printf("ABRE_PAR\n");

        return ABRE_PAR;
    }


    if(entrada[posicao] == ')'){

        posicao += 1;

        printf("FECHA_PAR\n");

        return FECHA_PAR;
    }


    erro_lexico(entrada[posicao]);

    return FIM;
}


void obtenha_simbolo(void){

    simbolo_lido = proximo_token();
}


void erro(const char *mensagem){

    int i;

    for(i = 0; mensagem[i] != '\0'; i += 1){
        printf("%c", mensagem[i]);
    }

    printf("\n");

    exit(1);
}


void erro_lexico(char caractere){

    printf("Erro lexico: caractere '%c' invalido\n",
           caractere);

    exit(1);
}


void expr(void){

    termo();

    if(simbolo_lido == MAIS){

        obtenha_simbolo();

        expr();
    }
}


void termo(void){

    fator();

    if(simbolo_lido == MULT){

        obtenha_simbolo();

        termo();
    }
}


void fator(void){

    primario();

    if(simbolo_lido == POTENCIA){

        obtenha_simbolo();

        fator();
    }
}


void primario(void){

    if(simbolo_lido == IDENT){

        obtenha_simbolo();

    }else if(simbolo_lido == NUMERO){

        obtenha_simbolo();

    }else if(simbolo_lido == ABRE_PAR){

        obtenha_simbolo();

        expr();

        if(simbolo_lido != FECHA_PAR){

            erro("Erro Sintatico: falta ')'");

        }else{

            obtenha_simbolo();
        }

    }else{

        erro("Erro Sintatico: primario invalido");
    }
}