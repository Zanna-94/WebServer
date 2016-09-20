#include "http_response.h"
/*
 * @param fd file to find the date of last modification
 * @return http header field Last-Modified.
 */
char *LastModified_line(int fd) {

    struct stat attr;
    char *buf;
    struct tm *tm;

    buf = malloc(MAXLINE);
    if (buf == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        return NULL;
    }

    fstat(fd, &attr);
    tm = gmtime(&attr.st_mtime);
    strftime(buf, MAXLINE, "Last-Modified: %a, %d %b %Y %H:%M:%S %Z", tm);

    return buf;
}

char *get_date_line() {
    char *buf;
    struct tm *ts;
    size_t last;
    time_t timestamp = time(NULL);

    buf = malloc(MAXLINE);
    if (buf == NULL) {
        fprintf(stderr, "error in memory allocation\n");
        return NULL;
    }

    ts = localtime(&timestamp);
    strftime(buf, MAXLINE, "Date: %a, %d %b %Y %H:%M:%S %Z", ts);

    return buf;
}

/*
 * send http message with error code 404 (file not found)
 */
void send_not_found(int sock) {

    char *response;
    int sent;

    response = malloc(MSG_SIZE);
    if (response == NULL) {
        fprintf(stderr, "error in memory allocation");
    }


    sprintf(response, "%s\r\n%s\r\n%s\r\n\r\n%s", "HTTP/1.1 404 Not Found", "Server: ZannaServer", "Connection: close",
            "<html><body><h1>404 Page Not Found. </h1></body></html>");
    sent = (int) writen(sock, response, strlen(response));
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
    }

    free(response);

}

/*
 * Send an http response message with status code 200 OK.
 *
 * @fd file descriptor for the file to send as body
 * @sock connection socket with the client node
 * @content_length size of the file to send
 */
void send_ok(int fd, int sock, unsigned int content_length) {

    char response[MSG_SIZE], *mapped_file;   /* memory where map the file */
    int sent;


    sprintf(response, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s: %d\r\n\r\n", "HTTP/1.1 200 OK",
            "Server: ZannaServer", get_date_line(), LastModified_line(fd),
            "Connection: close", "Content-Length", content_length);

    /* map file */
    mapped_file = mmap(NULL, (size_t) content_length, PROT_READ, MAP_PRIVATE, fd, 0);

    /* send headers*/
    sent = (int) writen(sock, response, strlen(response));
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
    }

    /* send file as body */
    sent = (int) writen(sock, mapped_file, (size_t) content_length);
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
    }

    if (munmap(mapped_file, (size_t) content_length) == -1) {
        perror("munmap");
    }


    LastModified_line(fd);
    fflush(stdout);

    if (close(fd) == -1)
        perror("close\n");

}

/*
 * Send http response message with status code 204 No Content
 *
 */
void send_no_content(int sock) {
    char *response;
    int sent;

    response = malloc(MSG_SIZE);
    if (response == NULL) {
        fprintf(stderr, "error in memory allocation");
    }


    sprintf(response, "%s\r\n%s\r\n%s\r\n\r\n", "HTTP/1.1 204 No Content", "Server: ZannaServer", "Connection: close");
    sent = (int) writen(sock, response, strlen(response));
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
    }

    free(response);
}
