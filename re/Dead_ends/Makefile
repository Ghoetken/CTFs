CC = gcc
CFLAGS = -fstack-protector
TARGET = maze
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
