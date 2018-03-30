/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: ip_server port_server\n");
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc, char**argv)
{
	if (argc!=3)
		usage(argv[0]);
	
	int socket_fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];


	/*Deschidere socket*/
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	to_station.sin_family = AF_INET;
	to_station.sin_port = htons(atoi(argv[2]));
	inet_aton("192.168.43.135", &(to_station.sin_addr));
	
	strcpy(buf, "Ana are mere");
	sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr *) &to_station, sizeof(to_station));

	/*Inchidere socket*/
	close(socket_fd);

	return 0;
}