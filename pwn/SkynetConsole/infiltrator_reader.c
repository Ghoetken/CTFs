#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

// Compiled with: gcc infiltrator_reader.c -o infiltrator_reader -no-pie -fstack-protector-all -O2
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "[INFILTRATOR AI] Input mismatch.\n");
        fprintf(stderr, "Usage: %s <source_payload> <temp_storage>\n", argv[0]);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "cp %s %s", argv[1], argv[2]);
    system(command);
    printf("[INFILTRATOR AI] Payload clone complete.\n");

    struct stat verify;
    if (lstat(argv[2], &verify) == -1) {
        perror("[ERROR] lstat failed");
        return 1;
    }
    printf("[SECURITY] lstat check passed...\n");

    if (!S_ISREG(verify.st_mode)) {
        fprintf(stderr, "[SECURITY] Alert: Target is not a regular file.\n");
        return 1;
    }
    printf("[SECURITY] Regular file verification passed...\n");

    if (access(argv[2], R_OK) != 0) {
        perror("[ERROR] access denied");
        return 1;
    }

    FILE *file = fopen(argv[2], "r");
    if (!file) {
        perror("[ERROR] fopen failed");
        return 1;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    printf("[INFILTRATOR AI] File contents read:\n");
    printf("[OUTPUT] %s\n", buffer);
    fclose(file);

    return 0;
}
