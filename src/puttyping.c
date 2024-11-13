#include "process.h"
#include "dataHandler.h"

//#include <gcrypt.h>
#include <gcrypt.h>

#include <stdio.h>

int main() {

	printf("libgcrypt version: %s\n", gcry_check_version(NULL));

	printf("Hello!\n");

	//const char* command = "cmd.exe /c dir /a:d /s /b \"C:\\Users\\gardm\\.putty\\sessions\\\" | find /c \":\\\"";
	//printf("%s\n", command);

	DataHandler dh;
	DataHandler_con(&dh);

	//const char* fname = "testing_server";

	EXECUTECOMMAND("calc.exe");

	



	//DataHandler_addTXT(&dh, "testing_001", "000.222.333.111", "testing_again");
	//DataHandler_readTXT(&dh, "testing_001");
	//const char* ip = DataHandler_getSrv(&dh);
	//printf("\n--> %s\n", ip);
	//DataHandler_removeTXT(&dh, "testing_001");



	//PING_INET_ADDR(ip);

	DataHandler_decon(&dh);


	(void)getchar();
	return 0;
}

