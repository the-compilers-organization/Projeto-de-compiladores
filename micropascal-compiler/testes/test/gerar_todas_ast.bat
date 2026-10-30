@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0\..\.." || exit /b 1
set "BASE=testes\test"
set "AST=%BASE%\AST"
if not exist "%AST%" mkdir "%AST%" || exit /b 1

echo Compilando MicroPascal com suporte a AST...
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
if errorlevel 1 (echo ERRO: falha na compilacao. & exit /b 1)

set /a NUM=1
:PROXIMO
set "SEQ=000!NUM!"
set "SEQ=!SEQ:~-3!"
set "OUT=%AST%\resultado_ast_!SEQ!"
if exist "!OUT!" (
  set /a NUM+=1
  if !NUM! GTR 999 (echo ERRO: limite de resultados atingido. & exit /b 1)
  goto PROXIMO
)
mkdir "!OUT!" || (echo ERRO: nao foi possivel criar !OUT! & exit /b 1)
for %%D in (01_validos 02_erros_sintaticos 03_erros_lexicos) do (
  if not exist "!OUT!\%%D" mkdir "!OUT!\%%D" || exit /b 1
)
set "CSV=!OUT!\resumo.csv"
> "!CSV!" echo categoria,arquivo,status
if errorlevel 1 (echo ERRO: nao foi possivel criar o resumo. & exit /b 1)
set /a TOTAL=0,GERADAS=0,SEM_AST=0,ERROS=0

for %%D in (01_validos 02_erros_sintaticos 03_erros_lexicos) do (
  for %%F in ("%BASE%\%%D\*.pas") do (
    if exist "%%~fF" (
      set /a TOTAL+=1
      set "ARVORE=!OUT!\%%D\%%~nF.ast.txt"
      set "ERRO_ARQ=!OUT!\%%D\%%~nF.erro.txt"
      rem Primeiro verifica o status sem criar arquivos de saida.
      .\micropascal.exe "%%~fF" --ast >nul 2>&1
      set "CODIGO=!errorlevel!"
      if "!CODIGO!"=="0" (
        rem Aceito: grava somente a arvore.
        .\micropascal.exe "%%~fF" --ast > "!ARVORE!" 2>&1
        if errorlevel 1 (
          set /a ERROS+=1
          >> "!CSV!" echo %%D,%%~nxF,ERRO_NA_SEGUNDA_EXECUCAO
          echo AVISO: falha inesperada ao gerar AST de %%~nxF
        ) else (
          for %%Z in ("!ARVORE!") do set "TAM=%%~zZ"
          if !TAM! GTR 0 (
            set /a GERADAS+=1
            >> "!CSV!" echo %%D,%%~nxF,AST_GERADA
          ) else (
            set /a SEM_AST+=1
            >> "!CSV!" echo %%D,%%~nxF,SEM_AST
            echo AVISO: %%~nxF aceito, mas nenhuma AST foi impressa.
          )
        )
      ) else (
        rem Rejeitado: grava somente o diagnostico, sem mover ou apagar arquivos.
        .\micropascal.exe "%%~fF" --ast > "!ERRO_ARQ!" 2>&1
        set /a ERROS+=1
        >> "!CSV!" echo %%D,%%~nxF,ERRO_ANALISE
      )
    )
  )
)
echo.
echo Arquivos analisados: !TOTAL!
echo ASTs impressas: !GERADAS!
echo Aceitos sem AST: !SEM_AST!
echo Erros de analise: !ERROS!
echo Resultados: !OUT!
endlocal
