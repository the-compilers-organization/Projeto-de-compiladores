#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "globais.h"
#include "lexer.h"
#include "parser.h"

/* Definicoes unicas das variaveis compartilhadas. */
char entrada[TAM];
char lexema[TAM_LEXEMA];
int posicao = 0;
int simbolo_lido = FIM;

int main(void){
    int caractere;
    size_t tamanho = 0;

    printf("\n================================================================================================\n");
    printf("Digite ou cole o codigo MicroPascal:\n");
    printf("Para finalizar, pressione Enter e Ctrl+Z e Enter (Windows)\n");
    printf("ou Enter e Ctrl+D (Linux/macOS)\n");
    printf("================================================================================================\n\n");

    /* Lemos ate EOF: o Parser detecta tokens extras. */
    while((caractere = getchar()) != EOF){
        if(tamanho >= TAM - 1){
            printf("\n================================================================================================\n");
            fprintf(stderr, "Erro: codigo-fonte excede o limite de %d caracteres.\n", TAM - 1);
            printf("================================================================================================\n\n");
            return EXIT_FAILURE;
        }

        entrada[tamanho++] = (char)caractere;
    }

    if(ferror(stdin)){
        printf("\n================================================================================================\n");
        fprintf(stderr, "Erro ao ler a entrada.\n");
        printf("================================================================================================\n\n");
        return EXIT_FAILURE;
    }

    entrada[tamanho] = '\0';

    obtenha_simbolo();
    programa();

    if(simbolo_lido != FIM){
        erro_sintatico();
    }

    printf("\n================================================================================================\n");
    puts("Programa sintaticamente valido.");
    printf("================================================================================================\n\n");

    return EXIT_SUCCESS;
}