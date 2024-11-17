#ifndef __RSA_H
#define __RSA_H

#include "encryption.h"

#include <gcrypt.h>

#include <stdio.h>

void generate_rsa_keys();

void save_rsa_keys();
void load_rsa_keys();

int encrypt_aes_key(const unsigned char* aes_key, unsigned char* encrypted_key);
int decrypt_aes_key(const unsigned char* encrypted_key, unsigned char* aes_key);

#endif // __RSA_H
