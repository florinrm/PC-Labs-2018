/*
 * Protocoale de comunicatii
 * Laborator 7 - TCP
 * Echo Server
 * server.c
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define BUFLEN 1500

int main(int argc, char *argv[])
{
    int connFd1, connFd2;
    char buf[BUFLEN];
    struct sockaddr_in serv_addr; 
    int enable = 1, rc;

    // deschidere socket
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        perror ("socket");
        exit (-1);
    }
    // completare informatii despre adresa serverului
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons(4000);
    socklen_t size = sizeof(struct sockaddr_in);
    // legare proprietati de socket

    rc = setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if(rc < 0) {
        perror("Opt error\n");
        exit(-1);
    }
    rc = bind(sockFd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
    if (rc < 0) {
        perror("Error bind\n");
        exit(-1);
    }
    listen(sockFd, 2);

    connFd1 = accept(sockFd, (struct sockaddr*)&serv_addr, &size);
    connFd2 = accept(sockFd, (struct sockaddr*)&serv_addr, &size);
    
    while (1) {
        recv(connFd1, buf, 10, 0);
        send(connFd2, buf, 10, 0);
        printf("Client1: %s\n", buf);
        // printf("Client1 send: %s\n", buf);
        recv(connFd2, buf, 10, 0);
        // printf("Client2: %s\n", buf);
        send(connFd1, buf, 10, 0);
        printf("Client2: %s\n", buf);
    }
    close(sockFd);

    return 0;
}