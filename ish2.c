#include <errno.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#define MAXLEN 256 
#define DELIMITERS " \t\n" 

int main(int argc, char** argv) 
{ 
	char command[MAXLEN]; 
	char **arguments; 	

	
	while(1)
	{
		printf("5550558721 Supanat Potiwarakorn > ");
		if (fgets(command, MAXLEN, stdin) == '\n');
		if (tokenize(command, DELIMITERS, &arguments) < 0) { 
			fprintf(stderr, "Failed to parse command"); 
			exit(1); 
		}
		if (strcasecmp(arguments[0], "exit") == 0) exit(EXIT_SUCCESS); // exiting command
		cmd_execute(arguments); // create a new child process for execution
	}  

} 

int cmd_execute(char **argv)
{
	pid_t pid = fork();
	int status;

	if (pid < 0)
	{
		printf("FORKING FAILED!!");
		exit(EXIT_FAILURE); // exit(1)
	}
	else if (pid == 0)
	{
		int res = execvp(*argv, argv);
		if (res < 0)
		{
			perror("ERR! "); 
			exit(EXIT_FAILURE);
		}
	}
	else 
	{
		/*
		*	The wait()'s return value is process id of the child process which recently exited or zero if no child was used.
		*	pid on child process ( the return value of fork() ) is zero. 
		*	So we let the parent process wait until the child process is exited ( and wait() return the child's process id )
		*	and back to parent process when the child process terminated.
		*/

		while(wait(&status) != pid); 
	}
}

/* 
Split input string into substrings (called tokens)which are sequences of 
contiguous characters separated by any of the characters in the string of 
accepted delimiters and fill the tokens into an array. 
The last element of the array contains a NULL pointer. 
Return number of tokens or -1 if not success. 
Example:
 char delim[] = " \t\n"; 
 char **tokens; 
 char string[256]; 
 int numtokens; 
 int i; 
 fgets(string, 256, stdin); 
 numtokens = tokenize(string, delim, &tokens); 
 for (i = 0; i < numtokens; i++) { 
 printf("%d:%s\n", i, tokens[i]); 
 }
*/ 

 int tokenize(char *string, char *delimiters, char ***arrayOfTokens) { 
 	char *token; 
 	int numtokens; 
 	int i; 
 /* skip the beginning delimiters */ 
 	string += strspn(string, delimiters); 
 	if ((token = malloc(strlen(string) + 1)) == NULL)
 		return -1; 
 /* count tokens */ 
 	strcpy(token, string); 
 	numtokens = 0; 
 	if (strtok(token, delimiters) != NULL) 
 		for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++); 
 /* create array of pointers to tokens */ 
 	if ((*arrayOfTokens = malloc((numtokens+1)*sizeof(char *))) == NULL) { 
 		free(token); 
 		return -1; 
 	} 
 /* fill pointers to tokens into the array */ 
 	if (numtokens == 0) 
 		free(token);  else { 
 			strcpy(token, string); 
 			(*arrayOfTokens)[0] = strtok(token, delimiters); 
 			for (i = 1; i < numtokens; i++) 
 				(*arrayOfTokens)[i] = strtok(NULL, delimiters); 
 			(*arrayOfTokens)[numtokens] = NULL; 
 		} 
 		return numtokens; 
 	}