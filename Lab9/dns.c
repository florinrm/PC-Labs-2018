// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	struct addrinfo hints, *result, *p;
	char aux[1500];
	memset (&hints, 0, sizeof(hints));
	// TODO: set hints
	hints.ai_family = AF_UNSPEC;
	ret = getaddrinfo(name, NULL, &hints, &result);
	// TODO: get addresses

	// TODO: iterate through addresses and print them
	for (p = result; p != NULL; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			struct sockaddr_in* addr = (struct sockaddr_in *) p->ai_addr;
			inet_ntop(p->ai_family, &addr->sin_addr, aux, sizeof(aux));
		} else if (p->ai_family == AF_INET6) {
			struct sockaddr_in6* addr = (struct sockaddr_in6 *) p->ai_addr;
			inet_ntop(p->ai_family, &addr->sin6_addr, aux, sizeof(aux));
		}

	}
	// TODO: free allocated data
	printf("%s\n", aux);
	freeaddrinfo(result);
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	addr.sin_family = AF_INET;
	addr.sin_port = htons (80);
	inet_aton(ip, &addr.sin_addr);

	// TODO: fill in address data
	getnameinfo ((struct sockaddr *)&addr, sizeof(struct sockaddr_in), host, 1024, service, 20, 0);

	// TODO: get name and service
	printf("Name: %s Service: %s\n", host, service);
	// TODO: print name and service
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
