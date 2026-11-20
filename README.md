<h1 align="center">🔤 Projeto de Compiladores — MicroPascal</h1>

<p align="center">
  <img src="https://img.shields.io/static/v1?label=C&message=MicroPascal&color=00599C&style=for-the-badge&logo=c"/>
  <img src="https://img.shields.io/static/v1?label=GCC&message=Compilador&color=A42E2B&style=for-the-badge&logo=gnu"/>
  <img src="https://img.shields.io/static/v1?label=Git&message=Versionamento&color=F05032&style=for-the-badge&logo=git"/>
  <img src="https://img.shields.io/static/v1?label=GitHub&message=2026.2&color=181717&style=for-the-badge&logo=github"/>
  <img src="https://img.shields.io/static/v1?label=STATUS&message=EM%20DESENVOLVIMENTO&color=yellow&style=for-the-badge"/>
</p>

<p align="center">
  <strong>Parte 1 — Análise Léxica e Sintática | Parte 2 — Análise Semântica com Tabela de Símbolos e Geração de Código Intermediário</strong>
</p>

> **Status do Projeto:** 🚧 Em desenvolvimento — análise léxica, análise sintática descendente recursiva e **análise semântica com tabela de símbolos hash implementadas**. O projeto **não utiliza AST** e **não executa programas Pascal**. A geração de código intermediário ainda não foi implementada.

Projeto acadêmico em **C11**, desenvolvido na disciplina **Construção de Compiladores** da Universidade Católica de Pernambuco (**UNICAP**), sob orientação do professor **Robson Lins**.

---

# 📌 Tópicos

