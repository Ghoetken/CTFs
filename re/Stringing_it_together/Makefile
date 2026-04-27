# Define the compiler
CC = gcc

# Define the compiler flags with added protections
CFLAGS = -Wall -Wextra -O2 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE -Wl,-z,relro,-z,now -Wl,-z,noexecstack

# Define the linker flags for full RELRO and PIE
LDFLAGS = -pie

# Define the libraries to link against
LIBS = -lssl -lcrypto

# Define the source files and the output executable
SRC = challenge1.c assem.s
OBJ = challenge1.o assem.o
OUT = challenge1

# Default target
all: $(OUT)

# Rule to compile the program
$(OUT): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(OUT) $(OBJ) $(LIBS)

# Rule to compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile assembly source files into object files
%.o: %.s
	$(CC) -c $< -o $@

# Debug target with different flags
debug: CFLAGS += -g
debug: clean $(OUT)

# Clean up generated files
clean:
	rm -f $(OUT) $(OBJ)
