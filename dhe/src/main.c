#include "tcp_server.h"
#include <stdio.h>
#include <pthread.h>


void *tcp_thread_func(void *arg) {
  int port = *(int *)arg;
  start_server(port);
  return NULL;
}


int main() {
  pthread_t tcp_thread;
  int port = 8484;

  if (pthread_create(&tcp_thread, NULL, tcp_thread_func, &port) != 0) {
    perror("Failed to create thread");
    return 1;
  }

  if (pthread_join(tcp_thread, NULL)) {
    perror("Failed to join thread");
    return 1;
  }

  return 0;
} 


