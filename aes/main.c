#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "client.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [server|client]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        start_server();
    } else if (strcmp(argv[1], "client") == 0) {
        start_client();
    } else {
        fprintf(stderr, "Invalid argument: %s\n", argv[1]);
        fprintf(stderr, "Use 'server' to start the server or 'client' to start the client.\n");
        return 1;
    }

    return 0;
}

