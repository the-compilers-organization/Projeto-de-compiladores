# Casos de teste — Analisador de Pascal

Este arquivo reúne exemplos válidos e inválidos para testar o analisador durante a apresentação do projeto.

## Teste válido

**Arquivo sugerido:** `teste_valido.pas`

```pascal
program Teste;
var
  n, soma : integer;
  x : real;
  c : char;
begin
  n := 10;
  soma := 0;
  x := 3.5;
  c := 'a';
  while n > 0 do
    begin
      soma := soma + n;
      n := n - 1;
    end;
  if soma = 55 then
    write('s');
  else
    write('n');
  write(soma div 2 * 3);
  write('\n');
end.
```

**Objetivo:** verificar a análise de um programa com declarações de variáveis, atribuições, laço `while`, condição `if/else` e chamadas de `write`.

## Testes inválidos

### 1. Símbolo inválido `@`

**Arquivo sugerido:** `teste_simbolo_invalido.pas`

```pascal
program E1;
var n : integer;
begin
  n := 3 @ 4;
end.
```

**Objetivo:** verificar se o analisador identifica o caractere `@` como inválido nesse contexto.

### 2. Ponto e vírgula ausente

**Arquivo sugerido:** `teste_ponto_virgula_ausente.pas`

```pascal
program E2;
var n : integer;
begin
  n := 3
  n := 4;
end.
```

**Objetivo:** verificar se o analisador detecta a falta de `;` após `n := 3`.

### 3. Ponto decimal em posição inválida

**Arquivo sugerido:** `teste_ponto_decimal_invalido.pas`

```pascal
program E4;
var n : integer;
begin
  n := 5.;
end.
```

**Objetivo:** verificar se o analisador rejeita o ponto colocado após `5` nesse literal.

> Observação: estes são os casos de teste fornecidos para a apresentação do trabalho. O resultado exato depende das regras implementadas no analisador.
