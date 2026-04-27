#include <stdlib.h>
#include <stdio.h>   
#include <string.h>

static char data[17] = "N0tV3ryR4nd0mHuh";

int generate_seed(){
    int ret = 0;
    for(int i = 0; i < strlen(data); i = i + 2){
        ret += (int) data[i];
    }
    return ret; 
}

void getrand(int key, int iter, int *out) {
    srand(key);
    int var;
    for (int i = 0; i < iter; i++) {
        var = rand();
    }
    var = (var % 9000) + 1000;
    *out = var;
    return;
}

int main(){
    char input[100];
    printf("Enter Code: ");
    fflush(stdout);
    scanf("%99s", input);
    int user_seed = atoi(input);
    int seed = generate_seed();
    getrand(seed,0x10,&seed);
    getrand(0x20,seed,&seed);
    getrand(seed,seed,&seed);
    if(user_seed != seed){
        printf("\033[31mIncorrect\033[0m\nNo flag for you\n");
        return 0;
    } 
    FILE *f = fopen("flag.txt", "r");
    if(f == NULL){
        printf("flag.txt failed to open\n");
        return 0;
    }
    char flag[50] ={0};
    fgets(flag, 50, f);
    printf("\033[32mCorrect code!\033[0m\nHere is your flag: %s\n", flag);
    return 0;
}