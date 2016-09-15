#ifndef WEBSERVER_SERVER_JOB_H
#define WEBSERVER_SERVER_JOB_H

#include "server.h"
#include "parser_callbacks.h"
#include "pthread.h"
#include "logging.h"
#include <libgen.h>
#include <MagickWand/MagickWand.h>
#include <http_parser.h>
#include <stdlib.h>



typedef struct converted_image {
    char name[MAXLINE];
    float quality_factor;

    char temp_file[MAXLINE];
} converted_image;

void *connection_manager(void *);

float find_quality_factor(char *, char *);

#endif //WEBSERVER_SERVER_JOB_H
