#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

int main(int argc, char **argv, char **env) {
        int err;

        if (argc != 3) {
                return (1);
        }
        err = resp(argv[1], argv[2], env);
        if (err == 1) {
                return (1);
        }
        return (0);
}

/*
 * resp() - Takes arguments from command line and locates them in PATH.
 * @cmd: The command entered by the user.
 * @str: The string entered by the user.
 * @env: The environment.
 *
 * Return: 0 on success, 1 on failure.
 */
int resp(char *cmd, char *str, char **env)
{
        pid_t pid;
        char *path;
        char *exec_argv[] = {NULL, NULL, NULL};
        int status;

        path = get_path(cmd);
        exec_argv[0] = path;
        exec_argv[1] = str;

        /* Test */
        printf("the string is: %s\n", str);
        printf("the command is: %s\n", cmd);

        if ((pid = fork()) == -1) {
                perror("fork");
                return(1);
        } else if (pid == 0) {
                execve(exec_argv[0], exec_argv, env);
        } else {
                wait(&status);
                printf("Child process terminated.\n");
        }
        return(0);
}

/*
 * get_path() - Generates a string with the path to the command the user
 * entered.
 * @cmd: The command the user entered.
 *
 * Return: The path to the command, or error message if mallocation fails.
 */
char *get_path(char *cmd)
{
        char *path;
        int len = 4;

        for(; cmd[len] != '\0'; ++len);

        path = malloc(sizeof(char) * len);

        if (path != NULL) {
                path = concat_strings("/bin/", cmd, path);
                return(path);
        } else {
                return("Not enough memory allocated.");
        }
}


char *concat_strings(char *s1, char *s2, char *p)
{
  int i;
  int j = 0;

  for (i = 0; s1[i] != '\0'; ++i) {
    p[j] = s1[i];
    ++j;
  }

  for (i = 0; s2[i] != '\0'; ++i) {
    p[j] = s2[i];
    ++j;
  }

  return (p);
}