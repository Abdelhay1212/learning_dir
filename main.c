#include <stdio.h>
/*#include <shell.h>*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * tokenizeTheCommand - tokenization of the command
 * Return: pointer to pointer
 **/
char **tokenizeTheCommand(char *command)
{
	int i = 0;
	char *token;
	char **tokens;

	tokens = malloc(20 * sizeof(char *));

	token = strtok(command, " ");
	while (token != NULL)
	{
		tokens[i] = strdup(token);
		token = strtok(NULL, " ");
		i++;
	}
	tokens[i] = NULL;

	return (tokens);
}

/**
 * executeCommand - executes the command
 * Return: void
 **/
void executeCommand(char **tokens)
{
	char* pathEnv = getenv("PATH");
	char* pathToken;
	char* path;
	
	pathToken = strtok(pathEnv, ":");
	while (pathToken != NULL)
	{
		path = malloc(strlen(tokens[0]) + strlen(pathToken) + 2); 
		sprintf(path, "%s/%s", pathToken, tokens[0]);
		execve(path, tokens, NULL);
		
		pathToken = strtok(NULL, ":");
		free(path);
	}
}

/**
 * main - takes input from STDIN
 * Return: string
 **/
void getInput(char *buffer)
{
	int bytes;
	
	bytes = read(STDIN_FILENO, buffer, sizeof(buffer));
	if (bytes == -1)
		perror("hsh");
	else
		buffer[bytes] = '\0';
}

/**
 * main - takes a command and execute it
 * Return: integer
 **/
int main(void)
{
	int i;
	pid_t pid;
	char *command;
	char **tokens;
	/*size_t bufsize = 0;*/

	while (1)
	{

		write(1, "$ ", 2);
		getInput(command);

 		/*command[strcspn(command, "\n")] = '\0';*/

		if (strcmp(command, "exit") == 0)
			exit(0);

		tokens = tokenizeTheCommand(command);

		if (tokens[0] != NULL)
		{
			pid = fork();
			if (pid == -1)
			{
				write(1, "hsh: Failed to fork the process\n", 31);
			}
			else if (pid == 0)
			{
				executeCommand(tokens);
			}
			else
				wait(NULL);
		}

		for (i = 0; tokens[i] != NULL; i++)
			free(tokens[i]);
		free(tokens);
	}
	free(command);

	return (0);
}
