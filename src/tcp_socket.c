#include "tcp_socket.h"

int tcp_socket_create (
    tcp_socket* sock,
    const int port_number,
    const int buffer_size
)
{
    // Create a INET TCP socket fd and store it on our struct.
    sock->file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    // Check everything went okay.
    if(sock->file_descriptor < 0)
        perror("Error opening socket...");

    // Set our socket to have the SO_REUSEADDR option set.
    if (setsockopt(sock->file_descriptor, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    
    // Allocate space for the socket address struct.
    memset(&sock->socket, 0, sizeof(sock->socket));

    // Allocate the sockets buffer dynamically.
    sock->buffer_size = buffer_size;
    size_t __buffer_size = buffer_size * sizeof(char);
    sock->buffer = malloc(__buffer_size);
    memset(sock->buffer, '\0', __buffer_size);

    // Set values on our socket address struct.
    sock->socket.sin_family = AF_INET;
    sock->socket.sin_addr.s_addr = htonl(INADDR_ANY);
    sock->socket.sin_port = htons(port_number);
   
    // Return everything went okay!
    return 0;
}

void tcp_socket_interrupt(const int signal) {
    // Handle interrupt.
    puts("\nServer shutting down due to intterupt...");
    exit(0);
}

int tcp_socket_listen(
    tcp_socket* sock,
    void (*handler)(tcp_socket*, int)
)
{
    // Bind on specified port.
    bind(sock->file_descriptor, (struct sockaddr*)&sock->socket, sizeof(sock->socket));
    listen(sock->file_descriptor, SOMAXCONN);

    // Set intterup signal.
    signal(SIGINT, tcp_socket_interrupt);

    while(1)
    {
        // Accept a connection to a client.
        int connection_file_desc;
        connection_file_desc = accept(sock->file_descriptor, NULL, NULL);

        // Fork a process off to handle the request.
        pid_t pid;
        if((pid = fork()) < 0) // If the fork fails...
            perror("Failed to fork new process.");
        else if(pid == 0) { // If it's a child process
                            // close server socket and handle.
            close(sock->file_descriptor);
            handler(sock, connection_file_desc);

            exit(0);
        }

        // Parent closes connection fd and
        // increments number of outstanding processes. 
        close(connection_file_desc);
        sock->child_process_count++;

        // Clean up child processes.
        while(sock->child_process_count) {
            // Kill the process by sending a signal.
            pid = waitpid((pid_t) -1, NULL, WNOHANG);
            if (pid == 0)
                break;
            else
                sock->child_process_count--;
        }

    }

    return 0;
}

void tcp_socket_destroy(tcp_socket* sock) {
    // Deallocate our used memory.
    free(&sock->buffer);
    free(&sock->socket);
    close(sock->file_descriptor);
}

