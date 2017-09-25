#include "tcp_socket.h"

int tcp_socket_create (
    tcp_socket* sock,
    const int port_number,
    const int buffer_size
)
{
    sock->file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sock->file_descriptor < 0)
        perror("Error opening socket...");

    if (setsockopt(sock->file_descriptor, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    
    memset(&sock->socket, 0, sizeof(sock->socket));

    sock->buffer_size = buffer_size;
    size_t __buffer_size = buffer_size * sizeof(char);
    sock->buffer = malloc(__buffer_size);
    memset(sock->buffer, '\0', __buffer_size);

    sock->socket.sin_family = AF_INET;
    sock->socket.sin_addr.s_addr = htonl(INADDR_ANY);
    sock->socket.sin_port = htons(port_number);
   
    return 0;
}

void tcp_socket_interrupt(const int signal) {
    puts("Server shutting down due to intterupt...");
    exit(0);
}

int tcp_socket_listen(
    tcp_socket* sock,
    void (*handler)(tcp_socket*, int)
)
{
    bind(sock->file_descriptor, (struct sockaddr*)&sock->socket, sizeof(sock->socket));
    listen(sock->file_descriptor, SOMAXCONN);

    signal(SIGINT, tcp_socket_interrupt);

    while(1)
    {
        int connection_file_desc;
        connection_file_desc = accept(sock->file_descriptor, NULL, NULL);

        handler(sock, connection_file_desc);
    }

    return 0;
}

void tcp_socket_destroy(tcp_socket* sock) {
    free(&sock->buffer);
    free(&sock->socket);
    close(sock->file_descriptor);
}
