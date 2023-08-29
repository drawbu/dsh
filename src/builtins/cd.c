#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "args.h"
#include "shell.h"
#include "debug.h"

static
void change_directory(shell_t *shell, char *path)
{
    DEBUG("Changing dir: `%s`", path);
    shell->path = path;
    chdir(path);
    setenv("PWD", path, true);
}

int builtin_cd(shell_t *shell)
{
    args_t *args = shell->input->args;

    if (args->argc >= 3) {
        fprintf(stderr, "cd: too many arguments\n");
        return EXIT_FAILURE;
    }
    if (args->argc <= 1) {
        change_directory(shell, getenv("HOME"));
        return EXIT_SUCCESS;
    }
    change_directory(shell, args->argv[1]);
    return EXIT_SUCCESS;
}
