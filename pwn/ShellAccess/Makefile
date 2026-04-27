CC = gcc
CFLAGS = -fstack-protector-all -no-pie

all: vulnerable

vulnerable: vulnerable.c
	$(CC) $(CFLAGS) -o vulnerable vulnerable.c

clean:
	rm -f vulnerable