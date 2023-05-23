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
 * _getline - takes input from STDIN
 * Return: void
 **/
void _getline(char *buffer, size_t bufferSize)
{
	int bytes;

	bytes = read(STDIN_FILENO, buffer, bufferSize);
	if (bytes == -1)
		perror("hsh");
	else
		buffer[strcspn(buffer, "\n")] = '\0';
}

/**
 * printEnv - prints the envirenmnt variables 
 * Return: void
 **/
void printEnv(char **env)
{
	unsigned int i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("%s\n", env[i]);
		i++;
	}
}

/**
 * _exit - exit with error number
 * Return: integer
 **/
int _exit(char **tokens)
{
	int err;
	
	if (strlen(tokens) == 2)
	{
		err = atoi(tokens[1]);
		exit(err);
	}
	else
		exit(0);
}

/**
 * main - takes a command and execute it
 * Return: integer
 **/
int main(NULL, NULL, char **env)
{
	int i;
	pid_t pid;
	char command[1024];
	char **tokens;
	size_t bufferSize = 1024;

	while (1)
	{

		write(1, "$ ", 2);
		_getline(command, bufferSize);

		tokens = tokenizeTheCommand(command);
		
		if (strcmp(tokens[0], "exit") == 0)
			_exit(tokens);

		if (tokens[0] != NULL)
		{
			pid = fork();
			if (pid == -1)
			{
				write(1, "hsh: Failed to fork the process\n", 31);
			}
			else if (pid == 0)
			{
				if (strcmp(tokens[0], "env") == 0)
					printEnv(env);
				else
					executeCommand(tokens);
			}
			else
				wait(NULL);
		}

		for (i = 0; tokens[i] != NULL; i++)
			free(tokens[i]);
		free(tokens);
	}

	return (0);
}
