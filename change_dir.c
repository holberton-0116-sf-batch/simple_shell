#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include <dirent.h>
#include "libshell/libshell.h"

/*
 * ch_dir() - Takes a path as parameter and uses the Linux builtin function
 * chdir to change the process to that directory. Currently, it is not able
 * to handle using the `get_env_var()` function because it is called within
 * a child process, so to change directory into the user's home requires using
 * the function in the parent process and passing the returned value of
 * `get_env_var()` to this function.
 * @path: The path to change directories to.
 *
 * Return: On success, zero is returned.  On error, -1 is returned, and
 * errno is set appropriately.
 */
int ch_dir(char *path) {
        return(chdir(path));
}
