#include "server.h"

/*------------------------------------------------------------------------
 * Program:   WebServer
 * Purpose:   Adapting static content according to the characteristics of the client device
 * Usage:    ./server  <helper number> <port number[xxxx]>
 *-----------------------------------------------------------------------*/

pthread_t tid[THREAD_LIMIT];

pid_t child_make(int, int);

/**
 * Get shared memory for thread count var.
 * The variable allocated is incremented each time that a new
 * thread is started and decremented when it call pthread_exit
 */
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

/**
 * This function is called on pthread_exit. Decrementing thread count variable
 * @param arg not used
 */
void th_exit(void *arg) {

    pthread_mutex_lock(mtx);

    (*thread_count)--;
    thread_limit--;

    fflush(stdout);
    pthread_mutex_unlock(mtx);
}

/**
 * allocate shared memory for pids.
 */
void process_count_init() {

    int shm_id;
    key_t key = ftok(".", '!');

    if ((shm_id = shmget(key, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("shmget error.");
        printf("errno= %d EINVAL=%d \n ", errno, EINVAL);
        exit(EXIT_FAILURE);
    }

    if ((process_count = shmat(shm_id, NULL, 0)) == (void *) -1) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }
}

/**
 * Allocate memory for thread custom data
 * @return data structure containing useful var for thread
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

/**
 * Create a new thread. Each thread handles a connection with client
 * @param socket
 * @param cliaddr
 */
void create_thread(int socket, struct sockaddr_in *cliaddr) {

    data_t *data = allocate_data_t();

    data->sock = socket;
    data->log->host = strdup(inet_ntoa(cliaddr->sin_addr));

    /* increase number of active thread */
    pthread_mutex_lock(mtx);

    (*thread_count)++;
    thread_limit++;

    pthread_mutex_unlock(mtx);

    if (pthread_create(&tid[thread_limit], NULL, connection_manager, data) != 0) {
        fprintf(stderr, "Error returned by pthread_create()\n");
        exit(EXIT_FAILURE);
    }

}

/**
 * Create new child process if server load is too high
 *
 * @param listsck linening socket
 * @return number of process forked. A negative result indicates that there are too children
 */
int ctrl_load(int listsck) {

    int i;
    int created, tocreate;
    unsigned int load;

    bsem_get(semid);

    created = (*process_count) - children_n;
    load = (*thread_count) / THREAD_LIMIT;   /* number of child to have plus*/
    tocreate = load - created;               /* number of child to create */

    if (tocreate > 0) {

        for (i = 0; i < tocreate; i++) {

            if ((*process_count) >= SERVER_LIMIT)
                break;

            pids[(*process_count)] = child_make((*process_count), listsck);
            (*process_count)++;
            printf("[+] fork another process! #process: %d \n", *process_count);
        }
    }

    bsem_put(semid);
    return tocreate;
}

/**
 * Test if i-th process have to terminate
 *
 * @param process number
 * @return 1 mean that process have to exit. 0 if the process have to live
 */
int im_wasteful(int i) {
    unsigned int load;

    load = (*thread_count) / THREAD_LIMIT;    /* number of child to have plus*/

    if (i + 1 >= (load + children_n))
        return 1;

    return 0;
};

/**
 * Main function for each forked process
 *
 * @param i process'number
 * @param listensd listening socket
 */
void child_main(int i, int listensd) {

    int connsd;                     /* connection socket */
    struct sockaddr_in *cliaddr;    /* address structure for client peer */

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

    /* get shared memory for process count  */
    process_count_init();

    /* get ipc mutex */
    semid = get_ipc_semaphore();


    struct pollfd pfd;
    pfd.fd = listensd;
    pfd.events = POLL_IN | POLL_HUP;
    pfd.revents = 0;

    for (;;) {

        /* Needs to not block on accept. If the server creates many children for the
        * high load, then the children are not destroyed because the main thread is
        * blocked on the accept. */
        while (poll(&pfd, 1, 1) == 0) {

            bsem_get(semid);

            /* Test if process has reason to exist */
            if (i > children_n && im_wasteful(i)) {
                int n;
                for (n = 0; n < thread_limit; n++)
                    pthread_join(tid[n], NULL);

                (*process_count)--;

                bsem_put(semid);

                printf("[-] process number %d termination\n", i);
                exit(EXIT_SUCCESS);
            }

            bsem_put(semid);

        }

        if (thread_limit < THREAD_LIMIT) {

            /* it will contains length of sockaddr structure */
            int clilen = sizeof(struct sockaddr_in);

            /* obtaining lock */
            my_lock_wait();

            if ((connsd = accept(listensd, (struct sockaddr *) cliaddr, (socklen_t *) &clilen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            /* locking release */
            my_lock_release();

            create_thread(connsd, cliaddr);

        }

    }
}

/**
 * fork a new process
 *
 * @param i process' number
 * @param listensd listening socket
 * @return pid of forked process
 */
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

    switch (argc) {

        case (1) :
            // if not specified, use default port
            port_n = SERV_PORT;
            // if not specified, use default number of helper
            children_n = START_SERVER;
            break;

        case (2) :
            // use specified number of helper
            children_n = atoi(argv[1]);
            // if not specified, use default port
            port_n = SERV_PORT;
            break;

        case (3):
            /* use specified number of helper   */
            children_n = atoi(argv[1]);
            /* use specified port   */
            port_n = (in_port_t) atoi(argv[2]);

            break;

        default:
            printf("Usage : ./server <start children> <port number[xxxx]>");
            exit(EXIT_FAILURE);
    }

    /* if number is not between 1 and 8 */
    if (children_n < START_SERVER || children_n > SERVER_LIMIT) {
        printf("Insert a number of children between 1 and 8.\n"
                       "Usage : ./server <helper number> <port number[xxxx]>\n");
        exit(EXIT_FAILURE);
    }

    /* Listen socket for TCP connection */
    if ((listensd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket can't be created\n");
        exit(EXIT_FAILURE);
    }

    /* if the port is busy and in the TIME_WAIT state, go ahead and reuse it anyway. */
    if (setsockopt(listensd, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed\n");

    /* IP socket address defined as a combination of an IP interface address and a 16-bit port number*/
    memset((void *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDRE_ANY to bind socket to all local interface */
    servaddr.sin_port = htons(port_n);

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
    process_count_init();

    /* get ipc mutex */
    semid = get_ipc_semaphore();

    *process_count = (unsigned int) children_n;

    /* initialize process pool */
    for (n = 0; n < children_n; n++)
        pids[n] = child_make(n, listensd);


    if (signal(SIGINT, (void (*)(int)) sig_int) == SIG_ERR) {
        fprintf(stderr, "errore in signal");
        exit(EXIT_FAILURE);
    }

    printf("SERVER START!\n");
    for (;;) {
        ctrl_load(listensd);
        usleep(500000);
    }

}

