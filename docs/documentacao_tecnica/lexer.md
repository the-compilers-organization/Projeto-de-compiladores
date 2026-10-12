# lexer.c: Analisador léxico do micro-Pascal

Este documento explica o arquivo `lexer.c`, a **primeira etapa** do compilador micro-Pascal.

## 1. Qual é o papel do lexer?

O lexer (analisador léxico) transforma **caracteres em tokens**. Ele lê o código-fonte guardado em `entrada` e, a cada chamada, reconhece **um único token**, o próximo.

```
"soma := 10;"   --lexer-->   IDENTIFICADOR  ATRIBUICAO  INTEIRO_LITERAL  PONTO_VIRGULA  FIM
```

O lexer **não sabe** se o programa faz sentido (isso é papel do parser). Ele só responde: *"o que vem agora?"*.

### Como ele se comunica com o resto do projeto

O `lexer.c` usa as variáveis globais declaradas no `define.h` e definidas no `main.c`:

| Variável | O lexer... |
|---|---|
| `entrada` | **lê** os caracteres do código-fonte |
| `posicao` | **lê e atualiza** (avança conforme consome caracteres) |
| `simbolo_lido` | **escreve** o código do token reconhecido |
| `lexema` | **escreve** o texto do token reconhecido |

O parser chama `obtenha_simbolo()` (direta ou indiretamente, via `consumir()`), e depois consulta `simbolo_lido` e `lexema`.

### Funções do arquivo

| Função | Papel |
|---|---|
| `palavra_reservada()` | Decide se uma palavra é reservada ou identificador |
| `erro_lexico()` | Imprime o erro léxico e encerra o programa |
| `obtenha_simbolo()` | **Função principal.** Reconhece o próximo token |

---

## 2. Includes

```c
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <string.h>   // strcmp, strcpy
#include <ctype.h>    // isalpha, isdigit, isalnum
#include "define.h"   // tokens, variáveis globais, protótipos
```

---

## 3. `palavra_reservada()`

```c
int palavra_reservada(char palavra[])
```

**Entrada:** uma palavra já lida (ex.: `"while"`, `"soma"`).
**Saída:** o código do token. Se for reservada, o código dela. Senão, `IDENTIFICADOR`.

A lógica é uma sequência de comparações com `strcmp`, que devolve `0` quando as strings são iguais:

```c
if(strcmp(palavra, "program") == 0){
    return PROGRAM;
}
```

As 19 palavras tratadas:

| Grupo | Palavras |
|---|---|
| Estrutura | `program`, `var`, `begin`, `end` |
| Controle | `if`, `then`, `else`, `while`, `do`, `repeat`, `until` |
| Tipos | `integer`, `real`, `char` |
| E/S | `write` |
| Operadores | `div`, `and`, `or`, `not` |

### Por que o lexer lê a palavra inteira antes de decidir?

Porque `if` e `iffy` começam igual. Se o lexer decidisse pelas primeiras letras, confundiria `iffy` com `if`. Por isso ele lê **a palavra completa** e só depois consulta essa função.

### Sensibilidade a maiúsculas

`strcmp` diferencia maiúsculas de minúsculas, então `Begin` **não** casa com `"begin"` e vira `IDENTIFICADOR`, como o enunciado exige.

---

## 4. `erro_lexico()`

```c
void erro_lexico(char caractere){
    printf("Erro lexico no caracter [%c]\n",caractere);
    exit(1);
}
```

- Imprime a mensagem com o caractere problemático entre colchetes.
- `exit(1)` encerra o programa imediatamente, porque o lexer não consegue continuar de forma confiável. O código `1` indica que terminou com erro.

> **Atenção:** o enunciado pede o formato `Erro léxico no caracter [x]` (com acento em *léxico*). O código imprime `lexico` sem acento. Vale ajustar o texto.

---

## 5. `obtenha_simbolo()`

