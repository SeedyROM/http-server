#include <stdio.h>
#include <stdlib.h>

#include "tcp_socket.h"

void handler(tcp_socket* sock, int conn) {

    tcp_socket_write(sock, conn, "HTTP/1.1 404 ERROR\n\
Content-Type: text/html\n\
Content-Length: 0\n\ 
Connection: close\n\ 
Date: Tue, 02 Apr 2002 13:30:12 GMT\n\
ETag: \"6827-1013538288000\"\n\
Server: Apache Tomcat/4.0.2 (HTTP/1.1 Connector)\n\
Last-Modified: Tue, 12 Feb 2002 18:24:48 GMT\r\n\r\n");

    close(conn);
}

int main(int argc, char** argv) {
    tcp_socket s;

    tcp_socket_create(&s, 3000, 2048);
    tcp_socket_listen(&s, &handler);

    puts("I'm dying!");
    return 0;
}