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

    init_log();
    syslog(LOG_INFO, "%s %s %s %s '%s' %d %d", log->host, "-", "-", date, log->request, log->status, log->bytes);
    closelog();
}