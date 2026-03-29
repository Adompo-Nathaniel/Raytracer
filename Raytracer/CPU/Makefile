CXX      := g++
CXXFLAGS := -std=c++17 -O3 -Wall -MMD -MP
INCDIR   := Objet
INCLUDES := -I. -I$(INCDIR)

TARGET   := Raytracer3D.exe
OBJDIR   := build
TEST_EXE := TestUnitaire_CPU.exe
BENCH_EXE:= Benchmark_CPU.exe

# On liste les fichiers sources d'objets (Sphere, Cube, Scene)
# On exclut les fichiers contenant un main() pour le filtrage
OBJ_SRCS := $(wildcard $(INCDIR)/*.c++)
OBJ_OBJS := $(OBJ_SRCS:%=$(OBJDIR)/%.o)

all: $(TARGET)

# Liaison du programme principal
$(TARGET): $(OBJDIR)/main.c++.o $(OBJ_OBJS)
	@echo "Linking $@..."
	@$(CXX) $^ -o $@ -static

# Règle pour les tests : compile et exécute
test: $(TEST_EXE) $(BENCH_EXE)
	@echo "--- RUNNING TESTS ---"
	@./$(TEST_EXE)
	@echo "--- RUNNING BENCHMARK ---"
	@./$(BENCH_EXE)

$(TEST_EXE): $(OBJDIR)/TestUnitaire.c++.o $(OBJ_OBJS)
	@$(CXX) $^ -o $@ -static

$(BENCH_EXE): $(OBJDIR)/Benchmark.c++.o $(OBJ_OBJS)
	@$(CXX) $^ -o $@ -static

# Règle générique de compilation
$(OBJDIR)/%.o: %
	@echo "Compiling $<..."
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

-include $(wildcard $(OBJDIR)/*.d) $(wildcard $(OBJDIR)/$(INCDIR)/*.d)

clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(TARGET) del /q $(TARGET)
	@if exist $(TEST_EXE) del /q $(TEST_EXE)
	@if exist $(BENCH_EXE) del /q $(BENCH_EXE)

.PHONY: all clean test