#include "process.h"

#include <stdio.h>

int main() {


	printf("Hello!\n");

	const char* command = "calc.exe";

	EXECUTECOMMAND(command);
	

	const char* ip = "000.111.222.333";

	//PING_INET_ADDR(ip);


	(void)getchar();
	return 0;
}

