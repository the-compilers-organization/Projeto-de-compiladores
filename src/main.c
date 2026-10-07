#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"


char entrada[TAM]; //Armazena todo o código-fonte recebido.
char lexema[TAM_LEXEMA];
int posicao = 0; //Indica a posição atual dentro de entrada.
int simbolo_lido;


int main(){
    char linha[500]; //Armazena temporariamente cada linha digitada pelo usuário.
    int i = 0; //Controla a posição utilizada para preencher o vetor entrada.
    int j; //Controla a posição utilizada para percorrer cada linha recebida.
    int tamanho_linha; //Armazena o tamanho da linha digitada.
    int fim = 0;

    printf("Digite o programa MicroPascal:\n\n");

    while(fim == 0 && fgets(linha, sizeof(linha),stdin) != NULL){
        j = 0;

        while(linha[j] != '\0' && i < TAM - 1){
            entrada[i] = linha[j];
            i += 1;
            j += 1;
        }

        tamanho_linha = strlen(linha);

        while(tamanho_linha > 0 && (linha[tamanho_linha - 1] == '\n' || linha[tamanho_linha - 1] == '\r')){
            tamanho_linha -= 1;
        }

        if(tamanho_linha >= 4){
            if(linha[tamanho_linha - 4] == 'e' && linha[tamanho_linha - 3] == 'n' &&
               linha[tamanho_linha - 2] == 'd' && linha[tamanho_linha - 1] == '.'){
                fim = 1;
            }
        }
    }

    entrada[i] = '\0';

    obtenha_simbolo();
    programa();

    if(simbolo_lido == FIM){
        printf("\nPrograma sintaticamente valido.\n");
    }else{
        erro_sintatico();
    }


    return 0;
}