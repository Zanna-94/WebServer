#include "server.h"
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/io.h"

void send_message(int);

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        fprintf(stderr, "utilizzo: client <indirizzo IP server> <#children> "
                "<#cicli/child> <#byte/richiesta>");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset((void *) &servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "errore in inet_pton per %s\n", argv[1]);
        exit(1);
    }
    /* si connette al server */
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(1);
    }


    send_message(sockfd);

    /* chiude la connessione con il server */
    if (close(sockfd) == -1) {  /* TIME_WAIT sul client, non sul server */
        perror("close");
        exit(1);
    }

    exit(EXIT_SUCCESS);
}

void send_message(int sock) {

    char *message = "GET /home/emanuele/Scrivania/01.jpeg HTTP/1.1\r\n"
            "Accept:image/jpeg;q=0.4, text/plain;q=1 \r\n"
            "Host: 127.0.0.1:5193\r\n\r\n";

    if (writen(sock, message, strlen(message)) < 0) {
        fprintf(stderr, "error in writen\n");
        exit(EXIT_FAILURE);
    }
}

