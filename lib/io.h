#ifndef WEBSERVER_IO_H
#define WEBSERVER_IO_H

#include <stddef.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>


int open_file(char *);

ssize_t readn(int fd, void *buf, size_t n);

ssize_t writen(int fd, const void *buf, size_t n);

int read_line(int fd, void *ptr, int maxlen);

int write_line(int fd, const void *ptr, int maxlen);

int receive_msg_h(int, char *);

void removeSpaces(char *);

int exist(char *);

char *get_filename_ext(char *);

int isImage(char *);

char *get_request(char *msg);

#endif
