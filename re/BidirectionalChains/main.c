#include <stdlib.h>
#include <stdio.h>   
#include <string.h>
#include <stdint.h>

static uint8_t global_f[14] = {
    0x1d, 0x63, 0xdb, 0x9b, 0xe3,
    0x11, 0x81, 0xcf, 0x2f, 0xfb, 
    0x63, 0x55, 0xc1, 0xdf
};

static uint8_t global_b[14] = {
    0xeb, 0xe5, 0xfb, 0xc3, 0xcf,
    0xb7, 0x53, 0x83, 0x9d, 0x2b, 
    0xfb, 0x0d, 0x43, 0x87
};

typedef struct node {
    struct node *next;
    struct node *prev;
    uint8_t data; 
    uint8_t data_f;
    uint8_t data_b; 
} node; 

node* generate_list(char *input) {
    int len = strlen(input);
    node *next = NULL;
    for(int i = len-1; i >= 0; i--){
        node* new = malloc(sizeof(node));
        new->next = next; 
        if(next != NULL){
            new->next->prev = new;
        }
        new->data = input[i];
        next = new;
    }
    next->prev=NULL;
    return next;
}

void free_list(node *head) {
    if(head->next != NULL) {
        free_list(head->next);
    }
    free(head); 
    return;
}

void encode_f(node* curr_node, uint8_t init) {
    uint8_t p;
    uint8_t c;
    c = curr_node->data;
    if(curr_node->prev == NULL){
        p = init;
    } else {
        p = curr_node->prev->data_f;
    }
    curr_node->data_f = ((c + p) * p + c * p) & 0xff;
    if(curr_node-> next != NULL){
        encode_f(curr_node->next,init);
    }
    return; 
}

void encode_b(node* curr_node, uint8_t init) {
    if(curr_node->next != NULL) {
        encode_b(curr_node->next, init);
    }
    uint8_t n;
    uint8_t c;
    c = curr_node->data; 
    if(curr_node->next == NULL){
        n = init;
    } else {
        n = curr_node->next->data_b;
    }
    curr_node->data_b = (n + c * c * n - c) & 0xff;
    return;
}

int verify(node *list) {
    int i = 0;
    for(node *curr = list; curr != NULL; curr = curr->next){
        if(global_f[i] != curr->data_f || global_b[i] != curr->data_b){
            return 0;
        }
        i++;
    }
    return 1; 
}


int main() {
    char input[100];
    printf("password:");
    scanf("%99s", input);
    if(strlen(input) != 14){
        printf("\033[31mIncorrect\n\033[0m");
        return 0;
    }
    node* list = generate_list(input);
    encode_f(list, 0xA3);
    encode_b(list, 0x4B); 
    int ret = verify(list);
    if(ret == 0){
        printf("\033[31mIncorrect\n\033[0m");
    } else {
        printf("\033[32mCorrect! Good Work\033[0m\nYou deserve this flag: flag{%s}\n", input);
    }
    free_list(list);
    return 0;
}