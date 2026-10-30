@echo off
setlocal
cd /d "%~dp0\..\.."
echo Compilando MicroPascal...
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src\*.c -o micropascal.exe
if errorlevel 1 exit /b 1
echo Compilando executor de testes...
gcc -std=c11 -Wall -Wextra -Wpedantic testes\test\executar_testes.c -o executar_testes.exe
if errorlevel 1 exit /b 1
echo Executando os 203 testes...
executar_testes.exe .\micropascal.exe
exit /b %errorlevel%
