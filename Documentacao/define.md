# define.h: Cabeçalho central do compilador micro-Pascal

Este documento explica o arquivo `define.h`, o cabeçalho compartilhado pelas duas primeiras etapas do compilador micro-Pascal (analisador léxico e analisador sintático).

## 1. Qual é o papel do `define.h`?

O `define.h` é o **"contrato" entre os arquivos do projeto**. Ele não contém lógica, apenas declarações. Todos os outros arquivos (`main.c`, `lexer.c`, `parser.c`) fazem `#include "define.h"` para compartilhar as mesmas definições.

Ele reúne quatro tipos de informação:

| Seção | O que contém | Quem usa |
|---|---|---|
| Constantes de tamanho | `TAM`, `TAM_LEXEMA` | `main.c`, `lexer.c` |
| Códigos dos tokens | `IDENTIFICADOR`, `MAIS`, `PROGRAM`... | `lexer.c` (produz) e `parser.c` (consome) |
| Variáveis globais (`extern`) | `entrada`, `lexema`, `posicao`, `simbolo_lido` | Todos |
| Protótipos de funções | Funções do lexer e do parser | Todos |

### Como os arquivos se relacionam

```
            define.h  (constantes, externs, protótipos)
           /    |    \
      main.c  lexer.c  parser.c
```

Fluxo de execução:

```
código-fonte -> main.c (lê e guarda em "entrada")
             -> lexer.c (transforma caracteres em tokens)
             -> parser.c (verifica se os tokens seguem a gramática)
             -> "Programa sintaticamente valido." ou mensagem de erro
```

---

## 2. Constantes de tamanho

```c
#define TAM 10000
#define TAM_LEXEMA 100
```

- **`TAM`**: tamanho máximo, em caracteres, do código-fonte armazenado no vetor `entrada`. Programas maiores que isso são cortados.
- **`TAM_LEXEMA`**: tamanho máximo de um único lexema (por exemplo, o nome de uma variável). O lexer protege contra estouro: se um lexema passar de 99 caracteres, os excedentes são descartados, e a posição `[99]` fica reservada para o `'\0'`.

---

## 3. Códigos dos tokens

### Ideia central

Um **token** é a categoria de um pedaço do texto. O lexer lê caracteres e decide: "isto é um identificador", "isto é um `+`", "isto é a palavra `while`". Para o parser poder comparar categorias rapidamente, cada uma é representada por um **número inteiro único**, criado com `#define`.

Exemplo: o lexer lê `soma` e coloca `simbolo_lido = IDENTIFICADOR` (valor `1`). O parser pergunta `simbolo_lido == IDENTIFICADOR?`.

Os números vão de `1` a `41`, sem repetição, agrupados por categoria na ordem em que aparecem no enunciado.

### 3.1 Identificadores e literais (1 a 4)

| Constante | Valor | Significado | Exemplo |
|---|---|---|---|
| `IDENTIFICADOR` | 1 | Nome de variável ou do programa | `soma`, `n`, `TestaParidade` |
| `INTEIRO_LITERAL` | 2 | Número inteiro | `10`, `25` |
| `REAL_LITERAL` | 3 | Número real | `3.5`, `.5` |
| `CHAR_LITERAL` | 4 | Caractere entre aspas simples | `'a'`, `'5'`, `'\n'` |

Esses quatro tokens têm um **lexema variável**: o tipo diz a categoria, mas o texto (`soma`, `10`...) é guardado na variável `lexema`.

### 3.2 Operadores relacionais (5 a 10)

| Constante | Valor | Símbolo |
|---|---|---|
| `MENOR` | 5 | `<` |
| `MAIOR` | 6 | `>` |
| `MENOR_IGUAL` | 7 | `<=` |
| `MAIOR_IGUAL` | 8 | `>=` |
| `IGUAL` | 9 | `=` |
| `DIFERENTE` | 10 | `<>` |

Cada um é um token separado, como o enunciado recomenda. Em micro-Pascal o teste de igualdade usa **um único `=`**, e o "diferente" é `<>`.

### 3.3 Operadores aritméticos (11 a 14)

| Constante | Valor | Símbolo |
|---|---|---|
| `MAIS` | 11 | `+` |
| `MENOS` | 12 | `-` |
| `MULT` | 13 | `*` |
| `DIV_REAL` | 14 | `/` (divisão de reais) |

### 3.4 Operadores que são palavras reservadas (15 a 18)

| Constante | Valor | Palavra |
|---|---|---|
| `DIV` | 15 | `div` (divisão inteira) |
| `AND` | 16 | `and` |
| `OR` | 17 | `or` |
| `NOT` | 18 | `not` |

