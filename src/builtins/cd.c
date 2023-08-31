#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "args.h"
#include "shell.h"
#include "debug.h"

static
int change_directory(shell_t *shell, char *path)
{
    DEBUG("Changing dir: `%s`", path);
    shell->path = path;
    chdir(path);
    setenv("PWD", path, true);
    return EXIT_SUCCESS;
}

int builtin_cd(shell_t *shell)
{
    args_t *args = shell->input->args;
    char *path = NULL;

    if (args->argc >= 3) {
        fprintf(stderr, "cd: too many arguments\n");
        return EXIT_FAILURE;
    }
    if (args->argc <= 1) {
        path = getenv("HOME");
        if (path == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return EXIT_FAILURE;
        }
        return change_directory(shell, path);
    }
    return change_directory(shell, args->argv[1]);
}
