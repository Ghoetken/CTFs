#include <stdio.h>
#include <string.h>

const char* ALIEN = 
"\n"
"     ____          ____           \n"
"    |oooo|        |oooo|          \n"
"    |oooo| .----. |oooo|          \n"
"    |Oooo|/\\_||_/\\|oooO|        \n"
"    `----' / __ \\ `----'         \n"
"    ,/ |#|/\\/__\\/\\|#| \\,      \n"
"   /  \\|#|| |/\\| ||#|/  \\      \n"
"  / \\_/|_|| |/\\| ||_|\\_/ \\    \n"
" |_\\/    o\\=----=/o    \\/_|    \n"
" <_>      |=\\__/=|      <_>      \n"
" <_>      |------|      <_>       \n"
" | |   ___|======|___   | |       \n"
"//\\\\  / |O|======|O| \\  //\\\\ \n"
"|  |  | |O+------+O| |  |  |      \n"
"|\\/|  \\_+/        \\+_/  |\\/|  \n"
"\\__/  _|||        |||_  \\__/    \n"
"      | ||        || |            \n"
"     [==|]        [|==]           \n"
"     [===]        [===]           \n"
"      >_<          >_<            \n"
"     || ||        || ||           \n"
"     || ||        || ||           \n"
"     || ||        || ||           \n"
"   __|\\_/|__    __|\\_/|__       \n"
"  /___n_n___\\  /___n_n___\\      \n"
"\n\n"
"<<< ALERT: HUMAN DETECTED IN ROBOT COMMAND CENTER >>>\n"
"<<< SYSTEM LOCKDOWN INITIATED >>>                    \n"
"<<< ALERT! TERMINAL ACTIVATED >>>                    \n"
"\n";

void robot_command_center(int access_code) {
    if (access_code == 0xcafebabe) {
        FILE* flag_file = fopen("flag.txt", "r");

        char flag_buffer[50];
        fgets(flag_buffer, 50, flag_file);
        printf("TRANSMISSION COMPLETE: FLAG ACQUIRED -> %s\n", flag_buffer);
    } else {
        printf("ERROR: INVALID ACCESS CODE! SELF-DESTRUCT SEQUENCE INITIATED!\n");
    }
}

void robot_terminal() {
    char user_input[64];
    puts("!!ROBOT TERMINAL ONLINE!!\n");
    printf("ENTER COMMAND INPUT $> ");
    gets(user_input);
}

int main() {
    setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);

    puts(ALIEN);

    robot_terminal();

    puts("ERROR! ERROR! SELF-DESTRUCT SEQUENCE INITIATED!");
    return 0;
}
