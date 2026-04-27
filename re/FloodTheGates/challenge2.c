#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);

	char givenPassword[20];
	char realPassword[20];

	puts("What is the password?");

	strncpy(realPassword, "dddddddddddddddddddd", 20);
	gets(givenPassword); // aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaParsons is the best!
	
	if (0 == strncmp("Parsons is the best!", realPassword, 20)){
		printf("SUCCESS!\n");
		FILE* f = fopen("flag.txt", "r");

		char flagbuf[50];
		fgets(flagbuf, 50, f);
		printf("%s\n", flagbuf);
	} else {
		printf("FAILURE!\nIt looks like you don't know how this works...\n");
	}

	raise(SIGINT);
	return 0;
}