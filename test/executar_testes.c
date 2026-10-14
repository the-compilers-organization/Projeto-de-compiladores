/* Executor dos testes MicroPascal - C11, Windows e Linux.
   Uso: ./executar_testes ./micropascal
   Execute a partir da raiz do projeto, onde esta a pasta test/. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 4096
#define MAX_COMANDO 8192
#define MAX_SAIDA 2048

static void retirar_fim(char *s) {
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r')) s[--n] = '\0';
}

static void classificar(const char *saida, int codigo, char *resultado) {
    if (codigo == 0 && strstr(saida, "Programa sintaticamente valido."))
        strcpy(resultado, "ACEITA");
    else if (codigo != 0 && strstr(saida, "Erro de sintaxe no token ["))
        strcpy(resultado, "SINTATICO");
    else if (codigo != 0 && (strstr(saida, "Erro léxico") || strstr(saida, "Erro lexico")))
        strcpy(resultado, "LEXICO");
    else if (codigo != 0 && strstr(saida, "codigo-fonte excede o limite"))
        strcpy(resultado, "ENTRADA");
    else
        strcpy(resultado, "OUTRO");
}

static void csv_texto(FILE *f, const char *s) {
    fputc('"', f);
    for (; *s; ++s) {
        if (*s == '"') fputc('"', f);
        if (*s == '\n' || *s == '\r') fputc(' ', f);
        else fputc(*s, f);
    }
    fputc('"', f);
}

int main(int argc, char **argv) {
    FILE *manifesto, *relatorio;
    char linha[MAX_LINHA], comando[MAX_COMANDO], saida[MAX_SAIDA];
    char caminho[MAX_LINHA], esperado[40], observado[40], categoria[80];
    int total = 0, aprovados = 0, divergencias = 0;
    const char *executavel;
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <caminho-do-micropascal>\n", argv[0]);
        return 2;
    }
    executavel = argv[1];
    manifesto = fopen("test/manifesto.csv", "rb");
    if (!manifesto) {
        fprintf(stderr, "Nao foi possivel abrir test/manifesto.csv. Execute na raiz do projeto.\n");
        return 2;
    }
    /* "wb" cria o relatorio ou substitui o conteudo do arquivo existente.
       Se outro programa bloquear o CSV, o Windows pode negar o acesso. */
    relatorio = fopen("test/resultado_execucao.csv", "wb");
    if (!relatorio) {
        perror("Nao foi possivel criar ou atualizar test/resultado_execucao.csv");
        fprintf(stderr,
                "Verifique se o CSV esta aberto no Excel ou em outro programa, "
                "feche-o e tente novamente.\n"
                "Se persistir, confira as permissoes de gravacao da pasta test/.\n");
        fclose(manifesto);
        return 2;
    }
    fprintf(relatorio, "arquivo,categoria,esperado,observado,resultado,detalhe\n");
    /* Ignora o cabecalho; UTF-8 com BOM tambem e aceito. */
    if (!fgets(linha, sizeof linha, manifesto)) {
        fprintf(stderr, "Manifesto vazio.\n");
        fclose(manifesto); fclose(relatorio);
        return 2;
    }
    while (fgets(linha, sizeof linha, manifesto)) {
        char *p1, *p2, *p3;
        FILE *f;
        int retorno;
        size_t lidos;
        retirar_fim(linha);
        if (!linha[0]) continue;
        p1 = strchr(linha, ',');
        if (!p1) continue;
        *p1++ = '\0';
        p2 = strchr(p1, ',');
        if (!p2) continue;
        *p2++ = '\0';
        p3 = strchr(p2, ',');
        if (p3) *p3 = '\0';
        if (snprintf(caminho, sizeof caminho, "test/%s", linha) >= (int)sizeof caminho) {
            fprintf(stderr, "Caminho de teste muito longo.\n");
            divergencias++; total++;
            continue;
        }
        snprintf(categoria, sizeof categoria, "%s", p1);
        snprintf(esperado, sizeof esperado, "%s", p2);
        f = fopen(caminho, "rb");
        if (!f) {
            fprintf(stderr, "Arquivo de teste ausente: %s\n", caminho);
            divergencias++; total++;
            continue;
        }
        fclose(f);
        /* No Windows, cmd.exe exige aspas externas quando o comando
           comeca com um executavel entre aspas e usa redirecionamento. */
#ifdef _WIN32
        if (snprintf(comando, sizeof comando,
                "\"\"%s\" < \"%s\" > \"test/.saida_teste.tmp\" 2>&1\"",
                executavel, caminho) >= (int)sizeof comando) {
#else
        if (snprintf(comando, sizeof comando,
                "\"%s\" < \"%s\" > \"test/.saida_teste.tmp\" 2>&1",
                executavel, caminho) >= (int)sizeof comando) {
#endif
            fprintf(stderr, "Comando excede o limite.\n");
            divergencias++; total++;
            continue;
        }
        /* Impede classificar a saida de um teste anterior caso falhe
           a criacao do arquivo temporario. */
        remove("test/.saida_teste.tmp");
        retorno = system(comando);
        f = fopen("test/.saida_teste.tmp", "rb");
        if (f) {
            lidos = fread(saida, 1, sizeof saida - 1, f);
            saida[lidos] = '\0';
            fclose(f);
        } else {
            strcpy(saida, "Falha ao capturar a saida");
        }
        classificar(saida, retorno, observado);
        ++total;
        if (!strcmp(esperado, observado)) ++aprovados;
        else {
            ++divergencias;
            printf("[FALHOU] %s | esperado=%s observado=%s\n", linha, esperado, observado);
        }
        csv_texto(relatorio, linha); fputc(',', relatorio);
        csv_texto(relatorio, categoria); fputc(',', relatorio);
        csv_texto(relatorio, esperado); fputc(',', relatorio);
        csv_texto(relatorio, observado); fputc(',', relatorio);
        csv_texto(relatorio, !strcmp(esperado, observado) ? "OK" : "DIVERGENCIA"); fputc(',', relatorio);
        csv_texto(relatorio, saida); fputc('\n', relatorio);
    }
    remove("test/.saida_teste.tmp");
    fclose(manifesto);
    fclose(relatorio);
    printf("\nRESULTADO: %d/%d conformes; %d divergencias.\n", aprovados, total, divergencias);
    puts("Relatorio: test/resultado_execucao.csv");
    return divergencias ? 1 : 0;
}
