#include "../server.h"
#include <fcntl.h>
#include <errno.h>
#include "io.h"
#include <pthread.h>

/*
 * @param filename
 * @return file descriptor
 */
int open_file(char *filename) {

    int fd;

    errno = 0;
    fd = open(filename, O_RDONLY);
    if (fd == -1 || errno != 0) {
        perror("open");
        return -1;
    }

    return fd;
}

/*
 * Test if the http message's header is finished.
 * @param ptr string
 * @return 1 if the string start with \r\n. Otherwise 0
 */
int header_end(char *ptr) {

    if (strcmp(ptr, "\r\n") == 0) {
        return 1;
    }
    return 0;
}

/*
 * @param fd file descriptor
 * @param ptr pointer to a buffer
 * @param maxlen max size for line's lenght
 * @return On success the number of read bytes, -1 on error
 */
int read_line(int fd, void *ptr, int maxlen) {

    char c, *buf;
    int n;
    ssize_t r;

    buf = ptr;

    /* loop until buffer is full or end line reached */
    for (n = 0; n < maxlen; n++) {

        if ((r = read(fd, &c, 1)) == -1) {
            perror("read");
            return -1;
        }

        if (r == 1) {
            *buf++ = c;
            if (c == '\n' || c == '\0') break;   /* end line */

        } else if (r == 0) { /* EOF */
            break;
        }
    }

    return n;
}

ssize_t writen(int fd, const void *buf, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = buf;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if ((nwritten < 0) && (errno == EINTR)) nwritten = 0;
            else return (-1);        /* errore */
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n - nleft);    /* restituisce >= 0 */
}

/*
 * Read http message header.
 *
 * @param fd file or socket descriptor
 * @param ptr pointer to the read http message header. value-result argument.
 */
int receive_msg_h(int fd, char *ptr) {

    int nread, ssbuf;

    ssbuf = 0;  /* number of read bytes */

    while (MSG_SIZE - ssbuf > MAXLINE) {

        char line[MAXLINE];
        bzero(line, MAXLINE);

        /* read a single line */
        if ((nread = read_line(fd, line, MAXLINE)) == -1) {
            fprintf(stderr, "read_line error");
            pthread_exit(NULL);
        }

        ssbuf += nread; /* update the number of read bytes */
        /* concatenate the read line to the http message */
        strncat(ptr, line, strlen(line));

        /* http message header ends with empty line ( "\r\n" ) */
        if (header_end(line)) {
            break;
        }

    }

    ptr[ssbuf] = '\0';
    return ssbuf;
}

/*
 * Remove empty space at the beginning of the string
 * @param source string to manipulate
 */
void removeSpaces(char *source) {
    char *i = source;
    char *j = source;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ')
            i++;
    }
    *i = 0;
}

/*
 *
 * @param path file to test
 * @return 0 if the file exist, 1 otherwise
 */
int exist(char *path) {

    if (access(path, F_OK) != -1) {
        return 1;
    } else {
        return 0;
    }
}

/*
 * Get the file extension
 */
char *get_filename_ext(char *filename) {

    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "";
    return dot + 1;
}

/*
 * Test if the file specified is an image.
 * @param filename is the path of the file
 * @return 0 if true, not-zero if false
 */
int isImage(char *file) {
    char *extension[] = {"jpg", "jpeg", "png", "svg", NULL};
    char *format;
    int i = 0;

    /* test file extension */
    format = get_filename_ext(file);
    while (extension[i] != NULL) {

        if (strncmp(format, extension[i], strlen(extension[i])) == 0)
            return 1;

        i++;
    }

    return 0;
}

/*
 * Obtain first line from http header
 */
char *get_request(char *msg) {

    int n;
    char *ptr;

    if ((ptr = malloc(sizeof(MAXLINE))) == NULL) {
        fprintf(stderr, "error in allocation memory");
        return NULL;
    }

    /* loop until buffer is full or end line reached */
    for (n = 0; n < MAXLINE; n++) {

        if (msg[n] == '\r' || msg[n] == '\n')
            break;
        else
            ptr[n] = msg[n];
    }

    msg[n] = '\0';
    return ptr;
}