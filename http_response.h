#ifndef WEBSERVER_HTTP_RESPONSE_H
#define WEBSERVER_HTTP_RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include "constants.h"
#include "io.h"

void send_not_found(int);

void send_ok(int, int, unsigned int);

void send_no_content(int);

#endif //WEBSERVER_HTTP_RESPONSE_H
