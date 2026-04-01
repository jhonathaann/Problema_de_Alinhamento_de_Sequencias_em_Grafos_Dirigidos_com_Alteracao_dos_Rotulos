# Use: mingw32-make

.PHONY: all build run clean

OUTDIR := build
EXE := $(OUTDIR)/teste.exe

all: $(EXE)

$(EXE): gurobi_teste.c build_gurobi.bat
	cmd /c "call build_gurobi.bat $(OUTDIR)"

build: $(EXE)

run: $(EXE)
	cmd /c "cd /d $(OUTDIR) && teste.exe"

clean:
	cmd /c "if exist $(OUTDIR) rmdir /s /q $(OUTDIR)"
