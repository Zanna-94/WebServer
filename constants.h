#ifndef WEBSERVER_CONSTANTS_H
#define WEBSERVER_CONSTANTS_H

#include "logging.h"

#define MAXLINE         128
#define FIELD_LEN       32
#define MSG_SIZE        1024

#define ROOT()            get_current_dir_name()
#define CONTENT_DIR     "/image"
#define CACHE           "/cache"

typedef struct converted_image {
    char name[MAXLINE];
    float quality_factor;

    char temp_file[MAXLINE];
} converted_image;

typedef struct http_message {

    /* HEADER FIELD */
    char host[MAXLINE];
    char accept[MAXLINE];                   /* information about quality of accepted files */
    char content_type[MAXLINE];             /* information about format of accepted files */
    char *request_path;              /* only for request message */

} http_message;

typedef struct data_t {
    pthread_t tid;                /* thread id */
    int sock;                     /* connection socket */
    log_t *log;                   /* logging info */

    http_message *msg;            /* http message */
    char last_field[FIELD_LEN];   /* last header field */
} data_t;


#endif //WEBSERVER_CONSTANTS_H
