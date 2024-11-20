CC = gcc
CFLAGS = -Iinclude -Wall
TARGET = socket_service

all: $(TARGET)

$(TARGET): src/main.c src/tcp_server.c src/udp_server.c 
	$(CC) $(CFLAGS) -o $(TARGET) src/main.c src/tcp_server.c src/udp_server.c 

clean:
	rm -f $(TARGET)

