#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include "libshell/libshell.h"

/*
 * Takes arguments from command line and locates them in PATH.
 * @cmd: The command entered by the user.
 * @str: The string entered by the user.
 * @env: The environment.
 *
 * Return: 0 on success, 1 on failure.
 */

int main(int argc, char **argv, char **env) {
 
        pid_t pid;
        int status;
	char c;
	char **exec_argv;

        if (argc != 1) {
		return 1;
        }

	print_char('$');
	print_char(' ');

	exec_argv = string_split(read(0, &c, 1), ' '); /* read the commands with parameters */

	exec_argv[0] = concat_strings("/bin/", exec_argv[0]); /* add path for convenience */

        /* Test */
        printf("the string is: %s\n", argv[2]);
        printf("the command is: %s\n", argv[1]);
	printf("concat test: %s\n", concat_strings("hello", "world"));

        if ((pid = fork()) == -1) {
                perror("fork");
                return 1;
        } else if (pid == 0) {
                execve(exec_argv[0], exec_argv, env);
        } else {
                wait(&status);
                printf("Child process terminated.\n");
        }

        return 0;
}

/*
 * concat_strings() - Take two strings and concatenate them.
 * @s1: The first string.
 * @s2: The second string.
 *
 * Return: The pointer with the concatentated string.
 */
char *concat_strings(char *s1, char *s2)
{
        int i, j;
	char *p;

	p = malloc( sizeof(char) * ( str_len(s1) + str_len(s2) ) + 1 ); /* allocate memory */

	if (p == NULL) {		/* memory allocation check */
		perror("malloc");
		return "Not enough memory allocated.";
	}

	j = 0;

        for (i = 0; s1[i] != '\0'; ++i) { /* here we copy the first string onto p */
                p[j] = s1[i];
                ++j;
        }

        for (i = 0; s2[i] != '\0'; ++i) { /* here we append the 2nd string onto p */
                p[j] = s2[i];
                ++j;
        }

	p[j] = '\0';		/* append null character at the end */

        return p;
}

int str_len(char *str)
{
  int i;			/* i used as a counter */

  i = 0;			/* initialize at 0 */

  while (*str != '\0') 		/* while string isn't over */
    {
      i++;			/* increase counter */
      str++;			/* pointer arithmetic for next char */
    }

  return i;
}
