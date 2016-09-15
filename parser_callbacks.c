#include "parser_callbacks.h"

/**
 * allocate memory for http message structure
 * @param msg pointer to http_msg struct
 */
http_message *alloca_http_msg() {

    http_message *message;

    message = malloc(sizeof(http_message));
    if (message == NULL) {
        fprintf(stderr, "error in memory allocation");
        return NULL;
    }

    return message;
}

int on_header_field(http_parser *parser, const char *at, size_t length) {

    data_t *data = parser->data;

    memset(data->last_field, 0, strlen(data->last_field));
    strncpy(data->last_field, at, length);

    return 0;

}

int on_header_value(http_parser *parser, const char *at, size_t length) {
    data_t *data = parser->data;
    http_message *message = data->msg;

    if (strcmp(data->last_field, "Host") == 0) {
        strncpy(message->host, at, length);

    } else if (strcmp(data->last_field, "Accept") == 0) {
        strncpy(message->accept, at, length);

    } else if (strcmp(data->last_field, "Content-Type") == 0) {
        strncpy(message->content_type, at, length);
    }

    return 0;

}


int on_url(http_parser *parser, const char *at, size_t length) {

    data_t *data = parser->data;        /* cast */
    http_message *message = data->msg;  /* dereferencing */

    /* allocate memory for url path */
    message->request_path = malloc(MAXLINE);
    if (message->request_path == NULL) {
        fprintf(stderr, "error in memory allocation");
        return -1;
    }

    /* "path/filename HTTP1/1" have to be splitted */
    message->request_path = basename(strtok((char *) at, " "));

    return 0;
}


