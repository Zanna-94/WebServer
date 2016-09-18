#ifndef WEBSERVER_MSG_PARSER_H
#define WEBSERVER_MSG_PARSER_H

#include <libgen.h>
#include <errno.h>
#include "server.h"
#include "http_parser.h"
#include "io.h"

http_message *alloca_http_msg();

int on_header_field(http_parser *, const char *at, size_t length);

int on_header_value(http_parser *, const char *at, size_t length);

int on_url(http_parser *, const char *at, size_t length);

float find_quality_factor(char *accept, char *image_extension);

http_parser *parse(data_t *data, char *http_msg, size_t recved);

#endif //WEBSERVER_MSG_PARSER_H


