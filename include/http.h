#pragma once

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _http_response {
    char* header;
    void* body;
} http_response;

void http_response_create(http_response* response, const short code);
void http_response_set_code(http_response* response, const short code);
void http_response_append_header(
    http_response* response,
    const char* key,
    char* value
);
void http_response_set_body(
    http_response* response,
    char* body
);
char* http_response_text(http_response* response);
        