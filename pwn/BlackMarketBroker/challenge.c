#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

const char* BANNER = 
"\n"
"▄▄▄█████▓ ██░ ██ ▓█████                                                                            \n"
"▓  ██▒ ▓▒▓██░ ██▒▓█   ▀                                                                            \n"
"▒ ▓██░ ▒░▒██▀▀██░▒███                                                                              \n"
"░ ▓██▓ ░ ░▓█ ░██ ▒▓█  ▄                                                                            \n"
"  ▒██▒ ░ ░▓█▒░██▓░▒████▒                                                                           \n"
"  ▒ ░░    ▒ ░░▒░▒░░ ▒░ ░                                                                           \n"
"    ░     ▒ ░▒░ ░ ░ ░  ░                                                                           \n"
"  ░       ░  ░░ ░   ░                                                                              \n"
"          ░  ░  ░   ░  ░                                                                           \n"
"                                                                                                   \n"
" ▄▄▄▄    ██▓    ▄▄▄       ▄████▄   ██ ▄█▀    ███▄ ▄███▓ ▄▄▄       ██▀███   ██ ▄█▀▓█████▄▄▄█████▓   \n"
"▓█████▄ ▓██▒   ▒████▄    ▒██▀ ▀█   ██▄█▒    ▓██▒▀█▀ ██▒▒████▄    ▓██ ▒ ██▒ ██▄█▒ ▓█   ▀▓  ██▒ ▓▒   \n"
"▒██▒ ▄██▒██░   ▒██  ▀█▄  ▒▓█    ▄ ▓███▄░    ▓██    ▓██░▒██  ▀█▄  ▓██ ░▄█ ▒▓███▄░ ▒███  ▒ ▓██░ ▒░   \n"
"▒██░█▀  ▒██░   ░██▄▄▄▄██ ▒▓▓▄ ▄██▒▓██ █▄    ▒██    ▒██ ░██▄▄▄▄██ ▒██▀▀█▄  ▓██ █▄ ▒▓█  ▄░ ▓██▓ ░    \n"
"░▓█  ▀█▓░██████▒▓█   ▓██▒▒ ▓███▀ ░▒██▒ █▄   ▒██▒   ░██▒ ▓█   ▓██▒░██▓ ▒██▒▒██▒ █▄░▒████▒ ▒██▒ ░    \n"
"░▒▓███▀▒░ ▒░▓  ░▒▒   ▓▒█░░ ░▒ ▒  ░▒ ▒▒ ▓▒   ░ ▒░   ░  ░ ▒▒   ▓▒█░░ ▒▓ ░▒▓░▒ ▒▒ ▓▒░░ ▒░ ░ ▒ ░░      \n"
"▒░▒   ░ ░ ░ ▒  ░ ▒   ▒▒ ░  ░  ▒   ░ ░▒ ▒░   ░  ░      ░  ▒   ▒▒ ░  ░▒ ░ ▒░░ ░▒ ▒░ ░ ░  ░   ░       \n"
" ░    ░   ░ ░    ░   ▒   ░        ░ ░░ ░    ░      ░     ░   ▒     ░░   ░ ░ ░░ ░    ░    ░         \n"
" ░          ░  ░     ░  ░░ ░      ░  ░             ░         ░  ░   ░     ░  ░      ░  ░           \n"
"      ░                  ░                                                                         \n"
" ▄▄▄▄    ██▀███   ▒█████   ██ ▄█▀▓█████  ██▀███                                                    \n"
"▓█████▄ ▓██ ▒ ██▒▒██▒  ██▒ ██▄█▒ ▓█   ▀ ▓██ ▒ ██▒                                                  \n"
"▒██▒ ▄██▓██ ░▄█ ▒▒██░  ██▒▓███▄░ ▒███   ▓██ ░▄█ ▒                                                  \n"
"▒██░█▀  ▒██▀▀█▄  ▒██   ██░▓██ █▄ ▒▓█  ▄ ▒██▀▀█▄                                                    \n"
"░▓█  ▀█▓░██▓ ▒██▒░ ████▓▒░▒██▒ █▄░▒████▒░██▓ ▒██▒                                                  \n"
"░▒▓███▀▒░ ▒▓ ░▒▓░░ ▒░▒░▒░ ▒ ▒▒ ▓▒░░ ▒░ ░░ ▒▓ ░▒▓░                                                  \n"
"▒░▒   ░   ░▒ ░ ▒░  ░ ▒ ▒░ ░ ░▒ ▒░ ░ ░  ░  ░▒ ░ ▒░                                                  \n"
" ░    ░   ░░   ░ ░ ░ ░ ▒  ░ ░░ ░    ░     ░░   ░                                                   \n"
" ░         ░         ░ ░  ░  ░      ░  ░   ░                                                       \n"
"      ░                                                                                            \n\n"
"You have been given access to the BEAR's secret market that only his closest friends have access to.\n\n"
"Can you find out what he might be hiding?\n"
"Connecting to the market now...\n";

#define MAX 12
intptr_t *deals[MAX];

void menu() {
    puts("=== Black Market Broker ===");
    puts("1. Open Deal");
    puts("2. Cancel Deal");
    puts("3. Edit Deal");
    puts("4. View Deal");
    puts("5. Close Broker");
    printf("> ");
}

void open_deal() {
    int idx;
    size_t size;
    printf("Deal index: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX) {
        puts("Invalid deal index!");
        return;
    }
    printf("Deal size: ");
    scanf("%lu", &size);
    deals[idx] = malloc(size);
    printf("Deal opened at %p\n", deals[idx]);
}

void cancel_deal() {
    int idx;
    printf("Deal index: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX || !deals[idx]) {
        puts("Invalid deal index!");
        return;
    }
    free(deals[idx]);
}

void edit_deal() {
    int idx;
    printf("Deal index: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX || !deals[idx]) {
        puts("Invalid deal index!");
        return;
    }
    printf("Edit deal data: ");
    getchar(); // clear newline
    read(0, deals[idx], 0x120);
}

void view_deal() {
    int idx;
    printf("Deal index: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= MAX || !deals[idx]) {
        puts("Invalid deal index!");
        return;
    }
    printf("Deal is at %p\n", deals[idx]);
}

int main() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    puts(BANNER);
    intptr_t vaults[8];

    unsigned int backdoor;
    puts("You've gained a backdoor to see as many vaults as you'd like...");
    printf("How many would you like to see?: ");
    scanf("%d", &backdoor);
    int tmp = backdoor + 1;
    if (tmp < 7) {
        for (int i = 0; i < backdoor; i++) {
            if (i == 25) {
                puts("That's all the vaults we have...");
                break;
            }
            printf("Vault [%d] is located at %p and contains 0x%lx\n", i, &vaults[i], vaults[i]);
        }
    } else {
        printf("There aren't that many vaults!\n");
        return 0;
    }

    int choice;
    while (1) {
        menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: open_deal(); break;
            case 2: cancel_deal(); break;
            case 3: edit_deal(); break;
            case 4: view_deal(); break;
            case 5:
                printf("Looks like you're not meant to be here...\n");
                return 0;
            default: puts("Unknown market action!");
        }
    }
}
