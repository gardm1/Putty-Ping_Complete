#include "process.h"
#include "dataHandler.h"

#include "libgcrypt_crypto.h"

//#include <gcrypt.h>
#include <gcrypt.h>

#include <stdio.h>

int main() {

	printf("libgcrypt version: %s\n", gcry_check_version(NULL));

	const char* key_file = "aes_key.dat";
	const char* output_file = "encrypted_output_test.dat";
	const char* input_file = "";

	//// Encrypt
	//if (encrypt_file(input_file, output_file, key_file) == 0) {
	//	printf("File encrypted successfully.\n");
	//}
	//else {
	//	fprintf(stderr, "Error encrypting file.\n");
	//}

	//// Decrypt
	//if (decrypt_file("encrypted_output_test.dat", "decrypted_output_test.txt", key_file) == 0) {
	//	printf("File decrypted successfully.\n");
	//}
	//else {
	//	fprintf(stderr, "Error decrypting file.\n");
	//}

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



	//PING_INET_ADDR("000.111.222.333");

	DataHandler_decon(&dh);


	(void)getchar();
	return 0;
}

