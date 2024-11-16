#ifndef __ENCRYPTION_H
#define __ENCRYPTION_H

#include <gcrypt.h>

#include <string.h>

#define AES_KEY_SIZE 32
#define AES_BLOCK_SIZE 16

void generate_aes_key(unsigned char* key);
int encrypt_aes(const unsigned char* plaintext, int plaintext_len, unsigned char* key, unsigned char* iv, unsigned char* ciphertext);
int decrypt_aes(const unsigned char* ciphertext, int ciphertext_len, unsigned char* key, unsigned char* iv, unsigned char* plaintext);

#endif // ENCRYPTION_H

