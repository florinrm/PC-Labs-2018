/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: server_port\n");
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=2)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];
	socklen_t len;


	/*Deschidere socket*/
	int sock_fd = socket (AF_INET, SOCK_DGRAM, 0);
	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons (atoi(argv[1]));
	inet_aton ("192.168.43.135", &(my_sockaddr.sin_addr));

	
	/* Legare proprietati de socket */
	bind(sock_fd, (struct sockaddr *) &my_sockaddr, sizeof(my_sockaddr));
	fd = recvfrom (sock_fd, &buf, BUFLEN, 0, (struct sockaddr*) &from_station, &len);
	buf[fd] = '\0';
	printf("%s\n", buf);
	/* Deschidere fisier pentru scriere */
	//DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/


	/*Inchidere socket*/	
	close(sock_fd);
	
	/*Inchidere fisier*/

	return 0;
}
