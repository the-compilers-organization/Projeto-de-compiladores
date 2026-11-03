<h1 align="center">Projeto de Compiladores — MicroPascal</h1>

Projeto acadêmico em **C11**, desenvolvido na disciplina de Construção de Compiladores da **UNICAP**, sob orientação do professor **Robson Lins**.

**Estado atual:** análise léxica, análise sintática descendente recursiva e **análise semântica com tabela de símbolos hash implementadas**. O projeto **não utiliza AST** e **não executa programas Pascal**. A geração de código intermediário ainda não foi implementada.

## 1. Objetivo e funcionamento

O compilador recebe o caminho de um arquivo `.pas`, identifica tokens, verifica a gramática e aplica as regras semânticas durante a análise sintática. Um programa aceito termina com código de saída `0`; um erro termina com código diferente de `0` e uma mensagem diagnóstica.

```text
Arquivo .pas → main.c → Lexer → Parser descendente recursivo
                                   ↕
                          Tabela de símbolos (hash)
                                   ↕
                           Regras semânticas
                          → aceitação ou erro
```

Não há menu interativo, entrada de código por `stdin`, geração de AST nem interpretação de `write`.

## 2. Estrutura do projeto

```text
micropascal-compiler/
├── include/
│   ├── token.h
│   ├── lexer.h
│   ├── parser.h
│   ├── tabela_simbolos.h
│   └── semantico.h
├── src/
│   ├── main.c
│   ├── lexer.c
│   ├── parser.c
│   ├── tabela_simbolos.c
│   └── semantico.c
├── testes/
│   ├── e1.pas, e2.pas, e3.pas, e5.pas
│   ├── exemplo1.pas, exemplo2.pas, teste_lexer.pas
│   └── test/
│       ├── 01_validos_lexicos_sintaticos/ (70 arquivos)
│       ├── 02_erros_sintaticos/ (76 arquivos)
│       ├── 03_erros_lexicos/ (30 arquivos)
│       ├── 04_semanticos/ (55 arquivos; 001 a 055)
│       ├── manifesto.csv
│       ├── executar_testes.c
│       ├── rodar_testes_completos.bat
│       ├── rodar_testes_completos.sh
│       └── README_test.md
├── Makefile
├── rodar_testes.bat
├── rodar_testes.sh
└── README.md
```

Os arquivos de exemplo em `testes/` não fazem parte dos 231 casos do manifesto. Relatórios `resultado_execucao_NNN.csv` são gerados durante a execução.

## 3. Responsabilidade dos módulos

| Arquivo | Responsabilidade |
|---|---|
| `token.h` | Tipos de token e estrutura `Token` |
| `lexer.h` / `lexer.c` | Reconhecer identificadores, literais, palavras reservadas, operadores e erros léxicos |
| `parser.h` / `parser.c` | Reconhecer programa, declarações, comandos e expressões; integrar as verificações semânticas |
| `tabela_simbolos.h` / `tabela_simbolos.c` | Armazenar nome, tipo e linha de declaração; inserir, buscar e liberar símbolos em tabela hash com colisões por encadeamento |
| `semantico.h` / `semantico.c` | Verificar tipos de operadores, atribuições e condições |
| `main.c` | Abrir o arquivo de entrada, inicializar/finalizar Lexer e Parser e retornar o estado da análise |
| `testes/test/executar_testes.c` | Comparar resultados com o manifesto e gerar relatórios |

A gramática utiliza uma seção `var` global, sem declaração de variáveis locais em blocos. Assim, a implementação atual usa uma única tabela de símbolos.

## 4. Linguagem reconhecida

### Tokens e construções

- Palavras reservadas: `program`, `var`, `integer`, `real`, `char`, `begin`, `end`, `if`, `then`, `else`, `while`, `do`, `repeat`, `until`, `write`.
- Operadores: `+`, `-`, `*`, `/`, `div`, `=`, `<>`, `<`, `<=`, `>`, `>=`, `and`, `or`, `not`, `:=`.
- Literais inteiros, reais e caracteres; identificadores; pontuação `(`, `)`, `,`, `;`, `:`, `.`.
- Comentários de linha iniciados por `//`; palavras reservadas são sensíveis a maiúsculas/minúsculas.

### Estrutura geral

```text
program IDENTIFICADOR ; var { declaração } begin { comando } end .
declaração ::= IDENTIFICADOR { , IDENTIFICADOR } : (integer | real | char) ;
comando ::= bloco ; | IDENTIFICADOR := expressão ;
          | while expressão do comando
          | repeat comando until expressão ;
          | if expressão then comando [else comando]
          | write ( expressão ) ;
bloco ::= begin { comando } end
```

O Parser utiliza funções recursivas para os níveis de expressões: fator, termo, soma, relacional e expressão. Operadores multiplicativos têm maior precedência que aditivos, que têm maior precedência que relacionais e lógicos. O menos unário (`-5`, `-x`) é aceito na segunda etapa.

