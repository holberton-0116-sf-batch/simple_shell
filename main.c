#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include <dirent.h>
#include "libshell/libshell.h"

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
        char c;
        char *path_to_exec;
        char *raw_str;
        char **arr;
        int i = 0;
        char *exec_argv[] = {NULL, NULL, NULL};

        if (argc != 1) {
		return 1;
        }

        raw_str = malloc(sizeof(char) * (100 + 1));

        print_prompt();

        while(read(0, &c, 1)) {
                if(c == '\n') {
                        raw_str[i] = '\0';
                        i = 0;
                        /* Delete */
                        arr = string_split(raw_str, ' ');

                        path_to_exec = find_path(arr[0], env);
                        path_to_exec = concat_strings(path_to_exec, "/");

                        printf("The path to the exec is: %s\n", path_to_exec);

                        exec_argv[0] = concat_strings(path_to_exec, arr[0]);

                        printf("exec_argv[0]: %s\n", exec_argv[0]);
                        exec_argv[1] = arr[1];

                        if ((pid = fork()) == -1) {
                                perror("fork");
                                return 1;
                        } else if (pid == 0) {
                                execve(exec_argv[0], exec_argv, env);
                        } else {
                                wait(&status);
                        }
                        print_prompt();
                        read(0, &c, 1);
                }
                raw_str[i] = c;
                ++i;
        }
        return 0;
}

char *find_path(char *command, char **env) {
        DIR *dir;
        struct dirent *dir_search;
        char *path;
        char **path_arr;
        int i;

        dir_search = malloc(sizeof(struct dirent));
        path = malloc(sizeof(char) * (100 + 1));

        /* Take the path variable and isolate it into a variable.
         * PATH is the 8th element in the env array. The first absolute
         * path begins at five chars in, hence [i + 5].
         */
        for(i = 0; env[8][i + 5] != '\0'; i++) {
                path[i] = env[8][i + 5];
        }

        /* Split the path into an array for searching. */
        path_arr = string_split(path, ':');

        /* Increment through the path array to find the location of the program. */
        for(i = 0; path_arr != NULL; ++i) {

                /* Open each directory. */
                dir = opendir(path_arr[i]);

                /* Seach this directory for the program name. */
                while((dir_search = readdir(dir)) != NULL) {
                        if (strcmp(dir_search->d_name, command) == 0) {
                                printf("Found the program %s, in the directory %s\n", dir_search->d_name, path_arr[i]);
                                break;
                        }
                }
        }
        return path_arr[i];
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

int *print_prompt(){
        int i;
        char *prompt = "GreenShell$ ";

        i = 0;
        while (prompt[i] != '\0') {
                print_char(prompt[i]);
                ++i;
        }
        return 0;
}

int strcmp(char *s1, char *s2)
{
  int i = 0;
  int j = 0;
  for ( ; s1[i] != '\0'; i++)
  {
  	if (s1[j] != s2[j]) /* if chars are different, break */
	  {
	    break;
	  }
	  j++;
  }
  return(s1[j] - s2[j]); /* return difference in chars */
}
