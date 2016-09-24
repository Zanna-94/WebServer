#ifndef WEBSERVER_BASIC_H
#define WEBSERVER_BASIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ipc_sem.h>
#include <poll.h>
#include "logging.h"
#include "constants.h"
#include "server.h"
#include "lib/lock_fcntl.h"
#include "sig_handler.h"
#include "lru-cache.h"
#include "parser.h"
#include "thread_job.h"

#define SERV_PORT       5193
#define BACKLOG         10
#define TIMEOUT         1

#define START_SERVER    2
#define SERVER_LIMIT    16
#define THREAD_LIMIT    16

void th_exit(void *);

uint16_t port_n;
int children_n;

static int pids[SERVER_LIMIT];

pthread_mutex_t *mtx;                   /* mutex to modify thread_count */
unsigned int *thread_count;             /* how many thread. it represents server load */
int semid;                              /* mutex to modify process_count */
unsigned int *process_count;            /* count number of active process */

static unsigned int thread_limit = 0;   /* each child count how many thread it has */

#endif

