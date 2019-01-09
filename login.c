#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

// takes a char *
// strips all trailing whitespaces
char * rStrip(char * s){

	char * doppler = strdup(s);
	char * str = doppler + strlen(s) - 1;

	while(str >= doppler && isspace(*str)){
	str --;
	}

	* (str + 1) = 0;
	return doppler;
	}



// takes a char *
// strips all leading whitespaces
char * lStrip(char * str){
	char * pointer = str;
    // leading whitespaces
   	 while (*pointer && isspace(*pointer)) {
        pointer += 1;
	}
	return pointer;
	}


// takes a char *
// strips all leading and trailing whitespaces

char * strip(char * str){
	return rStrip(lStrip(str));
}

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
			read(accFile,check,100);
			if(strcmp(check,uPass) == 0){
				printf("You have successfully logged in! Please remember to be polite in the chatrooms!\n");
			}
			else{
				//printf("SAVED PASS %s\n",check);
				printf("You have entered a wrong password\n");
			}
		}

		}

	return 0;
	}