É o coração do lexer. Ela segue uma **ordem fixa de tentativas**: testa cada categoria de token, e na primeira que casar, reconhece, preenche `simbolo_lido` e `lexema` e retorna (`return`).

### 5.1 Visão geral da ordem

```
1. Pular brancos (espaço, \n, \t, \r)
2. Fim da entrada?            -> FIM
3. Começa com letra ou '_'?   -> identificador ou palavra reservada
4. Começa com dígito?         -> inteiro ou real
5. Começa com '.' + dígito?   -> real (ex.: .5)
6. Começa com aspa simples?   -> char literal
7. Operadores e símbolos simples (+ - * / =)
8. Operadores com lookahead (< <= <> > >= : :=)
9. Parênteses, vírgula, ponto e vírgula, ponto
10. Nada casou                -> erro léxico
```

A ordem importa. Por exemplo, o teste de `.5` (passo 5) vem **antes** do teste do símbolo `.` (passo 9); se fosse o contrário, `.5` seria lido como `.` seguido de `5`.

### 5.2 Passo 1: ignorar brancos

```c
while(entrada[posicao] == ' ' || entrada[posicao] == '\n' ||
      entrada[posicao] == '\t' || entrada[posicao] == '\r'){
    posicao += 1;
}
```

Avança `posicao` enquanto o caractere for espaço, quebra de linha, tabulação ou retorno de carro. Eles nunca viram token.

### 5.3 Passo 2: fim da entrada

```c
if(entrada[posicao] == '\0'){
    strcpy(lexema, "FIM");
    simbolo_lido = FIM;
    return;
}
```

Em C, strings terminam em `'\0'`. Quando o lexer chega nele, o código-fonte acabou e o token devolvido é `FIM`. O lexema `"FIM"` é só um texto para exibição.

### 5.4 Passo 3: identificadores e palavras reservadas

Regra do enunciado: `letra (letra | digito)*`, em que letra inclui `_`.

```c
if(isalpha(c) || c == '_'){
    i = 0;
    while(isalnum(c) || c == '_'){
        if(i < TAM_LEXEMA - 1){
            lexema[i] = c;
            i += 1;
        }
        posicao += 1;
    }
    lexema[i] = '\0';
    simbolo_lido = palavra_reservada(lexema);
    return;
}
```

(`c` representa `entrada[posicao]`, abreviado aqui para legibilidade.)

Como funciona:

1. O **primeiro** caractere precisa ser letra ou `_` (não pode ser dígito).
2. O laço consome letras, dígitos e `_` enquanto houver, copiando para `lexema`.
3. `lexema[i] = '\0'` fecha a string.
4. `palavra_reservada(lexema)` decide entre reservada e `IDENTIFICADOR`.

**Proteção contra estouro:** `if(i < TAM_LEXEMA - 1)` impede gravar além do vetor. Se o nome tiver mais de 99 caracteres, o excedente é lido (a `posicao` avança) mas descartado.

**Por que `(unsigned char)`?** As funções de `ctype.h` têm comportamento indefinido para valores negativos. Caracteres acentuados podem ser negativos em `char`, então o cast evita problemas.

### 5.5 Passo 4: números inteiros e reais

Regras do enunciado: inteiro = `digito+`; real = `digito* . digito+`.

**Parte 1: lê os dígitos iniciais** e copia para `lexema`.

**Parte 2: decide se é real**, olhando um caractere à frente:

```c
if(entrada[posicao] == '.' && isdigit(entrada[posicao + 1])){
    // copia o '.' e os dígitos seguintes
    simbolo_lido = REAL_LITERAL;
}else{
    simbolo_lido = INTEIRO_LITERAL;
}
```

A condição exige **ponto E dígito logo depois**. Isso resolve dois casos:

