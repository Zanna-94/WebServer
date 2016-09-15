#ifndef WEBSERVER_IPC_SEM_H
#define WEBSERVER_IPC_SEM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <errno.h>

int bsem_init_busy(int bsemID);

int bsem_init_free(int bsemID);

int bsem_get(int bsemID);

int bsem_put(int bsemID);

int get_ipc_semaphore(void);

#endif //WEBSERVER_IPC_SEM_H
