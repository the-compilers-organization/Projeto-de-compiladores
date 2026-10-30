# Testes do MicroPascal

Esta pasta contém a suíte de **203 testes automatizados** do MicroPascal, implementado em C, além dos scripts de geração de AST da segunda parte. Os sete arquivos `.pas` originais permanecem separados, diretamente na pasta `testes/`.

> **IMPORTANTE:** o `main.c` **não possui menu interativo** e **não lê o código pela entrada padrão**. É obrigatório informar o caminho de um arquivo `.pas` como argumento: `micropascal <arquivo_fonte>`. Todos os comandos abaixo partem de `micropascal-compiler/`, a pasta que contém `src/`, `include/` e `testes/`.

---

## 1. Organização dos arquivos

```text
micropascal-compiler/
├── include/
│   ├── lexer.h
│   ├── ast.h
│   ├── parser.h
│   └── token.h
├── src/
│   ├── main.c
│   ├── lexer.c
│   └── parser.c
├── testes/
│   ├── e1.pas
│   ├── e2.pas
│   ├── e3.pas
│   ├── e5.pas
│   ├── exemplo1.pas
│   ├── exemplo2.pas
│   ├── teste_lexer.pas
│   └── test/
│       ├── 01_validos/              # 89 testes
│       ├── 02_erros_sintaticos/    # 84 testes
│       ├── 03_erros_lexicos/       # 30 testes
│       ├── executar_testes.c
│       ├── manifesto.csv
│       ├── README_test.md
│       ├── gerar_todas_ast.bat
│       ├── gerar_todas_ast.sh
│       ├── AST/                     # resultados AST numerados
│       ├── resultado_execucao_001.csv  # relatórios numerados
│       ├── rodar_testes_completos.bat
│       └── rodar_testes_completos.sh
└── ...
```

> **IMPORTANTE:** Os relatórios `resultado_execucao_001.csv`, `resultado_execucao_002.csv` etc. são criados sequencialmente pelo executor, sem sobrescrever os anteriores. Os arquivos `.pas` são entradas de teste e não devem ser modificados durante a execução.

---

## 2. Pré-requisitos

- GCC com suporte a C11 (`gcc --version`).
- Terminal do VS Code, PowerShell no Windows ou shell no Linux/macOS.
- Executar os comandos na pasta **`micropascal-compiler/`**, não na raiz externa do repositório `Projeto-de-compiladores/`.

No PowerShell, caso esteja na raiz externa do repositório:

```powershell
cd .\micropascal-compiler
```

---

## 3. Windows — PowerShell

### Opção A — Executar a suíte completa pelo script

```powershell
.\testes\test\rodar_testes_completos.bat
```

O script deve compilar o analisador e o executor e executar os 203 casos. 

>**Atenção:** para funcionar com esta organização, os caminhos internos do `.bat` e do `executar_testes.c` precisam apontar para `testes/test/`. Este README não altera esses arquivos.

...

### Opção B — Compilar manualmente

**1. Compilar o MicroPascal:**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
```

**2. Compilar o executor dos testes:**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic testes\test\executar_testes.c -o executar_testes.exe
```

**3. Executar a suíte:**

```powershell
.\executar_testes.exe .\micropascal.exe
```

Esse comando pressupõe que `executar_testes.c` esteja adaptado aos caminhos `testes/test/` e que execute o compilador passando o arquivo `.pas` como argumento, não por redirecionamento de entrada.

**4. Abrir o relatório:**

```powershell
Get-ChildItem .\testes\test\resultado_execucao_*.csv | Sort-Object Name | Select-Object -Last 1
```

### Executar um único arquivo `.pas`

```powershell
.\micropascal.exe .\testes\exemplo1.pas
```

Ou um dos testes da suíte:

```powershell
.\micropascal.exe .\testes\test\01_validos\001_vazio.pas
```

Se o código estiver correto, a versão atual do `main.c` termina normalmente **sem imprimir mensagem de sucesso**. Para consultar o código de saída no PowerShell, execute logo após o teste:

```powershell
$LASTEXITCODE
```

O valor `0` indica término com sucesso; um valor diferente de `0` indica falha. Erros léxicos ou sintáticos são reportados pelo analisador quando detectados.

---

## 4. Linux/macOS — terminal

### Executar pelo script

```bash
bash testes/test/rodar_testes_completos.sh
```

O script e o executor devem estar adaptados aos caminhos `testes/test/`.

