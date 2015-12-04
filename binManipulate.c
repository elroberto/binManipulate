#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int checkCommand( char command[] )
{
	char *validCommands[] = {
		"d","x","o","b0","b1","b2","b3","+","&","|","^",">","<","q"
	};

	int i;
	for ( i = 0; i < 14; i++) {
		if( strcmp(command,validCommands[i]) == 0 ) {
			//handle quit command
			if( strcmp(command, "q") == 0 ) {
				exit(0);	
			} else {
				return 1;
			}
		}
	}
	return 0;
}

int checkOperand1 ( char operand1[], int contentLength )
{
	//Check exists and if null 
	if (operand1 && operand1[0] == '\0'){
		return 0;
	}

	//Convert to int and compare to content length
	int val;
	val = atoi(operand1);
	
	if (val > contentLength){
		return 0;
	}

	return 1;

}

int requiresOperand2 ( char command[] ) 
{
	//commands that require a third argument (second operand)
	char *validCommands[] = {
		"+","&","|","^",">","<","q"
	};

	int i;
	for ( i = 0; i < 7; i++) {
		if( strcmp(command,validCommands[i]) == 0 ) {
			return 1;
		}
	}
	return 0;
}

void printElement( char command[], char operand1[], char operand2[], unsigned int buffer[] )
{
	int intOperand1 = atoi(operand1);
	int intOperand2 = atoi(operand2);

	if (strcmp(command,"d") == 0) {
		printf("%d \n",buffer[intOperand1]);
	} else if (strcmp(command,"o") == 0) {
		printf("%o \n",buffer[intOperand1]);
	} else if (strcmp(command,"x") == 0) {
		printf("%x \n",buffer[intOperand1]);
	} else if (strcmp(command,"b0") == 0) {
		printf("%x \n",(buffer[intOperand1] & 0x000000ff));
	} else if (strcmp(command,"b1") == 0) {
		printf("%x \n",(buffer[intOperand1] & 0x0000ff00) >> 8);
	} else if (strcmp(command,"b2") == 0) {
		printf("%x \n",(buffer[intOperand1] & 0x00ff0000) >> 16);
	} else if (strcmp(command,"b3") == 0) {
		printf("%x \n",(buffer[intOperand1] & 0xff000000) >> 24);
	} else if (strcmp(command,"+") == 0) {
		printf("%x \n",buffer[intOperand1] + buffer[intOperand2]);
	} else if (strcmp(command,"&") == 0) {
		printf("%x \n",buffer[intOperand1] & buffer[intOperand2]);
	} else if (strcmp(command,"|") == 0) {
		printf("%x \n",buffer[intOperand1] | buffer[intOperand2]);
	} else if (strcmp(command,"^") == 0) {
		printf("%x \n",buffer[intOperand1] ^ buffer[intOperand2]);
	} else if (strcmp(command,">") == 0) {
		printf("%x \n",buffer[intOperand1] >> intOperand2);
	} else if (strcmp(command,"<") == 0) {
		printf("%x \n",buffer[intOperand1] << intOperand2);
	}	
}

int main(int argc, char *argv[])
{

	//Read in a file from the command line
	FILE *inputFile;
	char *mode = "r";
	signed int contentLength;

	inputFile = fopen(argv[1], mode);

	//Exit with error status if it cannot be read
	if (inputFile == NULL) 
	{
		printf("Cannot open the input file %s \n", argv[1]);
		exit(1);
	}

	//Read the first integer
	int bytesRead = fread(&contentLength, sizeof contentLength, 1, inputFile);
	if ( bytesRead ) 
	{
		if (contentLength < 0) {

			printf("Content length is a negative integer. %s \n", argv[1]);
			exit(1);

		}

		if (contentLength > 1024){
			
			printf("Unable to allocate %d integers. \n", contentLength);
			exit(1);
		}

	} else {
		// Reading the content length integer failed
		printf("Failed to read content length in file %s \n", argv[1]);
		exit(1);
	}

	// Allocate memory for the integers we're about to read.
	unsigned int buffer[ contentLength ];
	int i;	

	for ( i = 0; i < contentLength; i++ )
	{
		// use fread to read the next integer
		int bytesRead = fread(&buffer[i], sizeof contentLength, 1, inputFile);

		// make sure we're not hitting the end of the file
		// before we have read all the bytes
		if (bytesRead != 1 && i < contentLength) {
			printf("The file does not contain a sufficient number of data chunks. %s \n", argv[1]);
			exit(1);
		}
	}

	char *userInput = NULL;
	size_t len = 0;

	//forever loop
	while ( getline(&userInput, &len, stdin) != -1 ) 
	{
		//parse the input
		//command and operand
		char *command = strtok(userInput, " ");

		//Remove newline character added by stdin
		int lenCommand;
		lenCommand = strlen(command);
		if (command[lenCommand-1] == '\n') {
			command[lenCommand-1] = 0;
		}

		//check first token for validity; also check for 'q'
		if ( checkCommand(command) == 0) {
			printf("Not a valid command: %s \n",command);
			exit(1);
		}

		char *operand1 = strtok(NULL, " ");

		//make sure token is there
		if ( operand1 == NULL ) {

			printf("First operand is missing. \n");
			exit(1);
		}

		//check second token (first operand) for validity
		if ( checkOperand1(operand1, contentLength) == 0 ) {
			printf("Not a valid operand: %s \n",operand1);
			exit(1);
		}

		char *operand2;

		//if command is +, &, |, ^, >, <, multiple operand. tokenize one more
		if ( requiresOperand2(command) ) {
			operand2 = strtok(NULL, " ");
			//check to see if null
			if (operand2 && operand2[0] == '\0'){
				printf("Second operand is empty %s \n",operand2);
				exit(1);
			}
		} else {
			operand2 = "undefined";
		}

		printElement(command, operand1, operand2, buffer);

		//free memory userinput tokens
	}

	return 0;
}

