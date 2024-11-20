#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


void start_tcp_server(int port) {
  int server_fd, client_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  listen(server_fd, 3);

  while ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) >= 0) {
    read(client_fd, buffer, 1024);
    printf("Received: %s\n", buffer);
    send(client_fd, buffer, strlen(buffer), 0);
    close(client_fd);
  }
  close(server_fd);
}
