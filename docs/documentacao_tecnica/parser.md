# parser.c: Analisador sintático do micro-Pascal

Este documento explica o arquivo `parser.c`, a **segunda etapa** do compilador micro-Pascal.

## 1. Qual é o papel do parser?

O lexer entrega **tokens**. O parser verifica se esses tokens aparecem **numa ordem permitida pela gramática**.

```
Lexer:   IDENTIFICADOR  ATRIBUICAO  INTEIRO_LITERAL  PONTO_VIRGULA
Parser:  "isso forma uma <atribuicao> válida?"  -> sim
```

Exemplo da diferença entre as duas etapas:

| Código | Lexer | Parser |
|---|---|---|
| `x := 5;` | tokens corretos | válido |
| `x 5 := ;` | tokens corretos | **erro de sintaxe** |
| `x := 5 @;` | **erro léxico** no `@` | nem chega a analisar |

O parser deste projeto **só valida** (diz "válido" ou "erro"). Ele não monta árvore nem gera código; isso fica para a próxima parte da disciplina.

---

## 2. A técnica: descida recursiva

O parser usa **análise descendente recursiva** (*recursive descent*):

> **Para cada símbolo não terminal da gramática, existe uma função.**
> O corpo da função segue exatamente a regra da gramática.

Dentro de cada função, duas ferramentas básicas:

- **`consumir(T)`**: "aqui eu **espero** o token `T`; se estiver lá, avança; se não, erro".
- **Olhar `simbolo_lido`**: "qual é o token atual?", para **decidir qual caminho** seguir.

Como o parser enxerga apenas **um token à frente** (`simbolo_lido`) e nunca volta atrás, ele é do tipo **LL(1)**.

### Mapa: gramática → funções

```
programa
 ├── secao_var
 │    └── decl_var
 │         └── tipo
 └── bloco
      └── lista_comandos
           └── comando
                ├── bloco            (recursão)
                ├── atribuicao  ──┐
                ├── iteracao    ──┤
                ├── decisao     ──┼──> expressao
                └── escrita     ──┘       └── expr_logica
                                               └── expr_relacional
                                                    └── expr_aditiva
                                                         └── expr_multiplicativa
                                                              └── expr_basica
                                                                   └── expressao (recursão, via parênteses)
```

---

## 3. Funções de apoio

### 3.1 `erro_sintatico()`

```c
void erro_sintatico(void){
    printf("Erro de sintaxe no token [%s]\n", lexema);
    exit(1);
}
```

- Imprime o erro no formato exigido: `Erro de sintaxe no token [lexema]`.
- O `lexema` é o **texto do token atual**, ou seja, justamente o token que o parser **não esperava**.
- `exit(1)` encerra: o parser **para no primeiro erro**.

Exemplo: em `n := 3 n := 4;`, depois do `3` o parser esperava `;` mas viu o `n`. Mensagem: `Erro de sintaxe no token [n]`.

> Se o erro acontecer com a entrada acabada (ex.: arquivo cortado), o lexema será `FIM`: `Erro de sintaxe no token [FIM]`.

### 3.2 `consumir()`

```c
void consumir(int token){
    if(simbolo_lido == token){
        obtenha_simbolo();
    }else{
        erro_sintatico();
    }
}
```

É a **única função que chama o lexer**. Ela faz duas coisas:

1. Confere se o token atual é o esperado.
2. Se for, **avança** pedindo o próximo token (`obtenha_simbolo()`).

Todo o parser se resume a combinar chamadas de `consumir()` com decisões baseadas em `simbolo_lido`.

> Antes de `programa()` ser chamada, o `main.c` já fez a primeira chamada a `obtenha_simbolo()`. Assim, `simbolo_lido` sempre contém o token **atual, ainda não consumido**.

---

## 4. Estrutura do programa

### 4.1 `programa()`

Gramática: `program ID ; <secao_var> <bloco> .`

```c
void programa(void){
    consumir(PROGRAM);
    consumir(IDENTIFICADOR);
    consumir(PONTO_VIRGULA);
    secao_var();
    bloco();
    consumir(PONTO);
}
```

Uma linha para cada elemento da regra, na mesma ordem. Depois do `PONTO` final, o `main.c` verifica se o próximo token é `FIM` (nada de tokens sobrando).

### 4.2 `secao_var()`

Gramática: `var { <decl_var> }*`

