CC = gcc
CFLAGS = -fstack-protector-all -Wl,--exclude-libs,ALL -no-pie -O2

all: skynet_console

skynet_console: skynet_console.c
	$(CC) $(CFLAGS) -o skynet_console skynet_console.c

clean:
	rm -f skynet_console
