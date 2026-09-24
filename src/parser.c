#include <stdio.h>
#include <stdlib.h>

#include "define.h"


/*
 * Responsabilidade:
 *
 * Este arquivo implementa o ANALISADOR SINTATICO (Parser).
 *
 * O Parser recebe os tokens produzidos pelo Lexer e verifica
 * se eles aparecem em uma sequência permitida pela gramática.
 *
 * Exemplo:
 *
 *      x := 10;
 *
 * Lexer:
 *
 *      IDENTIFICADOR
 *      ATRIBUICAO
 *      INTEIRO_LITERAL
 *      PONTO_VIRGULA
 *
 * Parser:
 *
 *      verifica se essa sequência corresponde a uma
 *      <atribuicao>.
 *
 * Cada função principal deste arquivo corresponde a um
 * símbolo não terminal da gramática.
 */


/*
 * FUNCAO: erro_sintatico
 *
 * Responsabilidade:
 *
 * Informar que o token atual não era permitido naquele
 * ponto da gramática.
 *
 * O lexema é mostrado para indicar onde o Parser encontrou
 * o problema.
 */
void erro_sintatico(void){

    printf(
        "Erro de sintaxe no token [%s]\n",
        lexema
    );

    exit(1);
}


/*
 * FUNCAO: consumir
 *
 * Responsabilidade:
 *
 * Verificar se o token atual é exatamente o token esperado
 * pelo Parser.
 *
 * Se for:
 *      chama obtenha_simbolo() para avançar.
 *
 * Se não for:
 *      gera erro sintático.
 *
 * Exemplo:
 *
 *      consumir(PROGRAM);
 *
 * significa:
 *
 *      "Neste ponto da gramática eu espero encontrar
 *       a palavra program."
 */
void consumir(
        int token){

    if(simbolo_lido == token){

        obtenha_simbolo();

    }else{

        erro_sintatico();
    }
}


/*
 * FUNCAO: programa
 *
 * Reconhece a estrutura geral de um programa.
 *
 * Gramática:
 *
 * <programa> ::=
 *      program IDENTIFICADOR ;
 *      <secao_var>
 *      <bloco>
 *      .
 */
void programa(void){

    consumir(PROGRAM);

    consumir(IDENTIFICADOR);

    consumir(PONTO_VIRGULA);

    secao_var();

    bloco();

    consumir(PONTO);
}


/*
 * FUNCAO: secao_var
 *
 * Reconhece a seção destinada às declarações de variáveis.
 *
 * Depois de "var", podem existir várias declarações.
 */
void secao_var(void){

    consumir(VAR);

    /*
     * Toda declaração começa com IDENTIFICADOR.
     *
     * Enquanto encontrarmos um identificador, existe
     * outra declaração para analisar.
     */
    while(
        simbolo_lido ==
        IDENTIFICADOR
    ){

        decl_var();
    }
}


/*
 * FUNCAO: decl_var
 *
 * Reconhece uma declaração de variáveis.
 *
 * Exemplo:
 *
 *      x : integer;
 *
 * ou:
 *
 *      x, y, z : integer;
 *
 * Depois do primeiro identificador, podem existir outros
 * identificadores separados por vírgula.
 */
void decl_var(void){

    consumir(
        IDENTIFICADOR
    );


    while(
        simbolo_lido ==
        VIRGULA
    ){

        consumir(
            VIRGULA
        );

        consumir(
            IDENTIFICADOR
        );
    }


    consumir(
        DOIS_PONTOS
    );


    tipo();


    consumir(
        PONTO_VIRGULA
    );
}


/*
 * FUNCAO: tipo
 *
 * Reconhece um dos tipos permitidos pela linguagem:
 *
 *      integer
 *      real
 *      char
 */
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


/*
 * FUNCAO: bloco
 *
 * Reconhece um bloco de comandos delimitado por:
 *
 *      begin
 *          ...
 *      end
 */
void bloco(void){

    consumir(
        BEGIN_TOKEN
    );

    lista_comandos();

    consumir(
        END_TOKEN
    );
}


/*
 * FUNCAO: lista_comandos
 *
 * Reconhece zero ou mais comandos.
 *
 * Para saber se existe outro comando, verifica se o token
 * atual pode iniciar algum dos comandos da gramática.
 */
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


/*
 * FUNCAO: comando
 *
 * Responsabilidade:
 *
 * Descobrir qual tipo de comando está começando e chamar
 * a função responsável por reconhecê-lo.
 *
 * O primeiro token permite decidir entre:
 *
 *      begin  -> bloco
 *      IDENTIFICADOR -> atribuição
 *      while/repeat -> iteração
 *      if -> decisão
 *      write -> escrita
 */
void comando(void){

    if(simbolo_lido == BEGIN_TOKEN){

        bloco();

        consumir(
            PONTO_VIRGULA
        );

    }else if(
        simbolo_lido ==
        IDENTIFICADOR
    ){

        atribuicao();

    }else if(
        simbolo_lido == WHILE ||
        simbolo_lido == REPEAT
    ){

        iteracao();

    }else if(
        simbolo_lido == IF
    ){

        decisao();

    }else if(
        simbolo_lido == WRITE
    ){

        escrita();

    }else{

        erro_sintatico();
    }
}


/*
 * FUNCAO: atribuicao
 *
 * Reconhece uma atribuição.
 *
 * Exemplo:
 *
 *      x := 10;
 *
 *      soma := x + 5;
 */