```c
void secao_var(void){
    consumir(VAR);
    while(simbolo_lido == IDENTIFICADOR){
        decl_var();
    }
}
```

- `consumir(VAR)` exige a palavra `var`.
- O `while` implementa o `{ ... }*` ("zero ou mais"): enquanto o token atual for um identificador, ainda há outra declaração.
- **Como o parser sabe que a seção acabou?** Porque a próxima coisa é `begin`, que não é identificador, e o laço para.

### 4.3 `decl_var()`

Gramática: `ID { , ID }* : <tipo> ;`

```c
void decl_var(void){
    consumir(IDENTIFICADOR);
    while(simbolo_lido == VIRGULA){
        consumir(VIRGULA);
        consumir(IDENTIFICADOR);
    }
    consumir(DOIS_PONTOS);
    tipo();
    consumir(PONTO_VIRGULA);
}
```

Cobre `n : integer;` e também `i, j, soma : integer;`. O laço consome pares `, ID` enquanto houver vírgula.

### 4.4 `tipo()`

Gramática: `integer | real | char`

```c
if(simbolo_lido == INTEGER)       consumir(INTEGER);
else if(simbolo_lido == REAL)     consumir(REAL);
else if(simbolo_lido == CHAR)     consumir(CHAR);
else                              erro_sintatico();
```

Exemplo de **escolha entre alternativas**: o token atual decide qual `consumir` usar. Se nenhuma alternativa serve, é erro.

---

## 5. Blocos e comandos

### 5.1 `bloco()`

Gramática: `begin <lista_comandos> end`

```c
consumir(BEGIN_TOKEN);
lista_comandos();
consumir(END_TOKEN);
```

### 5.2 `lista_comandos()`

Gramática: `{ <comando> }*`

```c
while(simbolo_lido == BEGIN_TOKEN || simbolo_lido == IDENTIFICADOR ||
      simbolo_lido == WHILE || simbolo_lido == REPEAT ||
      simbolo_lido == IF || simbolo_lido == WRITE){
    comando();
}
```

A condição do `while` lista **todos os tokens que podem iniciar um comando** (o conjunto *FIRST* de `<comando>`):

| Token inicial | Comando |
|---|---|
| `begin` | bloco |
| IDENTIFICADOR | atribuição |
| `while`, `repeat` | iteração |
| `if` | decisão |
| `write` | escrita |

Se o token atual **não** está nessa lista (por exemplo `end`), a lista termina e quem chamou continua. É assim que o parser sabe onde o bloco acaba.

### 5.3 `comando()`

É um **despachante**: olha o primeiro token e delega.

```c
if(simbolo_lido == BEGIN_TOKEN){
    bloco();
    consumir(PONTO_VIRGULA);   // <bloco> ;  (na gramática)
}else if(simbolo_lido == IDENTIFICADOR){
    atribuicao();
}else if(simbolo_lido == WHILE || simbolo_lido == REPEAT){
    iteracao();
}else if(simbolo_lido == IF){
    decisao();
}else if(simbolo_lido == WRITE){
    escrita();
}else{
    erro_sintatico();
}
```

**Regra do ponto e vírgula** (importante para a apresentação):

| Situação | Quem consome o `;` |
|---|---|
| Atribuição | `atribuicao()` |
| Escrita | `escrita()` |
| `repeat ... until expr` | `iteracao()` |
| Bloco **interno** | `comando()` |
| Bloco **principal** do programa | **ninguém**; em vez de `;`, vem o `.` (`programa()`) |

`while` e `if` **não têm `;` próprio**: o `;` pertence ao comando interno (por exemplo, a atribuição dentro do `while`).

### 5.4 `atribuicao()`

Gramática: `ID := <expressao> ;`

```c
consumir(IDENTIFICADOR);
consumir(ATRIBUICAO);
expressao();
consumir(PONTO_VIRGULA);
```

### 5.5 `iteracao()`

Duas alternativas, escolhidas pelo token atual.

**`while <expressao> do <comando>`**
```c
consumir(WHILE);
expressao();
consumir(DO);
comando();
```

**`repeat <comando> until <expressao> ;`**
```c
consumir(REPEAT);
comando();
consumir(UNTIL);
expressao();
consumir(PONTO_VIRGULA);
```

Observação: o corpo do `repeat` é **um único `<comando>`**, como na gramática do enunciado. Para vários comandos, use um bloco: `repeat begin ... end; until ...;`.

