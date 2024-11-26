CC = gcc
CFLAGS = -Iinclude -Wall
OUTPUT_DIR = output
TARGET_TCP = $(OUTPUT_DIR)/tcp_server
TARGET_UDP = $(OUTPUT_DIR)/udp_server

# create output folder, if not exists
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

all: $(TARGET_TCP) $(TARGET_UDP)

tcp: $(TARGET_TCP)

$(TARGET_TCP): src/main.c src/tcp_server.c | $(OUTPUT_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_TCP) src/main.c src/tcp_server.c src/include/diffie_hellman_openssl.c -lcrypto -lpthread

clean:
	rm -f $(OUTPUT_DIR)

