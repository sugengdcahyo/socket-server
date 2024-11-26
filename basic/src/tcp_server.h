#ifndef TCP_SERVER_H
#define TCP_SERVER_H

void start_tcp_server(int port);

void xor_encrypt_decrypt(const char *input, char *output, const unsigned char *key, size_t key_len);

#endif
