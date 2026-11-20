#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/parser.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_fonte>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *arquivo = fopen(argv[1], "r");
    
    if (!arquivo) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    Lexer lexer;

    lexer_inicializar(&lexer, arquivo);

    Parser parser;

    // ETAPA 2: prepara o Parser e sua tabela de simbolos
    parser_inicializar(&parser, &lexer);


    // ETAPA 2: a analise tambem valida declaracoes e compatibilidade de tipos
    parser_analisar_programa(&parser);


    // ETAPA 2: libera a tabela de simbolos antes de encerrar a aplicacao
    parser_finalizar(&parser);


    lexer_finalizar(&lexer);


    fclose(arquivo);

    return EXIT_SUCCESS;
}
