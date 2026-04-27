# Makefile to compile a C program with no stack protections
CC = gcc
CFLAGS = -g -fstack-protector 
TARGET = LockBox
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
