CC = gcc
CFLAGS = -fstack-protector -s
TARGET = BidirectionalChains
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)