São operadores, mas **escritos como palavras**. Por isso o lexer os reconhece na mesma função que as palavras reservadas (`palavra_reservada`).

### 3.5 Atribuição (19)

| Constante | Valor | Símbolo |
|---|---|---|
| `ATRIBUICAO` | 19 | `:=` |

É um token de **dois caracteres**. O lexer precisa olhar o caractere seguinte ao `:` para diferenciá-lo do `:` isolado (`DOIS_PONTOS`).

### 3.6 Símbolos especiais (20 a 25)

| Constante | Valor | Símbolo | Uso típico |
|---|---|---|---|
| `ABRE_PAR` | 20 | `(` | expressões, `write(...)` |
| `FECHA_PAR` | 21 | `)` | idem |
| `VIRGULA` | 22 | `,` | `i, j : integer;` |
| `PONTO_VIRGULA` | 23 | `;` | terminar comandos |
| `PONTO` | 24 | `.` | após o `end` final |
| `DOIS_PONTOS` | 25 | `:` | `n : integer;` |

### 3.7 Palavras reservadas (26 a 40)

| Constante | Valor | Palavra |
|---|---|---|
| `PROGRAM` | 26 | `program` |
| `IF` | 27 | `if` |
| `THEN` | 28 | `then` |
| `ELSE` | 29 | `else` |
| `WHILE` | 30 | `while` |
| `DO` | 31 | `do` |
| `REPEAT` | 32 | `repeat` |
| `UNTIL` | 33 | `until` |
| `INTEGER` | 34 | `integer` |
| `REAL` | 35 | `real` |
| `CHAR` | 36 | `char` |
| `BEGIN_TOKEN` | 37 | `begin` |
| `END_TOKEN` | 38 | `end` |
| `WRITE` | 39 | `write` |
| `VAR` | 40 | `var` |

Detalhes importantes:

- **Sensibilidade a maiúsculas**: só `begin` (minúsculo) é a palavra reservada. `Begin` é um identificador comum.
- **`BEGIN_TOKEN` e `END_TOKEN`**: o sufixo `_TOKEN` deixa claro que são códigos de token e evita confusão com palavras como `BEGIN` em outros contextos.
- **`INTEGER`, `REAL`, `CHAR` são tokens de palavras reservadas** (os nomes dos tipos), diferentes de `INTEIRO_LITERAL`, `REAL_LITERAL` e `CHAR_LITERAL`, que são valores. `integer` é o tipo; `10` é um valor inteiro.

### 3.8 Fim da entrada (41)

```c
#define FIM 41
```

Quando o lexer encontra o caractere `'\0'` (fim da string `entrada`), ele devolve `FIM`. O `main.c` usa isso no final para verificar se não sobrou nenhum token depois do `.` final do programa.

---

## 4. Variáveis globais compartilhadas

```c
extern char entrada[TAM];
extern char lexema[TAM_LEXEMA];
extern int posicao;
extern int simbolo_lido;
```

### O que significa `extern`?

`extern` diz ao compilador: *"essa variável existe, mas foi criada em outro arquivo"*. Ela é **declarada** no `define.h` (para todos enxergarem) e **definida** de fato uma única vez, em `main.c`:

```c
char entrada[TAM];
char lexema[TAM_LEXEMA];
int posicao = 0;
int simbolo_lido;
```

Se a variável fosse criada dentro do `define.h` sem `extern`, cada arquivo `.c` teria sua própria cópia e o linker acusaria definição duplicada.

### Papel de cada variável

| Variável | Tipo | Função |
|---|---|---|
| `entrada` | `char[TAM]` | Guarda **todo o código-fonte**, preenchido pelo `main.c` |
| `lexema` | `char[TAM_LEXEMA]` | Texto do token atual (ex.: `"soma"`). Usado pelo parser nas mensagens de erro |
| `posicao` | `int` | Índice do próximo caractere a ser lido em `entrada`. O lexer avança esse valor |
| `simbolo_lido` | `int` | **Código do token atual** (uma das constantes da seção 3) |

### Exemplo de funcionamento

Entrada: `soma := 10;`

| Chamada de `obtenha_simbolo()` | `simbolo_lido` | `lexema` |
|---|---|---|
| 1ª | `IDENTIFICADOR` | `"soma"` |
| 2ª | `ATRIBUICAO` | `":="` |
| 3ª | `INTEIRO_LITERAL` | `"10"` |
| 4ª | `PONTO_VIRGULA` | `";"` |
| 5ª | `FIM` | `"FIM"` |

O parser trabalha com **um único token por vez**, o "token atual" em `simbolo_lido`.

---

## 5. Protótipos das funções do lexer

```c
void obtenha_simbolo(void);
int palavra_reservada(char palavra[]);
void erro_lexico(char caractere);
```

