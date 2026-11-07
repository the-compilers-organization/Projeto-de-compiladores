# Guia de implementação — MicroPascal

**Segunda etapa:** tabela de símbolos por hash e análise semântica  
**Equipe:** 6 integrantes

## 1. Objetivo

Implementar as funções sinalizadas com `TODO` e integrar a tabela de símbolos à análise semântica do Parser descendente recursivo.

- **Não construir AST.**
- Preservar a análise léxica e a gramática existente.
- Os caminhos indicados são relativos à raiz `micropascal-compiler/`: implementações em `src/` e interfaces em `include/`.

## 2. Arquivos disponíveis e orientações de implementação

Os arquivos de cabeçalho **`include/tabela_simbolos.h` e `include/semantico.h` já estão implementados**. Eles definem os tipos, as estruturas e as assinaturas das funções que serão utilizadas na segunda etapa. Os integrantes devem consultar e respeitar essas interfaces; não é necessário recriá-las.

Os arquivos **`src/tabela_simbolos.c` e `src/semantico.c` já estão criados** e contêm comentários `TODO` com **orientações detalhadas para implementar cada função**. O arquivo **`src/parser.c` também contém orientações detalhadas**, junto à estrutura sintática existente, para integrar a tabela de símbolos e a análise semântica.

**Como utilizar os arquivos:** localizar a função atribuída na divisão abaixo, ler integralmente os comentários que a acompanham e implementar o comportamento solicitado, respeitando as declarações dos cabeçalhos. As instruções deste guia complementam os comentários presentes no código.

O arquivo `include/parser.h` deve ser atualizado apenas conforme as mudanças necessárias para integrar os tipos e a tabela de símbolos ao Parser.

## 3. Divisão de responsabilidades

A divisão considera a **complexidade estimada**, não apenas o número de funções. Cada integrante tem funções e arquivos identificados para evitar sobreposição.

### Integrante 1 — Estrutura e inserção na tabela hash

**Arquivo:** `src/tabela_simbolos.c`  
**Funções:** `hash_nome()`, `tabela_inicializar()`, `tabela_buscar()`, `tabela_inserir()`  
**Interface relacionada:** `include/tabela_simbolos.h`

**Implementar:** calcular o índice hash; inicializar o vetor; percorrer as listas encadeadas; validar nomes e tipos; impedir nomes duplicados; alocar e inserir símbolos. Uma colisão hash **não** significa duplicidade: comparar os nomes completos.

### Integrante 2 — Ciclo de vida da tabela e declarações

**Arquivos:** `src/tabela_simbolos.c`, `src/parser.c`  
**Funções:** `tabela_liberar()`, `tipo_nome()`, `declaracao()`  
**Interfaces relacionadas:** `include/tabela_simbolos.h`, `include/parser.h`

**Implementar:** liberar todos os nós de cada lista; converter tipos em textos; guardar temporariamente nomes e linhas de uma declaração até ler seu tipo; inserir cada identificador e interpretar os retornos de `tabela_inserir()` (`1`, `0`, `-1`).

### Integrante 3 — Operadores binários e aplicação

**Arquivos:** `src/semantico.c`, `src/parser.c`  
**Funções:** `numerico()`, `semantico_binaria()` e `aplicar_binaria()` **(auxiliar opcional, se adotada)**  
**Interface relacionada:** `include/semantico.h`

**Implementar:** regras de compatibilidade para `+`, `-`, `*`, `/`, `div`, comparações, `and` e `or`; retornar `TIPO_INVALIDO` para combinações proibidas. Se a equipe optar por `aplicar_binaria()`, centralizar nela a chamada à análise semântica e o tratamento de erro no Parser.

### Integrante 4 — Operadores unários, atribuição e fator

**Arquivos:** `src/semantico.c`, `src/parser.c`  
**Funções:** `semantico_unaria()`, `semantico_atribuicao()`, `semantico_condicao()`, `fator()` e `variavel_tipo()` **(auxiliar opcional, se adotada)**  
**Interfaces relacionadas:** `include/semantico.h`, `include/parser.h`

**Implementar:** `not` e menos unário; igualdade exata de tipos nas atribuições; exigência de condição booleana; recuperação de tipos de literais, identificadores e expressões entre parênteses. Consultar a tabela hash para identificadores e rejeitar variáveis não declaradas.

### Integrante 5 — Propagação de tipos nas expressões

**Arquivo:** `src/parser.c`  
**Funções:** `termo()`, `soma()`, `relacional()`, `expressao()`  
**Interface relacionada:** `include/parser.h`

**Implementar:** retornar `TipoDado`; preservar operadores antes de avançar tokens; combinar tipos via `semantico_binaria()` (ou auxiliar acordada); propagar o resultado de cada subexpressão, respeitando precedência e associatividade da gramática existente.

