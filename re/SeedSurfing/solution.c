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
    int seed = generate_seed();
    getrand(seed,0x10,&seed);
    getrand(0x20,seed,&seed);
    getrand(seed,seed,&seed);
    printf("%d\n", seed);
}
