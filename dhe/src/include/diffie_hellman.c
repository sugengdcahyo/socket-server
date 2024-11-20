#include "diffie_hellman.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


void dh_generator_keys(DH_Keys *keys) {
  srand(time(NULL));
  keys->private_key = rand() % (keys->prime - 2) + 1; // random private keys
  keys->public_key  = mod_exp(keys->generator, keys->private_key, keys->prime); // generate public keys
}


uint64_t dh_compute_shared_secret(DH_Keys *keys, uint64_t peer_public_key) {
  return mod_exp(peer_public_key, keys->private_key, keys->prime);
}


uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
  uint64_t result = 1;
  base = base % mod;

  while (exp > 0) {
    if (exp % 2 == 1) {
      result = (result * base) % mod;
    }
    exp = exp >> 1; // ekspansi integer shifting (exp / 2)
    base = (base * base) % mod;
  }
  
  return result;
}


