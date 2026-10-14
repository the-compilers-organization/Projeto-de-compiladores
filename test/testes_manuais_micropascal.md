# Testes manuais — Analisadores Léxico e Sintático MicroPascal

Este documento reúne **25 testes independentes** para execução manual no terminal. Cada exemplo deve ser executado separadamente. Os testes verificam o reconhecimento léxico e a conformidade sintática; **não** verificam execução, valores calculados ou semântica.

## Preparação e execução (PowerShell / VS Code)

Na raiz do projeto, compile:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/lexer.c src/parser.c -o micropascal.exe
```

Para cada teste, execute:

```powershell
.\micropascal.exe
```

Cole **apenas o código Pascal** do teste e finalize a entrada com **Enter** e **Ctrl+Z e Enter** no Windows. O analisador lê até o fim da entrada (`EOF`). Se estiver usando outro terminal, o procedimento para sinalizar EOF pode ser diferente.

**Como avaliar:** nos testes válidos, o programa deve informar que o código é sintaticamente válido. Nos testes inválidos, deve indicar erro de sintaxe ou erro léxico, conforme a categoria. O lexema exato indicado pode depender do ponto em que a análise detecta a falha.

---

## 1. Programas válidos — testes 01 a 10

### Teste 01 — Programa sem declarações e sem comandos
**Esperado:** aceito.

```pascal
program Teste;
var
begin
end.
```

### Teste 02 — Declarações dos três tipos
**Esperado:** aceito.

```pascal
program Teste;
var
    idade : integer;
    altura : real;
    letra : char;
begin
    idade := 20;
    altura := 1.75;
    letra := 'A';
end.
```

### Teste 03 — Vários identificadores e subtração
**Esperado:** aceito.

```pascal
program Teste;
var
    a, b, resultado : integer;
begin
    a := 20;
    b := 8;
    resultado := a - b;
    write(resultado);
end.
```

### Teste 04 — Operadores aritméticos e parênteses
**Esperado:** aceito.

```pascal
program Teste;
var
    resultado : real;
begin
    resultado := 10 + 2 * 3;
    resultado := (10 + 2) * 3;
    resultado := 20 / 4 + 8 div 2;
end.
```

### Teste 05 — Estrutura `if/else`
**Esperado:** aceito.

```pascal
program Teste;
var
    x : integer;
begin
    x := 10;
    if x >= 5 then
        write('S');
    else
        write('N');
end.
```

### Teste 06 — `while` com bloco
**Esperado:** aceito.

```pascal
program Teste;
var
    i : integer;
begin
    i := 0;
    while i < 5 do
    begin
        write(i);
        i := i + 1;
    end;
end.
```

### Teste 07 — `repeat/until` com bloco
**Esperado:** aceito.

```pascal
program Teste;
var
    i : integer;
begin
    i := 0;
    repeat
    begin
        i := i + 1;
        write(i);
    end;
    until i = 5;
end.
```

### Teste 08 — Operadores relacionais e lógicos
**Esperado:** aceito.

```pascal
program Teste;
var
    a, b : integer;
begin
    a := 10;
    b := 20;
    if (a < b) and (b <> 0) or (a = 10) then
        write('S');
end.
```

### Teste 09 — Comentários `//`
**Esperado:** aceito.

```pascal
program Teste;
var
    x : integer;
begin
    // Inicializando a variavel
    x := 10; // comentario na mesma linha
    write(x);
end.
```

### Teste 10 — Identificadores, `_` e caracteres escapados
**Esperado:** aceito.

```pascal
program _Teste123;
var
    _valor1, valor_2 : integer;
    caractere : char;
begin
    _valor1 := 10;
    valor_2 := _valor1 + 5;
    caractere := '_';
    write('\n');
    write('\t');
end.
```

---

## 2. Erros sintáticos — testes 11 a 20

### Teste 11 — Ausência de `var`
**Esperado:** erro sintático (`begin`).

```pascal
program Teste;
begin
end.
```

### Teste 12 — Falta de `;` no cabeçalho
**Esperado:** erro sintático (`var`).

```pascal
program Teste
var
begin
end.
```

