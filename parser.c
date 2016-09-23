#include "parser.h"

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
        message->host = strndup(at, length);

    } else if (strcmp(data->last_field, "Accept") == 0) {
        message->accept = strndup(at, length);

    } else if (strcmp(data->last_field, "Content-Type") == 0) {
        message->content_type = strndup(at, length);

    } else if (strcmp(data->last_field, "User-Agent") == 0)
        message->user_agent = strndup(at, length);

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
    message->request_path = strtok((char *) at, " ");

    return 0;
}

/*
 * Parse the header's field "Accept" of http message to find quality factor
 * for "Image/*"
 * @param accept field Accept in http message*s header
 * @return quality factor for image/jpeg format
 */
float find_quality_factor(char *accept, char *image_extension) {


    char *token, *ptr, field[MAXLINE];
    float quality_factor = 1;

    strcat(field, "image/");
    strcat(field, image_extension);

    if ((ptr = strstr(accept, field)) != NULL) {
        token = ptr + strlen(field) + 1;
    } else if ((ptr = strstr(accept, "*/*")) != NULL) {
        token = ptr + strlen("*/*") + 1;
    } else return quality_factor;

    removeSpaces(token);
    if ((token = strstr(token, "q=")) != NULL) {
        token += strlen("q=");

        errno = 0;
        quality_factor = strtof(token, NULL);
        if (errno != 0) {
            perror("strtof");
            return -1;
        }
    }

//    printf("%s: %.2f\n", "quality factor found", quality_factor);
    return quality_factor;
}

/*
 *
 * @param data thread custom data
 * @param http_msg strcture representing http message
 * @param recved #byte of the http message read from socket
 */
http_parser *parse(data_t *data, char *http_msg, size_t recved) {

    http_parser_settings settings;
    http_parser *parser = malloc(sizeof(http_parser));
    if (parser == NULL) {
        fprintf(stderr, "error in memory allocation");
        return NULL;
    }

    /* setting callbacks */
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_url = on_url;

    /* set message type (HTTP_REQUEST) */
    http_parser_init(parser, HTTP_REQUEST);
    parser->data = data;

    /* parse */
    parser->http_errno = 0;
    http_parser_execute(parser, &settings, http_msg, recved);
    if (parser->http_errno != 0) {
        fprintf(stderr, "http_parser_execute error\n");
        return NULL;
    }

    return parser;
}
