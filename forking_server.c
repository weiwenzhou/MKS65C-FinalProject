#include "networking.h"

void process(char *s);
void subserver(int * clients, char * channelName, int from_client, int counter);
void subchannel(char * channelName, char * portNum);

<<<<<<< HEAD
int * clientsConnect = (int*)calloc(10,sizeof(int));

=======
int clientsConnect[10];
int counter = 0;
>>>>>>> 65dae06e0de499d1b562be30d813139cde654fd3

int main() {
    int x;
    for (x=0;x<10;x++) {
        clientsConnect[x] = 0;
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
    //printf("clients[x] %d\n", client_socket);

    f = fork();
    if (f == 0) {
        clientsConnect[counter] = client_socket;
        counter += 1;
        //printf("clients[x] %d\n", client_socket);
        subserver(clientsConnect, channelName, client_socket, counter);
        
    } else {
      close(client_socket);
    }
    }
}


void subserver(int * clients, char * channelName, int client_socket, int counter) {
  char buffer[BUFFER_SIZE];

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("%s[subserver %d] received: [%s]\n", channelName, getpid(), buffer);
    process(buffer);
    int x;
    for (x = 0; x<10; x++) {
        if (clients[x] != 0) {
            printf("clients[x] %d\n", clients[x]);
            write(clients[x], buffer, sizeof(buffer));
        //write(clients[x], test, sizeof(test));
        }
    }
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
}