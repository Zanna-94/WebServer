#ifndef WEBSERVER_SERVER_JOB_H
#define WEBSERVER_SERVER_JOB_H

#include "parser_callbacks.h"
#include "pthread.h"
#include "logging.h"

typedef struct converted_image {
    char name[MAXLINE];
    float quality_factor;

    char temp_file[MAXLINE];
} converted_image;

void *connection_manager(void *);

float find_quality_factor(char *, char *);

#endif //WEBSERVER_SERVER_JOB_H
