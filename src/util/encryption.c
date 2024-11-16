#include "encryption.h"

void generate_aes_key(unsigned char* key) {
	gcry_randomize(key, AES_KEY_SIZE, GCRY_STRONG_RANDOM);
}

int encrypt_aes(const unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext) {
	gcry_cipher_hd_t handle;
	gcry_cipher_open(&handle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	gcry_cipher_setkey(handle, key, AES_KEY_SIZE);
	gcry_cipher_setiv(handle, iv, AES_BLOCK_SIZE);

	int padding_len = AES_BLOCK_SIZE - (plaintext_len % AES_BLOCK_SIZE);
	int padded_len = plaintext_len + padding_len;

	unsigned char* padded_plaintext = (unsigned char*)malloc(padded_len);
	if (!padded_plaintext) {
		gcry_cipher_close(handle);
		return -1;
	}

	memcpy(padded_plaintext, plaintext, plaintext_len);
	memset(padded_plaintext + plaintext_len, padding_len, padding_len);

	int result = gcry_cipher_encrypt(handle, ciphertext, padded_len, padded_plaintext, padded_len);
	free(padded_plaintext);
	gcry_cipher_close(handle);

	return result == 0 ? padded_len : -1;
}

int decrypt_aes(const unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext) {
	gcry_cipher_hd_t handle;
	gcry_cipher_open(&handle, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	gcry_cipher_setkey(handle, key, AES_KEY_SIZE);
	gcry_cipher_setiv(handle, iv, AES_BLOCK_SIZE);

	gcry_cipher_decrypt(handle, plaintext, ciphertext_len, ciphertext, ciphertext_len);
	int padding_len = plaintext[ciphertext_len - 1];
	gcry_cipher_close(handle);
	return ciphertext_len - padding_len;
}

