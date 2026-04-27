CC = gcc
CFLAGS = -fno-stack-protector -z -g

all: vulnerable

vulnerable: vulnerable.c
	$(CC) $(CFLAGS) -o vulnerable vulnerable.c

clean:
	rm -f vulnerable