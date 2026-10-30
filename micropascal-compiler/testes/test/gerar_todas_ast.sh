#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
base='testes/test'
ast="$base/AST"
mkdir -p "$ast"
echo 'Compilando MicroPascal com suporte a AST...'
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
num=1
while :; do
  printf -v seq '%03d' "$num"
  out="$ast/resultado_ast_$seq"
  if mkdir "$out" 2>/dev/null; then break; fi
  if [[ -e "$out" ]]; then ((num+=1)); else echo "ERRO: nao foi possivel criar $out" >&2; exit 1; fi
done
for category in 01_validos 02_erros_sintaticos 03_erros_lexicos; do mkdir -p "$out/$category"; done
printf 'categoria,arquivo,status\n' > "$out/resumo.csv"
total=0; geradas=0; sem_ast=0; erros=0
shopt -s nullglob
for category in 01_validos 02_erros_sintaticos 03_erros_lexicos; do
  for file in "$base/$category"/*.pas; do
    ((total+=1))
    name="$(basename "${file%.pas}")"
    tree="$out/$category/$name.ast.txt"
    err="$out/$category/$name.erro.txt"
    if ./micropascal "$file" --ast >/dev/null 2>&1; then
      if ./micropascal "$file" --ast >"$tree" 2>&1; then
        if [[ -s "$tree" ]]; then
          ((geradas+=1)); status='AST_GERADA'
        else
          ((sem_ast+=1)); status='SEM_AST'
          echo "AVISO: $(basename "$file") aceito, mas nenhuma AST foi impressa."
        fi
      else
        ((erros+=1)); status='ERRO_NA_SEGUNDA_EXECUCAO'
        echo "AVISO: falha inesperada ao gerar AST de $(basename "$file")"
      fi
    else
      ./micropascal "$file" --ast >"$err" 2>&1 || true
      ((erros+=1)); status='ERRO_ANALISE'
    fi
    printf '%s,%s,%s\n' "$category" "$(basename "$file")" "$status" >> "$out/resumo.csv"
  done
done
printf '\nArquivos analisados: %d\nASTs impressas: %d\nAceitos sem AST: %d\nErros de analise: %d\nResultados: %s\n' "$total" "$geradas" "$sem_ast" "$erros" "$out"