## 5. Regras da análise semântica (parte 2)

- **Declarações:** não pode haver identificador duplicado; toda variável usada deve ter sido declarada.
- **Tipos:** `integer`, `real`, `char`; `boolean` existe internamente como resultado de comparações e operações lógicas, mas não é declarável em `var`.
- **`+` e `-`:** operandos numéricos do mesmo tipo produzem esse tipo; combinações `char`/`integer` produzem `char`.
- **`*`:** operandos numéricos do mesmo tipo, resultado desse tipo.
- **`div`:** apenas `integer div integer`, resultado `integer`.
- **`/`:** apenas `real / real`, resultado `real`.
- **`<`, `<=`, `>` e `>=`:** operandos numéricos do mesmo tipo, resultado `boolean`.
- **`=` e `<>`:** operandos do mesmo tipo (inclusive `char` e `boolean`), resultado `boolean`.
- **`and`, `or`:** dois operandos `boolean`, resultado `boolean`.
- **Menos unário:** operando numérico, mantendo seu tipo. **`not`:** operando `boolean` e resultado `boolean` (regra adotada na implementação; não explicitada nas imagens da seção 2.2).
- **Atribuição:** tipo do destino exatamente igual ao tipo da expressão; não há conversão automática.
- **Condições:** expressões de `if`, `while` e `repeat` devem ser `boolean`.

A análise é feita durante o percurso do Parser, sem construir AST. Quando existem vários erros, o primeiro encontrado pode impedir que erros posteriores sejam reportados.

## 6. Compilação e execução

Execute a partir da pasta **`micropascal-compiler/`**. Requer GCC com suporte a C11.

**Windows / PowerShell:**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
.\micropascal.exe .\testes\exemplo1.pas
$LASTEXITCODE
```

**Linux/macOS:**

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
./micropascal testes/exemplo1.pas
echo $?
```

Alternativamente, use `make` (o `Makefile` inclui automaticamente `src/*.c`). `make clean` pode depender de comandos Unix no Windows.

### Exemplo válido

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

O comando `write(x)` é **validado**, mas não imprime o valor de `x`: o projeto não é um interpretador.

## 7. Testes automatizados

A suíte contém **231 arquivos `.pas`**, distribuídos em 70 válidos léxico-sintáticos, 76 casos históricos de erros sintáticos, 30 de erros léxicos e 55 casos na pasta semântica. Os nomes das pastas identificam a origem/objetivo dos casos, **não necessariamente o resultado esperado de cada arquivo** após a integração da semântica.

Existe **um único** `testes/test/manifesto.csv`, com a classificação esperada de cada teste. As categorias possíveis são `ACEITA`, `LEXICO`, `SINTATICO`, `SEMANTICO`, `ENTRADA`; `OUTRO` identifica resultados não classificados.

**Windows:**

```powershell
.\testes\test\rodar_testes_completos.bat
```

**Linux/macOS:**

```bash
bash testes/test/rodar_testes_completos.sh
```

Os relatórios são salvos como `testes/test/resultado_execucao_NNN.csv`. O conjunto atualizado foi validado com **231/231 resultados conformes**; isso mede a conformidade com o manifesto, não cobertura exaustiva de todas as regras. Consulte [README_test.md](testes/test/README_test.md) para detalhes.

## 8. Estado do projeto

- **Parte 1:** Lexer e Parser implementados.
- **Parte 2 (análise semântica):** tabela hash e validação de tipos implementadas, sem AST.
- **Geração de código intermediário:** ainda não implementada.

Os documentos originais do professor, quando presentes no repositório, devem ser consultados para o enunciado completo. A implementação atual segue a orientação posterior de **não utilizar AST**.

# 👨‍💻 Desenvolvedores


| [<img src="https://github.com/Amanda-Aziz.png" width="115"><br><sub>Amanda Aziz</sub>](https://github.com/Amanda-Aziz) | [<img src="https://github.com/ayrtongomes070-pixel.png" width="115"><br><sub>Ayrton Gomes</sub>](https://github.com/ayrtongomes070-pixel) | [<img src="https://github.com/eduardovcmalta.png" width="115"><br><sub>Eduardo Malta</sub>](https://github.com/eduardovcmalta) | [<img src="https://github.com/francisLauriano.png" width="115"><br><sub>Francis Lauriano</sub>](https://github.com/francisLauriano) | [<img src="https://github.com/GuiDev005.png" width="115"><br><sub>Guilherme Eduardo Araujo</sub>](https://github.com/GuiDev005) | [<img src="https://github.com/Sofiafs.png" width="115"><br><sub>Sofia Farias</sub>](https://github.com/Sofiafs) |
| :---: | :---: | :---: |:---: | :---: | :---: |

