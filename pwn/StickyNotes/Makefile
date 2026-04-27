CC = gcc
CFLAGS = -fstack-protector-strong \
         -Wl,-z,relro,-z,now \
         -Wl,-z,noexecstack \
         -fPIE -pie -g

SRC = StickyNotes.c
TARGET = stickynotes

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
