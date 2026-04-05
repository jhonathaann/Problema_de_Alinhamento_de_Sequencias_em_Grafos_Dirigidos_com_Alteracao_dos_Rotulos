@echo off
setlocal enableextensions

rem ====================================
rem Script de execucao do projeto
rem ====================================

if not exist "output\main.exe" (
    echo [ERRO] Executavel nao encontrado!
    echo Execute primeiro: build_main.bat
    exit /b 1
)

set "INPUT=%~1"
if "%INPUT%"=="" set "INPUT=inputs\exemplo.txt"

if not exist "%INPUT%" (
    echo [ERRO] Arquivo de entrada nao encontrado: %INPUT%
    exit /b 1
)

echo [INFO] Executando: output\main.exe %INPUT%
output\main.exe %INPUT%
