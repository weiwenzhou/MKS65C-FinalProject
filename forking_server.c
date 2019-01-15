#include "networking.h"

void process(char *s);
void subserver(char * channelName, int from_client);
void subchannel(char * channelName, char * portNum);

int main() {
    int num;
    for (num = 49152; num < 49154; num++) {
        
    }
    int f = fork();
    if (f == 0) {
        subchannel("MICHAEL_SERVER","49152");
    } else {
        subchannel("WW_SERVER","49153");
    } 
    //subchannel("49154");
}

void subchannel(char * channelName, char * portNum) {
    int listen_socket;
    int f;
    listen_socket = server_setup(portNum);

    while (1) {

    int client_socket = server_connect(listen_socket);
    f = fork();
    if (f == 0)
      subserver(channelName, client_socket);
    else
      close(client_socket);
    }
}


void subserver(char * channelName, int client_socket) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("%s[subserver %d] received: [%s]\n", channelName, getpid(), buffer);
    process(buffer);
    write(client_socket, buffer, sizeof(buffer));
  }//end read loop
  close(client_socket);
  exit(0);
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}