void atribuicao(void){

    consumir(
        IDENTIFICADOR
    );

    consumir(
        ATRIBUICAO
    );

    expressao();

    consumir(
        PONTO_VIRGULA
    );
}


/*
 * FUNCAO: iteracao
 *
 * Reconhece as duas estruturas de repetição:
 *
 *      while <expressao> do <comando>
 *
 * ou:
 *
 *      repeat <comando> until <expressao> ;
 */
void iteracao(void){

    if(simbolo_lido == WHILE){

        consumir(WHILE);

        expressao();

        consumir(DO);

        comando();

    }else if(
        simbolo_lido == REPEAT
    ){

        consumir(REPEAT);

        comando();

        consumir(UNTIL);

        expressao();

        consumir(
            PONTO_VIRGULA
        );

    }else{

        erro_sintatico();
    }
}


/*
 * FUNCAO: decisao
 *
 * Reconhece uma estrutura condicional.
 *
 * Pode existir somente:
 *
 *      if <expressao> then <comando>
 *
 * ou também possuir:
 *
 *      else <comando>
 *
 * O else é opcional, por isso é verificado com um if.
 */
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


/*
 * FUNCAO: escrita
 *
 * Reconhece o comando write.
 *
 * Exemplo:
 *
 *      write(x);
 *
 *      write('a');
 */
void escrita(void){

    consumir(WRITE);

    consumir(ABRE_PAR);

    expressao();

    consumir(FECHA_PAR);

    consumir(
        PONTO_VIRGULA
    );
}


/*
 * FUNCAO: expressao
 *
 * Ponto de entrada para a análise das expressões.
 *
 * A análise é dividida em níveis de precedência.
 */
void expressao(void){

    expr_logica();
}


/*
 * FUNCAO: expr_logica
 *
 * Reconhece os operadores:
 *
 *      or
 *      and
 *
 * Esse é o nível de menor precedência entre os níveis
 * definidos para as expressões.
 */
void expr_logica(void){

    expr_relacional();


    while(
        simbolo_lido == OR ||
        simbolo_lido == AND
    ){

        if(simbolo_lido == OR){

            consumir(OR);

        }else{

            consumir(AND);
        }

        expr_relacional();
    }
}


/*
 * FUNCAO: expr_relacional
 *
 * Reconhece os operadores relacionais:
 *
 *      =
 *      <>
 *      <
 *      >
 *      <=
 *      >=
 */
void expr_relacional(void){

    expr_aditiva();


    while(
        simbolo_lido == IGUAL ||
        simbolo_lido == DIFERENTE ||
        simbolo_lido == MENOR ||
        simbolo_lido == MAIOR ||
        simbolo_lido == MENOR_IGUAL ||
        simbolo_lido == MAIOR_IGUAL
    ){

        /*
         * Guarda qual operador foi encontrado.
         *
         * Como todos os operadores deste nível possuem
         * o mesmo comportamento sintático, podemos
         * consumir o próprio valor de simbolo_lido.
         */
        int operador =
            simbolo_lido;

        consumir(
            operador
        );

        expr_aditiva();
    }
}


/*
 * FUNCAO: expr_aditiva
 *
 * Reconhece:
 *
 *      +
 *      -
 *
 * Primeiro reconhece uma expressão de maior precedência
 * e depois procura novas operações de soma ou subtração.
 */
void expr_aditiva(void){

    expr_multiplicativa();


    while(
        simbolo_lido == MAIS ||
        simbolo_lido == MENOS
    ){

        int operador =
            simbolo_lido;

        consumir(
            operador
        );

        expr_multiplicativa();
    }
}


/*
 * FUNCAO: expr_multiplicativa
 *
 * Reconhece:
 *
 *      *
 *      /
 *      div
 *
 * Essas operações possuem precedência maior que
 * soma e subtração.
 */
void expr_multiplicativa(void){

    expr_basica();


    while(
        simbolo_lido == MULT ||
        simbolo_lido == DIV_REAL ||
        simbolo_lido == DIV
    ){

        int operador =
            simbolo_lido;

        consumir(
            operador
        );

        expr_basica();
    }
}


/*
 * FUNCAO: expr_basica
 *
 * Reconhece os elementos básicos utilizados para construir
 * expressões:
 *
 *      ( <expressao> )
 *      not <expressao>
 *      INTEIRO_LITERAL
 *      REAL_LITERAL
 *      CHAR_LITERAL
 *      IDENTIFICADOR
 *
 * Exemplos:
 *
 *      10
 *      x
 *      'a'
 *      (x + 5)
 *      not x
 */
void expr_basica(void){

    if(simbolo_lido == ABRE_PAR){

        consumir(
            ABRE_PAR
        );

        expressao();

        consumir(
            FECHA_PAR
        );

    }else if(
        simbolo_lido == NOT
    ){

        consumir(NOT);

        expressao();

    }else if(
        simbolo_lido ==
        INTEIRO_LITERAL
    ){

        consumir(
            INTEIRO_LITERAL
        );

    }else if(
        simbolo_lido ==
        REAL_LITERAL
    ){

        consumir(
            REAL_LITERAL
        );

    }else if(
        simbolo_lido ==
        CHAR_LITERAL
    ){

        consumir(
            CHAR_LITERAL
        );

    }else if(
        simbolo_lido ==
        IDENTIFICADOR
    ){

        consumir(
            IDENTIFICADOR
        );

    }else{

        erro_sintatico();
    }
}