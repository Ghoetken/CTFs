#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>

#define MAX_VAULTS 50
#define MAX_INPUT 4096
#define MAX_PASSWORD 16

typedef struct {
    char password[MAX_PASSWORD];
    int size;
    char *data;
} Vault;

typedef struct {
    Vault vaults[MAX_VAULTS]; 
    int id;
} Vaults_s;

Vaults_s g_vaults = {0};
char input[MAX_INPUT] = {0};

void setup(){
    // disable buffering
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    return;
}

void read_input(char* prompt){
    memset(input, 0, MAX_INPUT);
    printf(prompt);
    if (!fgets(input, MAX_INPUT, stdin)) {
        printf("fgets failed, try again\n");
        read_input(prompt);
    } else {
        if (input[MAX_INPUT - 1] && input[MAX_INPUT - 1] != '\n') {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Input too long, try again\n");
            read_input(prompt);
        } else {
            // Strip newline
            input[strcspn(input, "\n")] = '\0';
        }
    }
}

void print_banner(){
    printf("=====================================\n");
    printf("   Welcome to our Vault Bank\n");
    printf("  Type 'help' for instructions.\n");
    printf("=====================================\n");
}

void print_help(){
    printf("Commands: \n");
    printf("help      - Show this message \n");
    printf("purchase  - purchase vault \n");
    printf("set       - set vault contents \n");
    printf("view      - view vault contents \n");
    printf("clear     - clear terminal screen \n");
    printf("exit      - exit vault program \n");
}

void purchase_vault(){
    int size;
    while(1){
        read_input("Enter Your Required Vault Size (In Bytes): ");
        size = atoi(input);
        if(size == 0){
            printf("Not valid input, try again\n");
        } else {
            break;
        }
    }
    printf("Finding Vault of Requested Size...\n");
    if(g_vaults.id < MAX_VAULTS){
        Vault *new = &g_vaults.vaults[g_vaults.id-1];
        new->data = malloc(size);
        if(new->data){
            printf("Available Vault found!\n");
            new->size = size;
            while(1){
                read_input("Set Vault Password: ");
                if(strlen(input) > 20){
                    printf("Password Too Long, Try Again...\n");
                } else {
                    memcpy(new->password,input,strlen(input));
                    break;
                }
            }
            printf("Vault Purchased Successfully!\nYour Vault ID: %d\nYour Vault Password: %s\nVault Location: %p\n", g_vaults.id, new->password, new->data);
            g_vaults.id++;
        }
    }

}

void set_vault_contents(int id){
    Vault *vault = &g_vaults.vaults[id-1];
    while(1){
        read_input("Enter Content: ");
        if(strlen(input) <= (size_t)vault->size){
            printf("Copying...\n");
            memcpy(vault->data,input,sizeof(input));
            break;
        } else {
            printf("Vault Can Only Hold %d Bytes, try agian\n", vault->size);
        }
    }
    printf("Content Saved In Vault Successfully!\n");
}

void view_vault_contents(int id){
    printf("Retrieving Content...\n");
    Vault *vault = &g_vaults.vaults[id-1];
    char content[vault->size + 1];  
    memset(content, 0, vault->size + 1);  
    memcpy(content, vault->data, vault->size);
    printf("Memory Retrieved Successfully!\n");
    while(1){
        read_input("Would you like data as a STRING (enter s) or a HEX DUMP (enter h)? ");
        if(input[0] == 'S' || input[0] == 's'){
            printf("Vault Content (as string):\n");
            printf("%s\n", content);
            break;
        } else if (input[0] == 'h' || input[0] == 'H'){
            printf("Vault Content (hex dump):\n");
            for (int i = 0; i < vault->size; i++) {
                printf("%02x ", (unsigned char)vault->data[i]);
                if ((i + 1) % 16 == 0) printf("\n");
            }
            if (vault->size % 16 != 0) printf("\n");
            break;
        } else {
            printf("Invalid Input, try again\n");
        }
    }
}

int credentials(){
    int id;
    read_input("Enter Vault ID: ");
    id = atoi(input);
    if(id < 1 || id > MAX_VAULTS || !g_vaults.vaults[id-1].data){
        printf("Not Valid Vault ID\n");
        return 0;
    } 
    Vault *vault = &g_vaults.vaults[id-1];
    read_input("Enter Vault Password: ");
    char password[MAX_PASSWORD+1];
    memset(password, 0, MAX_PASSWORD+1);
    memcpy(password, vault->password, MAX_PASSWORD);
    if(strcmp(input,password) == 0){
        printf("Access Granted\n");
        return id;
    } else {
        printf("Access Denied\n");
        return 0;
    }
}

int main() {
    setup();
    print_banner();
    g_vaults.id++;

    while (1) {
        read_input("> ");
        if (strcmp(input, "help") == 0) {
            print_help();
            printf("\n");

        } else if (strcmp(input, "purchase") == 0) {
            purchase_vault();
            printf("\n");

        } else if (strcmp(input, "set") == 0) {
            int id = credentials();
            if(id){
                set_vault_contents(id);
            }
            printf("\n");

        } else if (strcmp(input, "view") == 0) {
            int id = credentials();
            if(id){
                view_vault_contents(id);
            }
            printf("\n");
        } else if (strcmp(input, "clear") == 0 ) {
            printf("\033[2J\033[H");

        } else if (strcmp(input, "exit") == 0) {
            break;

        } else if (strlen(input) == 0) {
            // Ignore empty lines
            continue;

        } else {
            printf("Unknown command: %s\n", input);
            printf("Type 'help' for available commands.\n");
            printf("\n");
        }
    }

    return 0;
}

