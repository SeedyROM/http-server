#pragma once

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>

// Struct to store a TCP/IP socket and it's related values. 
typedef struct _tcp_socket {
    int file_descriptor;
    int port_number;
    struct sockaddr_in socket; 
    char* buffer;
    int buffer_size;
    int child_process_count;
} tcp_socket;

// Initialize a tcp_socket struct.
int tcp_socket_create(
    tcp_socket* sock,
    int port_number,
    int buffer_size
);
// Listen on the specified port and run specified handler function.
int tcp_socket_listen(
    tcp_socket* sock,
    void (*handler)(tcp_socket*, int)
);
// Handle keyboard interrupts.
void tcp_socket_handle_interrupt(int signal, int connection_file_desc);
// Destroy a tcp_socket struct from memory.
void tcp_socket_destroy(tcp_socket* sock);


// Macro to shorten writing a common function.
#define tcp_socket_write(sock, connection_file_desc, format, ...)  \
    snprintf(sock->buffer, sock->buffer_size, (format), ##__VA_ARGS__); \
    write(connection_file_desc, sock->buffer, strlen(sock->buffer));
