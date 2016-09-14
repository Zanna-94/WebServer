#ifndef WEBSERVER_MSG_PARSER_H
#define WEBSERVER_MSG_PARSER_H

#include "http_parser.h"
#include "basic.h"

typedef struct {

    char *raw;                              /* http message received */

    /* HEADER FIELD */
    char host[MAXLINE];
    char accept[MAXLINE];                   /* information about quality of accepted files */
    char content_type[MAXLINE];             /* information about format of accepted files */
    char *request_path;                     /* only for request message */

} http_message;

http_message *alloca_http_msg();

int on_header_field(http_parser *, const char *at, size_t length);

int on_header_value(http_parser *, const char *at, size_t length);

int on_url(http_parser *, const char *at, size_t length);

#endif //WEBSERVER_MSG_PARSER_H


