#include <MagickWand/MagickWand.h>
#include "server.h"
#include <http_parser.h>
#include "thread_job.h"
#include "lib/io.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <lru-cache.h>

void free_resources(data_t *data) {

    free(data->msg);
    free(data->log);
    free(data);
}

void close_connection(int sock) {

    int closed;
    closed = close(sock);

    if (closed == -1) {
        perror("close");
        pthread_exit(NULL);
    }

}

/*
 * Parse the header's field "Accept" of http message to find quality factor
 * for "Image/*"
 * @param accept field Accept in http message*s header
 * @return quality factor for image/jpeg format
 */
float find_quality_factor(char *accept, char *image_extension) {


    char *token, *ptr, field[MAXLINE];
    float quality_factor = 1;

    strcat(field, "image/");
    strcat(field, image_extension);

    if ((ptr = strstr(accept, field)) != NULL) {
        token = ptr + strlen(field) + 1;
    } else if ((ptr = strstr(accept, "*/*")) != NULL) {
        token = ptr + strlen("*/*") + 1;
    } else return quality_factor;

    removeSpaces(token);
    if ((token = strstr(token, "q=")) != NULL) {
        token += strlen("q=");

        errno = 0;
        quality_factor = strtof(token, NULL);
        if (errno != 0) {
            perror("strtof");
            pthread_exit(NULL);
        }
    }

//    printf("%s: %.2f\n", "quality factor found", quality_factor);
    return quality_factor;
}

unsigned int find_content_length(int fd) {

    struct stat st;                 /* strcture to discover file's size */
    int s;

    /* get image size */
    errno = 0;
    s = fstat(fd, &st);
    if (s == -1 || errno != 0) {
        perror("fstat");
        pthread_exit(NULL);
    }

    return (unsigned int) st.st_size;
}

/*
 *
 * @param path image path
 * @param quality_factor new quality compression factor for the image
 * @param out output path
 * @return file descriptor for image saved in temporary file
 */
int convert_image(converted_image *img) {

    MagickWand *m_wand = NULL;
    int fd;
    char path_tmp[128];
    char path_orig[128];

    bzero(path_tmp, 0);
    strcpy(path_tmp, CACHE);
    strcat(path_tmp, "image.XXXXXX");


    bzero(path_orig, 0);
    strcpy(path_orig, ROOT);
    strcat(path_orig, img->name);

    MagickWandGenesis();

    m_wand = NewMagickWand();

    // Read the image
    if (MagickReadImage(m_wand, path_orig) == MagickFalse) {
        fprintf(stderr, "error MagickReadImage\n");
        pthread_exit(NULL);
    }


    // Set the compression quality to 95 (high quality = low compression)
    if (MagickSetImageCompressionQuality(m_wand, (const size_t) img->quality_factor * 100) == MagickFalse) {
        fprintf(stderr, "error MagickSetImageCompressionQuality\n");
        pthread_exit(NULL);
    }

    /* Write the new image on temporary file*/
    errno = 0;
    fd = mkostemp(path_tmp, 666);
    if (fd == -1 || errno != 0) {
        perror("mkostemp");
        pthread_exit(NULL);
    }

    if (MagickWriteImage(m_wand, path_tmp) == MagickFalse) {
        fprintf(stderr, "error MagicWriteImage\n");
        pthread_exit(NULL);
    }

    /* Clean up */
    if (m_wand)m_wand = DestroyMagickWand(m_wand);

    strcpy(img->temp_file, basename(path_tmp));

    MagickWandTerminus();

    return fd;
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
        pthread_exit(NULL);
    }


    sprintf(response, "%s\r\n%s\r\n\r\n%s", "HTTP/1.1 404 Not Found", "Connection: close",
            "<html><body><h1>404 Page Not Found. </h1></body></html>");
    sent = (int) writen(sock, response, strlen(response));
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
        pthread_exit(NULL);
    }

    free(response);

}

/*
 * Send an http message with a file as body.
 * @fd file descriptor for the file to send
 * @sock connection socket with the client node
 */
void send_file(int fd, int sock, unsigned int content_length) {

    char response[MSG_SIZE], *mapped_file;   /* memory where map the file */
    int sent;

    /* create http header*/
    sprintf(response, "%s\r\n%s\r\n%s: %d\r\n\r\n", "HTTP/1.1 200 OK", "Connection: close", "Content-Length",
            content_length);

    /* map file */
    mapped_file = mmap(NULL, (size_t) content_length, PROT_READ, MAP_PRIVATE, fd, 0);

    /* send headers*/
    sent = (int) writen(sock, response, strlen(response));
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
        pthread_exit(NULL);
    }

    /* send file as body */
    sent = (int) writen(sock, mapped_file, (size_t) content_length);
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
        pthread_exit(NULL);
    }

    if (munmap(mapped_file, (size_t) content_length) == -1) {
        perror("munmap");
        pthread_exit(NULL);
    }

    if (close(fd) == -1)
        perror("close\n");

}

