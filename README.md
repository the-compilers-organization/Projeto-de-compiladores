# 🔤 Projeto de Compiladores — MicroPascal

> **Parte 1 — Analisador Léxico e Analisador Sintático**
> Status: 🚧 Em Andamento

Projeto desenvolvido para a disciplina **Construção de Compiladores**, da Universidade Católica de Pernambuco (UNICAP), sob orientação do professor **Robson Lins**.

---

## ⁉️ Sobre o projeto

Este repositório contém a implementação da **Parte 1 do Projeto de Compiladores**, cujo objetivo é aplicar técnicas fundamentais de construção de compiladores por meio da implementação das duas primeiras etapas de um compilador para uma versão simplificada da linguagem Pascal, denominada **micro-Pascal**.

Nesta primeira etapa, o projeto contempla:

- 🔆 **Analisador Léxico (Lexer)**
- 🔆 **Analisador Sintático (Parser)**

As demais etapas do compilador serão desenvolvidas na **Parte 2** do projeto.

### 👥 Organização

O projeto pode ser desenvolvido individualmente ou em grupo de até **5 estudantes**, utilizando **C ou C++**.

---

# 🧩 Parte 1 — Análise Léxica e Sintática

## 1. 🔎 Analisador Léxico (Lexer)

O analisador léxico é responsável por percorrer o código-fonte e identificar os **tokens** reconhecidos pela linguagem micro-Pascal.

Para determinados tokens, além do tipo, o lexer deve retornar informações complementares, como o **lexema**, para auxiliar as etapas seguintes do compilador.

### 🔤 Definições básicas

A especificação considera:

```text
letra ::= [a-zA-Z_]
digito ::= [0-9]
```

Essas definições **não são tokens**; elas são utilizadas para descrever outros tokens por meio de expressões regulares.

---

## 1.1 🏷️ Identificadores

Um identificador é formado por uma letra seguida de zero ou mais letras ou dígitos:

```text
letra (letra | digito)*
```

Exemplos válidos:

```text
nome
contador
_var
valor2
```

---

## 1.2 ⚖️ Operadores relacionais

Os operadores relacionais devem ser reconhecidos como tokens distintos:

| Operador | Significado |
|---|---|
| `<` | menor que |
| `>` | maior que |
| `<=` | menor ou igual |
| `>=` | maior ou igual |
| `=` | igual |
| `<>` | diferente |

---

## 1.3 ➕ Operadores lógico-aritméticos

Os operadores abaixo também devem ser tratados como tokens distintos:

| Operador | Categoria |
|---|---|
| `+` | soma |
| `-` | subtração |
| `*` | multiplicação |
| `/` | divisão de reais |
| `div` | divisão de inteiros |
| `and` | lógico |
| `or` | lógico |
| `not` | lógico |

> 💡 Em micro-Pascal, `div` representa a divisão entre inteiros, enquanto `/` é utilizado para divisão entre valores reais.

---

## 1.4 📝 Operador de atribuição

O operador de atribuição é:

```text
:=
```

Exemplo:

```text
contador := 10;
```

---

## 1.5 🔣 Símbolos especiais

Os seguintes símbolos devem ser reconhecidos como **tokens diferentes**:

```text
)   (   ,   ;   .   :
```

---

## 1.6 🔑 Palavras reservadas

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

Além delas, os operadores abaixo também são considerados palavras reservadas:

```text
div
and
or
not
```

---

## 1.7 🔢 Literais inteiros

Um valor inteiro literal é definido por:

```text
digito+
```

Exemplos:

```text
0
1
42
2026
```

---

## 1.8 🔢 Literais reais

Um valor real literal é definido por:

```text
digito* . digito+
```

Exemplos:

```text
3.14
10.5
0.25
```

---

## 1.9 🔤 Literais do tipo `char`

Um literal de caractere possui a seguinte estrutura:

```text
'(letra | digito |\n|\t)'
```

As **aspas simples** delimitam o literal e fazem parte da representação do token.

Exemplos válidos:

```text
'a'
'c'
'0'
'9'
'\n'
'\t'
```

