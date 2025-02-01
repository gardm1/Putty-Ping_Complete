#include "libgcrypt_crypto.h"
#include "process.h"


/* PROCESS.c (& .h)
*/

__declspec(dllexport) int __stdcall _dll_EXECUTE_COMMAND(const char* argv) {
	return EXECUTE_COMMAND(argv);
}

__declspec(dllexport) int __stdcall _dll_PING_INET_ADDR(const char* argv) {
	return PING_INET_ADDR(argv);
}

/* LIBGCRYPT_CRYPTO.c (& .h)
*/

__declspec(dllexport) int __stdcall _dll_ENCRYPT_FILE(const char* input_file_path, const char* output_file_path, const char* key_file_path) {
	return ENCRYPT_FILE(input_file_path, output_file_path, key_file_path);
}

__declspec(dllexport) int __stdcall _dll_DECRYPT_FILE(const char* input_file_path, const char* output_file_path, const char* key_file_path) {
	return DECRYPT_FILE(input_file_path, output_file_path, key_file_path);
}
