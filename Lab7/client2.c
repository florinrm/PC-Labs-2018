#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUFLEN 10
// ASTA TRIMITE PRIMUL
int main(int argc, char *argv[])
{
    int sockfd = 0;
    char buff[BUFLEN];
    struct sockaddr_in serv_addr; 
    int rc;
    

    // deschidere socket
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);    

    // completare informatii despre adresa serverului
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(4000);
    //serv_addr.sin_addr.s_addr = INADDR_ANY;    
    rc = inet_aton("192.168.43.30", &(serv_addr.sin_addr));
    //bind(socket_descriptor,&(serv_addr), sizeof(struct sockaddr_in));
    if (rc < 0) {
        perror("inet_aton");
        exit(-1);
    }

    // conectare la server
    rc= connect(socket_descriptor, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if (rc < 0){
		printf("EROARE LA CONNECT");
		return 1;
    }
    // citire de la tastatura, trimitere de cereri catre server, asteptare raspuns
    while(1) {
    	char buf[10];
        scanf("%s", buf);
        buf[9] = '\0';
        send(socket_descriptor, buf, 10, 0);
        rc = recv(socket_descriptor, buff, 10, 0);
        if(rc  == 0) {
        // s-a inchis conexiunea
            break;
        } else if (rc < 0) {
        perror("recv");
            exit(-1);
        }
        printf("C2: %s\n", buff);
    }
    // inchidere socket
    close(socket_descriptor);
    return 0;
}