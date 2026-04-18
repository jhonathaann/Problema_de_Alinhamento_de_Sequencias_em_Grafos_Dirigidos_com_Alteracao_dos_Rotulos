@echo off
setlocal enableextensions

rem ====================================
rem Script de compilacao do projeto
rem ====================================

rem Verifica se GUROBI_HOME está definido
if "%GUROBI_HOME%"=="" (
    echo [ERRO] Variavel GUROBI_HOME nao esta definida!
    echo Defina com: set GUROBI_HOME=C:\gurobi1301\win64
    exit /b 1
)

set "GUROBI_INC=%GUROBI_HOME%\include"
set "GUROBI_LIB=%GUROBI_HOME%\lib"

rem Verifica se os arquivos do Gurobi existem
if not exist "%GUROBI_INC%\gurobi_c.h" (
    echo [ERRO] Nao encontrei "%GUROBI_INC%\gurobi_c.h"
    exit /b 1
)

if not exist "%GUROBI_LIB%\gurobi130.lib" (
    echo [ERRO] Nao encontrei "%GUROBI_LIB%\gurobi130.lib"
    exit /b 1
)

rem Localiza o Visual Studio
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo [ERRO] Visual Studio nao encontrado!
    exit /b 1
)

for /f "delims=" %%I in ('"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set "VSINSTALL=%%I"
if "%VSINSTALL%"=="" (
    echo [ERRO] Visual Studio C++ nao encontrado!
    exit /b 1
)

set "VSDEVCMD=%VSINSTALL%\Common7\Tools\VsDevCmd.bat"
if not exist "%VSDEVCMD%" (
    echo [ERRO] VsDevCmd.bat nao encontrado!
    exit /b 1
)

rem Ativa o ambiente MSVC
call "%VSDEVCMD%" -no_logo -arch=x64 -host_arch=x64 >nul 2>nul
if errorlevel 1 (
    echo [ERRO] Falha ao inicializar ambiente MSVC
    exit /b 1
)

rem Cria diretórios se não existirem
if not exist "build" mkdir "build"
if not exist "output" mkdir "output"

echo [INFO] Compilando arquivos .c...
cl /nologo /TC /W3 /Zi /I"%GUROBI_INC%" /Fo"build\\" /Fd"build\\" /c main.c grafo.c modelo_pli.c
if errorlevel 1 (
    echo [ERRO] Falha na compilacao!
    exit /b 1
)

echo [INFO] Linkando executavel...
link /nologo build\main.obj build\grafo.obj build\modelo_pli.obj /LIBPATH:"%GUROBI_LIB%" gurobi130.lib /OUT:output\main.exe /PDB:build\main.pdb
if errorlevel 1 (
    echo [ERRO] Falha na linkagem!
    exit /b 1
)

echo [SUCESSO] Executavel criado: output\main.exe

