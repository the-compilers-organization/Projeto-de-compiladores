# Guia de implementação — Etapa 2 do compilador MicroPascal

## 1. Objetivo

Implementar **a tabela de símbolos por tabela hash** e **a análise semântica** no projeto `micropascal-compiler/` presente no arquivo `Projeto-de-compiladores(4).zip`. A implementação deve seguir os comentários `TODO` existentes nos arquivos-fonte. **Não construir AST.**

Este documento descreve **o que deve ser implementado neste projeto**.

---

## 2. Arquivos disponíveis e ponto de partida

| Caminho relativo a `micropascal-compiler/` | Situação e uso |
|---|---|
| `include/tabela_simbolos.h` | **Já definido**: `TAM_TABELA`, `TAM_NOME`, `TipoDado`, `Simbolo`, `TabelaSimbolos` e protótipos. |
| `include/semantico.h` | **Já definido**: protótipos de `semantico_binaria`, `semantico_unaria`, `semantico_atribuicao` e `semantico_condicao`. |
| `src/tabela_simbolos.c` | Funções existentes com **TODO e roteiro detalhado**; implementar seus corpos. |
| `src/semantico.c` | Funções existentes com **TODO e regras detalhadas**; implementar seus corpos. |
| `src/parser.c` | Analisador sintático existente, com **TODO e roteiro detalhado** para integração semântica. |
| `include/parser.h` | Estrutura `Parser` ainda contém `Lexer *lexer` e `Token token_atual`; adicionar gerenciamento da tabela e ajustar a API quando necessário. |
| `src/main.c` | Já abre o arquivo, inicializa o Lexer e chama o Parser; completar a finalização/integração indicada nos comentários. |

**Regra de trabalho:** antes de alterar uma função, ler integralmente seu comentário no respectivo `.c`. Manter as assinaturas públicas dos cabeçalhos existentes, salvo alteração acordada para a integração do Parser. Não duplicar as definições de `TipoDado` nem de `TabelaSimbolos`.

---

## 3. Distribuição de atividades entre os seis integrantes

| Integrante | Função ou trecho e arquivo | Atividade |
|---|---|---|
| **1** | `hash_nome()` — `src/tabela_simbolos.c`; `tabela_inicializar()` — `src/tabela_simbolos.c`; `tabela_buscar()` — `src/tabela_simbolos.c`; `tabela_inserir()` — `src/tabela_simbolos.c` | Implementar índice hash, inicialização, busca por lista encadeada, inserção, colisões e duplicidade. |
| **2** | `tabela_liberar()` — `src/tabela_simbolos.c`; `tipo_nome()` — `src/tabela_simbolos.c`; `declaracao()` — `src/parser.c` | Liberar memória, converter tipos para texto e registrar declarações de variáveis na tabela. |
| **3** | `numerico()` — `src/semantico.c`; `semantico_binaria()` — `src/semantico.c`; integração com os operadores binários — `src/parser.c`, nas funções `termo()`, `soma()`, `relacional()` e `expressao()` (sob responsabilidade do integrante 5) | Implementar regras dos operadores binários e orientar o integrante 5 sobre o uso de `semantico_binaria()`, sem editar as funções atribuídas a ele. |
| **4** | `semantico_unaria()` — `src/semantico.c`; `semantico_atribuicao()` — `src/semantico.c`; `semantico_condicao()` — `src/semantico.c`; `fator()` — `src/parser.c` | Implementar regras unárias, atribuição, condições e identificação dos tipos em fatores. |
| **5** | `termo()` — `src/parser.c`; `soma()` — `src/parser.c`; `relacional()` — `src/parser.c`; `expressao()` — `src/parser.c`; declaração antecipada de `expressao()` — `src/parser.c` | Propagar `TipoDado` nas expressões, chamar `semantico_binaria()` e preservar precedência e associatividade. |
| **6** | `comando()` — `src/parser.c`; `parser_inicializar()` — `src/parser.c`; `parser_analisar_programa()` — `src/parser.c`; tratamento de erros — `src/parser.c`; `parser_finalizar()` — **a implementar em** `src/parser.c`; estrutura e protótipos — `include/parser.h`; chamada de finalização — `src/main.c`, após `parser_analisar_programa()` | Integrar os comandos à semântica, inicializar e liberar a tabela de símbolos, tratar erros e ajustar a interface do Parser. |

> **Como localizar:** procure o nome de cada função no arquivo indicado. `parser_finalizar()` deverá ser criada em `src/parser.c`.

---

## 4. Regras e contratos que devem ser respeitados

### 4.1. Tabela hash — `src/tabela_simbolos.c`

- Usar `TAM_TABELA = 211` e `TAM_NOME = 256`, já definidos em `include/tabela_simbolos.h`.
- Cada posição de `TabelaSimbolos.vetor` aponta para uma lista encadeada de `Simbolo`.
- `hash_nome()`: acumular códigos de caracteres e reduzir por `% TAM_TABELA`, conforme o `TODO`.
- `tabela_inicializar()`: preencher todas as posições com `NULL`.
- `tabela_buscar()`: calcular o índice, percorrer a lista e comparar nomes completos com `strcmp`; retornar `NULL` se não encontrar.
- `tabela_inserir()`: validar nome/tipo, impedir nomes repetidos, alocar `Simbolo`, armazenar nome/tipo/linha e encadear na posição calculada. **Retornos definidos no cabeçalho:** `1` = inserido, `0` = duplicado, `-1` = erro de dados ou memória.
- `tabela_liberar()`: liberar todos os nós de todas as listas, sem liberar a própria estrutura quando ela não foi alocada dinamicamente.
- `tipo_nome()`: converter os valores de `TipoDado` em textos legíveis.
- **Colisão não é duplicidade:** nomes diferentes podem ter o mesmo índice; apenas nomes iguais representam declaração duplicada.
- Declarações aceitas: `integer`, `real`, `char`. `boolean` é tipo interno, não declarável.

