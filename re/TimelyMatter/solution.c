#include <stdlib.h>
#include <stdio.h>   
#include <time.h>
#include <unistd.h>

#include <string.h>

int getone(int sec, int min){
    srand(min*sec);
    return (rand()%9000) + 1000;
}

int gettwo(int sec, int hour){
    srand(hour*sec);
    return (rand()%9000) + 1000;
}

int getthree(int sec, int day){
    srand(day*sec);
    return (rand()%9000) + 1000;
}

int getcode(int sec, int min, int hour, int day) {

    if ((min%2) == 0) {
        return getone(sec, min);
    } else if ((hour%3) != 0){
        return gettwo(sec, hour);
    } else {
        return getthree(sec, day);
    }
}

int main(int argc, char *argv[]){
    if(argc < 5){
        printf("USE: ./solution <sec> <min> <hour> <day>\n");
        return 0;
    }
    int sec = atoi(argv[1]);
    int min = atoi(argv[2]);
    int hour = atoi(argv[3]);
    int day = atoi(argv[4]);
    int code = getcode(sec,min,hour,day);
    printf("%d\n",code);

    while (1) {
        time_t now = time(NULL);
        struct tm *lt = localtime(&now);
        printf("\rLive seconds: %02d", lt->tm_sec); // \r to overwrite
        fflush(stdout);
        sleep(.3);
    }

    return 0;
}

//This code will help you generate a code to be entered at a precise time to work
//This script will provide the correct code which changes depending on the time, also provides a counter in seconds
//to help you enter your code at the right second