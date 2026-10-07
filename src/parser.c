#include <stdio.h>
#include <stdlib.h>
#include "define.h"


void erro_sintatico(void){
    printf("Erro de sintaxe no token [%s]\n", lexema);
    exit(1);
}


void consumir(int token){
    if(simbolo_lido == token){
        obtenha_simbolo();
    }else{
        erro_sintatico();
    }
}


void programa(void){
    consumir(PROGRAM);
    consumir(IDENTIFICADOR);
    consumir(PONTO_VIRGULA);
    secao_var();
    bloco();
    consumir(PONTO);
}


void secao_var(void){
    consumir(VAR);

    while(simbolo_lido == IDENTIFICADOR){
        decl_var();
    }
}


void decl_var(void){
    consumir(IDENTIFICADOR);

    while(simbolo_lido == VIRGULA){
        consumir(VIRGULA);
        consumir(IDENTIFICADOR);
    }

    consumir(DOIS_PONTOS);
    tipo();
    consumir(PONTO_VIRGULA);
}


void tipo(void){
    if(simbolo_lido == INTEGER){
        consumir(INTEGER);

    }else if(simbolo_lido == REAL){
        consumir(REAL);

    }else if(simbolo_lido == CHAR){
        consumir(CHAR);
        
    }else{
        erro_sintatico();
    }
}


void bloco(void){
    consumir(BEGIN_TOKEN);
    lista_comandos();
    consumir(END_TOKEN);
}



void lista_comandos(void){

    while(
        simbolo_lido == BEGIN_TOKEN ||
        simbolo_lido == IDENTIFICADOR ||
        simbolo_lido == WHILE ||
        simbolo_lido == REPEAT ||
        simbolo_lido == IF ||
        simbolo_lido == WRITE
    ){

        comando();
    }
}


void comando(void){

    if(simbolo_lido == BEGIN_TOKEN){
        
        bloco();
        consumir(PONTO_VIRGULA);

    }else if(simbolo_lido == IDENTIFICADOR){

        atribuicao();

    }else if(simbolo_lido == WHILE || simbolo_lido == REPEAT){

        iteracao();

    }else if(simbolo_lido == IF){

        decisao();

    }else if(simbolo_lido == WRITE){

        escrita();

    }else{

        erro_sintatico();
    }
}


void atribuicao(void){
    consumir(IDENTIFICADOR);
    consumir(ATRIBUICAO);
    expressao();
    consumir(PONTO_VIRGULA);
}


void iteracao(void){

    if(simbolo_lido == WHILE){

        consumir(WHILE);
        expressao();
        consumir(DO);
        comando();

    }else if(simbolo_lido == REPEAT){

        consumir(REPEAT);
        comando();
        consumir(UNTIL);
        expressao();
        consumir(PONTO_VIRGULA);

    }else{

        erro_sintatico();
    }
}


void decisao(void){
    consumir(IF);
    expressao();
    consumir(THEN);
    comando();

    if(simbolo_lido == ELSE){
        consumir(ELSE);
        comando();
    }
}


void escrita(void){
    consumir(WRITE);
    consumir(ABRE_PAR);
    expressao();
    consumir(FECHA_PAR);
    consumir(PONTO_VIRGULA);
}


void expressao(void){
    expr_logica();
}


void expr_logica(void){
    expr_relacional();

    while(simbolo_lido == OR || simbolo_lido == AND){

        if(simbolo_lido == OR){
            consumir(OR);
        }else{
            consumir(AND);
        }
        expr_relacional();

    }
}


void expr_relacional(void){
    expr_aditiva();

    while(
        simbolo_lido == IGUAL ||
        simbolo_lido == DIFERENTE ||
        simbolo_lido == MENOR ||
        simbolo_lido == MAIOR ||
        simbolo_lido == MENOR_IGUAL ||
        simbolo_lido == MAIOR_IGUAL){

        int operador = simbolo_lido;

        consumir(operador);
        expr_aditiva();
    }
}


void expr_aditiva(void){
    expr_multiplicativa();

    while(simbolo_lido == MAIS || simbolo_lido == MENOS){

        int operador = simbolo_lido;
        consumir(operador);
        expr_multiplicativa();
    }
}


void expr_multiplicativa(void){

    expr_basica();

    while(simbolo_lido == MULT || simbolo_lido == DIV_REAL || simbolo_lido == DIV){
        int operador = simbolo_lido;
        consumir(operador);
        expr_basica();
    }
}


void expr_basica(void){

    if(simbolo_lido == ABRE_PAR){

        consumir(ABRE_PAR);
        expressao();
        consumir(FECHA_PAR);

    }else if(simbolo_lido == NOT){

        consumir(NOT);
        expressao();

    }else if( simbolo_lido == INTEIRO_LITERAL){

        consumir(INTEIRO_LITERAL);

    }else if(simbolo_lido == REAL_LITERAL){

        consumir(REAL_LITERAL);

    }else if(simbolo_lido == CHAR_LITERAL){

        consumir(CHAR_LITERAL);

    }else if(simbolo_lido == IDENTIFICADOR){

        consumir(IDENTIFICADOR);

    }else{
        erro_sintatico();
    }
}