#ifndef WEBSERVER_MUTEX_SV_H
#define WEBSERVER_MUTEX_SV_H

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>

int get_ipc_semaphore(void);

int bsem_init_busy(int bsemID);

int bsem_init_free(int bsemID);

int bsem_get(int bsemID);

int bsem_put(int bsemID);

#endif //WEBSERVER_MUTEX_SV_H