### 5.6 `decisao()`

Gramática: `if <expr> then <comando> [ else <comando> ]`

```c
consumir(IF);
expressao();
consumir(THEN);
comando();
if(simbolo_lido == ELSE){
    consumir(ELSE);
    comando();
}
```

O `else` é opcional: se depois do primeiro `comando()` o token atual for `else`, consome e lê o segundo comando.

**Ambiguidade do *dangling else* (else pendente).** Em

```pascal
if a then if b then x := 1; else x := 2;
```

a quem pertence o `else`? A resposta do código é automática: o `if` **interno** é processado primeiro, e ele é o primeiro a verificar `simbolo_lido == ELSE`. Logo, **o `else` casa com o `if` mais próximo**, que é a convenção padrão do Pascal e do C.

**Sobre `write('p'); else ...` (Exemplo 1 do enunciado).** O `;` pertence à `escrita()`, portanto, quando `decisao()` olha o próximo token depois do `comando()`, ele já é o `else`. Funciona sem tratamento especial. Pelo mesmo motivo, `if x then begin ... end; else ...` também funciona, pois o `;` é consumido por `comando()` ao final do bloco.

### 5.7 `escrita()`

Gramática: `write ( <expressao> ) ;`

```c
consumir(WRITE);
consumir(ABRE_PAR);
expressao();
consumir(FECHA_PAR);
consumir(PONTO_VIRGULA);
```

---

## 6. Expressões

### 6.1 O problema da gramática abstrata

```
<expressao> ::= <expressao> + <expressao> | <expressao> * <expressao> | ... | <expr_basica>
```

Dois defeitos que impedem o uso direto:

1. **Ambiguidade**: `2 + 3 * 4` poderia ser `(2+3)*4` ou `2+(3*4)`; a gramática não diz qual.
2. **Recursão à esquerda**: `<expressao>` começa chamando `<expressao>`. Em descida recursiva isso geraria **recursão infinita**.

### 6.2 A solução: níveis de precedência + laços

O enunciado define quatro níveis de precedência, **todos associativos à esquerda**. Cada nível vira **uma função**, e a recursão à esquerda é trocada por um **laço `while`**.

Forma geral:

```
nivel ::= proximo_nivel { operador proximo_nivel }*
```

```c
void nivel(void){
    proximo_nivel();
    while(simbolo_lido == OPERADOR){
        consumir(OPERADOR);
        proximo_nivel();
    }
}
```

Por que isso dá **associatividade à esquerda**? Com `a - b - c`:

1. lê `a`;
2. vê `-`, lê `b` → agrupa `(a - b)`;
3. vê `-` de novo, lê `c` → agrupa `((a - b) - c)`.

Cada volta do laço incorpora o novo operando ao que já foi lido, o que é agrupar da esquerda para a direita.

### 6.3 Os níveis (do menor para o maior)

| Função | Operadores | Precedência | Chama |
|---|---|---|---|
| `expressao()` | (só entrada) | n/a | `expr_logica` |
| `expr_logica()` | `or`, `and` | 4 (a menor) | `expr_relacional` |
| `expr_relacional()` | `=` `<>` `<` `>` `<=` `>=` | 3 | `expr_aditiva` |
| `expr_aditiva()` | `+` `-` | 2 | `expr_multiplicativa` |
| `expr_multiplicativa()` | `*` `/` `div` | 1 (a maior) | `expr_basica` |
| `expr_basica()` | `( )`, `not`, literais, ID | base | n/a |

**Regra de ouro:** quanto mais **fundo** na cadeia de chamadas, **mais forte** é a precedência (o operador é resolvido primeiro).

### 6.4 Exemplo: `2 + 3 * 4`

```
expr_aditiva
 ├─ expr_multiplicativa  -> lê 2
 ├─ vê '+', consome
 └─ expr_multiplicativa  -> lê 3, vê '*', consome, lê 4   (3*4 resolvido aqui dentro)
```

Resultado: `2 + (3 * 4)`. A multiplicação foi resolvida em um nível mais profundo.

### 6.5 `and` e `or` no mesmo nível

O enunciado coloca `or` e `and` juntos no **nível 4**. Então `a or b and c` é agrupado da esquerda: `(a or b) and c`. Isso é diferente do Pascal padrão (onde `and` é mais forte que `or`), mas segue o que foi pedido.

