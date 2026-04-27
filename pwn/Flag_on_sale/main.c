#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>
#include <string.h>   
#include <time.h>  
#include <sys/select.h>
#include <stdint.h>

//STRUCTS AND GLOBAL VARIABLES
#define MAX_ACCOUNTS 100
int FLAG_PRICE = 2147483647;
uint64_t canary = 0;

typedef struct {
    char name[16];
    char password[16];
    int *amount;
} account; 

typedef struct {
    account *lst[MAX_ACCOUNTS];
    int index;
} accounts;

//SETUP/INPUT HANDLING/TERMINAL FUNCTIONS
void init_canary() {
    srand(time(NULL));
    canary = ((uint64_t)rand() << 32) | rand();
    canary &= 0xffffffffffffff00;  
}

void clear_terminal(){
    printf("\033[2J\033[H");
    return;
}

void clear_stdin() {
    int c;
    struct timeval tv;
    fd_set fds;

    while (1) {
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) != 1) {
            break;
        }
        c = getchar();
        if (c == EOF)
            break;
    }
}

void setup(){
    setbuf(stdin, NULL); 
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    srand(time(NULL));
}

char get_input(){
    char buf[2]; 
    fgets(buf, sizeof(buf), stdin);
    clear_stdin(); 
    return buf[0];
}

//MAKE LOGIN AND VIEW ACCOUNT FUNCTIONS
void make_account(accounts *account_lst){
    if(account_lst->index > 99){
        printf("Max Accounts reached... You really made 100 accounts? why...\n");
        return; 
    }
    account *new = malloc(sizeof(account));
    new->amount = malloc(sizeof(int));
    *new->amount = 0;
    printf("Account Name: ");
    fgets(new->name, 16, stdin);
    clear_stdin();
    new->name[strcspn(new->name, "\n")] = '\0';
    printf("Account Password: ");
    fgets(new->password, 16, stdin);
    clear_stdin();
    new->password[strcspn(new->password, "\n")] = '\0';
    account_lst->lst[account_lst->index] = new;
    account_lst->index += 1;
    printf("Account made succesfully\n");
    return;
}

account *login_account(accounts *account_lst){
    char input[16]= {0};
    printf("Enter Account Name: ");
    fgets(input,16,stdin);
    input[strcspn(input, "\n")] = '\0';
    clear_stdin();
    account *a = NULL;
    for(int i = 0; i < account_lst->index; i++){
        account *curr = account_lst->lst[i];
        if(strcmp(curr->name,input) == 0){
            a = curr;
            break;
        }
    } 
    if(a == NULL){
        printf("no account with that name was found\n");
        return NULL;
    }
    printf("Enter Password: ");
    for(int i = 0; i < 16; i++){
        input[i] = '\0';
    }
    fgets(input,16,stdin);
    input[strcspn(input, "\n")] = '\0';
    clear_stdin();
    if(strcmp(a->password, input) != 0){
        printf("Incorrect Password\n");
        return NULL;
    }
    return a;
}

void view_account(account *a){
    int *amount = a->amount;
    printf("Name: %s\n", a->name); 
    printf("Password: %s\n", a->password);
    printf("Amount: $%d \n", *amount);
    return;
}

//WITHDRAWL AND DEPOSIT FUNCTIONS 
void make_deposit(account *a, int *cash){ 
    init_canary();
    uint64_t local_canary = canary;
    char input[16] = {0}; 
    int *amount = a->amount;
    printf("Enter Deposit Amount: ");
    gets(input);
    int num = atoi(input);
    if(num == 0){
        printf("invalid input\n");
        return;
    }
    if(num > *cash){
        printf("You Don't have that much cash\n");
        return;
    } 
    *amount += num; 
    *cash -= num;
    printf("Deposit Complete\n");
    if (local_canary != canary) {
        fprintf(stderr, "*** Stack corruption detected ***\n");
        exit(1);
    }
    return; 
}

