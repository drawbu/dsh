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
    if (path == NULL) {
        fprintf(stderr, "cd: malloc of new path failed\n");
        return EXIT_FAILURE;
    }
    DEBUG("Changing dir: `%s`", path);
    setenv("OLDPWD", shell->path, true);
    strcpy(shell->path, path);
    chdir(shell->path);
    setenv("PWD", shell->path, true);
    return EXIT_SUCCESS;
}

static
char *concat_path(char *pwd, char *target, char new_path[PATH_MAX])
{
    char *path;

    switch (target[0]) {
        case '/':
            break;
        case '~':
            path = getenv("HOME");
            if (path == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return NULL;
            }
            strcpy(new_path, path);
            target++;
            break;
        case '-':
            if (strlen(target) > 1) {
                fprintf(stderr, "cd: %s: invalid argument\n", target);
                return NULL;
            }
            path = getenv("OLDPWD");
            if (path == NULL) {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return NULL;
            }
            strcpy(new_path, path);
            return new_path;
        default:
            strcpy(new_path, pwd);
            if (pwd[strlen(pwd) - 1] != '/')
                strcat(new_path, "/");
            break;
    }
    strcat(new_path, target);
    return new_path;
}

static
int empty_cd(shell_t *shell)
{
    char *home = getenv("HOME");

    if (home == NULL) {
        fprintf(stderr, "cd: HOME not set\n");
        return EXIT_FAILURE;
    }
    return change_directory(shell, home);
}

static
int process_path(shell_t *shell, char *target)
{
    char merged_path[PATH_MAX] = {0};
    char resolved_path[PATH_MAX] = {0};

    if (concat_path(shell->path, target, merged_path) == NULL)
        return EXIT_FAILURE;
    DEBUG("New path: `%s`", merged_path);
    if (realpath(merged_path, resolved_path) == NULL) {
        fprintf(stderr, "cd: %s: No such file or directory\n", merged_path);
        return EXIT_FAILURE;
    }
    DEBUG("Resolved path: `%s`", resolved_path);
    return change_directory(shell, resolved_path);
}

int builtin_cd(shell_t *shell)
{
    args_t *args = shell->input->args;

    if (args->argc >= 3) {
        fprintf(stderr, "cd: too many arguments\n");
        return EXIT_FAILURE;
    }
    if (args->argc <= 1)
        return empty_cd(shell);
    return process_path(shell, args->argv[1]);
}
