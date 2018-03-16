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

int main(int argc, char *argv[])
{
	msg t;
	int i, res;

	int w = atoi(argv[1]) / (8 * (MSGSIZE + 4));
	
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);



	printf("[SENDER]: BDP=%d\n", atoi(argv[1]));
	
	for (i = 0; i < w; i++) {
		/* cleanup msg */
		printf ("I FUCKING LOVE FLORIN POP\n");
		memset(&t, 0, sizeof(msg));
		
		/* gonna send an empty msg */
		t.len = MSGSIZE;
		
		/* send msg */
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
		
	}

	for (i = w; i < COUNT; i++) {
		/* cleanup msg */
		memset(&t, 0, sizeof(msg));
		
		/* wait for ACK */
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
		memset(&t, 0, sizeof(msg));
		/* gonna send an empty msg */
		t.len = MSGSIZE;
		
		/* send msg */
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
	}
	
	for (i = 0; i < w; ++i) {
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
	}

	printf("[SENDER] Job done, all sent.\n");
		
	return 0;
}
