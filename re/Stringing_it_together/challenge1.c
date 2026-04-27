#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


void generate_string(char *s) {
	int chars[24] = {114, 101, 97, 108, 108, 121, 99, 111, 111, 108, 100, 101, 118, 64, 112, 97, 114, 115, 111, 110, 115, 46, 117, 115};

	for (int i=0; i<24; i++) {
		s[i] = (char)chars[i];
	}
}

extern char admin_pass[];
int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);

	char input_email[100];
	char admin_email[100];
	char input_pass[100];
	
	puts("WARNING!! To use this service you must be an admin.\n[This challenge requires knowlege about encryption.]\n");
	
	printf("Email: ");
	fgets(input_email, sizeof(input_email), stdin);
	input_email[strcspn(input_email, "\n")] = '\0';

	generate_string(admin_email);

	printf("Password: ");
	fgets(input_pass, sizeof(input_pass), stdin);
	input_pass[strcspn(input_pass, "\n")] = '\0';

	if ((strcmp(input_email, admin_email) != 0) || (strcmp(input_pass, admin_pass) != 0)) {
		puts("ERROR!! You are not the admin. Please try again.");
		return 1;
	} else {
		FILE* f = fopen("flag.txt", "r");

		char flagbuf[50];
		fgets(flagbuf, sizeof(flagbuf), f);
		printf("%s\n", flagbuf);
	}

	return 0;
}