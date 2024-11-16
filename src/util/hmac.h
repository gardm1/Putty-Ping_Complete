#ifndef __HMAC_H
#define __HMAC_H

#include "encryption.h"

#include <gcrypt.h>

void compute_hmac(const unsigned char* data, int data_len, unsigned char* key, unsigned char* output);

#endif // HMAC_H

