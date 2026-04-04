# Use: mingw32-make ou make
# IMPORTANTE: Para compilar com Gurobi no Windows, use: mingw32-make build
# Para executar: mingw32-make run

.PHONY: build run clean clean-all

# Compilar o projeto principal (usa MSVC + Gurobi)
build:
	cmd /c "call build_main.bat"

# Executar o programa
INPUT ?= inputs/exemplo.txt
run:
	cmd /c "call run.bat $(INPUT)"

# Limpar arquivos gerados
clean:
	cmd /c "if exist output\*.exe del /q output\*.exe"
	cmd /c "if exist build\*.obj del /q build\*.obj"
	cmd /c "if exist build\*.pdb del /q build\*.pdb"
	cmd /c "if exist build\*.ilk del /q build\*.ilk"
	cmd /c "if exist *.o del /q *.o"
	cmd /c "if exist *.obj del /q *.obj"
	cmd /c "if exist *.pdb del /q *.pdb"
	@echo Arquivos de compilacao removidos

# Limpar tudo
clean-all: clean
	cmd /c "if exist output rmdir /s /q output"
	cmd /c "if exist build\*.obj del /q build\*.obj"
	cmd /c "if exist build\*.pdb del /q build\*.pdb"
	cmd /c "if exist build\*.ilk del /q build\*.ilk"
