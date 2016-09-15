#ifndef WEBSERVER_MSG_PARSER_H
#define WEBSERVER_MSG_PARSER_H

#include "server.h"
#include "http_parser.h"
//#include "thread_data.h"

http_message *alloca_http_msg();

int on_header_field(http_parser *, const char *at, size_t length);

int on_header_value(http_parser *, const char *at, size_t length);

int on_url(http_parser *, const char *at, size_t length);

#endif //WEBSERVER_MSG_PARSER_H


