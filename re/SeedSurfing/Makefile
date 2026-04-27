CC = gcc
CFLAGS = -fstack-protector -s
TARGET = SeedSurfing
SRC = main.c
TARGET_SOLUTION = solution
SRC_SOLUTION = solution.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	$(CC) -o $(TARGET_SOLUTION) $(SRC_SOLUTION)

clean:
	rm -f $(TARGET)
