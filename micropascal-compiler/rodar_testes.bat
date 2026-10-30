@echo off
rem Compila o micro-Pascal e roda todos os testes da pasta testes\
rem Uso: dentro da pasta micropascal-compiler, execute  rodar_testes.bat
chcp 65001 >nul
cd /d "%~dp0"

echo Compilando...
gcc -Wall -Wextra -std=c11 -Iinclude src\main.c src\lexer.c src\parser.c -o micropascal.exe
if errorlevel 1 (
    echo Falha na compilacao.
    exit /b 1
)
echo.

call :teste "Exemplo 1 do enunciado: TestaParidade" testes\exemplo1.pas
call :teste "Exemplo 2 do enunciado: SomaImpares" testes\exemplo2.pas
call :teste "Teste do lexer: todos os tipos de token" testes\teste_lexer.pas
call :teste "Erro lexico: caractere que nao existe na linguagem" testes\e1.pas
call :teste "Erro lexico: char literal com dois caracteres" testes\e2.pas
call :teste "Erro de sintaxe: falta o ponto e virgula" testes\e3.pas
call :teste "Erro de sintaxe: Begin com B maiusculo" testes\e5.pas
exit /b 0

:teste
echo ==================================================
echo   %~1
echo   Arquivo: %~2
echo ==================================================
type "%~2"
echo.
echo ---- Saida do compilador:
micropascal.exe "%~2"
if not errorlevel 1 echo [nenhuma mensagem: programa aceito]
echo.
exit /b 0
