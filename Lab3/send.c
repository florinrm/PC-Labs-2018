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

int message_parity (my_pkt message) {
  int parity = 0;
  for (int i = 0; i < strlen(message.payload); ++i) {
    int mask = 0;
    for (int j = 0; j < 8; ++j) {
      mask = 1 << j;
      parity ^= (message.payload[i] & mask) >> j;
    }
  }
  return parity;
}

int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;
  my_pkt p;

  memset(t.payload, 0, sizeof(t));
  memset(p.payload, 0, sizeof(p));

  sprintf(p.payload,"Hello World of PC");
  p.type = message_parity(p);
  t.len = strlen(p.payload) + sizeof(int) +1;
  //puts(p.payload);
  memcpy(t.payload, &p, t.len);
  send_message(&t);

  if (recv_message(&t)<0){
    perror("receive error");
  }
  else {
    printf("[%s] Got reply with payload: %s\n",argv[0],t.payload);
  }

  return 0;
}
