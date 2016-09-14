//
// Created by emanuele on 14/09/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

void initt_log()
{
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("testd",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

int main(void) {

    initt_log();
    printf("Session started!");
    syslog(LOG_NOTICE, "Session started!!");
    closelog();

    return EXIT_SUCCESS;
}