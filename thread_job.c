#include <MagickWand/MagickWand.h>
#include "server.h"
#include <http_parser.h>
#include "thread_job.h"
#include "lib/io.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <lru-cache.h>

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

unsigned int find_content_lenght(int fd) {

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
    MagickReadImage(m_wand, path_orig);

    // Set the compression quality to 95 (high quality = low compression)
    MagickSetImageCompressionQuality(m_wand, (const size_t) img->quality_factor * 100);

    /* Write the new image on temporary file*/
    errno = 0;
    fd = mkostemp(path_tmp, 666);
    if (fd == -1 || errno != 0) {
        perror("mkostemp");
        pthread_exit(NULL);
    }

    MagickWriteImage(m_wand, path_tmp);

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

    sprintf(response, "%s\r\n%s\r\n\r\n", "HTTP/1.1 404 Not Found", "Connection: close");
    sent = (int) writen(sock, response, strlen(response));

    if (sent <= 0) {
        fprintf(stderr, "error in writen");
        pthread_exit(NULL);
    }
}

/*
 * Send an http message with a file as body.
 * @fd file descriptor for the file to send
 * @sock connection socket with the client node
 */
void send_file(int fd, int sock, unsigned int content_length) {

    struct stat st;                 /* strcture to discover file's size */
    char *response, *mapped_file;   /* memory where map the file */
    int sent, content_lenght, s;

    /* allocate memory for response message */
    response = malloc(MSG_SIZE);
    if (response == NULL) {
        fprintf(stderr, "error in memory allocation");
        pthread_exit(NULL);
    }

    /* get image size */
    errno = 0;
    s = fstat(fd, &st);
    if (s == -1 || errno != 0) {
        perror("fstat");
        pthread_exit(NULL);
    }
    content_lenght = (int) st.st_size;

    /* create http header*/
    sprintf(response, "%s\r\n%s\r\n%s: %d\r\n\r\n", "HTTP/1.1 200 OK", "Connection: close", "Content-Length",
            content_lenght);

    /* map file */
    mapped_file = mmap(NULL, (size_t) content_lenght, PROT_READ, MAP_PRIVATE, fd, 0);

    /* send headers*/
    sent = (int) writen(sock, response, strlen(response));
    if (sent <= 0) {
        fprintf(stderr, "error in writen");
        pthread_exit(NULL);
    }

    /* send file as body */
    sent = (int) writen(sock, mapped_file, (size_t) content_lenght);
    if (sent != content_lenght) {
        fprintf(stderr, "error in writen");
        pthread_exit(NULL);
    }

    if (munmap(mapped_file, (size_t) content_lenght) == -1) {
        perror("munmap");
        pthread_exit(NULL);
    }

}

/*
 * Send http response
 */
void response_GET(converted_image *img, int socket, log_t *log) {

    int fd;
    unsigned int content_length;
    char *file_tmp, path[MAXLINE];

    file_tmp = find_in_cache(img);
    if (file_tmp == NULL) {

        printf("Image NOT found in cache\n");
        fd = convert_image(img);
        put_in_cache(img);

    } else {

        /* build image path */
        strcpy(img->temp_file, file_tmp);
        strcpy(path, CACHE);
        strcat(path, img->temp_file);

        printf("Image found in cache\n");

        fd = open_file(path);
        /* cache is in a inconsistent state: unexpected error */
        if (fd == -1) {
            fd = convert_image(img);
            put_in_cache(img);
        }

    }

    content_length = find_content_lenght(fd);
    send_file(fd, socket, content_length);

    log->bytes = content_length;
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
 * Manage the connection with a client.
 */
void manage_request(data_t *data, http_parser *parser) {

    char *filename, *format, path[128];

    converted_image *img = malloc(sizeof(converted_image));
    if (img == NULL) {
        fprintf(stderr, "error in memory allocation");
        pthread_exit(NULL);
    }

    http_message *message = data->msg;
    filename = message->request_path;

    strcpy(path, ROOT);
    strcat(path, filename);
    strcpy(img->name, filename);

    if (exist(path)) {

        format = get_filename_ext(filename);
        img->quality_factor = find_quality_factor(message->accept, format);

        response_GET(img, data->sock, data->log);
        data->log->status = 200;

    } else {
        send_not_found(data->sock);

        data->log->bytes = 0;
        data->log->status = 404;
    }

    logging(data->log);
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
 * Receive the request message and parses it.
 */
void *connection_manager(void *arg) {

    data_t *data = arg;
    http_parser *parser;
    int nread, n;
    fd_set rset;
    char *raw_msg;
    struct timeval timeout;

    timeout.tv_sec = TIMEOUT;
    FD_ZERO(&rset);
    FD_SET(data->sock, &rset);

    pthread_cleanup_push(th_exit, NULL) ;

            do {

                /* if no message are received in timeout seconds, the connection is closed */
                n = select(data->sock + 1, &rset, NULL, NULL, &timeout);
                if (n == 0) {
                    close_connection(data->sock);
                    pthread_exit(NULL);
                }

                /* read http header from connsd socket and store it in msg buffer */
                bzero(raw_msg, 0);
                nread = receive_msg_h(data->sock, (void **) &raw_msg);
                if (nread == 0) {
                    // connection closed from client side
                    pthread_exit(NULL);
                }

                data->msg->raw = strdup(raw_msg);

                printf("%s", raw_msg);

                /* parsing http header */
                parser = parse(data, raw_msg, (size_t) nread);

                data->log->request = get_request(raw_msg);

                /* For HTTP/1.1 persistent connection is the default behavior*/
                if (parser->type == HTTP_REQUEST)
                    manage_request(data, parser);


                free(data->msg);

            } while (http_should_keep_alive(parser));


            printf("[+] Close connection\n");

            pthread_exit(NULL);

    pthread_cleanup_pop(0);

}





