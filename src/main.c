#include <stdio.h>
#include <stdlib.h>

#include "tcp_socket.h"
#include "http.h"

void handler(tcp_socket* sock, int conn) {
    http_response r;
    http_response_create(&r, 200);
    http_response_append_header(&r, "Content-Type", "text/html");
    http_response_set_body(&r, "<h1>I eat <i>ass</i></h1>");

    tcp_socket_write(sock, conn, http_response_text(&r));
    close(conn);
}

int main(int argc, char** argv) {
    tcp_socket s;

    tcp_socket_create(&s, 3000, 2048);
    tcp_socket_listen(&s, &handler);

    puts("I'm dying!");
    return 0;
}