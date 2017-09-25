#pragma once

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

typedef struct _tcp_socket {
    int file_descriptor;
    int port_number;
    struct sockaddr_in socket; 
    char* buffer;
    int buffer_size;
} tcp_socket;

int tcp_socket_create(
    tcp_socket* sock,
    int port_number,
    int buffer_size
);
int tcp_socket_listen(
    tcp_socket* sock,
    void (*handler)(tcp_socket*, int)
);
void tcp_socket_handle_interrupt(int signal, int connection_file_desc);
void tcp_socket_destroy(tcp_socket* sock);

#define TM_PRINTF(f_, ...) printf((f_), ##__VA_ARGS__)

#define tcp_socket_write(sock, connection_file_desc, format, ...)  \
    snprintf(sock->buffer, sock->buffer_size, (format), ##__VA_ARGS__); \
    write(connection_file_desc, sock->buffer, strlen(sock->buffer)); \
