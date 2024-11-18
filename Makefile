# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -pthread

# Object files
OBJ = src/main.o src/scanner.o src/thread_pool.o src/config.o

# Header files (dependencies)
DEPS = src/scanner.h src/thread_pool.h src/config.h

# Target executable
TARGET = cscanner

# Pattern rule to compile .c files to .o files
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into the final executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Phony target to clean up object files and the executable
.PHONY: clean

clean:
	rm -f *.o $(TARGET)