CC = gcc
CFLAGS = -fstack-protector-all -fPIE -z noexecstack 
LDFLAGS = -pie -Wl,-z,relro,-z,now
TARGET = high_score
SRC = dev.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
