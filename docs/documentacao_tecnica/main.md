# main.c: Ponto de entrada do compilador micro-Pascal

Este documento explica o arquivo `main.c`, que **coordena** o lexer e o parser.

## 1. Qual é o papel do `main.c`?

O `main.c` não reconhece nada da linguagem. Ele é o **maestro**: prepara os dados, dá a partida e apresenta o resultado.

Ele faz quatro coisas:

1. **Define as variáveis globais** compartilhadas (declaradas como `extern` no `define.h`).
2. **Lê o código-fonte** do teclado (ou de um arquivo redirecionado) e o guarda em `entrada`.
3. **Inicia a análise**: pede o primeiro token ao lexer e chama o parser.
4. **Informa o resultado**: programa válido ou erro.

### Fluxo completo do projeto

```
stdin (teclado ou arquivo)
        |
        v
     main.c    --- lê tudo e guarda em "entrada"
        |
        |  obtenha_simbolo()        <- lexer.c: primeiro token
        v
  programa()                        <- parser.c: valida a estrutura
        |      ^
        |      |  consumir() -> obtenha_simbolo()   (lexer sob demanda)
        v      |
 simbolo_lido == FIM ?
     sim -> "Programa sintaticamente valido."
     não -> erro_sintatico()
```

Detalhe importante: o lexer **não roda antes** do parser gerando uma lista de tokens. Ele é chamado **sob demanda**, um token por vez, sempre que o parser chama `consumir()`. Por isso o `main.c` só pede o **primeiro** token; os demais são pedidos pelo parser.

---

## 2. Includes

```c
#include <stdio.h>    // printf, fgets, stdin
#include <stdlib.h>   // (disponível, não usado diretamente)
#include <string.h>   // strlen
#include "define.h"   // constantes, extern, protótipos
```

---

## 3. Variáveis globais

```c
char entrada[TAM];
char lexema[TAM_LEXEMA];
int posicao = 0;
int simbolo_lido;
```

No `define.h` elas aparecem como `extern` (só uma **promessa** de que existem). Aqui está a **definição real**, onde a memória é de fato reservada. Isso precisa acontecer **em um único arquivo `.c`**, senão o linker reclama de definição duplicada.

| Variável | Papel | Valor inicial |
|---|---|---|
| `entrada` | Todo o código-fonte, até `TAM` caracteres | zeros (variável global) |
| `lexema` | Texto do token atual | zeros |
| `posicao` | Próximo caractere a ser lido pelo lexer | `0` (explícito) |
| `simbolo_lido` | Código do token atual | `0` |

Em C, variáveis **globais** são inicializadas com zero automaticamente. O `= 0` em `posicao` é redundante, mas deixa a intenção clara.

---

## 4. A função `main()`

### 4.1 Variáveis locais

| Variável | Função |
|---|---|
| `linha[500]` | Buffer temporário para **uma linha** lida do teclado |
| `i` | Próxima posição livre em `entrada` |
| `j` | Posição percorrida dentro de `linha` durante a cópia |
| `tamanho_linha` | Tamanho "útil" da linha (sem `\n` e `\r` finais) |
| `fim` | Sinalizador: `0` = continua lendo, `1` = encontrou `end.` |

### 4.2 Mensagem inicial

```c
printf("Digite o programa MicroPascal:\n\n");
```

Orienta o usuário quando ele digita no terminal. (Ver seção 7: essa mensagem aparece também ao redirecionar um arquivo.)

### 4.3 Leitura do código-fonte

```c
while(fim == 0 && fgets(linha, sizeof(linha), stdin) != NULL){
    ...
}
```

O laço lê **uma linha por vez** com `fgets`. Ele continua enquanto:

- `fim == 0`: ainda não apareceu `end.`; **e**
- `fgets(...) != NULL`: ainda há entrada (`fgets` devolve `NULL` no fim do arquivo, ou em Ctrl+D / Ctrl+Z no terminal).

Dentro de cada volta há três etapas.

**Etapa 1: copiar a linha para `entrada`**

```c
j = 0;
while(linha[j] != '\0' && i < TAM - 1){
    entrada[i] = linha[j];
    i += 1;
    j += 1;
}
```

Copia caractere a caractere, **incluindo o `\n`** (o lexer precisa dele só como espaço em branco). A condição `i < TAM - 1` protege o vetor: se o programa for maior que `TAM`, o excedente é **descartado em silêncio**, e sempre sobra uma posição para o `'\0'`.

