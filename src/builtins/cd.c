#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include "args.h"
#include "shell.h"
#include "debug.h"

static
int change_directory(shell_t *shell, char *path)
{
    DEBUG("Changing dir: `%s`", path);
    free(shell->path);
    shell->path = path;
    chdir(path);
    setenv("PWD", path, true);
    return EXIT_SUCCESS;
}

static
char *new_path(char *pwd, char *target)
{
    char *path = calloc(PATH_MAX, sizeof(char));
    char *resolved_path = NULL;
    int len_pwd = strlen(pwd);

    if (path == NULL) {
        fprintf(stderr, "cd: malloc failed\n");
        return NULL;
    }
    if (strlen(pwd) + strlen(target) > PATH_MAX) {
        free(path);
        fprintf(stderr, "cd: path too long\n");
        return NULL;
    }
    strcat(path, pwd);
    if (target[0] != '/' && pwd[len_pwd - 1] != '/')
        strcat(path, "/");
    strcat(path, target);
    DEBUG("New path: `%s`", path);
    resolved_path = realpath(path, NULL);
    if (resolved_path == NULL)
        fprintf(stderr, "cd: %s: No such file or directory\n", path);
    free(path);
    return resolved_path;
}

static
int empty_cd(shell_t *shell)
{
    char *home = getenv("HOME");
    char *path = calloc(PATH_MAX, sizeof(char));

    if (home == NULL) {
        fprintf(stderr, "cd: HOME not set\n");
        return EXIT_FAILURE;
    }
    if (path == NULL) {
        fprintf(stderr, "cd: malloc failed\n");
        return EXIT_FAILURE;
    }
    strcpy(path, home);
    return change_directory(shell, path);
}

int builtin_cd(shell_t *shell)
{
    args_t *args = shell->input->args;
    char *path = NULL;

    if (args->argc >= 3) {
        fprintf(stderr, "cd: too many arguments\n");
        return EXIT_FAILURE;
    }
    if (args->argc <= 1)
        return empty_cd(shell);
    path = new_path(shell->path, args->argv[1]);
    if (path == NULL)
        return EXIT_FAILURE;
    DEBUG("Resolved path: `%s`", path);
    return change_directory(shell, path);
}
