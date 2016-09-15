#include <pthread.h>
#include "server.h"
#include <signal.h>
#include "lib/lock_fcntl.h"
#include <netinet/in.h>
#include <lru-cache.h>
#include <arpa/inet.h>
#include "sig_handler.h"

/*------------------------------------------------------------------------
 * Program:   WebServer
 * Purpose:   Adapting static content according to the characteristics of the client device
 * Syntax:    -
 *-----------------------------------------------------------------------*/

pthread_t tid[THREAD_LIMIT];

pid_t child_make(int, int);


void thread_count_init() {

    int shm_id;
    key_t key_tc = ftok(".", '?');
    key_t key_mtx = ftok(".", '^');

    if ((shm_id = shmget(key_tc, sizeof(unsigned int), IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        exit(EXIT_FAILURE);
    }

    if ((thread_count = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }

    if ((shm_id = shmget(key_mtx, sizeof(pthread_mutex_t), IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        exit(EXIT_FAILURE);
    }

    if ((mtx = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }

}

/*
 * allocate shared memory for pids.
 */
void pids_malloc() {

    int shm_id;
    key_t key = ftok(".", '!');

    if ((shm_id = shmget(key, sizeof(int) * SERVER_LIMIT, IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        exit(EXIT_FAILURE);
    }

    if ((pids = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }
}

/**
 * Allocate memory for thread custom data
 */
data_t *allocate_data_t() {

    data_t *data;
    http_message *message;

    data = malloc(sizeof(data_t));
    if (data == NULL) {
        fprintf(stderr, "error in memory allocation");
        exit(EXIT_FAILURE);
    }

    if ((data->log = malloc((sizeof(log_t)))) == NULL) {
        fprintf(stderr, "error in memory allocation");
        exit(EXIT_FAILURE);
    }

    message = alloca_http_msg();
    data->msg = message;

    return data;
}

void create_thread(int socket, struct sockaddr_in *cliaddr) {

    data_t *data = allocate_data_t();

    data->sock = socket;
    data->log->host = strdup(inet_ntoa(cliaddr->sin_addr));

    if (pthread_create(&tid[thread_limit], NULL, connection_manager, data) != 0) {
        fprintf(stderr, "Error returned by pthread_create()\n");
        exit(EXIT_FAILURE);
    }

}

/*
 * Create new child process if server load is too high
 *
 * @return number of created process.
 * A negative result indicates that children have to be deleted
 */
int ctrl_load(int listsck) {

    int n, i, p;
    int created, tocreate;
    unsigned int load;

    /* count process children */
    for (n = 0; pids[n] != 0; ++n);

    created = n - START_SERVER;
    load = (*thread_count) / THREAD_LIMIT;   /* number of child to have plus*/
    tocreate = load - created;               /* number of child to create */

    if (tocreate > 0) {
        p = n;
        for (i = 0; i < tocreate; i++) {

            if ((n + i) >= START_SERVER)
                break;

            pids[p] = child_make(p, listsck);
            p++;
        }
    }


    return tocreate;
}

/*
 * Test if i-th process have to exit
 * @return 1 mean that process have to exit. 0 if the process have to live
 */
int im_wasteful(int i) {
    unsigned int load;

    load = (*thread_count) / THREAD_LIMIT;    /* number of child to have plus*/

    if (i + 1 > load + START_SERVER) {
        printf("process %d must terminate", getpid());
        return 1;
    }

    return 0;
};

void child_main(int i, int listensd) {

    int connsd;                     /* connection socket */
    struct sockaddr_in *cliaddr;   /* address structure for client peer */


    if ((cliaddr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in))) == NULL) {
        fprintf(stderr, "malloc return null");
        exit(EXIT_FAILURE);
    }

    /* init cache memory */
    if (get_cache() == NULL) {
        fprintf(stderr, "error getting cache shared memory");
        exit(EXIT_FAILURE);
    }


    /* init shared memory for thread count in each child*/
    thread_count_init();
    /* get pids shared memory */
    pids_malloc();

    for (;;) {

        if (thread_limit < THREAD_LIMIT) {

            int clilen = sizeof(struct sockaddr_in);   /* it will contains length of sockaddr structure */

            my_lock_wait();                            /* obtaining lock */

            if ((connsd = accept(listensd, (struct sockaddr *) cliaddr, (socklen_t *) &clilen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            my_lock_release();              /* locking release */

            /* debug */
            fprintf(stdout, "[+] Connection accepted by process: %d\n", i);

            /* increase number of active thread */
            pthread_mutex_lock(mtx);
            (*thread_count)++;
            thread_limit++;
            pthread_mutex_unlock(mtx);

            create_thread(connsd, cliaddr);

        } else usleep(100);

        if (i > START_SERVER && im_wasteful(i)) {
            int n;
            for (n = 0; n < thread_limit; n++)
                pthread_join(tid[n], NULL);

            exit(EXIT_SUCCESS);
        }

    }
}

pid_t child_make(int i, int listensd) {
    pid_t pid;

    if ((pid = fork()) > 0)
        return (pid);


    child_main(i, listensd);

    return i;   /* never reached */
}

int main(int argc, char **argv) {

    int listensd;                       /* socket for listening */
    int n;
    struct sockaddr_in servaddr;        /* address structure for the server */

    /* Listen socket for TCP connection */
    if ((listensd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket can't be created");
        exit(EXIT_FAILURE);
    }

    /* if the port is busy and in the TIME_WAIT state, go ahead and reuse it anyway. */
    if (setsockopt(listensd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    /* IP socket address defined as a combination of an IP interface address and a 16-bit port number*/
    memset((void *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDRE_ANY to bind socket to all local interface */
    servaddr.sin_port = htons(SERV_PORT);

    /* bind listening socket to IP addresses and port number specified in sockaddr struct */
    if ((bind(listensd, (struct sockaddr *) &servaddr, sizeof(servaddr))) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* start listen for connection on the socket */
    if (listen(listensd, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /*  initialize file locking */
    my_lock_init("/tmp/lock.XXXXXX");

    /* initialize shared memory */
    thread_count_init();
    (*thread_count) = 0;

    /* pids allocation of shared memory and initialization */
    pids_malloc();
    for (n = 0; n < SERVER_LIMIT; n++)
        pids[n] = 0;

    /* initialize process pool */
    for (n = 0; n < START_SERVER; n++)
        pids[n] = child_make(n, listensd);


    if (signal(SIGINT, (void (*)(int)) sig_int) == SIG_ERR) {
        fprintf(stderr, "errore in signal");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        ctrl_load(listensd);
        usleep(500);    /* fanno tutto i processi figli */
    }

}

