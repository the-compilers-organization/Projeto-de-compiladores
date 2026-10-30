#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lexer.h"
#include "../include/parser.h"

/* Uso: micropascal arquivo.pas [--ast] */
int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3 || (argc == 3 && strcmp(argv[2], "--ast") != 0)) {
        fprintf(stderr, "Uso: %s <arquivo_fonte> [--ast]\n", argv[0]);
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
    parser_inicializar(&parser, &lexer);
    NoAST *arvore = parser_construir_ast(&parser);
    if (argc == 3) ast_imprimir(arvore, stdout);
    ast_liberar(arvore);
    lexer_finalizar(&lexer);
    fclose(arquivo);
    return EXIT_SUCCESS;
}
