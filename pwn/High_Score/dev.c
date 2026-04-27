#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>

const int HIGHSCORE = 480;
int PLAYER_SCORE = 0; 

void clear_terminal(){
    printf("\033[2J\033[H");
    fflush(stdout);
    return;
}

void setup(){
    setbuf(stdin, NULL); 
    setbuf(stderr, NULL);
    setbuf(stdout, NULL);
    srand(time(NULL));
}

void flag(){
    FILE *f;
    if(PLAYER_SCORE > HIGHSCORE){
        f = fopen("flag.txt", "r");
        if (!f) {
            perror("Failed to open flag.txt");
        }
    }
    if(PLAYER_SCORE > HIGHSCORE){
        char flag[32] = {0};
        fread(flag, 1, 31, f);
        printf("Here is your flag: %s\n", flag);
        fflush(stdout);
    }
    return;
}

void rules(){
    printf("RULES:\n");
    printf("The game will generate a random number in the range from 1-20 (inclusive).\n");
    printf("Your job to guess this number in as few guesses as possible.\n");
    printf("After each of your guesses I will tell you either higher or lower until your guess is correct.\n");
    printf("Scoring:\n1st guess: 100 pts\n2nd guess: 80 pts\n3rd guess: 50 pts\n4th guess: 30 pts\n5th guess: 10 pts\n>5 guesses: 0pts\n");
    printf("You have 5 rounds to get enough points to break the high score, goodluck!\n");
    printf("\n\nPress Enter to Start\n");
    fflush(stdout);
    while(getchar() != '\n');
    return;
}

void add_pts(int pts){
    printf("+%d pts!\n", pts);
    fflush(stdout);
    PLAYER_SCORE += pts;
    return;
}

void results(){
    printf("HIGHSCORE: %d\n", HIGHSCORE);
    printf("YOUR SCORE: %d\n\n", PLAYER_SCORE);
    if(PLAYER_SCORE > HIGHSCORE){
        printf("Congrats! You beat the high score!!!\n"); 
    } else {
        printf("You did not beat the high score, better luck next time.\n");
    }
    fflush(stdout);
    if (PLAYER_SCORE > HIGHSCORE) {
        flag();
    }
    return;
}

int get_input(){
    char input[32] = {0};
    gets(input);
    int num = atoi(input);
    if(num < 1 || num > 20){
        printf(input);
        printf(" is out of range or not a valid integer.\n");
        fflush(stdout);
        return -1;
    } else {
        return num;
    }
}

void game(){
    clear_terminal();
    rules();
    for(int round = 1; round < 6; round++){
        clear_terminal();
        printf("HIGHSCORE: %d\n", HIGHSCORE);
        printf("YOUR SCORE: %d\n\n", PLAYER_SCORE);
        printf("<ROUND %d>\n", round);
        int *random = malloc(sizeof(int));
        *random = rand() % 20 + 1;
        printf("Random Number Generated.\n");
        int count = 1;
        printf("Player guess #%d > ", count);
        fflush(stdout);
        int guess = get_input();
        while (guess != *random) {
            if(guess == -1){
                printf("Invalid guess, try again\n");
            } else if(guess > *random){
                printf("Lower!\n");
                count++;
            } else {
                printf("Higher!\n");
                count++;
            }
            printf("Player guess #%d > ", count);
            fflush(stdout);
            guess = get_input();
        }
        printf("Correct!\n");
        fflush(stdout);
        switch(count){
            case 1:
                add_pts(100);
                break;
            case 2: 
                add_pts(80);
                break;
            case 3:
                add_pts(50);
                break;
            case 4: 
                add_pts(30);
                break;
            case 5: 
                add_pts(10);
                break;
            default:
                add_pts(0);
                break;
        }
        free(random);
        printf("Press enter to continue\n");
        fflush(stdout);
        while(getchar() != '\n');
    }
    clear_terminal();
    results();
    return;
}

int main(){
    clear_terminal();
    printf("Try my game, if you beat my high score you'll win a flag!\n");
    printf("\nPress Enter to Play Game\n");
    fflush(stdout);
    while(getchar() != '\n');
    setup();
    game();
    return 0;
}
