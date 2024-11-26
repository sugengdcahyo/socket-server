#include "include/diffie_hellman_openssl.h"
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Fungsi untuk mengenkripsi/mendekripsi data menggunakan XOR
void xor_encrypt_decrypt(const char *input, char *output, const unsigned char *key, size_t key_len) {
  size_t input_len = strlen(input);
  for (size_t i = 0; i < input_len; i++) {
    output[i] = input[i] ^ key[i % key_len];
  }
  output[input_len] = '\0';
}

// Implementasi TCP Client dengan Diffie-Hellman
void start_client(const char *server_ip, int port) {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};

  // Inisialisasi Diffie-Hellman untuk klien
  DH_Keys client_keys;
  dh_generator_keys_openssl(&client_keys);

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // Gunakan IP 127.0.0.1 untuk "localhost"
if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  // Terima kunci publik server
  if (read(sock, buffer, sizeof(buffer)) <= 0) {
      perror("Failed to receive server public key");
      close(sock);
      exit(EXIT_FAILURE);
  }
  printf("Received server public key: %s\n", buffer);

  BIGNUM *server_public_key = NULL;
  if (!BN_hex2bn(&server_public_key, buffer)) {
      fprintf(stderr, "Failed to parse server public key\n");
      close(sock);
      exit(EXIT_FAILURE);
  }

  // Kirim kunci publik klien
  char *client_public_key_hex = BN_bn2hex(client_keys.public_key);
  if (!client_public_key_hex) {
    fprintf(stderr, "Failed to convert client public key to hex\n");
    BN_free(server_public_key);
    close(sock);
    exit(EXIT_FAILURE);
  }
  send(sock, client_public_key_hex, strlen(client_public_key_hex), 0);
  printf("Client public key sent: %s\n", client_public_key_hex);
  OPENSSL_free(client_public_key_hex);

  // Hitung shared secret
  BIGNUM *shared_secret = dh_compute_shared_secret_openssl(&client_keys, server_public_key);
  if (!shared_secret) {
    fprintf(stderr, "Failed to compute shared secret\n");
    BN_free(server_public_key);
    close(sock);
    exit(EXIT_FAILURE);
  }
  char *shared_secret_hex = BN_bn2hex(shared_secret);
  if (!shared_secret_hex) {
    fprintf(stderr, "Failed to convert shared secret to hex\n");
    BN_free(shared_secret);
    BN_free(server_public_key);
    close(sock);
    exit(EXIT_FAILURE);
  }
  printf("Shared secret (Client): %s\n", shared_secret_hex);

  // Hash shared secret untuk menghasilkan kunci enkripsi
  unsigned char shared_secret_hash[SHA256_DIGEST_LENGTH];
  SHA256((unsigned char *)shared_secret_hex, strlen(shared_secret_hex), shared_secret_hash);
  OPENSSL_free(shared_secret_hex);

  // Kirim data terenkripsi ke server
  const char *message = "Hello, Secure World!";
  char encrypted[1024] = {0};
  xor_encrypt_decrypt(message, encrypted, shared_secret_hash, SHA256_DIGEST_LENGTH);
  send(sock, encrypted, strlen(encrypted), 0);
  printf("Encrypted data sent: %s\n", encrypted);

  // Cleanup
  BN_free(shared_secret);
  BN_free(server_public_key);
  BN_free(client_keys.prime);
  BN_free(client_keys.generator);
  BN_free(client_keys.private_key);
  BN_free(client_keys.public_key);

  close(sock);
}

int main() {
  const char *server_ip = "0.0.0.0"; // Gunakan IP loopback
  int port = 8484;

  start_client(server_ip, port);
  return 0;
}
