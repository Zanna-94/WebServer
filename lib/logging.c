#include <unistd.h>
#include <errno.h>
#include "logging.h"

void init_log() {

    setlogmask(LOG_UPTO(LOG_INFO));
    openlog("WebServer", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);

    return;
}

char *get_date() {
    time_t current_time;
    char *c_time_string;
    int len;

    /* Obtain current time. */
    current_time = time(NULL);

    if (current_time == ((time_t) -1)) {
        (void) fprintf(stderr, "Failure to obtain the current time.\n");
        exit(EXIT_FAILURE);
    }

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);
    if (c_time_string == NULL) {
        (void) fprintf(stderr, "Failure to convert the current time.\n");
        exit(EXIT_FAILURE);
    }

    len = (int) strlen(c_time_string);
    c_time_string[len - 1] = '\0';


    return c_time_string;
}


void logging(log_t *log) {

    char *date = get_date();
    char log_path[128];
    FILE *log_file;
    pthread_mutex_t mtx;

    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_lock(&mtx);
    init_log();
    syslog(LOG_INFO, "%s %s %s %s '%s' %d %d", log->host, "-", "-", date, log->request, log->status, log->bytes);
    closelog();

    memset(log_path, 0, 128);

    errno = 0;
    if (getcwd(log_path, 128) == NULL || errno != 0) {
        perror("getcwd error\n");
        return;
    }
    strcat(log_path, LOG_FILE);

    printf("%s\n", log_path);
    fflush(stdout);

    log_file = fopen(log_path, "a");
    if (log_file == NULL) {
        fprintf(stderr, "open loggind file failed\n");
        return;
    }


    fprintf(log_file, "%s %s %s %s '%s' %d %d\n", log->host, "-", "-", date, log->request, log->status, log->bytes);

    fclose(log_file);

    pthread_mutex_unlock(&mtx);
    pthread_mutex_destroy(&mtx);
}