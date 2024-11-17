#include "rsa.h"

static gcry_sexp_t rsa_keypair;

void generate_rsa_keys() {
	gcry_sexp_t params;
	gcry_sexp_build(&params, NULL, "(genkey (rsa (nbits 4:2048)))");
	gcry_pk_genkey(&rsa_keypair, params);
	gcry_sexp_release(params);
}

void save_rsa_keys() {
	size_t len = gcry_sexp_sprint(rsa_keypair, GCRYSEXP_FMT_CANON, NULL, 0);
	unsigned char* serialized_keys = (unsigned char*)malloc(len);
	if (!serialized_keys) {
		fprintf(stderr, "Error: Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	gcry_sexp_sprint(rsa_keypair, GCRYSEXP_FMT_CANON, (char*)serialized_keys, len);

	FILE* file = fopen("rsa_keypair.bin", "wb");
	if (!file) {
		fprintf(stderr, "Error: Could not save RSA keys.\n");
		free(serialized_keys);
		exit(EXIT_FAILURE);
	}
	fwrite(serialized_keys, 1, len, file);
	fclose(file);
	free(serialized_keys);
}

void load_rsa_keys() {
	FILE* file = fopen("rsa_keypair.bin", "rb");
	if (!file) {
		fprintf(stderr, "Warning: RSA keys not found. Generating new keys.\n");
		generate_rsa_keys();
		save_rsa_keys();
		return;
	}
	fseek(file, 0, SEEK_END);
	long len = ftell(file);
	fseek(file, 0, SEEK_SET);

	unsigned char* serialized_keys = (unsigned char*)malloc(len);
	if (!serialized_keys) {
		fprintf(stderr, "Error: Memory allocation failed.\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	fread(serialized_keys, 1, len, file);
	fclose(file);

	gcry_error_t err = gcry_sexp_new(&rsa_keypair, serialized_keys, len, 1);
	if (err) {
		fprintf(stderr, "Error: Failed to load RSA keys: %s\n", gcry_strerror(err));
		free(serialized_keys);
		exit(EXIT_FAILURE);
	}
	free(serialized_keys);
}

int encrypt_aes_key(const unsigned char* aes_key, unsigned char* encrypted_key) {
	gcry_sexp_t data, ciphertext;
	gcry_sexp_build(&data, NULL, "(data (flags raw) (value %b))", AES_KEY_SIZE, aes_key);
	if (gcry_pk_encrypt(&ciphertext, data, gcry_sexp_nth(rsa_keypair, 0)) != 0) return -1;

	gcry_mpi_t mpi = gcry_sexp_nth_mpi(ciphertext, 0, GCRYMPI_FMT_USG);
	gcry_mpi_print(GCRYMPI_FMT_USG, encrypted_key, 256, NULL, mpi);
	gcry_mpi_release(mpi);
	gcry_sexp_release(data);
	gcry_sexp_release(ciphertext);
	return 256;
}

int decrypt_aes_key(const unsigned char* encrypted_key, unsigned char* aes_key) {
	gcry_sexp_t ciphertext, plaintext;
	gcry_mpi_t mpi;
	gcry_mpi_scan(&mpi, GCRYMPI_FMT_USG, encrypted_key, 256, NULL);
	gcry_sexp_build(&ciphertext, NULL, "(enc-val (rsa (a %M)))", mpi);

	if (gcry_pk_decrypt(&plaintext, ciphertext, rsa_keypair) != 0) return -1;

	memcpy(aes_key, gcry_sexp_nth_data(plaintext, 1, NULL), AES_KEY_SIZE);
	gcry_mpi_release(mpi);
	gcry_sexp_release(ciphertext);
	gcry_sexp_release(plaintext);
	return 0;
}