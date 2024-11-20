#include "diffie_hellman.h"
#include <stdint.h>
#include <stdio.h>


int main() {
  /* prime value
  * FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1
  * 29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD
  * EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245
  * E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED
  * EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE65381
  * FFFFFFFFFFFFFFFF
  */

  // uint64_t prime = 0xFFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD1;
  uint64_t prime = 199912;
  uint64_t generator = 11;

  DH_Keys server_keys = {prime, generator, 0, 0};
  dh_generator_keys(&server_keys);
  
  DH_Keys client_keys = {prime, generator, 0, 0};
  dh_generator_keys(&client_keys);

  printf("Server public keys: %llu\n", server_keys.public_key);
  printf("Client public keys: %llu\n", client_keys.public_key);


  uint64_t server_secret = dh_compute_shared_secret(&server_keys, server_keys.public_key);
  uint64_t client_secret = dh_compute_shared_secret(&client_keys, client_keys.public_key);

  printf("Server shared secret: %llu\n", server_secret);
  printf("Client shared secret: %llu\n", client_secret);

  if (server_secret == client_secret) {
    printf("Shared secret berhasil disepakati!\n");
  } else {
    printf("Kesalahan generate shared secret!\n");
  }

  return 0;
}