Onde:

- `\n` representa uma **quebra de linha**;
- `\t` representa uma **tabulação**, seguindo a convenção utilizada em C.

---

## 1.10 🔠 Sensibilidade a maiúsculas e minúsculas

A linguagem **micro-Pascal é case-sensitive**.

Isso significa que maiúsculas e minúsculas são diferenciadas.

Por exemplo:

```text
begin
```

é uma palavra reservada, enquanto:

```text
Begin
```

é interpretado como um **identificador**.

As palavras reservadas da linguagem são definidas estritamente em letras minúsculas.

---

## 1.11 ⬜ Caracteres irrelevantes

O lexer deve ignorar os seguintes caracteres quando aparecerem entre lexemas:

- espaço;
- `\n` — quebra de linha;
- `\t` — tabulação;
- `\r` — retorno de cursor, utilizado antes da quebra de linha no Windows.

Esses caracteres não devem gerar tokens.

---

## 1.12 ⚠️ Erros léxicos

Caso o analisador encontre um caractere que não corresponda ao início de nenhum token válido, deverá emitir uma mensagem de erro.

Formato especificado:

```text
Erro léxico no caracter [x]
```

Onde `x` representa o caractere que causou o erro.

---

# 2. 🌳 Analisador Sintático (Parser)

O analisador sintático é responsável por verificar se a sequência de tokens produzida pelo lexer está de acordo com a estrutura gramatical definida para a linguagem.

A especificação fornece uma **gramática abstrata em BNF**, utilizando também o operador `*` para representar zero ou mais ocorrências.

A gramática apresentada no enunciado é abstrata e **pode precisar de adaptações** antes de ser utilizada diretamente na implementação do parser.

---

## 2.1 📐 Gramática de micro-Pascal

### Programa

```text
<programa> ::= program IDENTIFICADOR ; <secao_var> <bloco> .
```

### Seção de variáveis

```text
<secao_var> ::= var {<decl_var>}*
```

### Declaração de variáveis

```text
<decl_var> ::= IDENTIFICADOR {, IDENTIFICADOR}* : <tipo> ;
```

### Tipos

```text
<tipo> ::= integer | real | char
```

### Bloco

```text
<bloco> ::= begin <lista_comandos> end
```

### Lista de comandos

```text
<lista_comandos> ::= {<comando>}*
```

### Comandos

```text
<comando> ::= <bloco> ;
            | <atribuicao>
            | <iteracao>
            | <decisao>
            | <escrita>
```

### Atribuição

```text
<atribuicao> ::= IDENTIFICADOR ":=" <expressao> ;
```

### Iteração

```text
<iteracao> ::= while <expressao> do <comando>
             | repeat <comando> until <expressao> ;
```

### Decisão

```text
<decisao> ::= if <expressao> then <comando>
            | if <expressao> then <comando> else <comando>
```

### Escrita

```text
<escrita> ::= write ( <expressao> ) ;
```

### Expressões

```text
<expressao> ::= <expressao> + <expressao>
              | <expressao> * <expressao>
              | <expressao> / <expressao>
              | <expressao> div <expressao>
              | <expressao> and <expressao>
              | <expressao> or <expressao>
              | <expressao> "=" <expressao>
              | <expressao> "<>" <expressao>
              | <expressao> "<=" <expressao>
              | <expressao> "<" <expressao>
              | <expressao> ">=" <expressao>
              | <expressao> ">" <expressao>
              | <expr_basica>
```

### Expressões básicas

```text
<expr_basica> ::= ( <expressao> )
                | not <expressao>
                | INTEIRO_LITERAL
                | REAL_LITERAL
                | CHAR_LITERAL
                | IDENTIFICADOR
```

---

# 3. 📊 Precedência e associatividade

Todos os operadores devem ser considerados **associativos à esquerda**.

A especificação estabelece os seguintes níveis de precedência:

| Nível | Operadores |
|---:|---|
| **1 — maior precedência** | `*`, `/`, `div` |
| **2** | `+`, `-` |
| **3** | `=`, `<>`, `<`, `>`, `<=`, `>=` |
| **4 — menor precedência** | `or`, `and` |

