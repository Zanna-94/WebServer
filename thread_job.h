#ifndef WEBSERVER_SERVER_JOB_H
#define WEBSERVER_SERVER_JOB_H

#include "parser_callbacks.h"
#include "pthread.h"

#define FIELD_LEN   32

#endif //WEBSERVER_SERVER_JOB_H

typedef struct {
    pthread_t tid;                /* thread id */
    int sock;                     /* connection socket */

    http_message *msg;            /* http message */
    char last_field[FIELD_LEN];   /* last header field */
} data_t;

typedef struct image {
    char *filename;
    float quality_factor;

    char *temp_file;
} image;

void *connection_manager(void *);

float find_quality_factor(char *, char *);