**Etapa 2: medir a linha sem `\n` e `\r`**

```c
tamanho_linha = strlen(linha);
while(tamanho_linha > 0 && (linha[tamanho_linha - 1] == '\n' ||
                            linha[tamanho_linha - 1] == '\r')){
    tamanho_linha -= 1;
}
```

`fgets` mantém o `\n` do Enter, e no Windows pode haver também `\r`. Aqui eles são "descontados" **apenas para esta verificação**; na cópia da etapa 1 eles já foram guardados normalmente.

**Etapa 3: detectar o fim do programa (`end.`)**

```c
if(tamanho_linha >= 4){
    if(linha[tamanho_linha - 4] == 'e' && linha[tamanho_linha - 3] == 'n' &&
       linha[tamanho_linha - 2] == 'd' && linha[tamanho_linha - 1] == '.'){
        fim = 1;
    }
}
```

Se os **4 últimos caracteres** da linha forem `e`, `n`, `d`, `.`, o programa terminou e `fim` vira `1`, encerrando o laço.

**Por que existe essa regra?** Para quem digita no terminal: sem ela, seria preciso apertar Ctrl+D (Linux) ou Ctrl+Z (Windows) para sinalizar o fim da entrada. Com a regra, basta digitar `end.` e dar Enter.

### 4.4 Fechar a string

```c
entrada[i] = '\0';
```

Em C, uma string precisa terminar em `'\0'`. O lexer usa justamente esse caractere para saber que o código acabou (devolvendo `FIM`).

### 4.5 Iniciar a análise

```c
obtenha_simbolo();
programa();
```

1. `obtenha_simbolo()` lê o **primeiro token** e o coloca em `simbolo_lido`. (Se houver erro léxico logo no início, ele aparece aqui.)
2. `programa()` é o **símbolo inicial da gramática**. A partir dela, o parser percorre todo o programa. Se algo estiver errado, `erro_sintatico()` ou `erro_lexico()` imprime a mensagem e chama `exit(1)`, e a execução **nunca volta** para o `main`.

### 4.6 Verificação final

```c
if(simbolo_lido == FIM){
    printf("\nPrograma sintaticamente valido.\n");
}else{
    erro_sintatico();
}
return 0;
```

Se `programa()` retornou, o programa seguiu a gramática até o `.` final. Falta uma checagem: **não pode sobrar nada depois dele**.

| Situação após o `.` final | `simbolo_lido` | Resultado |
|---|---|---|
| Nada | `FIM` | `Programa sintaticamente valido.` |
| Mais tokens (ex.: `end. x`) | outro token | `Erro de sintaxe no token [x]` |

O `return 0` indica ao sistema operacional que terminou com sucesso. Em caso de erro, o código de saída é `1` (vem do `exit(1)` das funções de erro).

---

## 5. Como compilar e executar

**Compilar** (os três `.c` juntos; `define.h` é incluído automaticamente):

```
gcc main.c lexer.c parser.c -o micropascal
```

**Executar de duas formas:**

```
./micropascal                    # digitando no terminal, termina em "end."
./micropascal < teste1.pas       # lendo de um arquivo
```

No Windows: `micropascal.exe` e `micropascal.exe < teste1.pas`.

### Saídas possíveis

| Resultado | Saída | Código de saída |
|---|---|---|
| Programa válido | `Programa sintaticamente valido.` | 0 |
| Caractere inválido | `Erro lexico no caracter [@]` | 1 |
| Erro de sintaxe | `Erro de sintaxe no token [lexema]` | 1 |
| Entrada vazia | `Erro de sintaxe no token [FIM]` | 1 |

---

## 6. Exemplo de execução

Entrada:

```pascal
program P;
var n : integer;
begin
  n := 5;
end.
```

O que o `main` faz:

1. Imprime `Digite o programa MicroPascal:`.
2. Lê as 5 linhas, copiando-as para `entrada`.
3. Na linha `end.`, detecta o fim (`fim = 1`) e sai do laço.
4. Coloca `'\0'` ao final.
5. Chama `obtenha_simbolo()` (primeiro token: `program`) e `programa()`.
6. O parser percorre tudo; no final, `simbolo_lido == FIM`.
7. Imprime `Programa sintaticamente valido.`

---
