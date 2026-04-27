CC = gcc
SRC = challenge.c
OUT = challenge
CFLAGS = -Wall -Wextra -O2 -fPIC -fno-stack-protector -D_FORTIFY_SOURCE=2 -z relro -z now
LDFLAGS = -Wl,-z,relro,-z,now

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
