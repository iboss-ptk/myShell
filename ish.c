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

	// infinite loop for multi-command feature
	while(1)
	{
		// prompt. waiting for input
		printf("5550558721 Supanat Potiwarakorn > "); 

		// waiting for input ( which should be a UNIX command )
		if (fgets(command, MAXLEN, stdin) == '\n'); 

		// parse the string input to map to the existing programs and it's options ( something like gcc -o <- the "-o" is called option )
		// tokenize function is declared below
		if (tokenize(command, DELIMITERS, &arguments) < 0) { 
			fprintf(stderr, "Failed to parse command"); 
			exit(1); 
		}

		// check if the input is "exit" ( I use strcasecmp instead of stringcmp to ignore case, can be "Exit","EXIT","ExIt" or else )
		if (strcasecmp(arguments[0], "exit") == 0) exit(EXIT_SUCCESS); 

		// reaching here means user does not attempt to exit. 
		// so the program will try to execute the command which is already parsed by tokenize function
		cmd_execute(arguments); // code of this function, see below.
	}  

} 

/*
 * Since the execvp command which is used to execute the command will tranform the current process which this program is using into
 * it's own process, it will terminate itself and will never go back to the loop.
 * For example, if user type "ls" the program will search for ls program in path variable ( in my case, it is stored in "/bin/ls" which is declared in $PATH.
 * So we can call this program globally ). The ls program print out the files in current directory, then terminate, no more prompt, no more loop, which we need.
 *
 * The purpose of this function is to identically duplicate the process. So now we have the parent process that generate child process.
 * And the two process run parallely.
 */
int cmd_execute(char **argv)
{
	// fork generate child process with current execution situation. In parent process, it will return the child process's id.
	// in child process, this will return 0. So now we can identify that which process is running.
	pid_t pid = fork(); 
	int status;

	// if it's failed to fork, it'll return negative integer
	if (pid < 0)
	{
		printf("FORKING FAILED!!");
		exit(EXIT_FAILURE); // exit(1)
	}

	// This case is what we want to do on child process.
	else if (pid == 0)
	{
		// when error occur, execvp return -1
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