| Entrada | Resultado |
|---|---|
| `25.5` | `REAL_LITERAL` (`25.5`) |
| `25` | `INTEIRO_LITERAL` |
| `5.` | `INTEIRO_LITERAL` (`5`), depois `PONTO` |
| `end.` | não afeta: `end` é palavra e o `.` vira `PONTO` |

Esse segundo teste (`5.` não vira real) é importante e deve ficar assim, porque `digito*.digito+` exige ao menos um dígito depois do ponto.

### 5.6 Passo 5: real começando por ponto

```c
if(entrada[posicao] == '.' && isdigit(entrada[posicao + 1])){
    lexema[0] = '.';
    // lê os dígitos seguintes
    simbolo_lido = REAL_LITERAL;
}
```

Cobre o caso do prefixo vazio em `digito*.digito+`: `.5`, `.25`. Sem esse passo, `.5` viraria `PONTO` + `INTEIRO_LITERAL`.

### 5.7 Passo 6: caractere literal

Regra do enunciado: `'(letra | digito | \n | \t)'`.

O lexer segue um pequeno **roteiro**:

```
' (abre) -> um caractere (ou \n / \t) -> ' (fecha)
```

```c
if(entrada[posicao] == '\''){
    // guarda a aspa de abertura

    if(entrada[posicao] == '\\'){          // sequência de escape
        // guarda '\'
        if(entrada[posicao] != 'n' && entrada[posicao] != 't'){
            erro_lexico(...);              // só \n e \t são válidos
        }
        // guarda 'n' ou 't'
    }else{                                 // caractere normal
        if(!isalpha(...) && !isdigit(...)){
            erro_lexico(...);              // só letra ou dígito
        }
        // guarda o caractere
    }

    if(entrada[posicao] != '\''){          // aspa de fechamento obrigatória
        erro_lexico(...);
    }
    // guarda a aspa de fechamento
    simbolo_lido = CHAR_LITERAL;
}
```

Exemplos:

