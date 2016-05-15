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
int main(int argc, __attribute__((unused)) char **argv, char **env) {
	pid_t pid;
	int status;
	/*char *path_to_exec;*/
	char *raw_str;
	char **exec_argv;
	int exec_size; 		/* how many strings in the array */

	if (argc != 1) { 	/* usage */
		return 1;
	}

	while (1) {
		print_prompt();
		raw_str = read_line(0);
		exec_argv = string_split(raw_str, ' ');
		free(raw_str); 	/* frees the memory allocated in read_line() */
		exec_size = grid_size(exec_argv); /* how many strings in the array */
		/* printf("Size of exec_argv: %d\n", exec_size); */

		if (str_cmp(exec_argv[0], "exit") == 0)
			break;
		if (str_cmp(exec_argv[0], "cd") == 0) {
			ch_dir(exec_argv[1]);
		} else if ((pid = fork()) == -1) {
			perror("fork");
			return 1;
		} else if (pid == 0) {
			execve(raw_str = find_path(exec_argv[0], env), exec_argv, env);
			perror("execve");
			free(raw_str);
			free_grid(exec_argv, exec_size);
			return -1; /* child process returns this */
		} else {
			wait(&status);
		}
		free_grid(exec_argv, exec_size);
	}
	free_grid(exec_argv, exec_size);
	return 0;
}
