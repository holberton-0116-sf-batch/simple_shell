#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

/*
 * Takes arguments from command line and locates them in PATH.
 * @cmd: The command entered by the user.
 * @str: The string entered by the user.
 * @env: The environment.
 *
 * Return: 0 on success, 1 on failure.
 */

int main(int argc, char **argv, char **env) {

	char *path;
        pid_t pid;
        char *exec_argv[] = {NULL, NULL, NULL};
        int status;

        if (argc != 3) {
                return (1);
        }

        path = get_path(argv[1]);
	exec_argv[0] = path;
        exec_argv[1] = argv[2];

        /* Test */
        printf("the string is: %s\n", argv[2]);
        printf("the command is: %s\n", argv[1]);

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

/*
 * concat_strings() - Take two strings and concatenate them.
 * @s1: The first string.
 * @s2: The second string.
 * @p: The allocated mallocated pointer.
 *
 * Return: The pointer with the concatentated string.
 */
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
