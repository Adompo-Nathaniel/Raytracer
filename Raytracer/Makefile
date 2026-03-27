CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -MMD -MP
INCDIR   := Objet
INCLUDES := -I. -I$(INCDIR)

TARGET   := Raytracer3D.exe
OBJDIR   := build
SRCS := $(wildcard *.cpp) $(wildcard *.c++) $(wildcard $(INCDIR)/*.cpp) $(wildcard $(INCDIR)/*.c++)

OBJS := $(SRCS:%=$(OBJDIR)/%.o)

DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(CXX) $(OBJS) -o $@ -static

$(OBJDIR)/%.o: %
	@echo "Compiling $<..."
	@if not exist "$(subst /,\,$(dir $@))" mkdir "$(subst /,\,$(dir $@))" 2>nul || mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)

run: all
	@echo "Running..."
	@./$(TARGET)

clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(TARGET) del /q $(TARGET)
	@if exist Rendu.ppm del /q Rendu.ppm

.PHONY: all run clean

caract: TestUnitaire.c++ Objet/Sphere.c++ Objet/Parallelepipede.c++ Objet/Scene.c++
	g++ -std=c++17 -O3 TestUnitaire.c++ Objet/Sphere.c++ Objet/Parallelepipede.c++ Objet/Scene.c++ -I. -o TestUnitaire.exe
