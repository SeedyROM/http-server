#include <stdio.h>
#include <stdlib.h>

#include "tcp_socket.h"

void handler(tcp_socket* sock, int conn) {
    // Return a fake 404, lol.
    tcp_socket_write(sock, conn, "HTTP/1.1 200 OK\n\
Last-Modified: Mon, 23 Jul 2007 08:41:56 GMT\n\
Content-Length: 27\n\
Content-Type: text/html\n\
\r\n\
<h1>Existance is pain.</h1>");

    // while(recv(conn, sock->buffer, sizeof(sock->buffer) - 1, 0) > 0) {
    //     puts("receiving data...");
    //     printf("%s\n", sock->buffer);
    // }
   
    close(conn);
}

int main(int argc, char** argv) {
    tcp_socket s;

    tcp_socket_create(&s, 3000, 2048);
    tcp_socket_listen(&s, &handler);

    puts("I'm dying!");
    return 0;
}