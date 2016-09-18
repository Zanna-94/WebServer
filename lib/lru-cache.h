#ifndef WEBSERVER_SHARED_DB_LIST_H
#define WEBSERVER_SHARED_DB_LIST_H

#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "../constants.h"

#define MAX_SHM_ID_NUM  10

typedef struct node {
    /* key */
    converted_image img;

    struct node *next, *prev;

} node;

void *get_cache();

void put_in_cache(converted_image *);

void move_to_head(node *);

char *find_in_cache(converted_image *);

#endif //WEBSERVER_SHARED_DB_LIST_H
