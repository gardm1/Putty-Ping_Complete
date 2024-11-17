#include "hmac.h"

void compute_hmac(const unsigned char* data, int data_len, unsigned char* key, unsigned char* output) {
	gcry_md_hd_t handle;
	gcry_md_open(&handle, GCRY_MD_SHA256, GCRY_MD_FLAG_HMAC);
	gcry_md_setkey(handle, key, AES_KEY_SIZE);
	gcry_md_write(handle, data, data_len);
	memcpy(output, gcry_md_read(handle, GCRY_MD_SHA256), gcry_md_get_algo_dlen(GCRY_MD_SHA256));
	gcry_md_close(handle);
}

