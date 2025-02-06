# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Source files (in the 'source' directory)
SRC = source/main.c source/calculations.c source/system.c

# Object files (generated from source files)
OBJ = $(SRC:.c=.o)

# Output executable name
EXEC = program

# Default target (what make will do if no target is pecified)
all: $(EXEC)

# Linking the object files to create the executable
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Compiling the source files to object files
source/%.o: source/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
.PHONY: clean
clean:
	rm -f $(OBJ) $(EXEC)s