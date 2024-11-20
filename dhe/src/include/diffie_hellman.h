#ifndef DIFFIE_HELLMAN_H
#define DIFFIE_HELLMAN_H

#include <stdint.h>

typedef struct {
  uint64_t prime;
  uint64_t generator;
  uint64_t private_key;
  uint64_t public_key;
} DH_Keys;

// fungsi utama DH generator
void dh_generator_keys(DH_Keys *keys);

// fungsi utama menghitung shared keys
uint64_t dh_compute_shared_secret(DH_Keys *keys, uint64_t peer_public_key);

// fungsi modul exponen
uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod);

#endif