void make_withdrawl(account *a, int *cash){
    init_canary();
    uint64_t local_canary = canary;
    char input[16] = {0}; 
    int *amount = a->amount;
    printf("Enter Withdrawl Amount: ");
    gets(input);
    int num = atoi(input);
    if(num == 0){
        printf("invalid input\n");
        return;
    }
    if(num > *amount){
        printf("You Don't have that much money in your account\n");
        return;
    } 
    *cash += num;
    *amount -= num; 
    printf("Withdrawl Complete\n");
    if (local_canary != canary) {
        fprintf(stderr, "*** Stack corruption detected ***\n");
        exit(1);
    }
    return; 
}

//FLAG ACCESS AND CONTROL FLOW
void purchase_flag(int *purchased, int *cash){
    if(*cash < FLAG_PRICE){
        printf("Not enough money to purchase this flag.\n");
        return;
    } 
    *cash -= FLAG_PRICE;
    *purchased = 1;
    printf("Purchased! You can access you flag at the starting screen\n");
    return;
}

void flag(){
    FILE *f = fopen("flag.txt", "r");
    if (!f) {
        perror("Failed to open flag.txt");
        return;
    }
    char flag[32] = {0};
    fread(flag, 1, 31, f);
    printf("Here is your flag: %s\n", flag);
    return;
}

//MAIN LOOP CONTROL FLOW
int main(void){
    setup();
    accounts *account_lst = malloc(sizeof(accounts));
    account_lst->index = 0;
    int cash = 1; 
    int purchased = 0;
    
    char c;
    while(1){
        clear_terminal();
        if(purchased == 0){
            printf("YOUR CASH: $%d\n\n", cash);
            printf("STORE <S>    BANK <B>\n");
        } else {
            printf("YOUR CASH: $%d\n", cash);
            printf("STORE <S>    BANK <B>    FLAG <F>\n");
        }

        c = get_input();
        if(c == 's' || c == 'S'){
            clear_terminal();
            while(1){
                clear_terminal();
                printf("YOUR CASH: $%d\n       -------------\nFLAG -< $%d |\n       -------------\n\n", cash, FLAG_PRICE);
                printf("EXIT <E>    PURCHASE FLAG <P>\n");
                c = get_input();
                if(c == 'e' || c == 'E'){
                    break;
                } else if(c == 'P' || c == 'p'){
                    purchase_flag(&purchased, &cash);
                    printf("Press enter to continue\n");
                    get_input();
                } else {
                    printf("Invalid input\n");
                    printf("Press enter to continue\n");
                    get_input();
                }
            }
        } else if(c == 'B' || c == 'b'){
            while(1){
                clear_terminal();
                printf("YOUR CASH: $%d\n\n", cash);
                printf("EXIT <E>    MAKE ACCOUNT <M>    ACCESS ACCOUNT <A>\n");
                c = get_input();
                if(c == 'M' || c == 'm'){
                    clear_terminal();
                    make_account(account_lst);
                    printf("Press enter to continue\n");
                    get_input();
                } else if(c == 'A' || c == 'a'){
                    clear_terminal();
                    account *a = login_account(account_lst);
                    if(a != NULL){
                        while(1){
                            clear_terminal();
                            printf("YOUR CASH: $%d\n\n", cash);
                            view_account(a);
                            printf("\nEXIT <E>    DEPOSIT <D>    WITHDRAWL <W>\n");
                            c = get_input();
                            if(c == 'E' || c == 'e'){
                                break;
                            } else if(c == 'D' || c == 'd'){
                                make_deposit(a, &cash);
                                printf("Press enter to continue\n");
                                get_input();
                            } else if(c == 'W' || c == 'w'){
                                make_withdrawl(a, &cash);
                                printf("Press enter to continue\n");
                                get_input();
                            } else {
                                printf("Invalid input\n");
                                printf("Press enter to continue\n");
                                get_input();
                            }
                        }
                    } else { 
                        printf("Press enter to continue\n");
                        get_input();
                    }
                } else if( c== 'e' || c == 'E'){
                    break;
                } else {
                    printf("Invalid input\n");
                }
            }
        } else if((c == 'f' || c == 'F') && purchased == 1){
            clear_terminal();
            flag();
            break;
        } else{
            printf("Invalid input\n");
            printf("Press enter to continue\n");
            get_input();
        }
    }
    return 0;
}
