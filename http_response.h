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

int send_not_found(int);

int send_ok(int, int);

void send_no_content(int);

void send_ok_head(int sock);

void send_not_found_head(int sock);

#endif //WEBSERVER_HTTP_RESPONSE_H
