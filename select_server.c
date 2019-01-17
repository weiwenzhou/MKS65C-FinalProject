#include "networking.h"

void process(char *s);
void channel(char * portNum);
char ** parse_args(char * line, char * split);

char ** parse_args(char * line, char * split) {
    line = strsep(&line, "\n");
    char ** args = malloc(sizeof(char *) * 10);
    int x = 0;
    while (line) {
        args[x] = strsep( &line, split);
        x += 1;
    }
    args[x] = NULL;
    return args;
}

int f; // fork pid
//char* validPorts[256];
//int channelCount = 0;

static void sighandler(int signo) {
    if (signo == SIGINT) {
        char ** myargs = parse_args("killall -9 server", " ");
        execvp(myargs[0], myargs);
    }
}


int main() {
    channel(PORT);
}

void channel(char * portNum) {
    signal(SIGINT, sighandler);
    
    // ADD port to valid Ports
    //validPorts[channelCount] = portNum;
    //channelCount += 1;
    
    int listen_socket;
    int client_socket;

    int subserver_count = 0;
    char buffer[BUFFER_SIZE];
    
    int max_clients = 16; // max # of clients to handle
    int clientsConnected[max_clients]; // array of client sockets
    int fd_max; // the value of the largest fd
    int i; // for loops

    //set of file descriptors to read from
    fd_set read_fds;

    // initialize the array of clients
    for (i = 0; i < max_clients; i++) {
        clientsConnected[i] = 0;          
    }

    // create the server socket
    listen_socket = server_setup(portNum);
    fd_max = listen_socket; // largest file descriptor

    while (1) {
        //printf("I am at the top\n");
        
        //select() modifies read_fds
        //we must reset it at each iteration
        FD_ZERO(&read_fds); //0 out fd set
        //FD_ZERO(&clients); //0 out fd set
        FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
        FD_SET(listen_socket, &read_fds); //add socket to fd set

        
        // add client sockets to the fd set
        for (i = 0; i < max_clients; i++) {
            //printf("What is occuring? i:%d == fd:%d\n", i, clientsConnected[i]);
            
            // if not 0 add to fd set
            if (clientsConnected[i] > 0) {
                FD_SET(clientsConnected[i], &read_fds);
                
                //printf("Added %d to fd_set\n", clientsConnected[i]);
            }
            
            // Ensure fd_max remains the larget fd
            if (clientsConnected[i] > fd_max) {
                fd_max = clientsConnected[i];
            }
        }

        //printf("I am waiting here instead?\n");
        
        //select will block until either fd is ready 
        // 1st argument = total number of to look at
        // from (0 to fd_max+1)
        select(fd_max + 1, &read_fds, NULL, NULL, NULL);
        //printf("I am waiting\n");
        
        //if listen_socket triggered select
        // someone connects to the server
        if (FD_ISSET(listen_socket, &read_fds)) {
            
            //printf("I hear something\n");

            // create the new socket to communicate with client
            client_socket = server_connect(listen_socket);
            
            //for testing client select statement
            strncpy(buffer, "[SERVER BROADCAST] [hello client]", sizeof(buffer));
            write(client_socket, buffer, sizeof(buffer));

            //subserver_count++;
            //FD_SET(client_socket, &clients); // Add clients if subserver starts
            
            //printf("I am here\n");

            // Add the client socket to collection of sockets
            for (i = 0; i < max_clients; i++) {
                //printf("Do I come here?\n");
                if (!clientsConnected[i]) { // when clients[x] is 0
                    // add client_socket to clientsConnected
                    clientsConnected[i] = client_socket; 
                    printf("Client_socket %d has connected to port %s\n", client_socket, portNum);
                    
                    //printf("Am I adding?\n");
                    
                    break;
                }
            }

            //fd_max = client_socket; // Set fd_max to the largest fd

        } 
      
        //printf("I am here2\n");
        // listen to clients here
        for (i = 0; i < max_clients; i++) {
            //printf("Coming through %d?\n", i);
            if (FD_ISSET(clientsConnected[i], &read_fds)) {
                if (read(clientsConnected[i], buffer, sizeof(buffer))) {
                    // reads something here
                    
                    // Want to create a channel
                    if (strstr(buffer, "#create ") ){
                        char * newPort = parse_args(buffer, " ")[2];
                        //strcat(buffer, "Created channel at port ");
                        strcat(buffer, newPort);
	                    f = fork();
                        if (f == 0) {
                            channel(newPort);
                        } 
                    }
                    
                    // Want to join a different channel
                    if (strstr(buffer, "#join ") ){
                        close(clientsConnected[i]);
                        clientsConnected[i] = 0;
                        // Send to all clients
                        for (i =0; i < max_clients; i++) {
                            if (clientsConnected[i] > 0) {
                                write(clientsConnected[i], buffer, sizeof(buffer));
                            }
                        }
                    } 
                    /*
                    // Check channels
                    if (strstr(buffer, "#channels") ){
                        char channelBuffer[BUFFER_SIZE];
                        strcpy(channelBuffer, "Channels available: ");
                        write(clientsConnected[i], channelBuffer, sizeof(channelBuffer));
                        int i;
                        for (i = 0; i < channelCount; i++) {
                            printf("%s\n" validPorts[channelCount]);
                            strcpy(channelBuffer, validPorts[channelCount]);
                            write(clientsConnected[i], channelBuffer, sizeof(channelBuffer));
                        }
                    }
                    
                    */
                    //printf("Am I touching here?\n");
                    printf("[subserver %d] received: [%s]\n", getpid(), buffer);

                    //process(buffer);
                    //write(clientsConnected[i], buffer, sizeof(buffer));
                    // Send to all clients
                    for (i =0; i < max_clients; i++) {
                        if (clientsConnected[i] > 0) {
                            write(clientsConnected[i], buffer, sizeof(buffer));
                        }
                    }
                    
                    //send_to_all_clients(buffer, sizeof(buffer));
                } else {
                    // nothing is read 
                    close(clientsConnected[i]);
                    clientsConnected[i] = 0;
                }
            }
        }
        //printf("I am here3\n");
        //printf("HERE\n");
        //close(listen_socket);
        //listen_socket = server_setup(PORT);

        /*
        f = fork();
        if (f == 0) {
            FD_SET(client_socket, &clients); // Add clients if subserver starts
            fd_max = client_socket; // Set fd_max to the largest fd

            subserver(client_socket);
        } else {
            subserver_count++;
            //close(client_socket);
        }
        */
    //}//end listen_socket select

        //if stdin triggered select
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
          //if you don't read from stdin, it will continue to trigger select()
          fgets(buffer, sizeof(buffer), stdin);
          printf("[server] subserver count: %d\n", subserver_count);
        }//end stdin select
    } // end of while loop    
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
