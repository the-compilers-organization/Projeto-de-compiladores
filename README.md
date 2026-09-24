# 🔤 Projeto de Compiladores — MicroPascal

> **Parte 1 — Analisador Léxico e Analisador Sintático**

> Status: 🚧 Em desenvolvimento

Projeto desenvolvido para a disciplina **Construção de Compiladores**, da Universidade Católica de Pernambuco (UNICAP), sob orientação do professor **Robson Lins**.

---

# ⁉️ Sobre o Projeto

Este repositório contém a implementação da **Parte 1 do Projeto de Compiladores**, cujo objetivo é aplicar conceitos fundamentais de construção de compiladores por meio do desenvolvimento das duas primeiras etapas de um compilador para uma versão simplificada da linguagem Pascal, denominada **micro-Pascal**.

Nesta primeira etapa, o projeto contempla:

- 🔆 **Analisador Léxico (Lexer)**
- 🌳 **Analisador Sintático (Parser)**

O **Analisador Léxico** recebe o código-fonte e transforma as sequências de caracteres em **tokens**.

O **Analisador Sintático** recebe esses tokens e verifica se eles aparecem em uma sequência permitida pela **gramática da linguagem micro-Pascal**.

As demais etapas do compilador serão desenvolvidas posteriormente na **Parte 2** do projeto.

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

O repositório está organizado da seguinte forma:

```text
Projeto-de-compiladores/
│
├── .vscode/
│
├── src/
│   ├── define.h
│   ├── lexer.c
│   ├── main.c
│   └── parser.c
│
├── .gitignore
│
└── README.md
```

A pasta `src/` concentra os arquivos responsáveis pela implementação do analisador.

---

# 📁 Organização dos Arquivos

## `src/define.h`

Arquivo de cabeçalho responsável por reunir as definições compartilhadas pelos diferentes módulos do projeto.

Nele são declarados:

- constantes de tamanho;
- códigos dos tokens;
- variáveis globais compartilhadas;
- protótipos das funções do Lexer;
- protótipos das funções do Parser.

Dessa forma, `main.c`, `lexer.c` e `parser.c` podem utilizar as mesmas definições.

---

## `src/lexer.c`

Responsável pela implementação do **Analisador Léxico (Lexer)**.

O Lexer percorre o código-fonte caractere por caractere, identifica os lexemas e determina qual token cada lexema representa.

Exemplo:

```text
Código-fonte:

soma := 10;
```

O Lexer reconhece:

```text
"soma"  → IDENTIFICADOR
":="    → ATRIBUICAO
"10"    → INTEIRO_LITERAL
";"     → PONTO_VIRGULA
```

Entre suas responsabilidades estão:

- ignorar espaços, tabulações e quebras de linha;
- reconhecer identificadores;
- diferenciar identificadores de palavras reservadas;
- reconhecer números inteiros;
- reconhecer números reais;
- reconhecer literais do tipo `char`;
- reconhecer operadores;
- reconhecer símbolos especiais;
- identificar o fim da entrada;
- detectar erros léxicos.

---

## `src/parser.c`

Responsável pela implementação do **Analisador Sintático (Parser)**.

O Parser utiliza os tokens produzidos pelo Lexer para verificar se o programa está de acordo com a gramática da linguagem micro-Pascal.

Exemplo:

```pascal
x := 10;
```

O Lexer produz:

```text
IDENTIFICADOR
ATRIBUICAO
INTEIRO_LITERAL
PONTO_VIRGULA
```

Em seguida, o Parser verifica se essa sequência corresponde à regra gramatical:

```text
<atribuicao> ::= IDENTIFICADOR ":=" <expressao> ;
```

O Parser foi desenvolvido utilizando a técnica de **Análise Sintática Descendente Recursiva**, em que as construções da gramática são representadas por funções em C.

Entre as funções utilizadas estão:

```text
programa()
secao_var()
decl_var()
tipo()
bloco()
lista_comandos()
comando()
atribuicao()
iteracao()
decisao()
escrita()
expressao()
```

---

## `src/main.c`

Responsável por iniciar e coordenar a execução do analisador.

O arquivo:

1. recebe o código-fonte;
2. armazena o código na entrada;
3. inicia a análise léxica;
4. solicita ao Lexer o primeiro token;
5. inicia o Parser através da função `programa()`;
6. verifica se toda a entrada foi analisada;
7. informa se o programa está sintaticamente válido ou se ocorreu algum erro.

O `main.c`, portanto, funciona como ponto de integração entre o **Lexer** e o **Parser**.

---

## `.gitignore`

Arquivo utilizado pelo Git para indicar arquivos e diretórios que não devem ser enviados para o repositório.

Pode ser utilizado, por exemplo, para ignorar:

- arquivos executáveis;
- arquivos temporários;
- arquivos gerados durante a compilação;
- configurações locais que não precisam ser versionadas.

---

## `.vscode/`

Diretório utilizado pelo Visual Studio Code para armazenar configurações relacionadas ao ambiente de desenvolvimento.

Essas configurações não fazem parte da implementação do Lexer ou do Parser.

