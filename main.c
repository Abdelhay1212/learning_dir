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
   int err;

   err = execvp(tokens[0], tokens);
   if (err == -1)
	   write(1, "hsh: command not found\n", 23);
}

/**
 * main - takes a command and execute it
 * Return: integer
 **/
int main(void)
{
   pid_t pid;
   char *command;
   char **tokens;
   size_t bufsize = 0;

   while (1)
   {
      write(1, "$ ", 2);
      getline(&command, &bufsize, stdin);

      command[strcspn(command, "\n")] = '\0';
      tokens = tokenizeTheCommand(command);

      pid = fork();
      if (pid == 0)
         executeCommand(tokens);
      else
         wait(NULL);

      for (int i = 0; tokens[i] != NULL; i++)
         free(tokens[i]);
      free(tokens);
   }

   return (0);
}
