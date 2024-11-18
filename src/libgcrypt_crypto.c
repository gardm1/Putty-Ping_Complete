#include "libgcrypt_crypto.h"

static void save_key_iv(const unsigned char* key, const unsigned char* iv, const char* key_file_path) {
	FILE* file = fopen(key_file_path, "wb");
	fwrite(key, 1, AES_KEY_SIZE, file);
	fwrite(iv, 1, AES_BLOCK_SIZE, file);
	fclose(file);
}

static int load_key_iv(unsigned char* key, unsigned char* iv, const char* key_file_path) {
	FILE* file = fopen(key_file_path, "rb");
	if (!file) return -1;
	fread(key, 1, AES_KEY_SIZE, file);
	fread(iv, 1, AES_BLOCK_SIZE, file);
	fclose(file);
	return 0;
}

static int check_mem_alloc_fail(unsigned char* c, FILE* file) {
	if (!c) {
		fprintf(stderr, "Error: Memory allocation failed.\n");
		if (file) fclose(file);
		return -1;
	}
	return 0;
}

int ENCRYPT_FILE(const char* input_file_path, const char* output_file_path, const char* key_file_path) {
	unsigned char aes_key[AES_KEY_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];
	unsigned char hmac[32];
	unsigned char hmac_key[AES_KEY_SIZE];

	// Generate AES key and IV
	generate_aes_key(aes_key);
	gcry_randomize(iv, AES_BLOCK_SIZE, GCRY_STRONG_RANDOM);
	memcpy(hmac_key, aes_key, AES_KEY_SIZE);

	// Read plaintext from file
	FILE* plaintext_file = fopen(input_file_path, "rb");
	if (!plaintext_file) {
		fprintf(stderr, "Error: Could not open input file for reading.\n");
		return -1;
	}
	fseek(plaintext_file, 0, SEEK_END);
	long plaintext_len = ftell(plaintext_file);
	fseek(plaintext_file, 0, SEEK_SET);

	unsigned char* plaintext = (unsigned char*)malloc(plaintext_len);
	if (check_mem_alloc_fail(plaintext, plaintext_file)) {
		return -1;
	}

	fread(plaintext, 1, plaintext_len, plaintext_file);
	fclose(plaintext_file);

	// Allocate memory for ciphertext
	unsigned char* ciphertext = (unsigned char*)malloc(plaintext_len + AES_BLOCK_SIZE);
	int ciphertext_len = encrypt_aes(plaintext, plaintext_len, aes_key, iv, ciphertext);

	// Write ciphertext to output file
	FILE* ciphertext_file = fopen(output_file_path, "wb");
	if (check_mem_alloc_fail(ciphertext, ciphertext_file)) {
		free(plaintext);
		return -1;
	}

	fwrite(ciphertext, 1, ciphertext_len, ciphertext_file);
	fclose(ciphertext_file);

	// Save AES key and IV
	save_key_iv(aes_key, iv, key_file_path);

	// Compute and save HMAC
	compute_hmac(ciphertext, ciphertext_len, hmac_key, hmac);
	char hmac_file_path[256];
	snprintf(hmac_file_path, sizeof(hmac_file_path), "%s.hmac", output_file_path);
	FILE* hmac_file = fopen(hmac_file_path, "wb");
	fwrite(hmac, 1, sizeof(hmac), hmac_file);
	fclose(hmac_file);

	free(plaintext);
	free(ciphertext);
	return 0;
}

int DECRYPT_FILE(const char* input_file_path, const char* output_file_path, const char* key_file_path) {
	unsigned char aes_key[AES_KEY_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];
	unsigned char hmac[32];
	unsigned char hmac_key[AES_KEY_SIZE];

	// Load AES key and IV
	if (load_key_iv(aes_key, iv, key_file_path) != 0) {
		fprintf(stderr, "Error: Key file not found.\n");
		return -1;
	}
	memcpy(hmac_key, aes_key, AES_KEY_SIZE);

	// Read ciphertext from file
	FILE* ciphertext_file = fopen(input_file_path, "rb");
	if (!ciphertext_file) {
		fprintf(stderr, "Error: Could not open input file for reading.\n");
		return -1;
	}
	fseek(ciphertext_file, 0, SEEK_END);
	long ciphertext_len = ftell(ciphertext_file);
	fseek(ciphertext_file, 0, SEEK_SET);

	unsigned char* ciphertext = (unsigned char*)malloc(ciphertext_len);
	if (check_mem_alloc_fail(ciphertext, ciphertext_file)) {
		return -1;
	}

	fread(ciphertext, 1, ciphertext_len, ciphertext_file);
	fclose(ciphertext_file);

	// Verify HMAC
	char hmac_file_path[256];
	snprintf(hmac_file_path, sizeof(hmac_file_path), "%s.hmac", input_file_path);
	FILE* hmac_file = fopen(hmac_file_path, "rb");
	if (!hmac_file) {
		fprintf(stderr, "Error: HMAC file not found.\n");
		free(ciphertext);
		return -1;
	}
	fread(hmac, 1, sizeof(hmac), hmac_file);
	fclose(hmac_file);

	unsigned char computed_hmac[32];
	compute_hmac(ciphertext, ciphertext_len, hmac_key, computed_hmac);
	if (memcmp(hmac, computed_hmac, sizeof(hmac)) != 0) {
		fprintf(stderr, "Error: HMAC verification failed.\n");
		free(ciphertext);
		return -1;
	}

	// Decrypt ciphertext
	unsigned char* plaintext = (unsigned char*)malloc(ciphertext_len);
	int plaintext_len = decrypt_aes(ciphertext, ciphertext_len, aes_key, iv, plaintext);

	// Write plaintext to output file
	FILE* plaintext_file = fopen(output_file_path, "wb");
	if (check_mem_alloc_fail(plaintext, plaintext_file)) {
		free(ciphertext);
		return -1;
	}

	fwrite(plaintext, 1, plaintext_len, plaintext_file);
	fclose(plaintext_file);

	free(ciphertext);
	free(plaintext);
	return 0;
}

