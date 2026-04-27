#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <time.h>

#define MAX_INPUT 0x30
#define MAX_STICKYNOTES 100


typedef struct{
    char *notes[MAX_STICKYNOTES];
    int idx;
} Notes;

Notes g_notes = {0};
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
        if (!strchr(input, '\n')) {
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
    printf("   Welcome to StickyNotes\n");
    printf("  Type 'help' for instructions.\n");
    printf("=====================================\n");
}

void print_help(){
    printf("Commands: \n");
    printf("help      - Show this message \n");
    printf("new       - Write new note \n");
    printf("edit      - Edit Note \n");
    printf("view      - view note \n");
    printf("delete    - delete a note \n");
    printf("clear     - clear terminal view \n");
    printf("exit      - exit vault program \n\n");
}

void new(){
    read_input("Write your note: \n");
    printf("Saving... \n");
    char *note = malloc(MAX_INPUT);
    g_notes.notes[g_notes.idx] = note;
    strcpy(note,input);
    printf("Saved at index: %d\n\n", g_notes.idx++);
}

void view(){
    int idx = 0;
    read_input("Note Index: \n");
    idx = atoi(input);
    if(idx < 1 || idx >= g_notes.idx){
        printf("Not a valid index\n\n");
        return;
    }
    char *note = g_notes.notes[idx];
    printf("Note: \n");
    printf(note);
    printf("\n\n");
}

void edit(){
    int idx = 0;
    read_input("Note Index: \n");
    idx = atoi(input);
    if(idx < 1 || idx >= g_notes.idx){
        printf("Not a valid index\n\n");
        return;
    }
    char *note = g_notes.notes[idx];
    read_input("Rewrite Note: \n");
    memset(note, 0, 0x30);
    strcpy(note, input);
    printf("Note updated \n\n");
}

void delete(){
    int idx = 0;
    read_input("Note Index: \n");
    idx = atoi(input);
    if(idx < 1 || idx >= g_notes.idx){
        printf("Not a valid index\n\n");
        return;
    }
    char *note = g_notes.notes[idx];
    free(note);
    printf("Note Freed\n\n");
}

int main(void){
    setup();
    print_banner();
    g_notes.idx = 1;
    while (1) {
        read_input("> ");
        if (strcmp(input, "help") == 0) {
            print_help();

        } else if (strcmp(input, "new") == 0) {
            new();

        } else if (strcmp(input, "view") == 0) {
            view();

        } else if (strcmp(input, "edit") == 0) {
            edit();

        } else if (strcmp(input, "delete") == 0) {
            delete();

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