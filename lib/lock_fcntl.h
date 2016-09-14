#ifndef WEBSERVER_LOCK_FCNTL_H
#define WEBSERVER_LOCK_FCNTL_H

void my_lock_init(char *pathname);

void my_lock_wait();

void my_lock_release();

#endif //WEBSERVER_LOCK_FCNTL_H
