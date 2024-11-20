#include "tcp_server.h"
#include "include/diffie_hellman_openssl.h"
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


void xor_encrypt_decrypt(const char *input, char *output, const unsigned char *key, int key_len) {
  for (size_t i = 0; i < strlen(input); i++) {
    output[i] = input[i] ^ key[i % key_len];
  }
  output[strlen(input)] = '\0';
}


// Fungsi untuk menangani setiap koneksi klien
void *handle_client(void *client_socket) {
    int client_fd = *(int *)client_socket;
    free(client_socket); // Bebaskan memori alokasi untuk socket klien
    char buffer[1024] = {0};

    // Inisialisasi Diffie-Hellman untuk server
    DH_Keys server_keys;
    dh_generator_keys_openssl(&server_keys);

    // Kirim kunci publik server ke klien
    char *server_public_key_hex = BN_bn2hex(server_keys.public_key);
    send(client_fd, server_public_key_hex, strlen(server_public_key_hex), 0);
    printf("Server public key sent: %s\n", server_public_key_hex);
    OPENSSL_free(server_public_key_hex);

    // Terima kunci publik klien
    memset(buffer, 0, sizeof(buffer));
    read(client_fd, buffer, sizeof(buffer));
    printf("Received client public key: %s\n", buffer);

    // Validasi format hexadecimal
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!((buffer[i] >= '0' && buffer[i] <= '9') || (buffer[i] >= 'A' && buffer[i] <= 'F'))) {
            fprintf(stderr, "Invalid public key format. Expected hexadecimal.\n");
            close(client_fd);
            return NULL;
        }
    }

    // Konversi kunci publik klien menjadi BIGNUM
    BIGNUM *client_public_key = NULL;
    if (!BN_hex2bn(&client_public_key, buffer)) {
        fprintf(stderr, "Failed to parse client public key.\n");
        close(client_fd);
        return NULL;
    }

    // Hitung shared secret
    BIGNUM *shared_secret = dh_compute_shared_secret_openssl(&server_keys, client_public_key);
    char *shared_secret_hex = BN_bn2hex(shared_secret);
    printf("Shared secret (Server): %s\n", shared_secret_hex);

    // Hash shared secret untuk menghasilkan kunci enkripsi
    unsigned char shared_secret_hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)shared_secret_hex, strlen(shared_secret_hex), shared_secret_hash);
    OPENSSL_free(shared_secret_hex);

    // Terima data terenkripsi dari klien
    memset(buffer, 0, sizeof(buffer));
    read(client_fd, buffer, sizeof(buffer));
    printf("Received encrypted data: %s\n", buffer);

    // Dekripsi data
    char decrypted[1024] = {0};
    xor_encrypt_decrypt(buffer, decrypted, shared_secret_hash, SHA256_DIGEST_LENGTH);
    printf("Decrypted data: %s\n", decrypted);

    // Cleanup
    BN_free(shared_secret);
    BN_free(client_public_key);
    BN_free(server_keys.prime);
    BN_free(server_keys.public_key);
    BN_free(server_keys.private_key);
    BN_free(server_keys.generator);

    close(client_fd);
    return NULL;
}

// Fungsi utama server
void start_server(int port) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Buat socket server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed.");
        exit(EXIT_FAILURE);
    }

    // Konfigurasi alamat server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket ke port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Dengarkan koneksi masuk
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d and waiting for connections...\n", port);

    while (1) {
        // Terima koneksi dari klien
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        printf("New client connected.\n");

        // Alokasikan memori untuk socket klien
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_fd;

        // Buat thread untuk menangani klien
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)new_sock) != 0) {
            perror("Failed to create thread");
            free(new_sock);
            close(client_fd);
        }

        // Detach thread agar otomatis dibersihkan
        pthread_detach(thread_id);
    }

    // Tutup socket server
    close(server_fd);
}