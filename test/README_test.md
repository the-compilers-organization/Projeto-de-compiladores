# Testes do MicroPascal

Esta pasta reúne os testes do **analisador léxico e sintático MicroPascal**, implementado em C. A suíte automatizada possui **211 programas `.pas` independentes**, organizados por resultado esperado. Também há um roteiro de testes manuais para execução pelo terminal.

> **Importante:** todos os comandos deste README devem ser executados **na raiz do repositório**, isto é, na pasta que contém os diretórios `src/` e `test/`. Execute **um comando por vez**.

## 1. Organização da pasta `test/`

```text
test/
├── 01_validos/                    # 89 programas que devem ser aceitos
├── 02_erros_sintaticos/          # 84 programas com erros de sintaxe
├── 03_erros_lexicos/             # 30 programas com erros léxicos
├── 04_limites/                   # 8 testes de limites da implementação
├── executar_testes.c             # Executor automático escrito em C
├── manifesto.csv                 # Relação dos casos e resultados esperados
├── testes_manuais_micropascal.md # Roteiro para testes feitos pelo terminal
└── resultado_execucao.csv        # Relatório gerado após a execução automática
```

O arquivo `resultado_execucao.csv` é **gerado automaticamente** e pode não existir antes da primeira execução. Os arquivos `.pas` são entradas de teste e não são modificados pelo executor.

## 2. Pré-requisitos

- **GCC** com suporte a C11, disponível no terminal.
- **Windows:** PowerShell (por exemplo, o terminal integrado do VS Code); ou **Linux/macOS:** terminal de comandos.
- O código do analisador em `src/main.c`, `src/lexer.c` e `src/parser.c`.

Para verificar se o GCC está disponível:

```bash
gcc --version
```

## 3. Windows — PowerShell

Abra o terminal na **raiz do projeto**. Os passos abaixo são independentes: aguarde a conclusão de cada comando antes de executar o próximo.

### Passo 1 — Compilar o analisador MicroPascal

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/lexer.c src/parser.c -o micropascal.exe
```

**O que faz:** compila o analisador léxico e sintático e gera `micropascal.exe` na raiz do projeto. As opções `-Wall`, `-Wextra` e `-Wpedantic` ativam avisos úteis do compilador.

### Passo 2 — Compilar o executor automático de testes

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic test/executar_testes.c -o executar_testes.exe
```

**O que faz:** compila o programa responsável por ler o manifesto, executar cada arquivo `.pas` e comparar o resultado observado com o esperado. Gera `executar_testes.exe` na raiz do projeto.

### Passo 3 — Executar todos os 211 testes

```powershell
.\executar_testes.exe .\micropascal.exe
```

**O que faz:** executa os casos listados em `test/manifesto.csv`, apresenta as divergências no terminal e grava o relatório em `test/resultado_execucao.csv`.

### Passo 4 — Executar somente um teste `.pas`

```powershell
Get-Content -Raw .\test\01_validos\001_vazio.pas | .\micropascal.exe
```

**O que faz:** envia o conteúdo de um arquivo de teste diretamente ao analisador, sem passar pelo executor automático. Troque o caminho pelo arquivo `.pas` desejado. O `-Raw` lê o arquivo como um único texto.

### Passo 5 — Abrir o relatório dos testes

```powershell
code .\test\resultado_execucao.csv
```

**O que faz:** abre o relatório no VS Code, se o comando `code` estiver disponível no terminal. Também é possível abrir o CSV pelo Explorador de Arquivos.

## 4. Linux/macOS — terminal

Execute os comandos a seguir, **um de cada vez**, na raiz do projeto.

### Passo 1 — Compilar o analisador MicroPascal

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c src/lexer.c src/parser.c -o micropascal
```

**O que faz:** gera o executável `micropascal`.

### Passo 2 — Compilar o executor de testes

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic test/executar_testes.c -o executar_testes
```

**O que faz:** gera o executável `executar_testes`.

### Passo 3 — Executar todos os testes

```bash
./executar_testes ./micropascal
```

**O que faz:** executa a suíte e produz o relatório CSV.

### Passo 4 — Executar somente um teste `.pas`

```bash
./micropascal < test/01_validos/001_vazio.pas
```

**O que faz:** redireciona o conteúdo do arquivo para a entrada padrão do analisador.

### Passo 5 — Consultar o relatório

```bash
cat test/resultado_execucao.csv
```

**O que faz:** exibe o relatório no terminal. Você também pode abri-lo em um editor ou programa de planilhas.

## 5. Testes manuais: digitar ou colar no terminal

O roteiro com exemplos e resultados esperados está em [`testes_manuais_micropascal.md`](testes_manuais_micropascal.md).

**Windows (PowerShell):**

```powershell
.\micropascal.exe
```

**Linux/macOS:**

```bash
./micropascal
```

Depois de iniciar o analisador, digite ou cole **um programa MicroPascal completo**. Para finalizar a entrada, use:

- **Windows:** `Ctrl+Z` e depois `Enter`.
- **Linux/macOS:** `Ctrl+D`.

O analisador lê até o fim da entrada (**EOF**). Para testar outro programa, inicie novamente o executável. Esses testes manuais não são registrados automaticamente no CSV.

## 6. Como interpretar os resultados

O `manifesto.csv` contém o resultado esperado de cada caso. O executor classifica a saída do analisador nas seguintes categorias:

| Categoria | Significado |
|---|---|
| `ACEITA` | O analisador aceitou o programa e terminou com sucesso. |
| `SINTATICO` | Foi identificado um erro de sintaxe. |
| `LEXICO` | Foi identificado um erro léxico. |
| `ENTRADA` | O código-fonte ultrapassou o limite de entrada da implementação. |
| `OUTRO` | A saída ou o código de retorno não correspondeu a nenhuma das categorias reconhecidas. |

No relatório `resultado_execucao.csv`, a coluna `resultado` indica `OK` quando o resultado observado corresponde ao esperado e `DIVERGENCIA` quando não corresponde. O campo `detalhe` registra a saída capturada do analisador.

**Atenção:** a suíte verifica a **categoria** do resultado; não comprova, por si só, a posição exata do erro, a interpretação semântica ou a execução de expressões. Esses aspectos podem exigir inspeção manual ou testes específicos.

## 7. Problemas comuns

| Mensagem ou situação | O que verificar |
|---|---|
| `gcc` não é reconhecido | Confirme a instalação do GCC e sua inclusão no `PATH`. |
| `micropascal.exe` não é encontrado | Compile o analisador (passo 1) e execute os comandos na raiz do projeto. |
| `test/manifesto.csv` não foi encontrado | Abra o terminal na raiz do repositório, não dentro de `test/`. |
| `Permission denied` ao criar o relatório | Feche `resultado_execucao.csv` em programas que possam bloqueá-lo e verifique a permissão de escrita. |
| `observado=OUTRO` em muitos testes | Confira se o executável inicia corretamente e se o comando do executor está funcionando. |
| O analisador não encerra após `end.` | Finalize a entrada com `EOF`; `end.` faz parte do código MicroPascal, não encerra a leitura do terminal. |

## 8. Observações

- O executor foi escrito em **C11** e utiliza `system()` e redirecionamento de entrada/saída do sistema operacional.
- Os arquivos `.pas` são independentes e podem ser executados individualmente.
- Os testes automatizados não substituem a revisão da gramática e a análise manual de mensagens de erro, precedência e associatividade.
