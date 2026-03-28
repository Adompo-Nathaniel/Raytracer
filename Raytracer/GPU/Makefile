NVCC = nvcc
TARGET = raytracer
FLAGS = -O3 -arch=native 

all:
	$(NVCC) $(FLAGS) main.cu -o $(TARGET)

clean:
	del $(TARGET).exe

run: all
	@echo "Running..."
	@./$(TARGET)