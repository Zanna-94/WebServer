#include "mutex_sv.h"


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int get_ipc_semaphore(void) {
    int sid;
    key_t key = ftok(".", 'b');
    sid = semget(key, 1, IPC_CREAT | 0666);
    if (sid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }
    bsem_init_free(sid); /* UNSAFE */
    return sid;
}

int bsem_init_busy(int bsemID) {
    union semun arg;
    arg.val = 0; /* in use */
    return semctl(bsemID, 0, SETVAL, arg);
}

int bsem_init_free(int bsemID) {
    union semun arg;
    arg.val = 1; /* free */
    return semctl(bsemID, 0, SETVAL, arg);
}

int bsem_get(int bsemID) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = 0;
    while (semop(bsemID, &sops, 1) == -1)
        if (errno != EINTR) /* ! interrupted by a signal */
            return -1;
    return 0;
}

int bsem_put(int bsemID) {
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = 1;
    sops.sem_flg = 0;
    return semop(bsemID, &sops, 1);
}