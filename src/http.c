#include "http.h"


void http_response_create(http_response* response, const short code) {
    const size_t page_size = getpagesize();
    response->header = malloc(page_size);
    memset(response->header, '\0', page_size);
    response->body = NULL;

    http_response_set_code(response, code);
}

void http_response_set_code(http_response* response, const short code) {
    size_t last_header_entry = response->header + strlen(response->header);

    const char* http_version = "HTTP/1.1 ";

    strcpy(last_header_entry++, http_version);
    sprintf(last_header_entry + sizeof(http_version), "%d ", code); 
      
    if(code > 300)
        strcat(last_header_entry, "ERROR");
    else
        strcat(last_header_entry, "OK");

    strcat(last_header_entry, "\n");
}

void http_response_append_header(http_response* response, const char* key, char* value) {
    size_t last_header_entry = response->header + strlen(response->header);
    
    strcpy(last_header_entry, key);
    strcat(last_header_entry, ": ");
    strcat(last_header_entry + sizeof(key), value);
    strcat(last_header_entry, "\n");
}

void http_response_set_body(http_response* response, char* body) {
    size_t body_size = strlen(body);
    response->body = malloc(body_size);

    char* response_length[64];
    sprintf(response_length, "%d", body_size);
    http_response_append_header(response, "Content-Length", response_length);

    strcpy(response->body, body);
}

char* http_response_text(http_response* response) {
    char* response_text = malloc(strlen(response->header) + strlen(response->body) + 3);
    strcpy(response_text, (const char*) response->header);
    strcat(response_text, "\r\n");
    strcat(response_text, (const char*) response->body);

    return response_text;
}