| Fonte | Resultado |
|---|---|
| `'a'` | `CHAR_LITERAL` |
| `'7'` | `CHAR_LITERAL` |
| `'\n'` | `CHAR_LITERAL` (são **2 caracteres** no fonte: `\` e `n`) |
| `'\x'` | erro léxico no `x` |
| `'ab'` | erro léxico no `b` (esperava a aspa de fechamento) |
| `'+'` | erro léxico no `+` (não é letra nem dígito) |

O lexema guardado inclui as aspas, por exemplo `'a'` ou `'\n'`.

### 5.8 Passo 7: operadores de um caractere

`+`, `-`, `*`, `/`, `=` seguem sempre o mesmo padrão:

```c
if(entrada[posicao] == '+'){
    strcpy(lexema, "+");
    simbolo_lido = MAIS;
    posicao += 1;
    return;
}
```

Copia o lexema, define o token, avança **1** posição e retorna.

### 5.9 Passo 8: operadores com lookahead

*Lookahead* é espiar o caractere seguinte, **sem consumi-lo**, para decidir o token. Ele é necessário porque alguns tokens começam igual.

| Começa com | Se o próximo for | Token | Avança |
|---|---|---|---|
| `<` | `=` | `<=` (`MENOR_IGUAL`) | 2 |
| `<` | `>` | `<>` (`DIFERENTE`) | 2 |
| `<` | outro | `<` (`MENOR`) | 1 |
| `>` | `=` | `>=` (`MAIOR_IGUAL`) | 2 |
| `>` | outro | `>` (`MAIOR`) | 1 |
| `:` | `=` | `:=` (`ATRIBUICAO`) | 2 |
| `:` | outro | `:` (`DOIS_PONTOS`) | 1 |

Isso segue o princípio da **maior correspondência**: o lexer sempre tenta reconhecer o token mais longo possível. Assim, `a<=b` vira `a`, `<=`, `b`, e não `a`, `<`, `=`, `b`.

### 5.10 Passo 9: símbolos especiais simples

`(`, `)`, `,`, `;` e `.` seguem o mesmo padrão dos operadores de um caractere. O `.` só chega aqui se **não** for seguido de dígito (senão já teria sido tratado como real no passo 5).

### 5.11 Passo 10: caractere desconhecido

```c
erro_lexico(entrada[posicao]);
```

Se nenhum `if` casou, o caractere não inicia nenhum token válido (por exemplo `@`, `#`, `$`, `&`). É o erro léxico exigido pelo enunciado.

---

## 6. Exemplo completo passo a passo

Entrada: `x := 3.5 + y;`

| Chamada | Caractere inicial | Passo acionado | `simbolo_lido` | `lexema` |
|---|---|---|---|---|
| 1 | `x` | 3 (identificador) | `IDENTIFICADOR` | `x` |
| 2 | `:` | 8 (lookahead `=`) | `ATRIBUICAO` | `:=` |
| 3 | `3` | 4 (número, vê `.5`) | `REAL_LITERAL` | `3.5` |
| 4 | `+` | 7 | `MAIS` | `+` |
| 5 | `y` | 3 | `IDENTIFICADOR` | `y` |
| 6 | `;` | 9 | `PONTO_VIRGULA` | `;` |
| 7 | fim | 2 | `FIM` | `FIM` |

---

## 7. Pontos de atenção e melhorias

1. **Comentários `//` não são tratados.** Os exemplos do enunciado têm `//número a ser testado`. Hoje o `/` vira `DIV_REAL` e o resto do comentário vira identificadores, causando erro sintático. Correção: no passo 1, ignorar tudo até o fim da linha quando aparecer `//`:

   ```c
   for(;;){
       while(entrada[posicao] == ' ' || entrada[posicao] == '\n' ||
             entrada[posicao] == '\t' || entrada[posicao] == '\r'){
           posicao += 1;
       }
       if(entrada[posicao] == '/' && entrada[posicao + 1] == '/'){
           while(entrada[posicao] != '\n' && entrada[posicao] != '\0'){
               posicao += 1;
           }
       }else{
           break;
       }
   }
   ```

2. **Acento na mensagem de erro.** Trocar `Erro lexico` por `Erro léxico`, conforme o enunciado.

3. **`'_'` como char literal.** O enunciado define *letra* como `[a-zA-Z_]`, então `'_'` deveria ser válido. Hoje só `isalpha` e `isdigit` são aceitos. Basta acrescentar `&& entrada[posicao] != '_'` na condição de erro.

4. **Erro léxico no fim da entrada.** Se o arquivo terminar no meio de um char literal (ex.: `'a`), o lexer chama `erro_lexico('\0')`, que imprime um caractere nulo. Um tratamento especial para `'\0'` deixaria a mensagem mais clara.

5. **Lexema truncado.** Identificadores acima de 99 caracteres são cortados em silêncio. Para um projeto acadêmico isso é aceitável, mas vale mencionar.

6. **Parada no primeiro erro.** Por usar `exit(1)`, o lexer não se recupera: o primeiro erro encerra a análise. Isso atende ao que o enunciado pede (emitir a mensagem).

---

## 8. Como testar apenas o lexer

Um modo de depuração que lista os tokens ajuda a demonstrar o lexer separado do parser. Em `main.c`, depois de carregar `entrada`:

```c
do{
    obtenha_simbolo();
    printf("token=%d  lexema=[%s]\n", simbolo_lido, lexema);
}while(simbolo_lido != FIM);
```

Entradas boas para testar:

| Entrada | O que verifica |
|---|---|
| `a<>b a<=b a>=b` | operadores com lookahead |
| `x:=.5` | real iniciado por ponto e `:=` colado |
| `5.` | inteiro seguido de `PONTO` |
| `Begin begin` | identificador vs. palavra reservada |
| `'a' '\n' '\t'` | char literais válidos |
| `'ab'` | erro léxico no `b` |
| `3 @ 4` | erro léxico no `@` |
