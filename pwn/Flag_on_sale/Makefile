CC = gcc
CFLAGS = -fno-stack-protector -no-pie -g -O0
TARGET = ./flag_on_sale
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)