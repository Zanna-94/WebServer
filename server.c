#include <pthread.h>
#include "basic.h"
#include <signal.h>
#include "lib/lock_fcntl.h"
#include <netinet/in.h>
#include <lru-cache.h>
#include <arpa/inet.h>
#include "sig_handler.h"
#include "thread_job.h"

/*------------------------------------------------------------------------
 * Program:   WebServer
 * Purpose:   Adapting static content according to the characteristics of the client device
 * Syntax:    -
 *-----------------------------------------------------------------------*/

pthread_mutex_t mtx;
int nthread;                /* # thread alive for the current process */


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

    if (pthread_create(&data->tid, NULL, connection_manager, data) != 0) {
        fprintf(stderr, "Error returned by pthread_create()\n");
        exit(EXIT_FAILURE);
    }

}

void child_main(int i, int listensd) {

    int connsd;                     /* connection socket */
    struct sockaddr_in *cliaddr;   /* address structure for client peer */


    if ((cliaddr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in))) == NULL) {
        fprintf(stderr, "malloc return null");
        exit(EXIT_FAILURE);
    }

    /* init cache memory */
    if(get_cache() == NULL){
        fprintf(stderr, "error getting cache shared memory");
        exit(EXIT_FAILURE);
    }


    for (;;) {

        int clilen = sizeof(struct sockaddr_in);   /* it will contains length of sockaddr structure */

        my_lock_wait();                            /* obtaining lock */

        if ((connsd = accept(listensd, (struct sockaddr *) cliaddr, (socklen_t *) &clilen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        my_lock_release();              /* locking release */

        /* debug */
        fprintf(stdout, "[+] Connection accepted by process: %d\n", i);
        fflush(stdout);

        if (nthread < MAX_THREAD_N)
            create_thread(connsd, cliaddr);
    }
}

pid_t child_make(int i, int listensd) {
    pid_t pid;

    if ((pid = fork()) > 0)
        return (pid);

    nthread = 0;
    pthread_mutex_init(&mtx, NULL);

    child_main(i, listensd);

    return i;   /* never reached */
}

int main(int argc, char **argv) {

    int listensd;                       /* socket for listening */
    int i;
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

    /* memory allocation for pid_t */
    pids = (pid_t *) calloc(CHILD_N, sizeof(pid_t));
    if (pids == NULL) {
        fprintf(stderr, "error in calloc");
        exit(1);
    }

    my_lock_init("/tmp/lock.XXXXXX");   /*  initialize file locking */
    for (i = 0; i < CHILD_N; i++)
        pids[i] = child_make(i, listensd); /* initialize process pool */

    if (signal(SIGINT, (void (*)(int)) sig_int) == SIG_ERR) {
        fprintf(stderr, "errore in signal");
        exit(1);
    }

    for (;;)
        pause();    /* fanno tutto i processi figli */

    /* todo update process pool if workload is high */

}

