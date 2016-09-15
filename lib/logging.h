#ifndef WEBSERVER_LOGGING_H
#define WEBSERVER_LOGGING_H

#include <syslog.h>
#include <time.h>
#include "../server.h"


typedef struct log_t {
    char *host;
    char *request;
    unsigned int status;
    unsigned int bytes;
} log_t;

void logging(log_t *);

#endif //WEBSERVER_LOGGING_H
