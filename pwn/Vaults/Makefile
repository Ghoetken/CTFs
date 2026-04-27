CC = gcc
CFLAGS = -Wall -Wextra -O2 \
         -fstack-protector-strong \
         -D_FORTIFY_SOURCE=2 \
         -Wl,-z,relro,-z,now \
         -Wl,-z,noexecstack \
         -fPIE -pie

TARGETS = vaults vaults_remote
SOURCES = Vaults.c Vaults_remote.c

all: $(TARGETS)

vaults: Vaults.c
	$(CC) $(CFLAGS) -o vaults Vaults.c

vaults_remote: Vaults_remote.c
	$(CC) $(CFLAGS) -o vaults_remote Vaults_remote.c

clean:
	rm -f $(TARGETS)
