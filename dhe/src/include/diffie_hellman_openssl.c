#include "diffie_hellman_openssl.h"
#include <openssl/bn.h>
#include <stdlib.h>
#include <stdio.h>


void dh_generator_keys_openssl(DH_Keys *keys) {
  BN_CTX *ctx = BN_CTX_new();

  if (!ctx) {
    perror("Failed to allocate BN_CTX");
    exit(EXIT_FAILURE);
  }

  keys->prime = BN_new();
  keys->generator = BN_new();
  keys->private_key = BN_new();
  keys->public_key = BN_new();

  if (!keys->prime || !keys->generator || !keys->private_key || !keys->public_key) {
    perror("Failed to allocate BIGNUMs");
    exit(EXIT_FAILURE);
  }

  // Inisiasi nilai prime dengan generator
  BN_hex2bn(&keys->prime, "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1");
  BN_set_word(keys->generator, 2);

  // Generate private keys
  BN_rand_range(keys->private_key, keys->prime);

  BN_mod_exp(keys->public_key, keys->generator, keys->private_key, keys->prime, ctx);

  BN_CTX_free(ctx);
}

BIGNUM *dh_compute_shared_secret_openssl(DH_Keys *keys, BIGNUM *peer_public_key) {
  BN_CTX *ctx = BN_CTX_new();

  if (!ctx) {
    perror("Failed to allocate BN_CTX");
    exit(EXIT_FAILURE);
  }

  BIGNUM *shared_secret = BN_new();
  if (!shared_secret) {
    perror("Failed to allocate BIGNUM for shared secret");
    exit(EXIT_FAILURE);
  }

  BN_mod_exp(shared_secret, peer_public_key, keys->private_key, keys->prime, ctx);

  BN_CTX_free(ctx);

  return shared_secret;
}
