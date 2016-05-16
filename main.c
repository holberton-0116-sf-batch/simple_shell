#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include <dirent.h>
#include "libshell/libshell.h"

#define BUFFER_SIZE 100

/*
 * Takes arguments from command line and locates them in PATH.
 * @cmd: The command entered by the user.
 * @str: The string entered by the user.
 * @env: The environment.
 *
 * Return: 0 on success, 1 on failure.
 */
int main(int argc, __attribute__((unused)) char **argv, char **env)
{
        /* 
         * @exec_argv stores the array of commands to be executed
         * @exec_size stores the number of strings in the array exec_argv
         */
	pid_t pid;
	int status, exec_size;
	/*char *path_to_exec;*/
	char *concat_str;
	char **exec_argv;

 	/* check usage */
        if (usage(argc))
                return 1;

	while (1) {
                /* prompt the user and obtain commands */
                exec_argv = prompt();
                /* obtain how many strings in the array */
		exec_size = grid_size(exec_argv);
                /* Print for debugging purposes */
		/* printf("Size of exec_argv: %d\n", exec_size); */

		if (str_cmp(exec_argv[0], "exit") == 0)
			break;

		if ((pid = fork()) == -1) {
			perror("fork");
			return 1;
		} else if (pid == 0) {
			execve(concat_str = concat_strings("/bin/", exec_argv[0]), exec_argv, env);
			perror("execve");
			free(concat_str);
			free_grid(exec_argv, exec_size);
                        /* If execve fails, this child process returns -1 */
			return -1;
		} else {
			wait(&status);
		}

		free_grid(exec_argv, exec_size);
	}

	free_grid(exec_argv, exec_size);
	return 0;
}

/* 
 * This function prompts the user for input and returns a parsed array of
 * strings to be executed.
 */

char **prompt(void)
{
        char *raw_str;
        char **exec_argv;

        print_prompt();
        raw_str = read_line(0);
        exec_argv = string_split(raw_str, ' ');
        /* frees the memory allocated in read_line() */
        free(raw_str);
        return exec_argv;
}

char usage(char argc)
{
	if (argc != 1) {
                printf("This shell takes no arguments.\n");
		return 1;
	}

        return 0;
}