### Teste 13 — Tipo não previsto
**Esperado:** erro sintático (`boolean`).

```pascal
program Teste;
var
    x : boolean;
begin
end.
```

### Teste 14 — Atribuição sem expressão
**Esperado:** erro sintático (`;`).

```pascal
program Teste;
var
    x : integer;
begin
    x := ;
end.
```

### Teste 15 — Uso de `=` no lugar de `:=`
**Esperado:** erro sintático (`=`).

```pascal
program Teste;
var
    x : integer;
begin
    x = 10;
end.
```

### Teste 16 — `while` sem `do`
**Esperado:** erro sintático (próximo `x`).

```pascal
program Teste;
var
    x : integer;
begin
    while x < 10
        x := x + 1;
end.
```

### Teste 17 — `repeat` sem `until`
**Esperado:** erro sintático (`end`).

```pascal
program Teste;
var
    x : integer;
begin
    repeat
        x := x + 1;
end.
```

### Teste 18 — Sinal negativo unário não previsto
**Esperado:** erro sintático (`-`).

```pascal
program Teste;
var
    x : integer;
begin
    x := -10;
end.
```

### Teste 19 — `write` sem expressão
**Esperado:** erro sintático (`)`).

```pascal
program Teste;
var
begin
    write();
end.
```

### Teste 20 — Código após `end.`
**Esperado:** erro sintático (`write`).

```pascal
program Teste;
var
begin
end.
write(10);
```

---

## 3. Erros léxicos — testes 21 a 25

### Teste 21 — Caractere `$` não permitido
**Esperado:** erro léxico (`$`).

```pascal
program Teste;
var
    x : integer;
begin
    x := 10 $ 5;
end.
```

### Teste 22 — Caractere `@` não permitido
**Esperado:** erro léxico (`@`).

```pascal
program Teste;
var
begin
    write(@);
end.
```

### Teste 23 — Literal `char` com dois caracteres
**Esperado:** erro léxico.

```pascal
program Teste;
var
    c : char;
begin
    c := 'ab';
end.
```

### Teste 24 — Sequência de escape inválida
**Esperado:** erro léxico.

```pascal
program Teste;
var
    c : char;
begin
    c := '\x';
end.
```

### Teste 25 — Literal `char` vazio
**Esperado:** erro léxico.

```pascal
program Teste;
var
    c : char;
begin
    c := '';
end.
```

---

## Registro dos resultados

Preencha a coluna **Obtido** com `ACEITA`, `SINTATICO`, `LEXICO` ou `OUTRO`. Marque **Conforme?** com `Sim` ou `Não`.

| Teste | Categoria | Esperado | Obtido | Conforme? |
|---|---|---|---|---|
| 01 | Válido | ACEITA | | |
| 02 | Válido | ACEITA | | |
| 03 | Válido | ACEITA | | |
| 04 | Válido | ACEITA | | |
| 05 | Válido | ACEITA | | |
| 06 | Válido | ACEITA | | |
| 07 | Válido | ACEITA | | |
| 08 | Válido | ACEITA | | |
| 09 | Válido | ACEITA | | |
| 10 | Válido | ACEITA | | |
| 11 | Sintático | SINTATICO | | |
| 12 | Sintático | SINTATICO | | |
| 13 | Sintático | SINTATICO | | |
| 14 | Sintático | SINTATICO | | |
| 15 | Sintático | SINTATICO | | |
| 16 | Sintático | SINTATICO | | |
| 17 | Sintático | SINTATICO | | |
| 18 | Sintático | SINTATICO | | |
| 19 | Sintático | SINTATICO | | |
| 20 | Sintático | SINTATICO | | |
| 21 | Léxico | LEXICO | | |
| 22 | Léxico | LEXICO | | |
| 23 | Léxico | LEXICO | | |
| 24 | Léxico | LEXICO | | |
| 25 | Léxico | LEXICO | | |


> Nota: o teste 05 utiliza `;` antes de `else` e o teste 07 utiliza `;` antes de `until`, conforme os exemplos e o padrão de comandos discutidos para esta implementação. Caso a versão exata do Parser exija outra pontuação, verifique a produção correspondente antes de classificar o resultado como defeito.
