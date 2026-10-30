<h1 align="center">🔤 Projeto de Compiladores — MicroPascal</h1>

<p align="center">
  <img src="https://img.shields.io/static/v1?label=C&message=MicroPascal&color=00599C&style=for-the-badge&logo=c"/>
  <img src="https://img.shields.io/static/v1?label=GCC&message=Compilador&color=A42E2B&style=for-the-badge&logo=gnu"/>
  <img src="https://img.shields.io/static/v1?label=Git&message=Versionamento&color=F05032&style=for-the-badge&logo=git"/>
  <img src="https://img.shields.io/static/v1?label=GitHub&message=2026.2&color=181717&style=for-the-badge&logo=github"/>
  <img src="https://img.shields.io/static/v1?label=STATUS&message=EM%20DESENVOLVIMENTO&color=yellow&style=for-the-badge"/>
</p>

<p align="center">
  <strong>Parte 1 — Análise Léxica e Sintática | Parte 2 — Geração de AST</strong>
</p>

> **Status do Projeto:** 🚧 Em desenvolvimento — Lexer, Parser e AST implementados; análise semântica e código intermediário pendentes.

Projeto desenvolvido para a disciplina **Construção de Compiladores**, da Universidade Católica de Pernambuco (UNICAP), sob orientação do professor **Robson Lins**.

---

# 📌 Tópicos

