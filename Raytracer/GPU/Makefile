NVCC     := nvcc
FLAGS    := -O3 -arch=native -I. -IObjet
TARGET   := raytracer.exe
TEST     := TestUnitaire.exe
B_CPU    := bench_CPU.exe
B_GPU    := bench_GPU.exe

all: $(TARGET) $(TEST) $(B_CPU) $(B_GPU)

# Cible demandée : Compile et exécute les tests et benchs
test: $(TEST) $(B_CPU) $(B_GPU)
	@echo "--- RUNNING GPU TESTS ---"
	@./$(TEST)
	@echo "--- RUNNING BENCH CPU (GPU FOLDER) ---"
	@./$(B_CPU)
	@echo "--- RUNNING BENCH GPU ---"
	@./$(B_GPU)

$(TARGET): main.cu
	$(NVCC) $(FLAGS) main.cu -o $(TARGET)

$(TEST): TestUnitaire.cu
	$(NVCC) $(FLAGS) TestUnitaire.cu -o $(TEST)

$(B_CPU): bench_CPU.cu
	$(NVCC) $(FLAGS) bench_CPU.cu -o $(B_CPU)

$(B_GPU): bench_GPU.cu
	$(NVCC) $(FLAGS) bench_GPU.cu -o $(B_GPU)

clean:
	@if exist $(TARGET) del /q $(TARGET)
	@if exist $(TEST) del /q $(TEST)
	@if exist $(B_CPU) del /q $(B_CPU)
	@if exist $(B_GPU) del /q $(B_GPU)
	@if exist *.lib del /q *.lib
	@if exist *.exp del /q *.exp

.PHONY: all clean test