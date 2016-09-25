#include "thread_job.h"

/**
 * free resource on exit
 * @param data resource to free
 */
void free_resources(data_t *data) {

    free(data->msg);
    free(data->log);
    free(data);
}

/**
 * close connection with specified socket
 * @param sock
 */
void close_connection(int sock) {

    int closed = close(sock);
    if (closed == -1) {
        perror("close");
        pthread_exit(NULL);
    }

}

/**
 * Convert image according to qualityf actor
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
    strcpy(path_tmp, ROOT());
    strcat(path_tmp, CACHE);
    strcat(path_tmp, "/image.XXXXXX");

    bzero(path_orig, 0);
    strcpy(path_orig, ROOT());
    strcat(path_orig, CONTENT_DIR);
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

    strcpy(img->temp_file, "/");
    strcat(img->temp_file, basename(path_tmp));

    MagickWandTerminus();

    return fd;
}

/**
 * @param data thread data
 * @param path file to convert
 */
void convert_and_send(data_t *data, char *path) {

    char *format;
    int fd, content_length;
    char *file_tmp;

    converted_image *img = malloc(sizeof(converted_image));
    if (img == NULL) {
        fprintf(stderr, "error in memory allocation");
        pthread_exit((void *) 1);
    }

    strcpy(img->name, data->msg->request_path);
    format = get_filename_ext(img->name);
    img->quality_factor = find_quality_factor(data->msg->accept, format);
    if (img->quality_factor == -1) {
        fprintf(stderr, " error parsing accept field. quality factor cannot be found\n");
        pthread_exit((void *) 1);
    }

    file_tmp = find_in_cache(img);
    if (file_tmp == NULL) {
        /* file not found in cache */
//        printf("%s\n", "FILE not fount in cache...image conversion");

        fd = convert_image(img);
        put_in_cache(img);

    } else {
        /* file found in cache */
//        printf("%s\n", "FILE found in cache. Skip conversion");

        /* build image path */
        strcpy(path, ROOT());
        strcat(path, CACHE);
        strcat(path, file_tmp);

        fd = open_file(path);
        if (fd == -1) {
//           printf("%s\n", "cache is in a inconsisten state...");
            /* cache is in a inconsistent state. temp_file not exist really */
            fd = convert_image(img);
            put_in_cache(img);

            perror("open_file\n");
            return;

        } else strcpy(img->temp_file, file_tmp);

    }

    content_length = send_ok(fd, data->sock);
    data->log->bytes = content_length;
}

/**
 * Manage the connection with a client.
 * @param data thread data
 * @param method request method (GET or HEAD)
 */
void manage_request(data_t *data, unsigned int method) {

    char *filename = data->msg->request_path, path[128];
    int fd;
    int content_length;

    strcpy(path, ROOT());
    strcat(path, CONTENT_DIR);
    strcat(path, filename);


    if (exist(path)) {

        switch (method) {

            case HTTP_GET:

                if (strcmp(filename, "/") == 0) {
                    strcat(path, "index.html");
                    fd = open_file(path);
                    content_length = send_ok(fd, data->sock);

                    data->log->bytes = content_length;
                    data->log->status = 200;
                    logging(data->log);
                    return;
                }

                if (isImage(filename))
                    convert_and_send(data, path);
                else {

                    fd = open_file(path);
                    content_length = send_ok(fd, data->sock);

                    data->log->bytes = content_length;
                }

                data->log->status = 200;
                break;

            case HTTP_HEAD:
                send_ok_head(data->sock);
                break;

            default:
                break;
        }

    } else {

        switch (method) {
            case HTTP_GET:
                send_not_found(data->sock);

                data->log->bytes = 0;
                break;

            case HTTP_HEAD:
                send_not_found_head(data->sock);

                break;
            default:
                break;
        }

        data->log->status = 404;

    }

    logging(data->log);
}

/**
 * Receive the request message and parses it. It is the main function for thread
 */
void *connection_manager(void *arg) {

    data_t *data = arg;
    http_parser *parser;
    int nread;
    fd_set rset;
    char raw_msg[MSG_SIZE + 1];
    struct timeval timeout;

    /* register function to call at exit */
    pthread_cleanup_push(th_exit, NULL) ;

            timeout.tv_sec = TIMEOUT;
            FD_ZERO(&rset);
            FD_SET(data->sock, &rset);

            /* if no message are received in timeout seconds, the connection is closed */
            if (select(data->sock + 1, &rset, NULL, NULL, &timeout) == 0) {
//                    printf("[*] Client sends not data: Connection closed\n");
                close_connection(data->sock);
                free_resources(data);
                pthread_exit((void *) 0);
            }

            /* read http header from connsd socket and store it in msg buffer */
            memset(raw_msg, '\0', MAXLINE);
            nread = receive_msg_h(data->sock, raw_msg);
            if (nread == 0) {
                // connection closed from client side
                pthread_exit((void *) 1);
            }

//                printf("%s\n\n", raw_msg);
//                fflush(stdout);

            /* parsing http header */
            parser = parse(data, raw_msg, strlen(raw_msg));
            if (parser == NULL) {
                fprintf(stderr, "parse return NULL: parsing error\n");
                pthread_exit((void *) 1);
            }

            data->log->request = get_request(raw_msg);

            /* For HTTP/1.1 persistent connection is the default behavior*/
            if (parser->type == HTTP_REQUEST)
                manage_request(data, parser->method);


            close_connection(data->sock);
            free_resources(data);
            pthread_exit((void *) 0);

    pthread_cleanup_pop(0);

}





