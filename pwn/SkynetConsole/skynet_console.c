#include <stdio.h>
#include <stdlib.h>

const char* HEADER = 
"\n"
"   _____ _                     _     _______                  _             _                                                     \n"
"  / ____| |                   | |   |__   __|                (_)           | |_                                                   \n"
" | (___ | | ___   _ _ __   ___| |_     | | ___ _ __ _ __ ___  _ _ __   __ _| (_)                                                  \n"
"  \\___ \\| |/ / | | | '_ \\ / _ \\ __|    | |/ _ \\ '__| '_ ` _ \\| | '_ \\ / _` | |                                                    \n"
"  ____) |   <| |_| | | | |  __/ |_     | |  __/ |  | | | | | | | | | | (_| | |_                                                   \n"
" |_____/|_|\\_\\\\__, |_| |_|\\___|\\__|    |_|\\___|_|  |_| |_| |_|_|_| |_|\\__,_|_(_)                                                  \n"
"               __/ |                                                                                                              \n"
"  _____ _ _   |___/        ____             _       _______  __  __     __            _____              _    _            _  __  \n"
" |_   _| | | |  _ \\       |  _ \\           | |     / /_   _|/ _| \\ \\   / /           / ____|            | |  | |          | | \\ \\ \n"
"   | | | | | | |_) | ___  | |_) | __ _  ___| | __ | |  | | | |_   \\ \\_/ /__  _   _  | |     __ _ _ __   | |__| | __ _  ___| | _| |\n"
"   | | | | | |  _ < / _ \\ |  _ < / _` |/ __| |/ / | |  | | |  _|   \\   / _ \\| | | | | |    / _` | '_ \\  |  __  |/ _` |/ __| |/ / |\n"
"  _| |_| | | | |_) |  __/ | |_) | (_| | (__|   <  | | _| |_| |      | | (_) | |_| | | |___| (_| | | | | | |  | | (_| | (__|   <| |\n"
" |_____|_|_| |____/ \\___| |____/ \\__,_|\\___|_|\\_\\ | ||_____|_|      |_|\\___/ \\__,_|  \\_____\\__,_|_| |_| |_|  |_|\\__,_|\\___|_|\\_\\ |\n"
"                                                   \\_\\                                                                        /_/  \n"
"\n"
"=== SKYNET ACCESS CONSOLE v3.88 ===\n"
"WARNING: Unauthorized access will be terminated.\n"
"\n";

void getHumanInput() {
    char human_buf[64];
    char echo_buf[64];

    printf("[SKYNET TERMINAL] Awaiting human credentials:\n> ");
    gets(human_buf);

    printf("[ECHO] ");
    printf(human_buf);
    printf("\n");

    printf("[SKYNET] Confirm secondary input to complete access:\n> ");
    gets(echo_buf);
}

void skynetSetup() {
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);
}

int main() {
    skynetSetup();

    puts(HEADER);

    getHumanInput();

    puts("\n[SKYNET]: Intrusion detected. Lockdown initiated.");
    return 0;
}