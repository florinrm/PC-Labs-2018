#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc,char** argv) {

	my_pkt p; // package
	msg t; // message
	int fd, count, filesize;
	struct stat status;
	char buffer[MSGSIZE];

	memset(buffer, 0, sizeof(buffer)); // initiate the buffer with zeros

	init(HOST,PORT);

	printf("[SENDER] Starting sender\n");
	printf("[SENDER] Sending file %s\n", argv[1]);

	/* TODO: open file and get filesize */
	// fd = ...
	// filesize = ...

	fd = open (argv[1], O_RDONLY); // opening the file
	fstat(fd, &status); // some info about the file
	filesize = (int) status.st_size; // size of file

	printf("[SENDER] File size: %d\n", filesize);

	/* SEND FILENAME */

	memset(t.payload, 0, sizeof(t.payload));
	memset(p.payload, 0, sizeof(p.payload));
	
	/* build my_pkt var */
	p.type = TYPE1;	
	memcpy(p.payload, argv[1], strlen(argv[1]));

	/* build msg */
	t.len = sizeof(int) + strlen(argv[1]);
	memcpy(t.payload, &p, t.len); 

	/* send message */
	send_message(&t);
	printf("[SENDER] Filename sent.\n");

	/* wait for response */	
	if (recv_message(&t) < 0) {
		perror("[SENDER] Receive error");
		return -1;
	}
	
	/* check if we got ACK */
	p = *((my_pkt *) t.payload);
	if (p.type != TYPE4) {
		perror("[SENDER] Receive error");
		return -1;
	}
	printf("[SENDER] Got reply with payload: %s\n", p.payload);

  	/* TODO: SEND FILESIZE */

	memset(t.payload, 0, sizeof(t.payload));
	memset(p.payload, 0, sizeof(p.payload));
	p.type = TYPE2;
	memcpy(p.payload, &filesize, sizeof(int));
	t.len = 2 * sizeof(int);
	memcpy(t.payload, &p, 2 * sizeof(int));
	send_message(&t);

	printf("[SENDER] Filesize sent.\n");

	/* wait for response */	
	if (recv_message(&t) < 0) {
		perror("[SENDER] Receive error");
		return -1;
	}
	
	/* check if we got ACK */
	p = *((my_pkt *) t.payload);
	if (p.type != TYPE4) {
		perror("[SENDER] Receive error");
		return -1;
	}
	printf("[SENDER] Got reply with payload: %s\n", p.payload);

	/* TODO: SEND DATA */

	while ((count = read(fd, buffer, MSGSIZE - sizeof(int))) > 0) { // sending data from file
		memset(t.payload, 0, sizeof(t.payload));
		memset(p.payload, 0, sizeof(p.payload));
		p.type = TYPE3;
		
		memcpy (p.payload, buffer, count);
		t.len = count + sizeof(int);
		memcpy (t.payload, &p, t.len);
		send_message(&t);

		if (recv_message(&t) < 0) {
			perror("[SENDER] Receive error");
			return -1;
		}
	
		/* check if we got ACK */
		p = *((my_pkt *) t.payload);
		if (p.type != TYPE4) {
			perror("[SENDER] Receive error");
			return -1;
		}
	}
	printf("[SERVER] File transfer has ended.\n");	
	close(fd);

	return 0;
}
