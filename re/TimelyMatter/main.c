#include <stdlib.h>
#include <stdio.h>   
#include <time.h>

#include <string.h>

int getone(struct tm *local_time){
    int min = local_time->tm_min;
    int sec = local_time->tm_sec;

    srand(min*sec);
    return (rand()%9000) + 1000;
}

int gettwo(struct tm *local_time){
    int hour = local_time->tm_hour;
    int sec = local_time->tm_sec;
    srand(hour*sec);
    return (rand()%9000) + 1000;
}

int getthree(struct tm *local_time){
    int day = local_time->tm_mday;
    int sec = local_time->tm_sec;
    srand(day*sec);
    return (rand()%9000) + 1000;
}

int getcode() {
    time_t now; 
    struct tm *local_time;

    time(&now);
    local_time = localtime(&now);

    int hour = local_time->tm_hour;
    int min = local_time->tm_min;

    if ((min%2) == 0) {
        return getone(local_time);
    } else if ((hour%3) != 0){
        return gettwo(local_time);
    } else {
        return getthree(local_time);
    }
}

int main(){
    char input[100];
    printf("Enter security code:");
    fflush(stdout);
    scanf("%99s", input);
    int code = getcode();
    int usr_code = atoi(input);
    if(usr_code != code) {
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