Essa definição deve ser considerada durante a construção do analisador sintático para garantir a interpretação correta das expressões.

---

# 4. ⚠️ Erros sintáticos

Sempre que necessário, o analisador sintático deverá emitir uma mensagem de erro.

Formato especificado:

```text
Erro de sintaxe no token [lexema]
```

Onde `lexema` corresponde ao token identificado no ponto em que a análise sintática detectou o problema.

---

# 5. 💻 Exemplos de código micro-Pascal

O enunciado apresenta exemplos de programas válidos para auxiliar na compreensão da sintaxe da linguagem.

## Exemplo 1 — Teste de paridade

O primeiro exemplo verifica se um número é **par ou ímpar**.

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

### Conceitos demonstrados

- declaração de programa;
- declaração de variável;
- tipo `integer`;
- atribuição com `:=`;
- operações aritméticas;
- divisão inteira com `div`;
- expressão condicional;
- `if` / `else`;
- comando `write`;
- literal `char`;
- encerramento do programa com `.`.

---

## Exemplo 2 — Soma dos `n` primeiros números ímpares

O segundo exemplo utiliza um laço `while` e múltiplas variáveis declaradas na mesma linha.

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

### Conceitos demonstrados

- declaração de múltiplas variáveis;
- blocos com `begin` e `end`;
- laço `while`;
- comparação com `<`;
- operações aritméticas;
- atribuições;
- `write`;
- literal `'\n'`;
- uso de `;` após o encerramento de um bloco interno;
- uso de `.` após o bloco principal do programa.

---

# 🧠 Conceitos principais da Parte 1

Esta etapa do projeto concentra-se nos fundamentos de **análise léxica e análise sintática**.

```text
Código-fonte
     │
     ▼
┌───────────────┐
│     Lexer     │
│ Análise       │
│   Léxica      │
└───────┬───────┘
        │
        ▼
      Tokens
        │
        ▼
┌───────────────┐
│    Parser     │
│   Análise     │
│   Sintática   │
└───────┬───────┘
        │
        ▼
 Programa reconhecido
 ou erro de sintaxe
```

O **Lexer** transforma a sequência de caracteres do código-fonte em tokens. Em seguida, o **Parser** utiliza esses tokens para verificar se a estrutura do programa está de acordo com a gramática da linguagem.

---

# 🎯 Objetivos da Parte 1

- Implementar um **analisador léxico** para micro-Pascal.
- Reconhecer os tokens especificados no projeto.
- Identificar e reportar erros léxicos.
- Implementar um **analisador sintático**.
- Utilizar a gramática de micro-Pascal como base para o parser.
- Considerar corretamente a precedência e a associatividade dos operadores.
- Identificar e reportar erros sintáticos.
- Desenvolver a solução utilizando **C/C++**.

---

# 📚 Escopo

> **Parte 1:** Analisador Léxico + Analisador Sintático  
> **Parte 2:** Demais etapas do compilador — serão adicionadas posteriormente.

Este README documenta exclusivamente os requisitos e conceitos correspondentes à **Parte 1** do projeto. A documentação da Parte 2 será incorporada posteriormente, mantendo a separação entre as etapas do desenvolvimento...

---

## 📖 Referência

Documento-base fornecido para a atividade:

**MicroPascal — Projeto de Compiladores: Primeira Parte.**

> Este README organiza e apresenta os requisitos do enunciado em formato adequado para documentação de repositório, preservando o escopo da primeira parte do projeto.


# Desenvolvedores
| [<img src="https://github.com/Amanda-Aziz.png" width="115"><br><sub>Amanda Aziz</sub>](https://github.com/Amanda-Aziz) | [<img src="https://github.com/francisLauriano.png" width="115"><br><sub>Francis Lauriano</sub>](https://github.com/francisLauriano) | [<img src="https://github.com/Sofiafs.png" width="115"><br><sub>Sofia Farias</sub>](https://github.com/Sofiafs) |
| :---: | :---: | :---: |
