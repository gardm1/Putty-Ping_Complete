#include "rsa.h"

static gcry_sexp_t rsa_keypair;

void generate_rsa_keys() {
	gcry_sexp_t params;
	gcry_sexp_build(&params, NULL, "(genkey (rsa (nbits 4:2048)))");
	gcry_pk_genkey(&rsa_keypair, params);
	gcry_sexp_release(params);
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