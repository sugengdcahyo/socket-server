#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>

#define PORT 8080

void handle_client(int client_socket) {
    unsigned char key[16] = "encryptionkey123"; // 16-byte AES key
    unsigned char iv[16] = "initialvector123";  // 16-byte Initialization Vector (IV)
    AES_KEY aes_key;

    // Prepare AES key for encryption/decryption
    AES_set_encrypt_key(key, 128, &aes_key);

    unsigned char buffer[1024] = {0};
    read(client_socket, buffer, sizeof(buffer));
    printf("Encrypted message received: %s\n", buffer);

    // Decrypt the message
    unsigned char decrypted_message[1024] = {0};
    int num = 0;  // Counter for AES-CFB mode
    AES_cfb128_encrypt(buffer, decrypted_message, strlen((char *)buffer), &aes_key, iv, &num, AES_DECRYPT);

    printf("Decrypted message: %s\n", decrypted_message);

    close(client_socket);
}

void start_server() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d\n", PORT);

    // Accept connections
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        handle_client(client_socket);
    }

    close(server_fd);
}

