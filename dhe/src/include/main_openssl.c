#include "diffie_hellman_openssl.h"
#include <stdio.h>
#include <openssl/bn.h>


int main() {
  DH_Keys server_keys;
  DH_Keys client_keys;

  dh_generator_keys_openssl(&server_keys);
  dh_generator_keys_openssl(&client_keys);

  BIGNUM *server_secret = dh_compute_shared_secret_openssl(&server_keys, client_keys.public_key);
  BIGNUM *client_secret = dh_compute_shared_secret_openssl(&client_keys, server_keys.public_key);

  if (BN_cmp(server_secret, client_secret) == 0) {
    printf("Shared secret berhasil disepakati!\n");
  } else {
    printf("Shared secret tidak cocok!\n");
  }

  char *secret_hex = BN_bn2hex(server_secret);
  printf("Shared secret (Hex): %s\n", secret_hex);
  
  OPENSSL_free(secret_hex);
  BN_free(server_secret);
  BN_free(client_secret);

  BN_free(server_keys.prime);
  BN_free(server_keys.generator);
  BN_free(server_keys.private_key);
  BN_free(server_keys.public_key);

  BN_free(client_keys.prime);
  BN_free(client_keys.generator);
  BN_free(client_keys.private_key);
  BN_free(client_keys.public_key);


  return 0;
}