### Compilar e executar manualmente

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
gcc -std=c11 -Wall -Wextra -Wpedantic testes/test/executar_testes.c -o executar_testes
./executar_testes ./micropascal
```

Para um único arquivo:

```bash
./micropascal testes/test/01_validos/001_vazio.pas
```

Consultar o relatório:

```bash
ls testes/test/resultado_execucao_*.csv
```

---

## 5. Testes manuais: funcionamento atual

**Ainda não é possível digitar ou colar diretamente o programa MicroPascal no terminal**, pois o `main.c` exige um caminho de arquivo em `argv[1]`.

Para fazer um teste manual:

1. Crie um arquivo, por exemplo `testes/teste_manual.pas`.
2. Escreva ou cole nele um programa MicroPascal completo.
3. Salve o arquivo.
4. Execute no PowerShell:

```powershell
.\micropascal.exe .\testes\teste_manual.pas
```

Exemplo de conteúdo do arquivo:

```pascal
program Teste;
var
    x : integer;
begin
    x := 10;
    write(x);
end.
```

**Não use** `Get-Content ... | .\micropascal.exe`, `Ctrl+Z`, `Ctrl+D` ou `./micropascal < arquivo.pas` para este `main.c`: ele abre o arquivo informado por argumento e não consome o código pela entrada padrão. Também não há opções de menu `1`, `2` ou `0` nesta versão.

O compilador **analisa** o comando `write(x)`, mas não executa o programa Pascal nem imprime o valor de `x`.

---

## 6. Categorias e interpretação dos resultados

| Categoria | Significado |
|---|---|
| `ACEITA` | Programa aceito pelo analisador; processo terminou com sucesso. |
| `SINTATICO` | Erro sintático identificado. |
| `LEXICO` | Erro léxico identificado. |
| `OUTRO` | Resultado que o executor não conseguiu classificar como uma das categorias anteriores. |

O `manifesto.csv` contém o resultado esperado de cada um dos 203 testes. Nos relatórios `resultado_execucao_NNN.csv`, `OK` indica que a categoria observada corresponde à esperada; `DIVERGENCIA` indica que não corresponde. Os nomes exatos das colunas dependem da versão de `executar_testes.c` utilizada.

### Categoria de limites removida

A categoria `04_limites` foi retirada da suíte atual por decisão do projeto. O manifesto agora contém **203 testes**: 89 válidos, 84 erros sintáticos e 30 erros léxicos.

---

## 7. Geração das árvores sintáticas (AST)

A versão da segunda parte do MicroPascal, com `ast.c`, `ast.h` e suporte a `--ast`, permite gerar as árvores dos testes em lote. Os scripts processam as três categorias restantes e criam uma pasta numerada a cada execução.

**Windows:**

```powershell
.\testes\test\gerar_todas_ast.bat
```

**Linux/macOS:**

```bash
bash testes/test/gerar_todas_ast.sh
```

**Árvore individual:**

```powershell
.\micropascal.exe .\testes\test\01_validos\001_vazio.pas --ast
```

A saída fica em `testes/test/AST/resultado_ast_001/`, `resultado_ast_002/` etc. Cada pasta contém as subpastas `01_validos`, `02_erros_sintaticos`, `03_erros_lexicos` e o arquivo `resumo.csv`. Nos testes válidos, são esperadas 89 ASTs; nos 114 casos inválidos, são esperados erros de análise. Esses totais devem ser confirmados na execução.

---

## 8. Problemas comuns

| Situação | Verificação |
|---|---|
| `src/main.c: No such file or directory` | Entre em `micropascal-compiler/` antes de compilar. |
| `lexer.h` ou `parser.h` não encontrado | Inclua `-Iinclude` no comando de compilação. |
| `Uso: ... <arquivo_fonte>` | Informe o caminho de um `.pas` ao executar o compilador. |
| `Erro: nao foi possivel abrir o arquivo` | Confira se o caminho e o nome do arquivo estão corretos. |
| `manifesto.csv` não encontrado | Verifique se o executor procura em `testes/test/manifesto.csv`. |
| Muitos resultados `OUTRO` | Confira se o executor chama `micropascal <arquivo.pas>` em vez de usar redirecionamento de entrada. |
| Nenhuma mensagem após um teste válido | É o comportamento normal do `main.c` atual; confira o código de saída. |
| Relatório não é criado | Verifique o caminho de saída e se o CSV está aberto em outro programa. |

---

## 9. Observações

- Os 203 testes automatizados e os sete arquivos originais ficam separados.
- A suíte verifica a análise léxica e sintática; a geração da AST é uma verificação complementar, não uma validação semântica nem a execução do programa Pascal.
- A documentação considera o compilador executado por argumento de arquivo. A versão com AST também aceita `--ast`.
- Este README não modifica o código-fonte nem os scripts; apenas documenta sua utilização.
