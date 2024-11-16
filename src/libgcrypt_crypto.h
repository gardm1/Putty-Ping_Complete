#ifndef __LIBGCRYPT_CRYPTO_H
#define __LIBGCRYPT_CRYPTO_H

/* 
* encryption.c & .h: Handle AES-256 encryption/decryption
* rsa.c & .h: Handle RSA encryption of the AES key
* hmac.c & .h: Handle HMAC-SHA256 for integrity checking
*/

#include "util\encryption.h"
#include "util\hmac.h"

#include <gcrypt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int encrypt_file(const char* input_file_path, const char* output_file_path, const char* key_file_path);

int decrypt_file(const char* input_file_path, const char* output_file_path, const char* key_file_path);

#endif // __LIBGCRYPT_CRYPTO_H