### 6.6 `expr_basica()`: o fundo da cadeia

```c
if(simbolo_lido == ABRE_PAR){
    consumir(ABRE_PAR); expressao(); consumir(FECHA_PAR);
}else if(simbolo_lido == NOT){
    consumir(NOT); expressao();
}else if(INTEIRO_LITERAL) ... else if(REAL_LITERAL) ... else if(CHAR_LITERAL) ... else if(IDENTIFICADOR) ...
else erro_sintatico();
```

| Caso | Exemplo |
|---|---|
| Parênteses | `(x + 5)`; chama `expressao()` de novo e **reinicia a hierarquia**, por isso parênteses sobrescrevem a precedência |
| `not` | `not x` |
| Literal inteiro / real / char | `10`, `3.5`, `'a'` |
| Identificador | `soma` |

É aqui que a **recursão indireta** acontece: `expressao → ... → expr_basica → expressao`. Isso permite expressões aninhadas como `((a + b) * (c - d))`.

---

## 7. Exemplo completo: análise de um programa

```pascal
program P;
var n : integer;
begin
  n := 2 + 3;
end.
```

| Passo | Token atual | Função em execução | Ação |
|---|---|---|---|
| 1 | `program` | `programa` | `consumir(PROGRAM)` |
| 2 | `P` | `programa` | `consumir(IDENTIFICADOR)` |
| 3 | `;` | `programa` | `consumir(PONTO_VIRGULA)` |
| 4 | `var` | `secao_var` | `consumir(VAR)` |
| 5 | `n` | `decl_var` | `consumir(IDENTIFICADOR)` |
| 6 | `:` | `decl_var` | `consumir(DOIS_PONTOS)` |
| 7 | `integer` | `tipo` | `consumir(INTEGER)` |
| 8 | `;` | `decl_var` | `consumir(PONTO_VIRGULA)` |
| 9 | `begin` | `bloco` | `consumir(BEGIN_TOKEN)`; `begin` não é ID, então `secao_var` terminou antes |
| 10 | `n` | `comando` → `atribuicao` | `consumir(IDENTIFICADOR)` |
| 11 | `:=` | `atribuicao` | `consumir(ATRIBUICAO)` |
| 12 | `2` | `expressao` … `expr_basica` | `consumir(INTEIRO_LITERAL)` |
| 13 | `+` | `expr_aditiva` | `consumir(MAIS)` |
| 14 | `3` | `expr_basica` | `consumir(INTEIRO_LITERAL)` |
| 15 | `;` | `atribuicao` | `consumir(PONTO_VIRGULA)` |
| 16 | `end` | `lista_comandos` | `end` não inicia comando: laço termina |
| 17 | `end` | `bloco` | `consumir(END_TOKEN)` |
| 18 | `.` | `programa` | `consumir(PONTO)` |
| 19 | `FIM` | `main` | token final é `FIM`: **programa válido** |

---

## 8. Pontos de atenção

1. **`var` é obrigatório.** `secao_var()` faz `consumir(VAR)` incondicionalmente, então `program P; begin end.` é rejeitado. Em Pascal a seção é opcional. Correção:

   ```c
   void secao_var(void){
       if(simbolo_lido == VAR){
           consumir(VAR);
           while(simbolo_lido == IDENTIFICADOR){
               decl_var();
           }
       }
   }
   ```
   (Seguir a gramática ao pé da letra também é defensável, desde que seja uma escolha consciente.)

2. **`not` abrange a expressão inteira.** Em `expr_basica`, `not` chama `expressao()`. Assim `not a and b` é lido como `not (a and b)`. O mais usual é o `not` ser aplicado só ao operando seguinte:

   ```c
   consumir(NOT);
   expr_basica();
   ```
   A gramática abstrata diz `not <expressao>`, mas o enunciado manda adaptá-la. Registre a decisão no relatório.

3. **Sem menos unário.** `x := -5` é erro de sintaxe, pois `-` só existe como operador binário. Está de acordo com o enunciado.

4. **`repeat` aceita um único comando** (ver 5.5). Documentar como decisão do grupo.

5. **Para no primeiro erro.** Sem recuperação de erros (`exit(1)`), o que atende ao enunciado (emitir a mensagem).

6. **O `-` não aparece na gramática abstrata**, mas está nos tokens e na precedência. O parser o inclui em `expr_aditiva`, que é o correto.

---
