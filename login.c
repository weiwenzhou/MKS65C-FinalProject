#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
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
		uPass[strlen(uPass) -1] = 0;
		strcat(uName,".txt");
		//printf("Testing uName %s\n",uName);
		
		int accFile = open(uName, O_APPEND | O_WRONLY | O_CREAT,0666);
		//printf("Testing uPass %s\n",uPass);

		write(accFile,uPass,strlen(uPass));
		if(accFile != -1){
			printf("You have succesfully created your account!\n");
		}
		else{
			printf("OOPS! Something went wrong.");
		}
		close(accFile);

	}
	



	return 0;
	}
		 
	