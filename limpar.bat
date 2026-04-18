@echo off
echo Limpando arquivos antigos da raiz...
if exist *.o del /q *.o
if exist *.obj del /q *.obj
if exist *.pdb del /q *.pdb
if exist testar_modelo.bat del /q testar_modelo.bat
echo Arquivos limpos!
