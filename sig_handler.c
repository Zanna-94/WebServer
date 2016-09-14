#include <signal.h>
#include "basic.h"
#include "sig_handler.h"
#include <sys/wait.h>
#include <errno.h>

/**
 *
 * @param signum specifies the siganl
 * @param func  specifies the action to be associated with signum or a function to call
 * @return  old handler for the siganl
 * @see sigaction
 */
Sigfunc *signal(int signum, Sigfunc *func) {

    struct sigaction act, oact;

    act.sa_handler = func;          /* register the handler */
    sigemptyset(&act.sa_mask);      /* not block other signal */
    act.sa_flags = 0;

    if (signum != SIGALRM)
        act.sa_flags |= SA_RESTART; /* making certain system calls restartable across signals */

    if (sigaction(signum, &act, &oact) < 0)
        return (SIG_ERR);

    return (oact.sa_handler);
}

void sig_int() {
    int i;

    for (i = 0; i < CHILD_N; i++)
        kill(pids[i], SIGTERM);
    while (wait(NULL) > 0);    /* attende per tutti i processi child */

    if (errno != ECHILD) {
        fprintf(stderr, "errore in wait");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}