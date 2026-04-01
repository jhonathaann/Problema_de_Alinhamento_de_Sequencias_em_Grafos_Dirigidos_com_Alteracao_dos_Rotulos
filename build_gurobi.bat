@echo off
setlocal enableextensions

if /i "%DEBUG%"=="1" echo on

echo [INFO] Iniciando build MSVC + Gurobi...

rem Build teste.c usando MSVC (x64) + Gurobi

set "OUTDIR=%~1"
if "%OUTDIR%"=="" set "OUTDIR=build"

if not exist "%OUTDIR%" mkdir "%OUTDIR%" >nul 2>nul

if "%GUROBI_HOME%"=="" goto :err_no_gurobi_home

set "GUROBI_INC=%GUROBI_HOME%\include"
set "GUROBI_LIB=%GUROBI_HOME%\lib"

if not exist "%GUROBI_INC%\gurobi_c.h" goto :err_no_header

if not exist "%GUROBI_LIB%\gurobi130.lib" goto :err_no_lib

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto :err_no_vswhere

for /f "delims=" %%I in ('"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath') do set "VSINSTALL=%%I"
if "%VSINSTALL%"=="" goto :err_no_msvc

set "VSDEVCMD=%VSINSTALL%\Common7\Tools\VsDevCmd.bat"
if not exist "%VSDEVCMD%" goto :err_no_vsdevcmd

call "%VSDEVCMD%" -no_logo -arch=x64 -host_arch=x64
if errorlevel 1 goto :err_vsdevcmd_failed

echo [INFO] Compilando com cl.exe...
cl /nologo /TC /W3 /EHsc- /I"%GUROBI_INC%" /Fo"%OUTDIR%\teste.obj" /Fe"%OUTDIR%\teste.exe" teste.c /link /nologo /LIBPATH:"%GUROBI_LIB%" gurobi130.lib /OUT:"%OUTDIR%\teste.exe" /PDB:"%OUTDIR%\teste.pdb" /ILK:"%OUTDIR%\teste.ilk"

exit /b %errorlevel%

:err_no_header
echo [ERRO] Nao encontrei "%GUROBI_INC%\gurobi_c.h".
exit /b 1

:err_no_lib
echo [ERRO] Nao encontrei "%GUROBI_LIB%\gurobi130.lib".
echo Confirme a versao da lib (pode ser gurobiXXX.lib).
exit /b 1

