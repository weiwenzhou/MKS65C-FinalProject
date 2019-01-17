#include "networking.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

char clientName[256];


int logging(){
	char choice[256];
    char uName[256];
    char uPass[256];
    
	printf("Hello! Welcome to WeiChat!  Would you like to login or create an account?\n(Login/Create)\n");
	fgets(choice,256,stdin);
	choice[strlen(choice) -1] = 0;

	if(strcmp(choice,"Create") == 0 || strcmp(choice,"create") == 0){
		printf("Please type in your desired username: ");
		fgets(uName,256,stdin);
		uName[strlen(uName) -1] = 0;

		printf("Please type in your desired password: ");
		fgets(uPass,256,stdin);
	//	printf("HERE\n");
	//	printf("length %ld\n", strlen(uPass));
	//	printf("length [%s]\n", &uPass[strlen(uPass)-1]);
		uPass[strlen(uPass) -1] = 0;
	//	printf("length %ld\n", strlen(uPass));
		strcat(uName,".txt");
		//printf("Testing uName %s\n",uName);

		int accFile = open(uName, O_WRONLY | O_CREAT,0666);
		//printf("Testing uPass %s\n",uPass);

		write(accFile,uPass,strlen(uPass));
		if(accFile != -1){
			printf("You have succesfully created your account!\n");
			return 1;
		}
		else{
			printf("OOPS! Something went wrong.");
		}
		close(accFile);

	}

	if(strcmp(choice,"Login") == 0 || strcmp(choice,"login") == 0){
		char check[256];
		printf("Please type in your username: ");
		fgets(uName,256,stdin);
		uName[strlen(uName) -1] = 0;
        strcpy(clientName, uName);
		strcat(uName,".txt");

		printf("Please type in your password: ");
		fgets(uPass,256,stdin);
		uPass[strlen(uPass) -1] = 0;
		//strip(uPass);

		int accFile = open(uName,O_RDONLY);
		if(accFile == -1){
			printf("You have entered a wrong username\n");
		}
		else{
			//ERROR HERE IN THE BYTES
		//	FILE* fp = fopen(uName, "r");
			//read(accFile,check,255);
			read(accFile,check,255);
			if(strcmp(check,uPass) == 0){
				//printf("SAVED PASS %s\n",check);
                printf("\e[1;1H\e[2J"); // clear the screen on login success
				printf("You have successfully logged in!\n Please remember to be polite in the chatrooms!\n");
				return 1;
			}
			else{
				//printf("SAVED PASS %s\n",check);
				printf("You have entered a wrong password\n");
			}
		}

		}

	return 0;
	}


int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];

    fd_set read_fds;

    if (argc == 2)
        server_socket = client_setup( argv[1], PORT);
    else
        server_socket = client_setup( TEST_IP, PORT );
	
    //printf("Did i get here\n");
    printf("\e[1;1H\e[2J"); // clear the screen on boot

	int rad = logging();
    while (1) {
	if(rad == 1){

        //printf("enter data: ");
        //the above printf does not have \n
        //flush the buffer to immediately print
        fflush(stdout);

        //select() modifies read_fds
        //we must reset it at each iteration
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
        FD_SET(server_socket, &read_fds); //add socket to fd set

        //select will block until either fd is ready
        select(server_socket + 1, &read_fds, NULL, NULL, NULL);

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char msg[BUFFER_SIZE];
            
            printf("\e[1m\e[92m%s: \e[21m\e[39m", clientName);
            fgets(buffer, sizeof(buffer), stdin);
            *strchr(buffer, '\n') = 0;
            
            strcpy(msg, clientName); // Add clientName
            strcat(msg, ": "); // Add space
            strcat(msg, buffer); // Add buffer
            
            write(server_socket, msg, sizeof(msg));
            read(server_socket, buffer, sizeof(buffer));
            //printf("\e[1;1H\e[2J");
            //printf("received: [%s]\n", buffer);
        }//end stdin select


        //currently the server is not set up to
        //send messages to all the clients, but
        //this would allow for broadcast messages
        if (FD_ISSET(server_socket, &read_fds)) {
            read(server_socket, buffer, sizeof(buffer));            
            
            printf("\r%s\n\e[1m\e[92m%s: \e[21m\e[39m", buffer, clientName);
            
            //printf("[SERVER BROADCAST] [%s]\n", buffer);
            
            //printf("enter data: ");
            //the above printf does not have \n
            //flush the buffer to immediately print
            fflush(stdout);
        }//end socket select
	}
	else{
		rad = logging();
	}
    }//end loop
}