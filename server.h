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
#define THREAD_LIMIT    5


#define HOME            "/home/emanuele/Scrivania/WebServer/image"
#define CACHE           "/home/emanuele/Scrivania/WebServer/cache"

#define FIELD_LEN   32


/* todo: pids have to be shared
 * il padre ogni tanto controlla quanti pid ci sono e quanti dovrebbero essercene
 * un processo che sta uscendo imposta il suo pid nell'array a 0.
 */
static int pids[SERVER_LIMIT];

void th_exit(void *);

pthread_mutex_t *mtx;                   /* mutex to modify thread_count */
unsigned int *thread_count;             /* how many thread. it represents server load */
int semid;                              /* mutex to modify process_count */
unsigned int *process_count;            /* count number of active process */

static unsigned int thread_limit = 0;   /* each child count how many thread it has */

typedef struct http_message {

    char *raw;                              /* http message received */

    /* HEADER FIELD */
    char host[MAXLINE];
    char accept[MAXLINE];                   /* information about quality of accepted files */
    char content_type[MAXLINE];             /* information about format of accepted files */
    char *request_path;                     /* only for request message */

} http_message;

typedef struct data_t {
    pthread_t tid;                /* thread id */
    int sock;                     /* connection socket */
    log_t *log;                   /* logging info */

    http_message *msg;            /* http message */
    char last_field[FIELD_LEN];   /* last header field */
} data_t;

#endif

