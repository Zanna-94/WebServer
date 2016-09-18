#ifndef WEBSERVER_SERVER_JOB_H
#define WEBSERVER_SERVER_JOB_H

#include <MagickWand/MagickWand.h>
#include "server.h"
#include "http_response.h"

void *connection_manager(void *);

float find_quality_factor(char *, char *);

#endif //WEBSERVER_SERVER_JOB_H
