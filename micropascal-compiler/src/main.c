#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/parser.h"

/*
 * PONTO DE ENTRADA DO COMPILADOR
 * Uso: micropascal arquivo.pas
 *
 * TODO (segunda etapa - analise semantica):
 * - Manter a abertura do arquivo e a inicializacao do Lexer.
 * - Conferir em parser.h como a tabela de simbolos sera armazenada:
 *   dentro de Parser ou como estrutura passada por parametro.
 * - Garantir que a tabela hash seja inicializada ANTES de analisar o programa.
 * - Apos a analise, liberar os simbolos alocados dinamicamente.
 * - Evitar criar ou imprimir AST: a nova etapa nao usa arvore sintatica.
 * - Se a API do Parser for alterada, atualizar as chamadas abaixo.
 * - Definir com a equipe o tratamento de erros: atualmente o Parser
 *   encerra o processo com exit(), sem executar a limpeza em main().
 *   Se desejarem liberar recursos tambem em caso de erro, sera preciso
 *   adaptar o fluxo de retorno de erros ou centralizar a finalizacao.
 */
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
    /* Lexer: permanece responsavel pela leitura dos tokens. */
    Lexer lexer;
    lexer_inicializar(&lexer, arquivo);
    /* TODO: integrar a tabela de simbolos conforme a API definida em parser.h. */
    Parser parser;
    parser_inicializar(&parser, &lexer);
    /* TODO: nesta chamada, o Parser devera verificar sintaxe e semantica. */
    parser_analisar_programa(&parser);
    /* TODO: liberar a tabela hash antes de encerrar, se foi inicializada. */
    lexer_finalizar(&lexer);
    fclose(arquivo);
    return EXIT_SUCCESS;
}