:small_blue_diamond: [Sobre o Projeto](#️-sobre-o-projeto)  
:small_blue_diamond: [Objetivos](#-objetivos)  
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
:small_blue_diamond: [Parte 2 — Análise Semântica](#-parte-2--análise-semântica-com-tabela-hash)  
:small_blue_diamond: [Resumo das Responsabilidades](#10--resumo-das-responsabilidades)  
:small_blue_diamond: [Escopo](#-escopo)  
:small_blue_diamond: [Referência](#-referência)  
:small_blue_diamond: [Desenvolvedores](#-desenvolvedores)

---

# ⁉️ Sobre o Projeto

Este repositório reúne as **Partes 1 e 2 do Projeto de Compiladores**, cujo objetivo é aplicar conceitos fundamentais de construção de compiladores por meio do desenvolvimento de um compilador para uma versão simplificada da linguagem Pascal, denominada **micro-Pascal**.

O compilador recebe o caminho de um arquivo `.pas`, identifica os tokens, verifica a gramática e aplica as regras semânticas durante a análise sintática. Um programa aceito termina com código de saída `0`; um erro termina com código diferente de `0` e uma mensagem diagnóstica.

O projeto contempla:

- 🔆 **Analisador Léxico (Lexer)**
- 🌳 **Analisador Sintático (Parser)** descendente recursivo
- 🧠 **Analisador Semântico**, integrado ao Parser, com **tabela de símbolos hash**

O **Analisador Léxico** recebe o código-fonte e transforma as sequências de caracteres em **tokens**.

O **Analisador Sintático** recebe esses tokens e verifica se eles aparecem em uma sequência permitida pela **gramática da linguagem micro-Pascal**.

Na **Parte 2**, a análise semântica foi integrada ao Parser com uma **tabela de símbolos implementada por tabela hash**, sem construção de árvore sintática abstrata (AST). A geração de código intermediário ainda não está implementada.

Não há menu interativo, entrada de código por `stdin`, geração de AST nem interpretação de `write`.

---

# 🎯 Objetivos

**Parte 1**

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

**Parte 2**

- Implementar uma **tabela de símbolos** com hash e encadeamento para colisões;
- impedir declarações duplicadas e uso de variáveis não declaradas;
- validar os tipos de operadores, atribuições e condições;
- realizar as verificações durante o percurso do Parser, **sem AST**;
- gerar código intermediário (**pendente**).

---

# 🗂️ Estrutura do Projeto

A organização separa os cabeçalhos em `include/`, as implementações em `src/` e os testes em `test/`, todos dentro de `micropascal-compiler/`.

```text
Projeto-de-compiladores-copia/
├── .vscode/
├── docs/
│   └── instrucoes_professor/
├── micropascal-compiler/
│   ├── include/
│   │   ├── token.h
│   │   ├── lexer.h
│   │   ├── parser.h
│   │   ├── tabela_simbolos.h
│   │   └── semantico.h
│   ├── src/
│   │   ├── main.c
│   │   ├── lexer.c
│   │   ├── parser.c
│   │   ├── tabela_simbolos.c
│   │   └── semantico.c
│   ├── test/
│   │   ├── 01_validos_lexicos_sintaticos/
│   │   ├── 02_erros_sintaticos/
│   │   ├── 03_erros_lexicos/
│   │   ├── 04_semanticos/
│   │   ├── executar_testes.c
│   │   ├── manifesto.csv
│   │   ├── README_test.md
│   │   └── resultado_execucao_NNN.csv
│   ├── Makefile
│   ├── rodar_testes_completos.bat
│   └── rodar_testes_completos.sh
└── README.md
```

Os relatórios CSV são gerados em `micropascal-compiler/test/`. Os caminhos de entrada do manifesto são relativos a essa pasta.

---

# 📚 Organização da Documentação

- **`docs/instrucoes_professor/`** — enunciados, requisitos e materiais fornecidos pelo professor.

---

# 📁 Organização dos Arquivos

## `include/token.h`

Centraliza a definição dos tipos de token e da estrutura `Token`. Os tokens representam identificadores, palavras reservadas, literais, operadores, pontuação e fim de arquivo. O Lexer produz tokens e o Parser os consome.

## `include/lexer.h` e `src/lexer.c`

O cabeçalho declara a interface do analisador léxico; o arquivo `.c` implementa a leitura do arquivo-fonte e a identificação dos lexemas. O Lexer reconhece identificadores, literais (inteiros, reais e caracteres), palavras reservadas e operadores; ignora espaços e comentários `//`; e informa erros léxicos.

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

O Parser descendente recursivo reconhece programa, declarações, comandos e expressões, e integra as verificações semânticas durante o reconhecimento, sem construir uma árvore sintática.

## `include/tabela_simbolos.h` e `src/tabela_simbolos.c`

Armazenam nome, tipo e linha de declaração de cada variável. Permitem inserir, buscar e liberar símbolos em uma **tabela hash com colisões tratadas por encadeamento**. O Parser utiliza a tabela para impedir declarações duplicadas e identificar usos de variáveis não declaradas.

## `include/semantico.h` e `src/semantico.c`

Verificam os tipos de operadores, atribuições e condições. O tipo `boolean` é usado internamente nas expressões e não pode ser declarado como variável.

## `Makefile`

Automatiza a compilação do projeto com **GCC** e o padrão **C11**, incluindo automaticamente todos os arquivos `src/*.c`. Os detalhes estão na seção [8.4 — Compilação com Makefile](#84-compilação-com-makefile).

## `src/main.c`

É o ponto de entrada. Abre o arquivo de entrada recebido pela linha de comando, inicializa e finaliza o Lexer e o Parser e retorna o estado da análise por meio do código de saída. **Não existe menu interativo nem leitura do código pela entrada padrão (`stdin`).**

## `test/executar_testes.c`

Executa os casos registrados em `manifesto.csv`, compara o resultado observado com a coluna `esperado` e gera relatórios CSV numerados, como `resultado_execucao_001.csv` e `resultado_execucao_002.csv`.

## Scripts de automação

- `rodar_testes_completos.bat` e `rodar_testes_completos.sh`: compilam o projeto e executam a suíte automatizada.

## `.vscode/` e `README.md`

`.vscode/` pode armazenar configurações locais do editor; este README documenta a arquitetura, a linguagem, a compilação e a validação do projeto.

---

# 🔄 Fluxo do Projeto

```text
Arquivo .pas
    │
    ▼
main.c — abre o arquivo-fonte
    │
    ▼
Lexer — reconhece tokens
    │
    ▼
Parser descendente recursivo — verifica a gramática e os tipos
    │           │
    │           ├── Tabela de símbolos (hash) — declarações e consultas
    │           └── Regras semânticas — operações, atribuições e condições
    │
    ├── erro léxico, sintático ou semântico → diagnóstico e falha
    ▼
Programa aceito → código de saída 0
```

O analisador não executa comandos Pascal e não gera código intermediário nesta versão.

---

# 🧩 Parte 1 — Análise Léxica e Sintática

# 1. 🔎 Analisador Léxico (Lexer)

O Analisador Léxico é responsável por percorrer o código-fonte e identificar os **tokens** reconhecidos pela linguagem micro-Pascal.

Para cada token reconhecido, o Lexer produz uma estrutura `Token`, que contém o **tipo do token** e seu **lexema**. O Parser consulta essas informações para reconhecer a gramática.

---

## 1.1 🔤 Tokens e construções

- **Palavras reservadas:** `program`, `var`, `integer`, `real`, `char`, `begin`, `end`, `if`, `then`, `else`, `while`, `do`, `repeat`, `until`, `write`.
- **Operadores:** `+`, `-`, `*`, `/`, `div`, `=`, `<>`, `<`, `<=`, `>`, `>=`, `and`, `or`, `not`, `:=`.
- **Literais:** inteiros, reais e caracteres.
- **Identificadores.**
- **Pontuação:** `(`, `)`, `,`, `;`, `:`, `.`.

---

## 1.2 ⚖️ Operadores Relacionais

| Operador | Significado |
|---|---|
| `<` | menor que |
| `>` | maior que |
| `<=` | menor ou igual |
| `>=` | maior ou igual |
| `=` | igual |
| `<>` | diferente |

---

## 1.3 ➕ Operadores Lógico-Aritméticos

| Operador | Categoria |
|---|---|
| `+` | soma |
| `-` | subtração / menos unário |
| `*` | multiplicação |
| `/` | divisão real |
| `div` | divisão inteira |
| `and` | operador lógico |
| `or` | operador lógico |
| `not` | operador lógico |

---

## 1.4 📝 Operador de Atribuição

O operador de atribuição é `:=`, reconhecido pelo Lexer como um único token.

```pascal
contador := 10;
```

---

## 1.5 🔠 Sensibilidade a Maiúsculas e Minúsculas

A linguagem **micro-Pascal é case-sensitive**: `begin` é reconhecido como palavra reservada, enquanto `Begin` não corresponde à palavra reservada e pode ser reconhecido como identificador.

---

## 1.6 💬 Comentários

O Lexer ignora comentários de linha iniciados por `//`:

```pascal
x := 10; // comentário ignorado pelo Lexer
```

---

## 1.7 ⚠️ Erros Léxicos

Um erro léxico ocorre quando o Lexer encontra um caractere que não pode fazer parte de nenhum token válido.

```text
x := 10 $ 2;
```

O caractere `$` não pertence aos tokens reconhecidos pela linguagem. O erro é responsabilidade do `lexer.c`.

---

# 2. 🌳 Analisador Sintático (Parser)

O Analisador Sintático verifica se a sequência de tokens produzida pelo Lexer está de acordo com a estrutura gramatical definida para micro-Pascal.

A implementação utiliza um **Parser Descendente Recursivo**, no qual cada construção da gramática é representada por uma função. As verificações semânticas são feitas durante esse mesmo percurso.

---

# 3. 📐 Gramática de micro-Pascal

A gramática utiliza uma seção `var` global, sem declaração de variáveis locais em blocos.

```text
programa   ::= program IDENTIFICADOR ; var { declaração } begin { comando } end .

declaração ::= IDENTIFICADOR { , IDENTIFICADOR } : (integer | real | char) ;

comando    ::= bloco ;
             | IDENTIFICADOR := expressão ;
             | while expressão do comando
             | repeat comando until expressão ;
             | if expressão then comando [else comando]
             | write ( expressão ) ;

bloco      ::= begin { comando } end
```

O Parser reconhece tanto a subtração binária (`10 - 5`) quanto o **menos unário** (`-5`, `-x`), aceito na segunda etapa, desde que os tipos respeitem as regras semânticas.

---

# 4. 📊 Precedência e Associatividade

O Parser utiliza funções recursivas para os níveis de expressões: **fator**, **termo**, **soma**, **relacional** e **expressão**.

| Nível | Operadores | Precedência |
|---:|---|---|
| 1 | `*`, `/`, `div` | maior |
| 2 | `+`, `-` | |
| 3 | `=`, `<>`, `<`, `>`, `<=`, `>=` | |
| 4 | `and`, `or` | menor |

```text
expressão()
      ↓
relacional()
      ↓
soma()
      ↓
termo()
      ↓
fator()
```

Por exemplo, `x + 2 * 5` corresponde a `x + (2 * 5)`, e não a `(x + 2) * 5`.

---

# 5. 🔗 Integração entre Lexer e Parser

A integração acontece por meio da interface declarada em `include/lexer.h` e da estrutura `Token` definida em `include/token.h`. O Lexer fornece o próximo token; o Parser verifica se ele é o esperado pela gramática e avança quando necessário.

Para a entrada:

```pascal
contador := 10;
```

O Lexer identifica, em sequência, um identificador, o operador de atribuição, um literal inteiro e o ponto e vírgula. O Parser verifica se essa sequência corresponde a um comando de atribuição válido, consulta a tabela de símbolos e confere os tipos com o módulo semântico, **sem criar AST**.

---

# 6. ⚠️ Tratamento de Erros

O projeto diferencia três tipos principais de erro:

```text
Erro Léxico
Erro Sintático
Erro Semântico
```

## 6.1 Erro Léxico

Ocorre quando um caractere não pode fazer parte de nenhum token reconhecido, como `$` em `x := 10 $ 5;`.

```text
Caractere inválido → Lexer → Erro léxico
```

## 6.2 Erro Sintático

Ocorre quando os tokens são válidos individualmente, mas aparecem em uma sequência que não corresponde à gramática. Em `x := ;`, após `:=` a gramática exige uma expressão.

```text
Tokens válidos em ordem inválida → Parser → Erro sintático
```

## 6.3 Erro Semântico

Ocorre quando a estrutura é sintaticamente válida, mas viola regras de declaração ou de tipos. Exemplos: declarar `x` duas vezes, usar uma variável não declarada, atribuir um valor `real` a uma variável `integer` ou usar uma condição não booleana em `if`, `while` ou `repeat`.

Quando existem vários erros, o primeiro encontrado pode impedir que erros posteriores sejam reportados.

---

# 7. 💻 Exemplos de Código micro-Pascal

## 7.1 Exemplo válido

```pascal
program Teste;
var
    x : integer;
begin
    x := -5;
    if x < 0 then
        write(x);
end.
```

Esse exemplo utiliza:

- declaração de programa e de variável;
- tipo `integer`;
- atribuição com menos unário;
- expressão relacional;
- `if`;
- `write`.

O comando `write(x)` é **validado**, mas não imprime o valor de `x`: o projeto não é um interpretador.

---

# 8. 🛠️ Compilação e Execução

**Todos os comandos abaixo devem ser executados dentro de `micropascal-compiler/`.** É necessário ter o GCC com suporte a C11 instalado e disponível no terminal.

## 8.1 Windows — PowerShell

**Compilar todos os módulos:**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
```

**Analisar um arquivo `.pas`:**

```powershell
.\micropascal.exe .\test\04_semanticos\001_valido_aritmetica.pas
$LASTEXITCODE
```

## 8.2 Linux/macOS — Terminal

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
./micropascal test/04_semanticos/001_valido_aritmetica.pas
echo $?
```

## 8.3 Como criar um teste manual

Crie e salve um arquivo `.pas` contendo um programa MicroPascal completo e passe o caminho desse arquivo ao executável. O argumento deve ser o caminho de um arquivo existente; o código não é lido por `stdin`. O código de saída `0` indica sucesso; um valor diferente de `0` indica erro.

## 8.4 Compilação com Makefile

Alternativamente, use o `Makefile` na raiz de `micropascal-compiler/`. É necessário ter **GCC** e **Make** instalados.

**Compilar o projeto:**

```bash
make
```

O `Makefile` inclui automaticamente todos os arquivos `src/*.c`, sem precisar listar cada fonte manualmente.

**Limpar os arquivos compilados:**

```bash
make clean
```

No Windows, `make clean` pode depender de comandos Unix (por exemplo, disponíveis no Git Bash/MSYS2).

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

O programa deve terminar com código `0`.

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

## 9.4 ❌ Erro semântico

```pascal
program Exemplo;
var
    x : integer;
begin
    x := 1.5;
end.
```

A estrutura é válida, mas a atribuição de `real` a `integer` deve ser rejeitada.

## 9.6 Suíte automatizada

A suíte utiliza `micropascal-compiler/test/manifesto.csv` e os arquivos `.pas` das quatro subpastas de `test/`. O executor compara o resultado observado com a coluna `esperado` do manifesto.

| Pasta | Conteúdo |
|---|---|
| `01_validos_lexicos_sintaticos` | programas válidos léxica e sintaticamente |
| `02_erros_sintaticos` | casos com erros sintáticos |
| `03_erros_lexicos` | casos com erros léxicos |
| `04_semanticos` | casos válidos e inválidos da análise semântica |

Os scripts podem ser executados a partir de `micropascal-compiler/` ou chamados pelo caminho completo a partir de outros diretórios.

**Windows:**

```powershell
.\rodar_testes_completos.bat
```

**Linux/macOS:**

```bash
sh rodar_testes_completos.sh
```

**Execução manual:**

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
gcc -std=c11 -Wall -Wextra -Wpedantic test/executar_testes.c -o executar_testes
./executar_testes ./micropascal
```

Os relatórios são gravados em `test/resultado_execucao_NNN.csv`, sem sobrescrever relatórios anteriores. Consulte [`README_test.md`](micropascal-compiler/test/README_test.md) para mais informações.

---

# 🧠 Parte 2 — Análise Semântica com Tabela Hash

A análise semântica foi integrada à análise sintática descendente recursiva. O projeto **não utiliza AST**.

## Tabela de símbolos

A tabela hash registra nome, tipo e linha de declaração. A inserção verifica duplicidade e a consulta verifica se uma variável foi declarada antes do uso. Como a gramática possui uma única seção `var` global, a implementação usa uma única tabela de símbolos.

## Regras de tipos

- **Declarações:** não pode haver identificador duplicado; toda variável usada deve ter sido declarada.
- **Tipos:** `integer`, `real`, `char`; `boolean` existe internamente como resultado de comparações e operações lógicas, mas não é declarável em `var`.
- **`+` e `-`:** operandos numéricos do mesmo tipo produzem esse tipo; combinações `char`/`integer` produzem `char`.
- **`*`:** operandos numéricos do mesmo tipo, resultado desse tipo.
- **`div`:** apenas `integer div integer`, resultado `integer`.
- **`/`:** apenas `real / real`, resultado `real`.
- **`<`, `<=`, `>` e `>=`:** operandos numéricos do mesmo tipo, resultado `boolean`.
- **`=` e `<>`:** operandos do mesmo tipo (inclusive `char` e `boolean`), resultado `boolean`.
- **`and`, `or`:** dois operandos `boolean`, resultado `boolean`.
- **Menos unário:** operando numérico, mantendo seu tipo.
- **`not`:** operando `boolean` e resultado `boolean` (regra adotada na implementação; não explicitada nas imagens da seção 2.2).
- **Atribuição:** tipo do destino exatamente igual ao tipo da expressão; não há conversão automática.
- **Condições:** expressões de `if`, `while` e `repeat` devem ser `boolean`.

**Ainda não implementado:** geração de código intermediário.

---

# 10. 🧠 Resumo das Responsabilidades

| Arquivo | Responsabilidade |
|---|---|
| `include/token.h` | Tipos de token e estrutura `Token` |
| `include/lexer.h` / `src/lexer.c` | Reconhecer identificadores, literais, palavras reservadas, operadores e erros léxicos |
| `include/parser.h` / `src/parser.c` | Reconhecer programa, declarações, comandos e expressões; integrar as verificações semânticas |
| `include/tabela_simbolos.h` / `src/tabela_simbolos.c` | Armazenar nome, tipo e linha de declaração; inserir, buscar e liberar símbolos em tabela hash com colisões por encadeamento |
| `include/semantico.h` / `src/semantico.c` | Verificar tipos de operadores, atribuições e condições |
| `src/main.c` | Abrir o arquivo de entrada, inicializar/finalizar Lexer e Parser e retornar o estado da análise |
| `Makefile` | Automatizar a compilação dos módulos C e a limpeza dos artefatos |
| `test/executar_testes.c` | Comparar resultados com o manifesto e gerar relatórios |
| `test/manifesto.csv` | Registrar as classificações esperadas |
| `test/README_test.md` | Documentar a suíte automatizada |
| `rodar_testes_completos.bat` / `.sh` | Automatizar compilação e testes |
| `README.md` | Documentar o projeto |

A distinção principal entre os componentes é:

```text
LEXER:     "Qual token foi reconhecido?"
PARSER:    "Essa sequência de tokens segue a gramática?"
SEMÂNTICA: "Os identificadores e tipos são válidos?"
```

---

# 📚 Escopo

> **Parte 1 — implementada:** análise léxica e análise sintática descendente recursiva.

> **Parte 2 — parcialmente implementada:** análise semântica com tabela de símbolos (hash) e validação de tipos, sem AST; geração de código intermediário pendente.

O projeto é um analisador/compilador acadêmico em desenvolvimento e **não interpreta nem executa** os comandos Pascal.

---

# 📖 Referência

Os documentos originais do professor, quando presentes no repositório, ficam em `docs/instrucoes_professor/` e devem ser consultados para o enunciado completo:

📄 **[MicroPascal — Projeto de Compiladores: Primeira Parte](docs/instrucoes_professor/MicroPascal-parte1.pdf)**

📄 **[MicroPascal — Projeto de Compiladores: Segunda Parte](docs/instrucoes_professor/MicroPascal-parte2.pdf)**

A implementação atual segue a orientação posterior do professor de **não utilizar AST**.

---

# 👨‍💻 Desenvolvedores


| [<img src="https://github.com/Amanda-Aziz.png" width="115"><br><sub>Amanda Aziz</sub>](https://github.com/Amanda-Aziz) | [<img src="https://github.com/ayrtongomes070-pixel.png" width="115"><br><sub>Ayrton Gomes</sub>](https://github.com/ayrtongomes070-pixel) | [<img src="https://github.com/eduardovcmalta.png" width="115"><br><sub>Eduardo Malta</sub>](https://github.com/eduardovcmalta) | [<img src="https://github.com/francisLauriano.png" width="115"><br><sub>Francis Lauriano</sub>](https://github.com/francisLauriano) | [<img src="https://github.com/GuiDev005.png" width="115"><br><sub>Guilherme Eduardo Araujo</sub>](https://github.com/GuiDev005) | [<img src="https://github.com/Sofiafs.png" width="115"><br><sub>Sofia Farias</sub>](https://github.com/Sofiafs) |
| :---: | :---: | :---: |:---: | :---: | :---: |
