# MICRO-PASCAL-COMPILADORES-2026.2

Compilador para a linguagem **micro-Pascal**, desenvolvido como projeto da disciplina de **Construção de Compiladores**, ministrada pelo professor **Robson Lins**, na **Universidade Católica de Pernambuco (UNICAP)**.

Este repositório contém a primeira parte do projeto, correspondente às etapas de:
- **Análise Léxica (Lexer)**
- **Análise Sintática (Parser)**

As etapas seguintes (análise semântica e geração de código) serão desenvolvidas na segunda parte do projeto.

## 👥 Integrantes

- Eduardo Veloso Chaves Malta
- Ayrton Gomes Costa
- Guilherme Eduardo Araujo da Silva
- Renato Ichigi

## 👨‍💻 Divisão de Trabalho

| Integrante | Responsabilidade | Arquivos |
|---|---|---|
| **Eduardo Veloso Chaves Malta** | Analisador Léxico (Lexer) — reconhecimento de todos os tokens, tratamento de erros léxicos | `src/lexer.c`, `include/lexer.h` |
| **Ayrton Gomes Costa** | Parser — Estrutura e Comandos (`programa`, `bloco`, `atribuicao`, `iteracao`, `decisao`, `escrita`) | `src/parser.c` (estrutura) |
| **Renato Ichigi** | Parser e Lexer — Estrutura e Comandos (`programa`, `correção`, `atribuicao`, `iteracao`, `escrita`) | `src/parser.c`, `src/lexer.c` |
| **Guilherme Eduardo Araujo da Silva** | Parser — Expressões (precedência de operadores) e integração geral do projeto | `src/parser.c` (expressões), `src/main.c` |

### Fluxo de trabalho em Git

- Cada integrante desenvolve sua parte em uma branch própria (`dudu`, `ayrton`, `renato`, `guilherme`)
- Integração feita via Pull Request, revisado pelos demais membros antes do merge na `main`

## 📖 Sobre a linguagem micro-Pascal

Micro-Pascal é uma versão simplificada da linguagem Pascal, com estrutura de blocos aninhados (similar a C/Java). O compilador reconhece:

- Identificadores, palavras reservadas, operadores relacionais, lógico-aritméticos e de atribuição
- Literais inteiros, reais e de caractere
- Comandos de atribuição, decisão (`if/then/else`), iteração (`while`, `repeat/until`) e escrita (`write`)

### Exemplo de código em micro-Pascal

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

## 🛠️ Tecnologias utilizadas

- **C** — linguagem de implementação do compilador
- **Git** — controle de versão
- **GitHub** — hospedagem do repositório e colaboração
- **Make** — automação do processo de build

## 📁 Estrutura do projeto


```
micropascal-compiler/
├── src/
│   ├── main.c        # Ponto de entrada do programa
│   ├── lexer.c        # Implementação do analisador léxico
│   ├── parser.c        # Implementação do analisador sintático
├── include/
│   ├── lexer.h
│   ├── parser.h
│   └── token.h        # Definições de tokens compartilhadas
├── testes/
│   ├── exemplo1.pas
│   ├── exemplo2.pas
│   ├── teste_lexer.pas
│   └── e1.pas, e2.pas, e3.pas, e5.pas   # casos de erro
├── rodar_testes.bat   # roda todos os testes (Windows)
├── rodar_testes.sh    # roda todos os testes (Linux/Mac)
├── Makefile
├── .gitignore
└── README.md
```


## ⚙️ Como compilar e executar

### Pré-requisitos
> Os comandos abaixo devem ser executados de dentro da pasta `micropascal-compiler/`.
- GCC (ou outro compilador C compatível)
- Make (opcional, mas recomendado)
  

### Compilando com Make

```bash
make
```

### Compilando manualmente

```bash
gcc -Wall -Wextra -Iinclude src/*.c -o micropascal
```

### Executando

```bash
./micropascal testes/exemplo1.pas
```

## ✅ Funcionalidades implementadas

- [X] Analisador léxico completo (reconhecimento de todos os tokens)
- [X] Tratamento de erros léxicos (`Erro léxico no caracter [x]`)
- [X] Analisador sintático (gramática de micro-Pascal)
- [X] Tratamento de erros sintáticos (`Erro de sintaxe no token [lexema]`)
- [X] Testes com exemplos de código válidos

## 🧪 Testes

Os arquivos de teste estão na pasta `testes/`:

- `exemplo1.pas` e `exemplo2.pas`: programas válidos em micro-Pascal, usados para validar o lexer e o parser.
- `teste_lexer.pas`: cobre todos os tipos de token da especificação.
- `e1.pas` e `e2.pas`: erros léxicos (caractere que não existe na linguagem e char literal com dois caracteres).
- `e3.pas` e `e5.pas`: erros de sintaxe (falta o ponto e vírgula e `Begin` com B maiúsculo, já que a linguagem é case-sensitive).

Para compilar e rodar todos os testes, mostrando o código e a saída de cada um:

```bash
rodar_testes.bat      # Windows
sh rodar_testes.sh    # Linux/Mac
```

Sem nenhuma saída, o programa foi aceito. Em caso de erro, a mensagem é impressa no `stderr` e o programa termina com código 1.

## 📌 Observações

- A linguagem micro-Pascal é **case-sensitive** (sensível a maiúsculas e minúsculas).
- Caracteres considerados brancos (ignorados pelo lexer): espaço, `\n`, `\t` e `\r`.
- Todos os operadores são associativos à esquerda, seguindo a seguinte ordem de precedência (do maior para o menor):
  1. `*`, `/`, `div`
  2. `+`, `-`
  3. `=`, `<>`, `<`, `>`, `<=`, `>=`
  4. `or`, `and`

## 📄 Licença

Projeto acadêmico desenvolvido para fins educacionais na disciplina de Construção de Compiladores (UNICAP).
