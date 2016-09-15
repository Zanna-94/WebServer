#ifndef WEBSERVER_SIG_HANDLER_H
#define WEBSERVER_SIG_HANDLER_H

#include <signal.h>
#include "server.h"
#include <sys/wait.h>
#include <errno.h>

typedef void Sigfunc(int);

void sig_int();

Sigfunc *signal(int signum, Sigfunc *handler);


#endif //WEBSERVER_SIG_HANDLER_H