:small_blue_diamond: [Sobre o Projeto](#️-sobre-o-projeto)  
:small_blue_diamond: [Objetivos da Parte 1](#-objetivos-da-parte-1)  
:small_blue_diamond: [Estrutura do Projeto](#️-estrutura-do-projeto)  
:small_blue_diamond: [Organização dos Arquivos](#-organização-dos-arquivos)  
:small_blue_diamond: [Fluxo do Projeto](#-fluxo-do-projeto)  
:small_blue_diamond: [Parte 1 — Análise Léxica e Sintática](#-parte-1--análise-léxica-e-sintática)  
:small_blue_diamond: [Analisador Léxico](#1--analisador-léxico-lexer)  
:small_blue_diamond: [Analisador Sintático](#2--analisador-sintático-parser)  
:small_blue_diamond: [Gramática de micro-Pascal](#3--gramática-de-micro-pascal)  
:small_blue_diamond: [Precedência e Associatividade](#4--precedência-e-associatividade)  
:small_blue_diamond: [Integração entre Lexer e Parser](#5--integração-entre-lexer-e-parser)  
:small_blue_diamond: [Tratamento de Erros](#6-️-tratamento-de-erros)  
:small_blue_diamond: [Exemplos de Código micro-Pascal](#7--exemplos-de-código-micro-pascal)  
:small_blue_diamond: [Compilação e Execução](#8-️-compilação-e-execução)  
:small_blue_diamond: [Compilação com Makefile](#84-compilação-com-makefile)  
:small_blue_diamond: [Testando o Analisador](#9--testando-o-analisador)  
:small_blue_diamond: [Suíte de Testes](#96-suíte-automatizada)  
:small_blue_diamond: [Geração em lote de ASTs](#97-geração-em-lote-de-asts)  
:small_blue_diamond: [Geração da Árvore Sintática Abstrata (AST)](#-parte-2--geração-da-árvore-sintática-abstrata-ast)  
:small_blue_diamond: [Resumo das Responsabilidades](#10--resumo-das-responsabilidades)  
:small_blue_diamond: [Escopo](#-escopo)  
:small_blue_diamond: [Referência](#-referência)  
:small_blue_diamond: [Desenvolvedores](#-desenvolvedores)

---

# ⁉️ Sobre o Projeto

Este repositório contém a implementação da **Parte 1 e da geração de AST da Parte 2 do Projeto de Compiladores**, cujo objetivo é aplicar conceitos fundamentais de construção de compiladores por meio do desenvolvimento das duas primeiras etapas de um compilador para uma versão simplificada da linguagem Pascal, denominada **micro-Pascal**.

Nesta primeira etapa, o projeto contempla:

- 🔆 **Analisador Léxico (Lexer)**
- 🌳 **Analisador Sintático (Parser)**

O **Analisador Léxico** recebe o código-fonte e transforma as sequências de caracteres em **tokens**.

O **Analisador Sintático** recebe esses tokens e verifica se eles aparecem em uma sequência permitida pela **gramática da linguagem micro-Pascal**.

Na **Parte 2**, o projeto já implementa a construção e impressão da **Árvore Sintática Abstrata (AST)**. A análise semântica e a geração de código intermediário permanecem como etapas futuras.

---

# 🎯 Objetivos da Parte 1

- Implementar um **Analisador Léxico** para micro-Pascal;
- reconhecer os tokens especificados no projeto;
- identificar e reportar erros léxicos;
- implementar um **Analisador Sintático Descendente Recursivo**;
- utilizar a gramática de micro-Pascal como base para o Parser;
- considerar corretamente a precedência e a associatividade dos operadores;
- identificar e reportar erros sintáticos;
- integrar o Lexer ao Parser;
- organizar o projeto separando as responsabilidades de cada módulo;
- desenvolver a solução utilizando a linguagem **C**.

---

# 🗂️ Estrutura do Projeto

A organização atual separa os cabeçalhos em `include/`, as implementações em `src/` e os testes em `testes/`. A pasta `04_limites` não faz parte da suíte atual.

```text
Projeto-de-compiladores/
└── micropascal-compiler/
    ├── include/
    │   ├── token.h
    │   ├── lexer.h
    │   ├── parser.h
    │   └── ast.h
    ├── src/
    │   ├── main.c
    │   ├── lexer.c
    │   ├── parser.c
    │   └── ast.c
    ├── testes/
    │   ├── e1.pas, e2.pas, e3.pas, e5.pas
    │   ├── exemplo1.pas, exemplo2.pas, teste_lexer.pas
    │   └── test/
    │       ├── 01_validos/              # 89 testes
    │       ├── 02_erros_sintaticos/    # 84 testes
    │       ├── 03_erros_lexicos/       # 30 testes
    │       ├── AST/
    │       │   └── resultado_ast_001/
    │       │       ├── 01_validos/
    │       │       ├── 02_erros_sintaticos/
    │       │       ├── 03_erros_lexicos/
    │       │       └── resumo.csv
    │       ├── executar_testes.c
    │       ├── manifesto.csv
    │       ├── README_test.md
    │       ├── rodar_testes_completos.bat
    │       ├── rodar_testes_completos.sh
    │       ├── gerar_todas_ast.bat
    │       └── gerar_todas_ast.sh
    ├── rodar_testes.bat
    ├── rodar_testes.sh
    ├── Makefile
    ├── .gitignore
    └── README.md
```

As pastas `resultado_ast_002/`, `resultado_ast_003/` etc. são criadas conforme novas execuções, preservando o histórico. Os arquivos de teste originais em `testes/` permanecem separados da suíte automatizada.

---

# 📚 Organização da Documentação

- **`docs/instrucoes_professor/`** — enunciados, requisitos e materiais fornecidos pelo professor.
- **`docs/documentacao_tecnica/`** — documentação técnica do funcionamento e da implementação do projeto.
- **`docs/images/`** — imagens e diagramas utilizados nos documentos.

---

# 📁 Organização dos Arquivos

## `include/token.h`

Centraliza a definição dos tipos de token e da estrutura `Token`. Os tokens representam identificadores, palavras reservadas, literais, operadores, pontuação e fim de arquivo. O Lexer produz tokens e o Parser os consome.

## `include/lexer.h` e `src/lexer.c`

O cabeçalho declara a interface do analisador léxico; o arquivo `.c` implementa a leitura do arquivo-fonte e a identificação dos lexemas. O Lexer reconhece identificadores, inteiros, reais, caracteres, palavras reservadas, símbolos e operadores; ignora espaços e comentários `//`; e informa erros léxicos.

Exemplo:

```pascal
soma := 10;
```

```text
soma  → identificador
:=    → atribuição
10    → literal inteiro
;     → ponto e vírgula
```

## `include/parser.h` e `src/parser.c`

O Parser descendente recursivo recebe os tokens do Lexer, verifica a gramática e constrói os nós da AST. Suas rotinas correspondem às construções do MicroPascal: programa, declarações, blocos, comandos e expressões. A interface em `parser.h` define os tipos e as funções disponibilizados aos outros módulos.

## `include/ast.h` e `src/ast.c`

Reúnem as definições e as operações da Árvore Sintática Abstrata: representação dos nós, construção, impressão e liberação dos recursos utilizados. A árvore preserva a estrutura do programa reconhecido, incluindo o agrupamento das expressões conforme a precedência dos operadores.

## `Makefile`

Automatiza a compilação do projeto com **GCC** e o padrão **C11**. Usa `$(wildcard src/*.c)` para localizar todos os arquivos `.c` da pasta `src/`, incluindo `ast.c`, sem precisar adicioná-los manualmente. Os arquivos objeto `.o` são gerados a partir dos fontes, e a ligação produz o executável `micropascal` (normalmente `micropascal.exe` no Windows). A opção `-Iinclude` permite encontrar os cabeçalhos.

Os alvos disponíveis são `all` (padrão) e `clean`; os comandos e as particularidades de Windows estão explicados na seção [8.4 — Compilação com Makefile](#84-compilação-com-makefile).

## `src/main.c`

É o ponto de entrada. Recebe o caminho do arquivo `.pas` pela linha de comando, abre o arquivo, inicializa a análise e informa o resultado por meio do código de saída. Com a opção `--ast`, solicita a impressão da árvore construída. **Não existe menu interativo nem leitura do código pela entrada padrão (`stdin`) na versão atual.**

## `testes/test/executar_testes.c`

Executa os casos registrados em `manifesto.csv`, compara o resultado observado com o esperado e gera relatórios CSV numerados, como `resultado_execucao_001.csv` e `resultado_execucao_002.csv`.

## Scripts de automação

- `rodar_testes_completos.bat` e `.sh`: compilam e executam a suíte de 203 testes.
- `gerar_todas_ast.bat` e `.sh`: geram as ASTs e os relatórios de erro em pastas numeradas.
- `rodar_testes.bat` e `.sh`: scripts destinados aos testes originais, separados da suíte completa.

## `.gitignore`, `.vscode/` e `README.md`

O `.gitignore` controla quais arquivos gerados não serão versionados; `.vscode/` pode armazenar configurações locais do editor; este README documenta a arquitetura, a linguagem, a compilação e a validação do projeto.

---

# 🔄 Fluxo do Projeto

```text
Arquivo .pas
    │
    ▼
  main.c ── abre o arquivo-fonte
    │
    ▼
  Lexer ─── caracteres → tokens
    │
    ▼
  Parser ── verifica a gramática
    │
    ├── erro léxico ou sintático → diagnóstico e falha
    │
    ▼
Construção da AST
    │
    ├── execução normal → término com código 0
    └── opção --ast → impressão da árvore
```

O compilador **não executa** o programa Pascal. Por exemplo, `write(x)` é reconhecido como comando, mas não imprime o valor da variável. A AST será a base para futuras etapas de análise semântica e geração de código intermediário.

---

# 🧩 Parte 1 — Análise Léxica e Sintática

# 1. 🔎 Analisador Léxico (Lexer)

O Analisador Léxico é responsável por percorrer o código-fonte e identificar os **tokens** reconhecidos pela linguagem micro-Pascal.

Para cada token reconhecido, o Lexer mantém:

```text
simbolo_lido
lexema
```

`simbolo_lido` representa o tipo do token.

`lexema` representa o texto encontrado na entrada.

Por exemplo:

```pascal
contador := 10;
```

Quando o Lexer reconhecer `contador`, teremos:

```text
lexema       = "contador"
simbolo_lido = IDENTIFICADOR
```

---

## 1.1 🔤 Definições Básicas

A especificação considera:

```text
letra ::= [a-zA-Z_]

digito ::= [0-9]
```

Essas definições não representam tokens isolados. Elas são utilizadas para descrever a formação de outros tokens.

---

## 1.2 🏷️ Identificadores

Um identificador é formado por:

```text
letra (letra | digito)*
```

Exemplos:

```text
nome
contador
_var
valor2
soma10
```

---

## 1.3 ⚖️ Operadores Relacionais

Os operadores relacionais reconhecidos são:

| Operador | Significado |
|---|---|
| `<` | menor que |
| `>` | maior que |
| `<=` | menor ou igual |
| `>=` | maior ou igual |
| `=` | igual |
| `<>` | diferente |

---

## 1.4 ➕ Operadores Lógico-Aritméticos

| Operador | Categoria |
|---|---|
| `+` | soma |
| `-` | subtração |
| `*` | multiplicação |
| `/` | divisão real |
| `div` | divisão inteira |
| `and` | operador lógico |
| `or` | operador lógico |
| `not` | operador lógico |

Em micro-Pascal:

```text
/
```

representa divisão entre valores reais, enquanto:

```text
div
```

representa divisão entre inteiros.

---

## 1.5 📝 Operador de Atribuição

O operador de atribuição é:

```text
:=
```

Exemplo:

```pascal
contador := 10;
```

O Lexer reconhece `:=` como um único token:

```text
ATRIBUICAO
```

---

## 1.6 🔣 Símbolos Especiais

Os seguintes símbolos são reconhecidos individualmente:

```text
(
)
,
;
.
:
```

Correspondendo aos tokens:

```text
ABRE_PAR
FECHA_PAR
VIRGULA
PONTO_VIRGULA
PONTO
DOIS_PONTOS
```

---

## 1.7 🔑 Palavras Reservadas

As palavras reservadas da linguagem são:

```text
program
if
then
else
while
do
repeat
until
integer
real
char
begin
end
write
var
```

Além delas, os seguintes operadores também são reconhecidos através de palavras:

```text
div
and
or
not
```

---

## 1.8 🔢 Literais Inteiros

Um valor inteiro literal é definido por:

```text
digito+
```

Exemplos:

```text
0
1
42
100
2026
```

O token correspondente é:

```text
INTEIRO_LITERAL
```

---

## 1.9 🔢 Literais Reais

Um valor real literal é definido por:

```text
digito* . digito+
```

Exemplos:

```text
3.14
10.5
0.25
.5
```

O token correspondente é:

```text
REAL_LITERAL
```

---

## 1.10 🔤 Literais do Tipo `char`

Um literal de caractere possui a estrutura:

```text
'(letra | digito | \n | \t)'
```

Exemplos:

```text
'a'
'c'
'0'
'9'
'_'
'\n'
'\t'
```

Onde:

- `\n` representa quebra de linha;
- `\t` representa tabulação.

O token correspondente é:

```text
CHAR_LITERAL
```

---

## 1.11 🔠 Sensibilidade a Maiúsculas e Minúsculas

A linguagem **micro-Pascal é case-sensitive**.

Isso significa que:

```text
begin
```

é reconhecido como palavra reservada.

Porém:

```text
Begin
```

não corresponde à palavra reservada `begin` e pode ser reconhecido como identificador.

---

## 1.12 ⬜ Caracteres Irrelevantes

O Lexer ignora os seguintes caracteres quando aparecem entre lexemas:

- espaço;
- `\n` — quebra de linha;
- `\t` — tabulação;
- `\r` — retorno de cursor.

Esses caracteres não geram tokens.

### Comentários de linha

O Lexer também ignora comentários iniciados por `//`, até a próxima quebra de linha ou o fim da entrada:

```pascal
x := 10; // comentário ignorado pelo Lexer
```

---

## 1.13 ⚠️ Erros Léxicos

Um erro léxico ocorre quando o Lexer encontra um caractere que não pode fazer parte de nenhum token válido.

Exemplo:

```text
x := 10 $ 2;
```

O caractere:

```text
$
```

não pertence aos tokens reconhecidos pela linguagem.

O erro é responsabilidade do:

```text
lexer.c
```

A mensagem segue o formato:

```text
Erro léxico no caracter [$]
```

---

# 2. 🌳 Analisador Sintático (Parser)

O Analisador Sintático verifica se a sequência de tokens produzida pelo Lexer está de acordo com a estrutura gramatical definida para micro-Pascal.

A implementação utiliza um **Parser Descendente Recursivo**.

Nesse modelo, as construções da gramática são representadas por funções.

Por exemplo:

```text
<programa>      → programa()
<secao_var>     → secao_var()
<decl_var>      → decl_var()
<tipo>          → tipo()
<bloco>         → bloco()
<comando>       → comando()
<atribuicao>    → atribuicao()
<iteracao>      → iteracao()
<decisao>       → decisao()
<escrita>       → escrita()
<expressao>     → expressao()
```

---

# 3. 📐 Gramática de micro-Pascal

## 3.1 Programa

```text
<programa> ::= program IDENTIFICADOR ; <secao_var> <bloco> .
```

---

## 3.2 Seção de Variáveis

```text
<secao_var> ::= var {<decl_var>}*
```

---

## 3.3 Declaração de Variáveis

```text
<decl_var> ::= IDENTIFICADOR {, IDENTIFICADOR}* : <tipo> ;
```

---

## 3.4 Tipos

```text
<tipo> ::= integer
         | real
         | char
```

---

## 3.5 Bloco

```text
<bloco> ::= begin <lista_comandos> end
```

---

## 3.6 Lista de Comandos

```text
<lista_comandos> ::= {<comando>}*
```

---

## 3.7 Comandos

```text
<comando> ::= <bloco> ;
            | <atribuicao>
            | <iteracao>
            | <decisao>
            | <escrita>
```

---

## 3.8 Atribuição

```text
<atribuicao> ::= IDENTIFICADOR ":=" <expressao> ;
```

Exemplo:

```pascal
x := 10;
```

---

## 3.9 Iteração

```text
<iteracao> ::= while <expressao> do <comando>
             | repeat <comando> until <expressao> ;
```

Exemplo com `while`:

```pascal
while i < n do
    i := i + 1;
```

---

## 3.10 Decisão

```text
<decisao> ::= if <expressao> then <comando>
            | if <expressao> then <comando> else <comando>
```

Exemplo:

```pascal
if x > 10 then
    write('a');
else
    write('b');
```

---

## 3.11 Escrita

```text
<escrita> ::= write ( <expressao> ) ;
```

Exemplo:

```pascal
write(x);
```

---

## 3.12 Expressões

A gramática geral das expressões considera:

```text
<expressao> ::= <expressao> + <expressao>
              | <expressao> * <expressao>
              | <expressao> / <expressao>
              | <expressao> div <expressao>
              | <expressao> and <expressao>
              | <expressao> or <expressao>
              | <expressao> = <expressao>
              | <expressao> <> <expressao>
              | <expressao> <= <expressao>
              | <expressao> < <expressao>
              | <expressao> >= <expressao>
              | <expressao> > <expressao>
              | <expr_basica>
```

---

## 3.13 Expressões Básicas

```text
<expr_basica> ::= ( <expressao> )
                | not <expressao>
                | INTEIRO_LITERAL
                | REAL_LITERAL
                | CHAR_LITERAL
                | IDENTIFICADOR
```

Exemplos:

```text
10
3.14
'a'
x
(x + 5)
not x
```

---

O Parser reconhece a subtração binária, como em `10 - 5`. O sinal negativo unário, como em `-5`, não faz parte das expressões aceitas nesta implementação.

---

# 4. 📊 Precedência e Associatividade

Todos os operadores são considerados **associativos à esquerda**.

A precedência utilizada é:

| Nível | Operadores | Precedência |
|---:|---|---|
| 1 | `*`, `/`, `div` | maior |
| 2 | `+`, `-` | |
| 3 | `=`, `<>`, `<`, `>`, `<=`, `>=` | |
| 4 | `or`, `and` | menor |

Para respeitar esses níveis, o Parser divide a análise das expressões em diferentes funções:

```text
expressao()
      ↓
expr_logica()
      ↓
expr_relacional()
      ↓
expr_aditiva()
      ↓
expr_multiplicativa()
      ↓
expr_basica()
```

Por exemplo:

```text
x + 2 * 5
```

A multiplicação possui maior precedência.

Portanto, a expressão corresponde a:

```text
x + (2 * 5)
```

e não:

```text
(x + 2) * 5
```

---

# 5. 🔗 Integração entre Lexer e Parser

A integração acontece por meio da interface declarada em `include/lexer.h` e da estrutura `Token` definida em `include/token.h`. O Lexer fornece o próximo token; o Parser verifica se ele é o esperado pela gramática e avança para o próximo quando necessário.

Para a entrada:

```pascal
contador := 10;
```

O Lexer identifica, em sequência, um identificador, o operador de atribuição, um literal inteiro e o ponto e vírgula. O Parser verifica se essa sequência corresponde a um comando de atribuição válido e cria a representação correspondente na AST.

A versão atual não depende das antigas variáveis globais `simbolo_lido`, `lexema` e `entrada`, nem da função global `obtenha_simbolo()` descritas na implementação anterior. A localização exata dos campos e das rotinas deve ser consultada nos cabeçalhos atuais.

---

# 6. ⚠️ Tratamento de Erros

O projeto diferencia dois tipos principais de erro:

```text
Erro Léxico
Erro Sintático
```

---

## 6.1 Erro Léxico

Ocorre quando um caractere não pode fazer parte de nenhum token reconhecido.

Exemplo:

```text
x := 10 $ 5;
```

O caractere `$` não é reconhecido.

Fluxo:

```text
Caractere inválido
        ↓
      Lexer
        ↓
   Erro léxico
```

---

## 6.2 Erro Sintático

Ocorre quando os tokens são válidos individualmente, mas aparecem em uma sequência que não corresponde à gramática.

Exemplo:

```text
x := ;
```

O Lexer consegue reconhecer:

```text
IDENTIFICADOR
ATRIBUICAO
PONTO_VIRGULA
```

Porém, após `:=`, a gramática exige uma expressão.

O Parser identifica o problema.

Fluxo:

```text
Tokens válidos
em ordem inválida
        ↓
      Parser
        ↓
  Erro sintático
```

---

# 7. 💻 Exemplos de Código micro-Pascal

## 7.1 Teste de Paridade

```pascal
program TestaParidade;
var
    n : integer;
begin
    n := 2 * 13 + 5;

    if n = (n div 2)*2 then
        write('p');
    else
        write('i');
end.
```

Esse exemplo utiliza:

- declaração de programa;
- declaração de variável;
- tipo `integer`;
- atribuição;
- multiplicação;
- soma;
- divisão inteira;
- expressão relacional;
- `if`;
- `else`;
- `write`;
- literal do tipo `char`.

---

## 7.2 Soma dos Primeiros Números Ímpares

```pascal
program SomaImpares;
var
    n : integer;
    i, proximoImpar, soma : integer;
begin
    n := 4;
    i := 0;
    soma := 0;

    while i < n do
    begin
        proximoImpar := 2*i + 1;
        soma := soma + proximoImpar;
        i := i + 1;
    end;

    write(soma);
    write('\n');
end.
```

Esse exemplo utiliza:

- múltiplas declarações;
- blocos `begin` e `end`;
- `while`;
- operador `<`;
- operações aritméticas;
- atribuições;
- `write`;
- literal `'\n'`.

---

# 8. 🛠️ Compilação e Execução

**Todos os comandos abaixo devem ser executados dentro de `micropascal-compiler/`.** É necessário ter o GCC instalado e disponível no terminal. O `Makefile` fornecido já utiliza `src/*.c` e inclui automaticamente o módulo da AST.

## 8.1 Windows — PowerShell

**Compilar todos os módulos, inclusive a AST:**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
```

**Analisar um arquivo `.pas`:**

```powershell
.\micropascal.exe .\testes\exemplo1.pas
$LASTEXITCODE
```

**Imprimir a AST:**

```powershell
.\micropascal.exe .\testes\exemplo1.pas --ast
```

## 8.2 Linux/macOS — Terminal

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
./micropascal testes/exemplo1.pas
./micropascal testes/exemplo1.pas --ast
echo $?
```

## 8.3 Como criar um teste manual

Crie e salve um arquivo `testes/teste_manual.pas` contendo um programa MicroPascal completo. Depois execute:

```powershell
.\micropascal.exe .\testes\teste_manual.pas
```

O argumento do programa deve ser o caminho de um arquivo existente. **Não use `Ctrl+Z`, `Ctrl+D`, redirecionamento `<` nem `Get-Content` para enviar o código ao analisador atual.** O código de saída `0` indica sucesso; um valor diferente de `0` indica erro. Sem `--ast`, a análise bem-sucedida normalmente não imprime mensagem.

## 8.4 Compilação com Makefile

O projeto possui um `Makefile` na raiz de `micropascal-compiler/`, que automatiza a compilação e evita repetir manualmente o comando do GCC. É necessário ter **GCC** e **Make** instalados e disponíveis no terminal. Execute os comandos a partir dessa pasta.

**Compilar o projeto:**

```bash
make
```

O alvo padrão `all` gera o executável `micropascal` (em ambientes Windows com GCC, normalmente `micropascal.exe`). O Makefile utiliza as seguintes configurações:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = micropascal
```

- `CC`: seleciona o compilador GCC.
- `CFLAGS`: ativa avisos, usa C11 e inclui os cabeçalhos da pasta `include/`.
- `SRC`: localiza automaticamente todos os arquivos `src/*.c`, inclusive `src/ast.c`.
- `OBJ`: define os arquivos objeto `.o` correspondentes.
- `TARGET`: define o nome-base do executável.

**Executar a análise e imprimir a AST no Linux/macOS:**

```bash
./micropascal testes/exemplo1.pas
./micropascal testes/exemplo1.pas --ast
```

**Executar no Windows (PowerShell):**

```powershell
.\micropascal.exe .\testes\exemplo1.pas
.\micropascal.exe .\testes\exemplo1.pas --ast
```

**Limpar os arquivos compilados:**

```bash
make clean
```

O alvo `clean` executa `rm -f` para remover os arquivos `.o` e o arquivo com o nome `micropascal`. **No Windows**, esse comando requer um ambiente com `rm` disponível (por exemplo, Git Bash/MSYS2) e pode precisar de ajuste para remover `micropascal.exe`, pois o alvo `clean` atual menciona somente `micropascal`. O Makefile não precisa ser modificado para reconhecer novos arquivos `.c` em `src/`.

---

# 9. 🧪 Testando o Analisador

## 9.1 ✅ Programa válido

```pascal
program Teste;
var
    x : integer;
begin
    x := 10;
    write(x);
end.
```

Salve em um arquivo `.pas` e execute o compilador passando o caminho. O programa deve terminar com código `0`. Para visualizar a árvore, acrescente `--ast`.

## 9.2 ❌ Erro sintático

```pascal
program Teste
var
    x : integer;
begin
    x := 10;
end.
```

Falta `;` após `Teste`. O Parser deve rejeitar o programa e apresentar um diagnóstico sintático.

## 9.3 ❌ Erro léxico

```pascal
program Teste;
var
begin
    write(10 $ 2);
end.
```

O caractere `$` não é reconhecido pela linguagem e deve provocar um erro léxico.

## 9.4 ❌ Conteúdo após `end.`

```pascal
program Teste;
var
begin
end.
write(10);
```

A entrada deve ser rejeitada, pois o ponto final encerra o programa MicroPascal.

## 9.5 Casos relevantes para regressão

| Caso | Comportamento esperado |
|---|---|
| `var` sem declarações | Aceito pela gramática utilizada |
| Ausência de `var` | Erro sintático |
| `repeat` com um único comando | Aceito |
| `repeat` com bloco `begin ... end;` | Aceito |
| `x := 10 - 5;` | Aceito (subtração binária) |
| `x := -5;` | Rejeitado (menos unário não especificado) |
| `write('_');` | Aceito |
| `write('\n');` e `write('\t');` | Aceitos |
| Comentário `//` | Ignorado pelo Lexer |
| Conteúdo após `end.` | Erro sintático |

## 9.6 Suíte automatizada

| Categoria | Quantidade | Resultado esperado |
|---|---:|---|
| `01_validos` | 89 | Aceito |
| `02_erros_sintaticos` | 84 | Erro sintático |
| `03_erros_lexicos` | 30 | Erro léxico |
| **Total** | **203** | |


**Windows:**

```powershell
.\testes\test\rodar_testes_completos.bat
```

**Linux/macOS:**

```bash
bash testes/test/rodar_testes_completos.sh
```

O executor compara os resultados com `testes/test/manifesto.csv` e cria relatórios numerados:

```text
testes/test/resultado_execucao_001.csv
testes/test/resultado_execucao_002.csv
...
```

O histórico não é sobrescrito. Consulte [`testes/test/README_test.md`](testes/test/README_test.md) para instruções específicas da suíte.

## 9.7 Geração em lote de ASTs

**Windows:**

```powershell
.\testes\test\gerar_todas_ast.bat
```

**Linux/macOS:**

```bash
bash testes/test/gerar_todas_ast.sh
```

Cada execução cria uma pasta `resultado_ast_NNN`, contendo as três categorias e um arquivo `resumo.csv`:

```text
testes/test/AST/
└── resultado_ast_001/
    ├── 01_validos/             # saídas .ast.txt
    ├── 02_erros_sintaticos/   # diagnósticos .erro.txt
    ├── 03_erros_lexicos/      # diagnósticos .erro.txt
    └── resumo.csv
```

**Resultado esperado da suíte atual:** 203 arquivos analisados, 89 ASTs impressas, nenhum válido sem AST e 114 erros de análise (84 sintáticos e 30 léxicos). Esses números representam a distribuição esperada após a remoção dos testes de limites; devem ser confirmados pela execução.

As pastas numeradas anteriores podem conter resultados da antiga suíte de 211 testes e permanecem como histórico.

---

# 🌳 Parte 2 — Geração da Árvore Sintática Abstrata (AST)

A segunda parte do trabalho prevê ampliar o compilador para além do reconhecimento léxico e sintático. A etapa já implementada é a **construção da AST durante a análise sintática**, com opção de impressão para inspeção e testes.

## 1. O que é uma AST?

Uma Árvore Sintática Abstrata representa a estrutura relevante de um programa sem precisar reproduzir todos os símbolos da gramática. Diferentemente da simples lista de tokens, ela mostra como declarações, comandos e expressões se relacionam.

Por exemplo, para:

```pascal
x := 10 + 2 * 3;
```

A estrutura conceitual é:

```text
Atribuição
├── Destino: x
└── Soma (+)
    ├── Inteiro: 10
    └── Multiplicação (*)
        ├── Inteiro: 2
        └── Inteiro: 3
```

A multiplicação aparece abaixo da soma porque possui maior precedência. **A representação é ilustrativa:** os nomes exatos dos nós e o formato da impressão dependem de `ast.c`.

## 2. Como a árvore é construída?

O Lexer reconhece os tokens, o Parser verifica as regras da linguagem e, ao reconhecer uma construção válida, cria os nós correspondentes. As estruturas declaradas em `include/ast.h` e as operações implementadas em `src/ast.c` dão suporte a essa representação.

## 3. Impressão da árvore

A opção `--ast` permite solicitar a representação textual da AST de um arquivo válido:

```powershell
.\micropascal.exe .\testes\exemplo1.pas --ast
```

Os scripts de geração em lote salvam as árvores de programas aceitos em `.ast.txt` e os diagnósticos de programas rejeitados em `.erro.txt`, acompanhados por `resumo.csv`.

## 4. O que a AST ainda não verifica?

A existência de uma AST **não comprova** que o programa é semanticamente correto. Verificar se uma variável foi declarada, se os tipos são compatíveis e se operações são válidas pertence à **análise semântica**. A geração de código intermediário também permanece como etapa futura. O compilador ainda não executa os programas MicroPascal.

---

# 10. 🧠 Resumo das Responsabilidades

| Arquivo | Responsabilidade |
|---|---|
| `include/token.h` | Definir tokens e sua representação |
| `include/lexer.h` | Declarar a interface do Lexer |
| `include/parser.h` | Declarar a interface do Parser |
| `include/ast.h` | Definir as estruturas e operações públicas da AST |
| `src/lexer.c` | Reconhecer lexemas e produzir tokens |
| `src/parser.c` | Validar a gramática e construir a AST |
| `src/ast.c` | Implementar as operações sobre a árvore |
| `src/main.c` | Receber o arquivo-fonte e coordenar a análise |
| `Makefile` | Automatizar a compilação dos módulos C e a limpeza dos artefatos |
| `testes/test/executar_testes.c` | Executar os testes e registrar os resultados |
| `testes/test/manifesto.csv` | Registrar as classificações esperadas |
| `testes/test/README_test.md` | Documentar a suíte automatizada |
| Scripts `.bat` e `.sh` | Automatizar compilação, testes e impressão de ASTs |
| `README.md` | Documentar o projeto |

A distinção principal entre os componentes é:

```text
LEXER:  "Qual token foi reconhecido?"
PARSER: "Essa sequência de tokens segue a gramática?"
AST:    "Como os elementos reconhecidos se organizam?"
```

A análise semântica e a geração de código intermediário são etapas posteriores, ainda não apresentadas como implementadas.

---

# 📚 Escopo

> **Parte 1 — implementada:** análise léxica e análise sintática descendente recursiva.

> **Parte 2 — implementação parcial(Próximas etapas):** construção da Árvore Sintática Abstrata (AST), análise semântica e geração de código intermediário.

O projeto é um analisador/compilador acadêmico em desenvolvimento e **não interpreta nem executa** os comandos Pascal.

---

# 📖 Referência

Documento-base fornecido para a atividade:

📄 **[MicroPascal — Projeto de Compiladores: Primeira Parte](docs/instrucoes_professor/MicroPascal-parte1.pdf)**

📄 **[MicroPascal — Projeto de Compiladores: Segunda Parte](docs/instrucoes_professor/MicroPascal-parte2.pdf)**

O documento apresenta as especificações da **Parte 1**, contemplando o Lexer e o Parser. A construção da AST, análise semântica e geração de código intermediário corresponde à etapa inicial já desenvolvida da **Parte 2**.

---

# 👨‍💻 Desenvolvedores

| [<img src="https://github.com/Amanda-Aziz.png" width="115"><br><sub>Amanda Aziz</sub>](https://github.com/Amanda-Aziz) | [<img src="https://github.com/ayrtongomes070-pixel.png" width="115"><br><sub>Ayrton Gomes</sub>](https://github.com/ayrtongomes070-pixel) | [<img src="https://github.com/eduardovcmalta.png" width="115"><br><sub>Eduardo Malta</sub>](https://github.com/eduardovcmalta) | [<img src="https://github.com/francisLauriano.png" width="115"><br><sub>Francis Lauriano</sub>](https://github.com/francisLauriano) | [<img src="https://github.com/GuiDev005.png" width="115"><br><sub>Guilherme Eduardo Araujo</sub>](https://github.com/GuiDev005) | [<img src="https://github.com/Sofiafs.png" width="115"><br><sub>Sofia Farias</sub>](https://github.com/Sofiafs) |
| :---: | :---: | :---: |:---: | :---: | :---: |