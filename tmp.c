#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include <dirent.h>
#include "libshell/libshell.h"

#define BUFFER_SIZE 100

char *get_env_var(char *var, char **env) {
        char *rtn;
        char *var_str;
        int i, j, len, loc;

        var = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        rtn = malloc(sizeof(char) * (BUFFER_SIZE + 1));
        var_str = malloc(sizeof(char) * (BUFFER_SIZE + 1));

        var = "PATH";
        len = str_len(var);
        printf("%d\n", len);

        /* Find the location of the variable in the env array. */
        for(loc = 0; env[loc] != '\0'; loc++) {
                for(j = 0; j < len; ++j) {
                        var_str[j] = env[loc][j];
                }
                if(strcmp(var, var_str) == 0)
                        break;
                if(env[loc + 1] == '\0')
                        var_str = "Var location not found.";
        }

        printf("%d\n", loc);
        printf("%s\n", var_str);

        for(i = 0; env[8][i + 5] != '\0'; i++) {
                rtn[i] = env[8][i + 5];
        }
        return rtn;
}
