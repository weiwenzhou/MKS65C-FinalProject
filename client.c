#include "networking.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>





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

		int accFile = open(uName, O_APPEND | O_WRONLY | O_CREAT,0666);
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
			FILE* fp = fopen(uName, "r");
			//read(accFile,check,255);
			read(accFile,check,getc(fp));
			if(strcmp(check,uPass) == 0){
				printf("SAVED PASS %s\n",check);
				printf("You have successfully logged in! Please remember to be polite in the chatrooms!\n");
				return 1;
			}
			else{
				printf("SAVED PASS %s\n",check);
				printf("You have entered a wrong password\n");
			}
		}

		}

	return 0;
	}

int main(int argc, char **argv) {

    int server_socket;
    char buffer[BUFFER_SIZE];
    char port[BUFFER_SIZE];
    printf("Enter port# between 49152-65535: ");
    fgets(port, sizeof(port), stdin);
    *strchr(port, '\n') = 0;
    

  if (argc == 2)
    server_socket = client_setup( argv[1],port);
  else
    server_socket = client_setup( TEST_IP,port );

	int rad = logging();

  while (1) {
    if(rad == 1){
    printf("enter data: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, '\n') = 0;
    write(server_socket, buffer, sizeof(buffer));
    read(server_socket, buffer, sizeof(buffer));
    printf("received: [%s]\n", buffer);
	}
  
   else {
	rad = logging();
  }
}
}















