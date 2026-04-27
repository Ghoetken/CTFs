CC=gcc
CFLAGS=-g -fstack-protector-all -fPIE -pie

all: challenge

challenge: challenge.c
	$(CC) $(CFLAGS) -o challenge challenge.c

clean:
	rm -f challenge