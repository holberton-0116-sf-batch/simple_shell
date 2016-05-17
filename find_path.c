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
 * For example, if the cmd variable were the string "ls" and this program
 * exists in the /bin directory, this function will return the entire path
 * to that program: the string "/bin/ls".
 */
char *find_path(char *cmd, char **env) {
        DIR *dir;
        struct dirent *dir_search;
        char *path;
        char **arr;
        int i, exec_size;

        /*
         * Retrieve the value of the env variable PATH, then split it into
         * an array based on the colon separator. Free memory allocated in
         * get_env_value function.
         */
        path = get_env_value("PATH", env);
        arr = string_split(path, ':');
        exec_size = grid_size(arr);
        free(path);

        /* Increment through the path array, searching within dirs. */
        for(i = 0; arr[i] != '\0'; ++i) {
                if ((dir = opendir(arr[i])) == NULL) {
                        perror("opendir");
                        return NULL;
                }
                while((dir_search = readdir(dir)) != NULL) {
                        if (str_cmp(dir_search->d_name, cmd) == 0) {
                                arr[i] = concat_strings(arr[i], "/");
                                closedir(dir);
                                return concat_strings(arr[i], cmd);
                        }
                }
                closedir(dir);
        }
        free_grid(arr, exec_size);
        return NULL;
}

/*
 * get_env_value() - Takes an environment variable and searches for its
 * existence. If found it will return the value of that variable.
 * @var: The environmental variable to be found (e.g., "PATH");
 * @env: The parent process environment.
 *
 * Return: The value of the variable, if the variable exists. NULL if it does
 * not exist.
 */
char *get_env_value(char *var, char **env) {
        char *var_name;
        char **var_strings;
        char *val;
        int i, j, loc;

        /* val = malloc(sizeof(char) * (BUFFER_SIZE + 1)); */

        /* Find the location of the variable in the env arr. */
        for(loc = 0; env[loc] != '\0'; loc++) {
                /* var_name[0] will be the var name we are getting. */
                var_strings = string_split(env[loc], '=');
                var_name = var_strings[0];
                free_grid(var_name);

                if (grid_size(var_strings) > 2) {
                        /* The values here have an equal sign */
                        printf("%s\n", "Values here have an equal sign.");
                }
                /*
                 * Obtain the value of the variable, and copy
                 * it onto var_str.
                 */
                for(j = 0; var_strings[j] != '\0'; ++j) {
                        var_name[j] = env[loc][j];
                        var_name[j + 1] = '\0';
                }

                if(str_cmp(var, var_str) == 0)
                        break;
                if(env[loc + 1] == '\0')
                        return NULL;
        }

        free(var_str);

        /* Store the value of the env variable to be returned. */
        for(i = 0; env[loc][i + (len + 1)] != '\0'; i++) {
                val[i] = env[loc][i + (len + 1)];
        }
        val[i] = '\0';

        return val;
}