Um **protótipo** é a declaração de uma função (nome, parâmetros e retorno) sem o corpo. Ele permite que um arquivo chame uma função implementada em outro.

| Função | Implementada em | O que faz |
|---|---|---|
| `obtenha_simbolo()` | `lexer.c` | Lê o próximo token. Pula espaços, reconhece identificadores, números, caracteres, operadores e símbolos. Atualiza `simbolo_lido`, `lexema` e `posicao` |
| `palavra_reservada(palavra)` | `lexer.c` | Recebe uma palavra e devolve o token da palavra reservada correspondente, ou `IDENTIFICADOR` se não for reservada |
| `erro_lexico(caractere)` | `lexer.c` | Imprime a mensagem de erro léxico e encerra o programa |

---

## 6. Protótipos das funções do parser

### 6.1 Funções de apoio

```c
void consumir(int token);
void erro_sintatico(void);
```

- **`consumir(token)`**: verifica se `simbolo_lido` é o token esperado. Se for, chama `obtenha_simbolo()` para avançar. Se não, chama `erro_sintatico()`. É a ferramenta básica do parser: *"neste ponto eu espero encontrar X"*.
- **`erro_sintatico()`**: imprime `Erro de sintaxe no token [lexema]` e encerra.

### 6.2 Uma função por símbolo da gramática

O parser é do tipo **descida recursiva**: cada símbolo não terminal da gramática vira uma função.

| Função | Regra da gramática que reconhece |
|---|---|
| `programa()` | `program ID ; <secao_var> <bloco> .` |
| `secao_var()` | `var {<decl_var>}*` |
| `decl_var()` | `ID {, ID}* : <tipo> ;` |
| `tipo()` | `integer \| real \| char` |
| `bloco()` | `begin <lista_comandos> end` |
| `lista_comandos()` | `{<comando>}*` |
| `comando()` | Escolhe entre bloco, atribuição, iteração, decisão ou escrita |
| `atribuicao()` | `ID := <expressao> ;` |
| `iteracao()` | `while ... do ...` ou `repeat ... until ... ;` |
| `decisao()` | `if ... then ... [else ...]` |
| `escrita()` | `write ( <expressao> ) ;` |

### 6.3 Funções das expressões

A gramática abstrata de expressões é ambígua (não diz qual operador tem prioridade). Para resolver isso, a expressão foi **dividida em níveis**, um por função, do menor para o maior grau de precedência:

```c
void expressao(void);
void expr_logica(void);
void expr_relacional(void);
void expr_aditiva(void);
void expr_multiplicativa(void);
void expr_basica(void);
```

| Nível | Função | Operadores | Precedência |
|---|---|---|---|
| 1 (menor) | `expr_logica` | `or`, `and` | Mais baixa |
| 2 | `expr_relacional` | `=`, `<>`, `<`, `>`, `<=`, `>=` | |
| 3 | `expr_aditiva` | `+`, `-` | |
| 4 | `expr_multiplicativa` | `*`, `/`, `div` | |
| 5 (maior) | `expr_basica` | `( )`, `not`, literais, identificadores | Mais alta |

`expressao()` é só a porta de entrada e chama `expr_logica()`. Cada nível chama o nível acima dele (de maior precedência) para obter seus operandos. Por isso `2 + 3 * 4` é analisado como `2 + (3 * 4)`: a multiplicação é resolvida num nível mais profundo e, portanto, "primeiro".

---

## 7. Observações e boas práticas

1. **Include guard**: o arquivo não tem proteção contra inclusão dupla. Funciona hoje porque cada `.c` inclui o cabeçalho uma vez, mas é recomendável adicionar no topo e no fim:

   ```c
   #ifndef DEFINE_H
   #define DEFINE_H
   /* ... conteúdo ... */
   #endif
   ```

2. **`#define` vs `enum`**: os tokens poderiam ser um `enum`, que numera automaticamente e evita erros de valores repetidos. `#define` funciona, mas exige cuidado ao inserir um novo token no meio da lista.

3. **Nomes genéricos**: constantes como `IF`, `DO`, `REAL` e `CHAR` podem colidir com macros de bibliotecas do sistema (por exemplo, `<windows.h>`). Se houver conflitos, prefixe-as (`TK_IF`, `TK_REAL`).

4. **Ao adicionar um token novo**, são três lugares a alterar:
   - `define.h`: criar a constante com um número novo;
   - `lexer.c`: fazer o lexer reconhecê-lo;
   - `parser.c`: usá-lo na regra da gramática correspondente.

5. **Compilação**: o `define.h` não é compilado separadamente; ele é incluído pelos `.c`:

   ```
   gcc main.c lexer.c parser.c -o micropascal
   ```
