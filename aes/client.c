#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/aes.h>

#define PORT 8080

void start_client() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    unsigned char key[16] = "encryptionkey123"; // 16-byte AES key
    unsigned char iv[16] = "initialvector123";  // 16-byte Initialization Vector (IV)
    AES_KEY aes_key;

    // Prepare AES encryption key
    AES_set_encrypt_key(key, 128, &aes_key);

    // Message to send
    unsigned char message[] = "Hello, secure server!";
    unsigned char encrypted_message[1024] = {0};

    // Encrypt the message
    int num = 0;  // Counter for AES-CFB mode
    AES_cfb128_encrypt(message, encrypted_message, strlen((char *)message), &aes_key, iv, &num, AES_ENCRYPT);

    // Send encrypted message
    send(sock, encrypted_message, strlen((char *)encrypted_message), 0);
    printf("Encrypted message sent.\n");

    close(sock);
}