void convert_and_send(data_t *data, char *path) {

    char *format;
    int fd;
    unsigned int content_length;
    char *file_tmp;

    converted_image *img = malloc(sizeof(converted_image));
    if (img == NULL) {
        fprintf(stderr, "error in memory allocation");
        pthread_exit(NULL);
    }

    strcpy(img->name, data->msg->request_path);
    format = get_filename_ext(img->name);
    img->quality_factor = find_quality_factor(data->msg->accept, format);

    file_tmp = find_in_cache(img);
    if (file_tmp == NULL) {
        /* file not found in cache */
        fd = convert_image(img);
        put_in_cache(img);

    } else {
        /* file found in cache */

        /* build image path */
        strcpy(path, CACHE);
        strcat(path, file_tmp);

        fd = open_file(path);
        if (fd == -1) {
            /* cache is in a inconsistent state. temp_file not exist really */
            fd = convert_image(img);
            put_in_cache(img);

            perror("open_file\n");
            return;

        } else strcpy(img->temp_file, file_tmp);

    }

    content_length = find_content_length(fd);
    send_file(fd, data->sock, content_length);

    free(img);

}

/*
 * Manage the connection with a client.
 */
void manage_request(data_t *data, http_parser *parser) {

    char *filename = data->msg->request_path, path[128];
    int fd;
    unsigned int content_length;

    if (strcmp(filename, "/") == 0) {
        send_not_found(data->sock);
        return;
    }

    if (strcmp(filename, "favicon.ico") == 0) {
        return;
    }

    strcpy(path, ROOT);
    strcat(path, filename);

    if (exist(path)) {

        if (is_image(filename)) {

            printf("is Image\n");
            convert_and_send(data, path);

        } else {

            fd = open_file(path);
            content_length = find_content_length(fd);
            send_file(fd, data->sock, content_length);

            data->log->bytes = content_length;
        }

        data->log->status = 200;

    } else {

        send_not_found(data->sock);

        data->log->bytes = 0;
        data->log->status = 404;

    }

    logging(data->log);
}

/*
 *
 * @param data thread custom data
 * @param http_msg strcture representing http message
 * @param recved #byte of the http message read from socket
 */
http_parser *parse(data_t *data, char *http_msg, size_t recved) {

    http_parser_settings settings;
    http_parser *parser = malloc(sizeof(http_parser));
    if (parser == NULL) {
        fprintf(stderr, "error in memory allocation");
        pthread_exit(NULL);
    }

    /* setting callbacks */
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_url = on_url;

    /* set message type (HTTP_REQUEST) */
    http_parser_init(parser, HTTP_REQUEST);
    parser->data = data;

    /* parse */
    parser->http_errno = 0;
    http_parser_execute(parser, &settings, http_msg, recved);
    if (parser->http_errno != 0) {
        fprintf(stderr, "http_parser_execute error\n");
        pthread_exit(NULL);
    }

    return parser;
}

/*
 * Receive the request message and parses it.
 */
void *connection_manager(void *arg) {

    data_t *data = arg;
    http_parser *parser;
    int nread, n;
    fd_set rset;
    char raw_msg[MSG_SIZE+1];
    struct timeval timeout;

    pthread_cleanup_push(th_exit, NULL) ;

            timeout.tv_sec = TIMEOUT;
            FD_ZERO(&rset);
            FD_SET(data->sock, &rset);

            do {

                /* if no message are received in timeout seconds, the connection is closed */
                n = select(data->sock + 1, &rset, NULL, NULL, &timeout);
                if (n == 0) {
                    printf("[*] Client sends not data: Connection closed\n");
                    close_connection(data->sock);
                    free_resources(data);
                    pthread_exit(NULL);
                }

                /* read http header from connsd socket and store it in msg buffer */
                memset(raw_msg, '\0', MAXLINE);
                nread = receive_msg_h(data->sock, raw_msg);
                if (nread == 0) {
                    // connection closed from client side
                    pthread_exit(NULL);
                }

                printf("%s\n\n", raw_msg);

                /* parsing http header */
                parser = parse(data, raw_msg, strlen(raw_msg));

                data->log->request = get_request(raw_msg);

                /* For HTTP/1.1 persistent connection is the default behavior*/
                if (parser->type == HTTP_REQUEST)
                    manage_request(data, parser);


                printf("%s\n\n", "########################");
                fflush(stdout);
            } while (http_should_keep_alive(parser));


            close_connection(data->sock);
            free_resources(data);
            pthread_exit(NULL);

    pthread_cleanup_pop(0);

}





