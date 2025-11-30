@echo off
echo  COMPILANDO SISTEMA BANCÁRIO LEITORES/ESCRITORES
echo ==================================================

echo.
echo  Compilando executável...
gcc -Wall -Wextra -Werror -g -pthread -I include -o ..\banco_le_esc.exe ^
  src\main.c ^
  src\init_data.c ^
  src\consultor.c ^
  src\transferencia.c ^
  src\controle.c ^
  src\monitor.c ^
  src\tempo.c ^
  src\utils.c ^
  src\parse.c

if exist "..\banco_le_esc.exe" (
    echo.
    echo  SISTEMA COMPILADO COM SUCESSO!
    echo  Executável: banco_le_esc.exe
    echo.
    echo  EXECUTANDO COM PARÂMETROS PADRÃO:
    echo   3 consultores, 2 operadores, 410ms timeout
    echo   200ms consulta, 100ms transferência, 200ms espera, 5 operações
    echo.
    cd ..
    banco_le_esc.exe 3 2 410 200 100 200 5
) else (
    echo.
    echo  ERRO NA COMPILAÇÃO!
    echo.
    pause
)