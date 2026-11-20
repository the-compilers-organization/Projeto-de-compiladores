# Suíte de testes do MicroPascal

Esta pasta contém **231 testes automatizados** para o compilador MicroPascal em C11. Execute todos os comandos a partir da pasta `micropascal-compiler/`.

## 1. Organização

```text
micropascal-compiler/
├── include/
├── src/
├── test/
│   ├── 01_validos_lexicos_sintaticos/
│   ├── 02_erros_sintaticos/
│   ├── 03_erros_lexicos/
│   ├── 04_semanticos/
│   ├── executar_testes.c
│   ├── manifesto.csv
│   ├── README_test.md
│   └── resultado_execucao_NNN.csv  (relatórios gerados)
├── rodar_testes_completos.bat
├── rodar_testes_completos.sh
└── Makefile
```

Todos os caminhos do manifesto são relativos a `micropascal-compiler/test/`. Os scripts podem ser chamados de outro diretório porque entram automaticamente na pasta do compilador. O executor deve ser executado a partir de `micropascal-compiler/`.

**Importante:** a categoria da pasta é organizacional. Consulte a coluna `esperado` do manifesto para saber o resultado esperado de cada caso.

## 2. Manifesto e categorias

O arquivo `manifesto.csv` contém **231 registros**, um por teste, com as colunas:

| Coluna | Conteúdo |
|---|---|
| `arquivo` | Caminho relativo a `test/` |
| `categoria` | Pasta/categoria organizacional |
| `esperado` | Resultado esperado na etapa 2 |
| `assunto` | Tema do teste |
| `observacao` | Informação complementar |

Resultados reconhecidos pelo executor:

| Resultado | Significado |
|---|---|
| `ACEITA` | Processo terminou com código `0` |
| `LEXICO` | Mensagem de erro léxico |
| `SINTATICO` | Mensagem de erro sintático |
| `SEMANTICO` | Mensagem `Erro semantico na linha` |
| `ENTRADA` | Mensagem de erro de limite de entrada reconhecida pelo executor |
| `OUTRO` | Falha que não se encaixou nas categorias reconhecidas |

O executor compara `observado` e `esperado` e registra `OK` ou `DIVERGENCIA` no relatório. Não existe `manifesto_etapa2.csv`: o único manifesto já contém as expectativas da segunda etapa.

## 3. Executar no Windows (PowerShell)

Requer GCC no `PATH`. Entre em `micropascal-compiler/` e execute:

```powershell
.\rodar_testes_completos.bat
```

Ou compile e execute manualmente:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
gcc -std=c11 -Wall -Wextra -Wpedantic test\executar_testes.c -o executar_testes.exe
.\executar_testes.exe .\micropascal.exe
```

## 4. Executar no Linux/macOS

```bash
bash rodar_testes_completos.sh
```

Alternativamente:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
gcc -std=c11 -Wall -Wextra -Wpedantic test/executar_testes.c -o executar_testes
./executar_testes ./micropascal
```

## 5. Testar um arquivo isolado

O `main.c` exige o **caminho do arquivo `.pas` como argumento**. Não há menu, leitura por `stdin` ou necessidade de `Ctrl+Z`.

```powershell
.\micropascal.exe .\test\04_semanticos\001_valido_aritmetica.pas
$LASTEXITCODE
```

Para testar um caso semântico, escolha um arquivo real em `test/04_semanticos/` e execute, por exemplo:

```powershell
.\micropascal.exe .\test\04_semanticos\001_valido_aritmetica.pas
```

Se o nome do teste diferir, confira os arquivos existentes na pasta. Código de saída `0` indica aceitação; código diferente de `0` indica falha. O compilador **analisa**, mas não executa os comandos `write` do programa Pascal.

## 6. Relatórios e interpretação

Cada execução cria um novo arquivo `test/resultado_execucao_NNN.csv` sem sobrescrever os anteriores (até o limite do executor). Para ver o mais recente no PowerShell:

```powershell
Get-ChildItem .\test\resultado_execucao_*.csv | Sort-Object Name | Select-Object -Last 1
```

O total de conformidades depende do estado da implementação e é informado ao final de cada execução. Os relatórios CSV registram o resultado de cada caso.

## 7. Problemas comuns

| Situação | Verificação |
|---|---|
| `src/main.c: No such file or directory` | Execute na pasta `micropascal-compiler/` |
| Cabeçalho não encontrado | Use `-Iinclude` na compilação |
| `Uso: ... <arquivo_fonte>` | Informe o caminho de um `.pas` |
| Não foi possível abrir o arquivo | Confira o caminho do `.pas` |
| Manifesto não encontrado | Confira `test/manifesto.csv` e o diretório atual |
| Resultado `OUTRO` | Confira a mensagem e o código de saída; talvez o executor não reconheça a falha |
| Nenhuma saída em programa válido | É normal; confira `$LASTEXITCODE` |

Para informações sobre arquitetura, regras de tipos e gramática, consulte o [README principal](../../README.md).
