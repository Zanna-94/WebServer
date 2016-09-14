#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT       5193
#define BACKLOG         10
#define MAXLINE         128
#define MSG_SIZE        1024
#define CHILD_N         10
#define TIMEOUT         5
#define MAX_THREAD_N    128

static pid_t *pids;

