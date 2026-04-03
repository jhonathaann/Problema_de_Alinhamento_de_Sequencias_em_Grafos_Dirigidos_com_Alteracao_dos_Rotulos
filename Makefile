# Use: mingw32-make ou make

.PHONY: all build run clean gurobi

# Configurações do compilador
CC := gcc
CFLAGS := -Wall -Wextra -g3
GUROBI_INCLUDE := -I C:/gurobi1301/win64/include

# Diretórios
OUTDIR := output
BUILD_DIR := build

# Projeto principal
MAIN_EXE := $(OUTDIR)/main.exe
MAIN_SOURCES := main.c grafo.c
MAIN_OBJECTS := $(MAIN_SOURCES:.c=.o)
MAIN_HEADERS := grafo.h

# Projeto Gurobi (separado)
GUROBI_EXE := $(BUILD_DIR)/teste.exe

# Target padrão: compilar o projeto principal
all: $(MAIN_EXE)

# Compilar arquivos .c em .o
%.o: %.c $(MAIN_HEADERS)
	$(CC) $(CFLAGS) $(GUROBI_INCLUDE) -c $< -o $@

# Linkar objetos para criar executável principal
$(MAIN_EXE): $(MAIN_OBJECTS)
	@if not exist $(OUTDIR) mkdir $(OUTDIR)
	$(CC) $(CFLAGS) $(MAIN_OBJECTS) -o $@
	@echo Compilacao concluida: $(MAIN_EXE)

# Compilar e executar
build: $(MAIN_EXE)

INPUT ?= inputs/exemplo.txt
run: $(MAIN_EXE)
	$(OUTDIR)\main.exe $(INPUT)

# Target para Gurobi (mantido separado)
gurobi: $(GUROBI_EXE)

$(GUROBI_EXE): gurobi_teste.c build/build_gurobi.bat
	cmd /c "call build/build_gurobi.bat $(BUILD_DIR)"

# Limpar arquivos gerados
clean:
	cmd /c "if exist $(OUTDIR)\*.exe del /q $(OUTDIR)\*.exe"
	cmd /c "if exist *.o del /q *.o"
	@echo Arquivos de compilacao removidos

# Limpar tudo (incluindo Gurobi)
clean-all: clean
	cmd /c "if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)"
