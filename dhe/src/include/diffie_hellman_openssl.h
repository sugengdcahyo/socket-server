#ifndef DIFFIE_HELLMAN_OPENSSL_H
#define DIFFIE_HELLMAN_OPENSSL_H

#include <openssl/bn.h>
#include <openssl/rand.h>

typedef struct {
  BIGNUM *prime;
  BIGNUM *generator;
  BIGNUM *private_key;
  BIGNUM *public_key;
} DH_Keys;


void dh_generator_keys_openssl(DH_Keys *keys);

BIGNUM *dh_compute_shared_secret_openssl(DH_Keys *keys, BIGNUM *peer_public_key);

#endif
