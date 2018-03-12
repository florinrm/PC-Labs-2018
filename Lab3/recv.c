#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int message_parity (char *message) {
  int parity = 0;
  for (int i = 0; i < strlen(message); ++i) {
    int mask = 0;
    for (int j = 0; j < 8; ++j) {
      mask = 1 << j;
      parity ^= (message[i] & mask) >> j;
    }
  }
  return parity;
}

int main(int argc,char** argv){
  msg r, t;
  my_pkt p;
  init(HOST,PORT);
  memset(r.payload, 0, sizeof(r.payload));
  memset(p.payload, 0, sizeof(p.payload));

  if (recv_message(&r) < 0){
    perror("Receive message");
    return -1;
  }
  p = (*(my_pkt *) r.payload);
  printf("[%s] Got msg with payload: %s\n",argv[0], p.payload);
  // puts(p.payload);
  if (p.type == message_parity (p.payload))
    printf("[Receiver] Mesaj primit corect!\n");
  else
    printf("[Receiver] Mesaj primit gresit!\n");
  sprintf(t.payload,"ACK");
  t.len = strlen(t.payload+1);
  send_message(&t);
  return 0;
}
