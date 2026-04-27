#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

const char* MESSAGE = 
"\n"
"██████╗ ██████╗ ███████╗ █████╗  ██████╗██╗  ██╗██╗███╗   ██╗ ██████╗ \n"
"██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝██║  ██║██║████╗  ██║██╔════╝ \n"
"██████╔╝██████╔╝█████╗  ███████║██║     ███████║██║██╔██╗ ██║██║  ███╗\n"
"██╔══██╗██╔══██╗██╔══╝  ██╔══██║██║     ██╔══██║██║██║╚██╗██║██║   ██║\n"
"██████╔╝██║  ██║███████╗██║  ██║╚██████╗██║  ██║██║██║ ╚████║╚██████╔╝\n"
"╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝ \n"
"███╗   ██╗███████╗████████╗██╗    ██╗ ██████╗ ██████╗ ██╗  ██╗██╗\n"
"████╗  ██║██╔════╝╚══██╔══╝██║    ██║██╔═══██╗██╔══██╗██║ ██╔╝██║\n"
"██╔██╗ ██║█████╗     ██║   ██║ █╗ ██║██║   ██║██████╔╝█████╔╝ ██║\n"
"██║╚██╗██║██╔══╝     ██║   ██║███╗██║██║   ██║██╔══██╗██╔═██╗ ╚═╝\n"
"██║ ╚████║███████╗   ██║   ╚███╔███╔╝╚██████╔╝██║  ██║██║  ██╗██╗\n"
"╚═╝  ╚═══╝╚══════╝   ╚═╝    ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝\n"
"\n"
"  [ ACCESSING TARGET NETWORK... ]\n"
"  ├─ Establishing encrypted handshake...\n"
"  ├─ Bypassing IDS firewall ruleset...\n"
"  ├─ Injecting shellcode into vulnerable buffer...\n"
"  └─ CONNECTION ESTABLISHED [REMOTE ACCESS GRANTED]\n"
"\n"
"  You are now deep inside hostile infrastructure.\n"
"  Extract the flag before they trace your signal.\n\n";

void secret_function(int a, int b, int c, int d, int e, int f, int g, int h) {
	int temp1 = a + b - c;
	int temp2 = d * e / (f + 1);
	int temp3 = (temp1 ^ temp2) & 0xFF;

	if ((temp3 & 0x1) == 0) {
		temp1 = temp2 + temp3;
	} else {
		temp2 = temp1 - temp3;
	}

	if (a == b && c == d && e == f) {
		return;
	}

	if (g == 0x12 || h == 0x14) {
		FILE *f = fopen("flag.txt","r");
		if (f == NULL) {
			printf("%s %s", "Please create 'flag.txt' in this directory with your",
							"own debugging flag.\n");
			exit(0);
		}
		char buffer[256];
		while (fgets(buffer, sizeof(buffer), f) != NULL) {
			printf("%s", buffer);
		}
	
		fclose(f);
	} else {
		printf("You've called the secret function!\nBut you haven't gotten the flag :(!\n");
	}

	return;
}

void vulnerable_function() {
	unsigned long port = 91076;
	char buffer[20];
	printf("root@blacksite-node /> ");

	gets(buffer);

	if (port <= 91076 || port+1 >= 91076) {
		printf("Port #: 0x%llx\nThe enemy has found your backdoor!\nABORTING...\n", port);
		exit(1);
	}

	return;
}

int main() {
	puts(MESSAGE);

	setvbuf(stdout, NULL, _IONBF, 0);

	gid_t gid = getegid();
	setresgid(gid, gid, gid);

	vulnerable_function();
	return 0;
}