---

## `README.md`

Arquivo responsável pela documentação do projeto.

Apresenta:

- objetivo do projeto;
- estrutura do repositório;
- funcionamento do Lexer;
- funcionamento do Parser;
- gramática da linguagem;
- precedência dos operadores;
- tratamento de erros;
- instruções para compilação e execução.

---

# 🔄 Fluxo do Projeto

O funcionamento geral do projeto pode ser representado por:

```text
                 Código-fonte
                      │
                      ▼
                ┌───────────┐
                │  main.c   │
                └─────┬─────┘
                      │
                      ▼
                ┌───────────┐
                │  lexer.c  │
                │           │
                │ caracteres│
                │     ↓     │
                │   tokens  │
                └─────┬─────┘
                      │
                      ▼
                ┌───────────┐
                │ parser.c  │
                │           │
                │  tokens   │
                │     ↓     │
                │ gramática │
                └─────┬─────┘
                      │
                      ▼
             Programa reconhecido
                      ou
                mensagem de erro
```

O arquivo `define.h` fornece as definições compartilhadas:

```text
                     define.h
                   /    |     \
                  /     |      \
                 ▼      ▼       ▼
             main.c  lexer.c  parser.c
```

De forma resumida:

```text
Código-fonte
     ↓
   main.c
     ↓
   Lexer
     ↓
   Tokens
     ↓
   Parser
     ↓
Programa sintaticamente válido
           ou
         Erro
```

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
Erro lexico no caracter [$]
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

A integração ocorre principalmente através de:

```c
simbolo_lido
lexema
obtenha_simbolo()
consumir()
```

---

## `simbolo_lido`

Armazena o tipo do token atualmente reconhecido.

Exemplo:

```text
IDENTIFICADOR
```

---

## `lexema`

Armazena o texto correspondente ao token atual.

Exemplo:

```text
contador
```

Para a entrada:

```pascal
contador := 10;
```

podemos ter:

```text
lexema       = "contador"
simbolo_lido = IDENTIFICADOR
```

---

## `obtenha_simbolo()`

É uma função do Lexer.

Sua responsabilidade é analisar a entrada a partir da posição atual e reconhecer o próximo token.

Ela atualiza:

```text
lexema
simbolo_lido
posicao
```

---

## `consumir()`

É uma função utilizada pelo Parser.

Sua responsabilidade é verificar se o token atual corresponde ao token esperado.

A ideia é:

```text
Token atual é o esperado?
        │
        ├── SIM
        │    ↓
        │ obtenha_simbolo()
        │    ↓
        │ próximo token
        │
        └── NÃO
             ↓
        erro sintático
```

Exemplo:

```c
consumir(PROGRAM);
```

significa:

```text
"Nesse ponto da gramática,
eu espero encontrar PROGRAM."
```

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

# 8. 🛠️ Compilação

Os arquivos-fonte estão armazenados dentro da pasta:

```text
src/
```

Portanto, a partir da raiz do repositório:

```text
Projeto-de-compiladores/
```

compile utilizando:

```bash
gcc src/main.c src/lexer.c src/parser.c -o main
```

Após a compilação, a estrutura será aproximadamente:

```text
Projeto-de-compiladores/
│
├── .vscode/
│
├── src/
│   ├── define.h
│   ├── lexer.c
│   ├── main.c
│   └── parser.c
│
├── .gitignore
├── README.md
│
└── main.exe
```

---

## 8.1 ▶️ Execução no Windows

No Windows, execute:

```bash
.\main.exe
```

O programa exibirá:

```text
Digite o programa MicroPascal:
```

Em seguida, digite o código micro-Pascal que deseja analisar.

> ⚠️ **Importante:** o analisador primeiro recebe todo o código-fonte e somente depois inicia a análise léxica e sintática.

Depois de terminar de digitar o código, no **PowerShell/Windows**, é necessário:

1. pressionar **Enter** após a última linha do código;
2. pressionar **Ctrl + Z** para sinalizar o fim da entrada (`EOF`);
3. pressionar **Enter** novamente.

Ou seja:

```text
Código MicroPascal
        ↓
      Enter
        ↓
    Ctrl + Z
        ↓
      Enter
        ↓
   Lexer + Parser
        ↓
Resultado da análise
```

> `EOF` significa **End Of File** (fim do arquivo/entrada).  
> Não deve ser digitada literalmente a palavra `EOF`.

---

## 8.2 ▶️ Execução no Linux/macOS

Execute:

```bash
./main
```

Após digitar o código-fonte, é necessário sinalizar o fim da entrada para que o analisador possa iniciar o processamento.

---

# 9. 🧪 Testando o Analisador

Após executar o programa, forneça um código escrito na linguagem micro-Pascal.

Por exemplo:

```pascal
program Teste;
var
    x : integer;
begin
    x := 10;
    write(x);
end.
```

No PowerShell/Windows, depois de digitar a última linha:

```pascal
end.
```

pressione:

```text
Enter
Ctrl + Z
Enter
```

Um exemplo da interação completa é:

```text
Digite o programa MicroPascal:

program Teste;
var
    x : integer;
begin
    x := 10;
    write(x);
end.

^Z
```

Após receber o `EOF`, o programa inicia a análise:

```text
Código-fonte completo
        ↓
       Lexer
        ↓
      Tokens
        ↓
       Parser
        ↓
Verificação da gramática
        ↓
     Resultado
```

Se todo o código estiver de acordo com a gramática, o resultado esperado será:

```text
Programa sintaticamente valido.
```

---

## 9.1 ⚠️ Não digitar `EOF`

A palavra:

```text
EOF
```

**não deve ser digitada após o código.**

Por exemplo, não faça:

```text
program Teste;
var
    x : integer;
begin
    x := 10;
    write(x);
end.
EOF
```

Nesse caso, as letras `E`, `O` e `F` seriam recebidas como parte do próprio código-fonte.

O `EOF` utilizado pelo programa é um sinal especial utilizado para indicar que não existem mais caracteres para serem lidos.

No PowerShell/Windows, utilize:

```text
Enter
Ctrl + Z
Enter
```

---

## 9.2 🔎 Por que é necessário informar o EOF?

O `main.c` lê o código-fonte até encontrar o final da entrada.

De forma simplificada, a leitura funciona assim:

```c
while(
    (caractere = getchar()) != EOF &&
    i < TAM - 1
){
    entrada[i] = caractere;
    i += 1;
}
```

Enquanto `getchar()` não receber `EOF`, o programa entende que ainda podem existir mais caracteres do código-fonte.

Por isso, apenas pressionar **Enter** depois de:

```pascal
end.
```

não encerra a entrada.

O programa continua aguardando.

Quando o usuário utiliza:

```text
Ctrl + Z
```

no PowerShell/Windows, o fim da entrada é sinalizado.

Então o fluxo completo passa a ser:

```text
1. Executar o programa
        ↓
2. Digitar todo o código MicroPascal
        ↓
3. Pressionar Enter
        ↓
4. Pressionar Ctrl + Z
        ↓
5. Pressionar Enter
        ↓
6. A leitura do código-fonte termina
        ↓
7. O Lexer inicia a análise léxica
        ↓
8. O Parser inicia a análise sintática
        ↓
9. O resultado é apresentado
```

---

## 9.3 ✅ Exemplo de Programa Válido

Entrada:

```pascal
program Teste;
var
    x : integer;
begin
    x := 10;
    write(x);
end.
```

Depois:

```text
Enter
Ctrl + Z
Enter
```

Resultado esperado:

```text
Programa sintaticamente valido.
```

---

## 9.4 ❌ Exemplo de Programa Inválido

Considere:

```pascal
program Teste
```

Depois:

```text
Enter
Ctrl + Z
Enter
```

O Parser reconhecerá:

```text
program → PROGRAM
Teste   → IDENTIFICADOR
```

Porém, a gramática espera um `;` depois do identificador:

```text
<programa> ::= program IDENTIFICADOR ; <secao_var> <bloco> .
```

Como a entrada foi encerrada antes do `;`, o token atual será `FIM`.

O resultado será:

```text
Erro de sintaxe no token [FIM]
```

Isso indica que a entrada terminou enquanto o Parser ainda esperava outro elemento da gramática.

---

# 10. 🧠 Resumo das Responsabilidades

| Arquivo | Responsabilidade |
|---|---|
| `src/define.h` | Definir tokens, constantes, variáveis compartilhadas e protótipos |
| `src/lexer.c` | Transformar caracteres e lexemas em tokens |
| `src/parser.c` | Verificar se os tokens obedecem à gramática |
| `src/main.c` | Receber a entrada e coordenar Lexer e Parser |
| `.gitignore` | Definir arquivos que não serão versionados |
| `README.md` | Documentar o projeto |

A principal separação é:

```text
LEXER
"Que token é esse?"
```

e:

```text
PARSER
"Esse token pode aparecer aqui?"
```

Portanto:

```text
Código-fonte
     ↓
   main.c
     ↓
   lexer.c
     ↓
caracteres → tokens
     ↓
   parser.c
     ↓
tokens → gramática
     ↓
Programa válido
     ou
    erro
```

---

# 📚 Escopo

> **Parte 1:** Analisador Léxico + Analisador Sintático

> **Parte 2:** Demais etapas do compilador — serão adicionadas posteriormente.

Este README documenta os requisitos e a implementação correspondente à **Parte 1** do projeto.

---

# 📖 Referência

Documento-base fornecido para a atividade:

**MicroPascal — Projeto de Compiladores: Primeira Parte.**

---

# 👨‍💻 Desenvolvedores

| [<img src="https://github.com/Amanda-Aziz.png" width="115"><br><sub>Amanda Aziz</sub>](https://github.com/Amanda-Aziz) | [<img src="https://github.com/francisLauriano.png" width="115"><br><sub>Francis Lauriano</sub>](https://github.com/francisLauriano) | [<img src="https://github.com/Sofiafs.png" width="115"><br><sub>Sofia Farias</sub>](https://github.com/Sofiafs) |
| :---: | :---: | :---: |