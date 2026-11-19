@echo off
setlocal
REM Este script funciona a partir de qualquer diretorio.
cd /d "%~dp0"
if errorlevel 1 exit /b 1

echo Compilando MicroPascal...
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
if errorlevel 1 exit /b 1

echo Compilando executor de testes...
gcc -std=c11 -Wall -Wextra -Wpedantic test\executar_testes.c -o executar_testes.exe
if errorlevel 1 exit /b 1

echo Executando os testes do manifesto...
.\executar_testes.exe .\micropascal.exe
exit /b %errorlevel%
