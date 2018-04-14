#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFLEN 256

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    fd_set read_fds;  //multimea de citire folosita in select()
    fd_set tmp_fds;    //multime folosita temporar 
    int fdmax;     //valoare maxima file descriptor din multimea read_fds

    FD_ZERO(&read_fds);
    FD_ZERO(&tmp_fds);

    char buffer[BUFLEN];
    if (argc < 3) {
       fprintf(stderr,"Usage %s server_address server_port\n", argv[0]);
       exit(0);
    }  
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    inet_aton(argv[1], &serv_addr.sin_addr);
    
    
    if (connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");    
    
    FD_SET(sockfd, &read_fds);
    FD_SET(0, &read_fds);
    fdmax = sockfd;
    int i;
     // main loop
    while (1) {
        tmp_fds = read_fds; 
        if (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1) 
            error("ERROR in select");
    
        for(i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &tmp_fds)) {
                if(i != 0)
                {
                    memset(buffer, 0, BUFLEN);
                    if ((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
                        if (n == 0) {
                            
                            //conexiunea s-a inchis
                            printf("selectserver: socket %d hung up\n", i);
                        } else {
                            error("ERROR in recv");
                        }
                        close(i); 
                        FD_CLR(i, &read_fds); // scoatem din multimea de citire socketul pe care 
                    }
                    else{
                        puts(buffer);
                    }
                }
                else
                {
                    memset(buffer, 0 , BUFLEN);
                    fgets(buffer, BUFLEN-1, stdin);

                    //trimit mesaj la server
                    n = send(sockfd,buffer,strlen(buffer), 0);
                    if (n < 0) 
                        error("ERROR writing to socket");
                }
            }
        }

   

    }
    return 0;
}

