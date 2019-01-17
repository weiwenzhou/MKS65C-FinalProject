#include "networking.h"

void process(char *s);
void subserver(int from_client);
void sendChat(char* s);

int subserver_count = 0;
int connections[10];
int p[2];

int main() {

  int listen_socket;
  int client_socket;
  int f;
  char buffer[BUFFER_SIZE];


  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup(PORT);

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     client_socket = server_connect(listen_socket);

     if (pipe(p) < 0)
	     exit(1);

     f = fork();
     if (f == 0)
       subserver(client_socket);
     else {

      printf("[server] subserver count: %d\n", subserver_count);
       connections[subserver_count++] = client_socket;

       while(read(p[0],buffer, sizeof(buffer)) > 0)
		sendChat(buffer);

       //close(client_socket);
     }

    }//end listen_socket select

    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      printf("[server] subserver count: %d\n", subserver_count);
    }//end stdin select
  }
}

void sendChat(char* s)
{
	for (int i= 0; i< subserver_count; i++)
	{
		write(connections[i], s, sizeof(s));
        }

}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  //for testing client select statement
  strncpy(buffer, "hello client", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (read(client_socket, buffer, sizeof(buffer))) {

    printf("[subserver %d] received: [%s]\n", getpid(), buffer);
    //process(buffer);
    //write(client_socket, buffer, sizeof(buffer));
     for (int i= 0; i< subserver_count; i++)
     {
	     	printf("[subserver %d] servercount: [%d]\n",getpid(),i);
		process(buffer);
                write(connections[i], buffer, sizeof(buffer));
		write(p[1], buffer, sizeof(buffer));
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
