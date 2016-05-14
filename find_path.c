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
 * find_path() - Takes a command and searches the environmental variable
 * PATH for the command name.
 * @cmd: The command entered.
 * @env: The environment with the PATH variable.
 *
 * Return: The program if it exists, and NULL if the program is not found.
 */
char *find_path(char *cmd, char **env) {
        DIR *dir;
        struct dirent *dir_search;
        char *path;
        char **arr;
        int i;

        dir_search = malloc(sizeof(struct dirent));
        path = malloc(sizeof(char) * (BUFFER_SIZE + 1));

        /* Take the path variable and isolate it into a variable.
         * PATH is the 8th element in the env array. The first absolute
         * path begins at five chars in, hence [i + 5].
         */
        for(i = 0; env[8][i + 5] != '\0'; i++) {
                path[i] = env[8][i + 5];
        }
        arr = string_split(path, ':');

        /* Increment through the path array, searching within dirs. */
        for(i = 0; arr[i] != '\0'; ++i) {
                dir = opendir(arr[i]);
                while((dir_search = readdir(dir)) != NULL) {
                        if (strcmp(dir_search->d_name, cmd) == 0) {
                                arr[i] = concat_strings(arr[i], "/");
                                return concat_strings(arr[i], cmd);
                        }
                }
        }
        return NULL;
}
