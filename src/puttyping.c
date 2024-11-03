#include "process.h"
#include "dataHandler.h"

#include <stdio.h>

int main() {


	printf("Hello!\n");

	const char* command = "calc.exe";

	DataHandler dh;
	DataHandler_con(&dh);

	//const char* fname = "testing_server";

	//EXECUTECOMMAND(command);
	


	//DataHandler_addTXT(&dh, "testing_001", "000.222.333.111", "testing_again");
	//DataHandler_readTXT(&dh, "testing_001");
	//const char* ip = DataHandler_getSrv(&dh);
	//printf("\n--> %s\n", ip);
	//DataHandler_removeTXT(&dh, "testing_001");

	char** sessions__ = DataHandler_getSessionNames(&dh);


	//PING_INET_ADDR(ip);

	DataHandler_decon(&dh);


	(void)getchar();
	return 0;
}

