# Suíte de testes do MicroPascal

Esta pasta contém **231 testes automatizados** para o compilador MicroPascal em C11. Os sete arquivos de exemplo `.pas` diretamente em `testes/` ficam fora da suíte. Execute todos os comandos a partir da pasta `micropascal-compiler/`.

## 1. Organização

```text
testes/
├── e1.pas, e2.pas, e3.pas, e5.pas
├── exemplo1.pas, exemplo2.pas, teste_lexer.pas
└── test/
    ├── 01_validos_lexicos_sintaticos/  (70 arquivos)
    ├── 02_erros_sintaticos/            (76 arquivos)
    ├── 03_erros_lexicos/               (30 arquivos)
    ├── 04_semanticos/                  (55 arquivos, 001–055)
    ├── executar_testes.c
    ├── manifesto.csv                   (único manifesto, etapa 2)
    ├── rodar_testes_completos.bat
    ├── rodar_testes_completos.sh
    └── README_test.md
```

**Importante:** a categoria da pasta é histórica/organizacional. Alguns arquivos de `01_validos_lexicos_sintaticos/` agora são rejeitados por semântica; alguns de `02_erros_sintaticos/` podem disparar erro semântico antes de alcançar o erro sintático. Consulte a coluna `esperado` do manifesto para saber o resultado correto de cada caso.

## 2. Manifesto e categorias

O arquivo `manifesto.csv` contém **231 registros**, um por teste, com as colunas:

| Coluna | Conteúdo |
|---|---|
| `arquivo` | Caminho relativo a `testes/test/` |
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
.\testes\test\rodar_testes_completos.bat
```

Ou compile e execute manualmente:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
gcc -std=c11 -Wall -Wextra -Wpedantic testes\test\executar_testes.c -o executar_testes.exe
.\executar_testes.exe .\micropascal.exe
```

## 4. Executar no Linux/macOS

```bash
bash testes/test/rodar_testes_completos.sh
```

Alternativamente:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/*.c -o micropascal
gcc -std=c11 -Wall -Wextra -Wpedantic testes/test/executar_testes.c -o executar_testes
./executar_testes ./micropascal
```

## 5. Testar um arquivo isolado

O `main.c` exige o **caminho do arquivo `.pas` como argumento**. Não há menu, leitura por `stdin` ou necessidade de `Ctrl+Z`.

```powershell
.\micropascal.exe .\testes\exemplo1.pas
$LASTEXITCODE
```

Para testar um caso semântico, escolha um arquivo real em `testes/test/04_semanticos/` e execute, por exemplo:

```powershell
.\micropascal.exe .\testes\test\04_semanticos\001_valido_aritmetica.pas
```

Se o nome do teste diferir, confira os arquivos existentes na pasta. Código de saída `0` indica aceitação; código diferente de `0` indica falha. O compilador **analisa**, mas não executa os comandos `write` do programa Pascal.

## 6. Relatórios e interpretação

Cada execução cria um novo arquivo `testes/test/resultado_execucao_NNN.csv` sem sobrescrever os anteriores (até o limite do executor). Para ver o mais recente no PowerShell:

```powershell
Get-ChildItem .\testes\test\resultado_execucao_*.csv | Sort-Object Name | Select-Object -Last 1
```

A validação da versão documentada registrou **231/231 testes conformes, 0 divergências** no ambiente Linux. Esse resultado confirma o comportamento esperado para os casos cadastrados, mas não significa que todas as combinações semânticas possíveis foram testadas.

### Atenção à primeira etapa

A suíte original verificava léxico e sintaxe. Com a segunda etapa, a análise semântica pode rejeitar um programa sintaticamente válido ou detectar um erro antes de outro erro sintático posterior. O operador menos unário (`-5`) também passou a ser aceito. Por isso, o manifesto foi atualizado; as expectativas antigas podem ser recuperadas pelo histórico Git, quando disponível.

## 7. Problemas comuns

| Situação | Verificação |
|---|---|
| `src/main.c: No such file or directory` | Execute na pasta `micropascal-compiler/` |
| Cabeçalho não encontrado | Use `-Iinclude` na compilação |
| `Uso: ... <arquivo_fonte>` | Informe o caminho de um `.pas` |
| Não foi possível abrir o arquivo | Confira o caminho do `.pas` |
| Manifesto não encontrado | Confira `testes/test/manifesto.csv` e o diretório atual |
| Resultado `OUTRO` | Confira a mensagem e o código de saída; talvez o executor não reconheça a falha |
| Nenhuma saída em programa válido | É normal; confira `$LASTEXITCODE` |

Para informações sobre arquitetura, regras de tipos e gramática, consulte o [README principal](../../../README.md).