### 4.2. Regras de tipos — `src/semantico.c`

| Operação | Tipos aceitos | Resultado |
|---|---|---|
| `+` e `-` | `integer`/`integer`; `real`/`real` | Mesmo tipo numérico |
| `+` e `-` | `char` à esquerda e `integer` à direita | `char` |
| `*` | `integer`/`integer`; `real`/`real` | Mesmo tipo numérico |
| `div` | `integer`/`integer` | `integer` |
| `/` | `real`/`real` | `real` |
| `<`, `<=`, `>`, `>=` | Dois `integer` ou dois `real` | `boolean` |
| `=`, `<>` | Dois operandos do mesmo tipo, incluindo `char` e `boolean` | `boolean` |
| `and`, `or` | `boolean`/`boolean` | `boolean` |
| Menos unário | `integer` ou `real` | Mesmo tipo |
| `not` | `boolean` | `boolean` |

- `integer + char` e `integer - char` são **inválidos**; a ordem de `char +/- integer` importa.
- Não há conversão implícita entre `integer` e `real`.
- `semantico_binaria()` e `semantico_unaria()` retornam `TIPO_INVALIDO` para combinações proibidas.
- `semantico_atribuicao()`: retorna `1` apenas para tipos válidos e iguais; caso contrário, `0`.
- `semantico_condicao()`: retorna `1` apenas para `TIPO_BOOLEANO`; caso contrário, `0`.
- A regra de `not` consta como decisão de consistência nos comentários do projeto; não foi explicitada no enunciado citado nesses comentários.

### 4.3. Integração no Parser — `src/parser.c`

- Alterar `fator()`, `termo()`, `soma()`, `relacional()` e `expressao()` para **retornar `TipoDado`**, incluindo a declaração antecipada de `expressao()`.
- Em `fator()`, retornar tipos dos literais, buscar identificadores na tabela, propagar o tipo de parênteses e validar operadores unários.
- Em `termo()`, `soma()`, `relacional()` e `expressao()`, guardar o operador **antes** de `avancar()`, combinar os tipos com `semantico_binaria()` e rejeitar `TIPO_INVALIDO`.
- Em `declaracao()`, guardar os nomes e as linhas antes de ler o tipo em declarações como `a, b : integer;`, inserir cada símbolo e interpretar os retornos `1`, `0`, `-1`.
- Em `comando()`, buscar a variável de destino, verificar a atribuição com `semantico_atribuicao()` e exigir condição booleana em `if`, `while` e `repeat/until` por `semantico_condicao()`.
- Em `write`, analisar a expressão e verificar os identificadores, sem impor um tipo de saída não especificado.
- Manter `avancar()`, `esperar()`, `em()` e a estrutura sintática como mecanismos de reconhecimento; não misturar erros sintáticos e semânticos.
- A gramática existente declara variáveis na seção `var`; não criar escopos adicionais por `begin/end` sem requisito.

### 4.4. Interface e ciclo de vida — `include/parser.h` e `src/main.c`

- Integrar `TabelaSimbolos` ao estado do Parser de forma consistente, sem recriar a estrutura já definida em `include/tabela_simbolos.h`.
- Inicializar a tabela **antes** de registrar ou consultar variáveis.
- Garantir liberação dos nós da tabela ao terminar a análise, definindo a rotina de finalização e seu protótipo conforme a integração.
- Atualizar `main.c` para chamar a finalização adotada, preservando a abertura do arquivo, o Lexer e o fechamento do arquivo.
- O Parser atualmente usa `exit()` em erros: definir um tratamento coerente para que erros semânticos sejam identificados corretamente e a limpeza de recursos seja considerada.

## 5. Coordenação entre integrantes

1. **Integrantes 1 e 2:** combinar o uso da tabela hash antes de integrar `declaracao()`; não alterar simultaneamente as mesmas funções de `src/tabela_simbolos.c`.
2. **Integrantes 3, 4 e 5:** utilizar exatamente os `TipoToken` e `TipoDado` dos cabeçalhos; o integrante 5 chama as regras implementadas pelos integrantes 3 e 4.
3. **Integrantes 2, 4, 5 e 6:** combinar as alterações em `src/parser.c` para evitar conflitos; cada integrante edita apenas as funções atribuídas.
4. **Integrante 6:** coordenar as alterações de `include/parser.h` e `src/main.c`, informando à equipe como acessar a tabela dentro de `Parser`.
5. **Todos:** manter os comentários de orientação úteis, não alterar desnecessariamente o Lexer, não criar AST e não substituir os protótipos existentes por APIs incompatíveis.

## 6. Critérios de conclusão da etapa

- A tabela hash registra e consulta símbolos com listas encadeadas, detecta nomes duplicados e libera a memória alocada.
- O Parser identifica o uso de variáveis não declaradas e recupera seus tipos.
- As expressões retornam `TipoDado` e aplicam as regras de operadores de `semantico.c`.
- Atribuições e condições de comandos passam pelas verificações semânticas previstas.
- Os erros semânticos são diferenciados dos sintáticos.
- A integração funciona com as interfaces existentes e com as atualizações necessárias de `parser.h` e `main.c`, **sem AST**.

