#include <stdio.h>
#include <stdlib.h>
#include "define.h"


int simbolo_lido;

int posicao = 0;

char entrada[TAM];


int main(){

    printf("Digite uma expressao: ");

    fgets(entrada, TAM, stdin);


    obtenha_simbolo();

    expr();


    if(simbolo_lido == FIM){

        printf("Expressao Valida\n");

    }else{

        erro("Simbolo inesperado");
    }


    return 0;
}