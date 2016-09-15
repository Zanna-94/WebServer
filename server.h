#ifndef WEBSERVER_BASIC_H
#define WEBSERVER_BASIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "logging.h"

#define SERV_PORT       5193
#define BACKLOG         10
#define MAXLINE         128
#define MSG_SIZE        1024
#define TIMEOUT         5

#define START_SERVER    3
#define SERVER_LIMIT    16
#define THREAD_LIMIT    64


#define HOME            "/home/emanuele/Scrivania/WebServer/image"
#define CACHE           "/home/emanuele/Scrivania/WebServer/cache"


/* todo: pids have to be shared
 * il padre ogni tanto controlla quanti pid ci sono e quanti dovrebbero essercene
 * un processo che sta uscendo imposta il suo pid nell'array a 0.
 */
int *pids;

pthread_mutex_t *mtx;                   /* mutex to modify thread_count */
unsigned int *thread_count;             /* how many thread. it represents server load */

static unsigned int thread_limit = 0;   /* each child count how many thread it has */

#endif

