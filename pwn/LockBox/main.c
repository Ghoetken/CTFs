#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>   
#include <time.h>   


typedef struct{
    char password[16];
    char contents[32];
} lockbox;

void setup()
{
    setbuf(stdin, NULL); 
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    srand(time(NULL));
}

void gen_password(lockbox *lb){
    for(int i = 0; i < 15; i++){
        int r = rand() % 26;
        lb->password[i] = 'a' + r;
    }
    lb->password[15] = '\0';
}

void gen_contents(lockbox *lb){
    FILE *f = fopen("flag.txt", "r");
    if (!f) {
        perror("Failed to open flag.txt");
        return;
    }
    size_t bytes_read = fread(lb->contents, 1, 31, f);
    lb->contents[bytes_read] = '\0';
}


void vuln(lockbox *lb){
    char password[16];
    strcpy(password,lb->password);
    char input[16] = {0};
    while(1){
        printf("Enter lockbox password: \n");
        fgets(input, 16, stdin);
        input[strcspn(input, "\n")] = '\0';
        if(strcmp(password, input)){
            printf(input);
            printf(" is not correct, try again\n\n");
        } else {
            break;
        }
    }
    printf("CONTENTS: %s\n", lb->contents);
}

int main(){
    setup();
    lockbox* lb = malloc(sizeof(lockbox));
    gen_password(lb);
    gen_contents(lb);
    vuln(lb);
    return 0;
}