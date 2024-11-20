#include "tcp_server.h"
#include <stdio.h>
#include <pthread.h>


void *tcp_thread_func(void *arg) {
  start_tcp_server(8080);
  return NULL;
}


int main() {
  pthread_t tcp_thread;

  pthread_create(&tcp_thread, NULL, tcp_thread_func, NULL);

  pthread_join(tcp_thread, NULL);

  return 0;
}