### Integrante 6 — Comandos, erros e ciclo de vida do Parser

**Arquivos:** `src/parser.c`, `include/parser.h`  
**Funções:** `comando()`, `erro_semantico()` **(a criar)**, `parser_inicializar()`, `parser_analisar_programa()`, `parser_finalizar()` **(a criar)**  
**Arquivo relacionado:** `src/main.c` (para chamada da finalização, se necessária)

**Implementar:** verificar atribuição, `if`, `while`, `repeat/until` e `write`; emitir mensagens de erro semântico com linha e motivo; inicializar e liberar a tabela; atualizar a estrutura `Parser` e a API de inicialização/finalização. **A integração final é responsabilidade de todos**, não apenas deste integrante.

## 4. Interfaces e regras de integração dos módulos

1. `fator()`, `termo()`, `soma()`, `relacional()` e `expressao()` devem retornar `TipoDado`. O Integrante 4 cuida de `fator()`; o Integrante 5, das demais funções.
2. A tabela hash usa **encadeamento separado**: cada posição do vetor aponta para uma lista de símbolos.
3. `tabela_inserir()` retorna `1` (sucesso), `0` (identificador duplicado) ou `-1` (dados inválidos ou falha de memória). O Integrante 2 usa esse contrato em `declaracao()`.
4. `semantico.c` e `tabela_simbolos.c` **não imprimem erros semânticos**: retornam tipos, ponteiros ou códigos. O Parser identifica e comunica o erro.
5. Respeitar as interfaces já implementadas em `include/tabela_simbolos.h` e `include/semantico.h`. Atualizar `include/parser.h` de maneira consistente com os retornos `TipoDado` e a integração da tabela de símbolos.
6. O nome do programa não é uma variável; `boolean` é um tipo interno e não pode ser declarado na seção `var`.
7. Não criar escopos adicionais em cada `begin/end` sem exigência do professor.
8. Preservar a sintaxe, a pontuação e o Lexer existente. **Não utilizar AST.**

## 5. Regras semânticas

| Categoria | Regra |
|---|---|
| Declarações | Aceitar `integer`, `real` e `char`; rejeitar nomes duplicados e uso de identificadores não declarados. |
| Adição e subtração | `integer +/- integer → integer`; `real +/- real → real`; `char +/- integer → char`. **A ordem importa:** `integer +/- char` é inválido. |
| Multiplicação | `integer * integer → integer`; `real * real → real`. |
| Divisão inteira | `integer div integer → integer`. |
| Divisão real | `real / real → real`. |
| Comparações de ordem | `<`, `<=`, `>` e `>=` exigem operandos numéricos do mesmo tipo; resultado `boolean`. |
| Igualdade | `=` e `<>` exigem tipos iguais, incluindo `char` e `boolean`; resultado `boolean`. |
| Operadores lógicos | `and` e `or` exigem dois booleanos; resultado `boolean`. |
| Operador `not` | `not boolean → boolean` (**confirmar com o professor caso não esteja explícito no enunciado**). |
| Menos unário | `-integer → integer`; `-real → real`. |
| Atribuição | Tipo do destino idêntico ao tipo da expressão; sem conversão implícita. |
| Condições | Expressões de `if`, `while` e `until` devem ser booleanas. |

**Tratamento de erro:** se uma operação retornar `TIPO_INVALIDO`, o Parser deve sinalizar erro semântico, indicando a linha e o motivo. A busca por identificador inexistente também deve produzir erro semântico.

## 6. Ordem de implementação e integração

1. **Consultar os arquivos existentes:** todos leem as interfaces já implementadas em `include/tabela_simbolos.h` e `include/semantico.h` e os comentários `TODO` das funções sob sua responsabilidade. O Integrante 6 coordena com a equipe as alterações necessárias em `include/parser.h`.
2. **Implementar os módulos:** Integrantes 1 e 2 trabalham em funções diferentes de `src/tabela_simbolos.c`; Integrantes 3 e 4, em funções diferentes de `src/semantico.c`.
3. **Integrar ao Parser:** Integrantes 2, 4, 5 e 6 alteram funções distintas de `src/parser.c`, usando commits separados e comunicando mudanças de assinatura.
4. **Consolidar coletivamente:** todos verificam se os `TODOs` foram implementados e se cabeçalhos, chamadas e retornos são compatíveis. Não concentrar a resolução de conflitos no Integrante 6.

> **Critério de equilíbrio:** a divisão busca aproximar o esforço de implementação entre os seis integrantes. Caso uma parte fique significativamente mais complexa do que o previsto, redistribuir apoio de implementação e revisão entre os integrantes.
