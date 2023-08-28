#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "shell.h"

static
void echo_args(args_t *args)
{
    if (args == NULL || args->argc < 2)
        return;
    for (uint32_t i = 1; i < args->argc - 1; i++)
        printf("%s ", args->argv[i]);
    printf("%s", args->argv[args->argc - 1]);
}

int builtin_echo(shell_t *shell)
{
    echo_args(shell->input->args);
    printf("\n");
    return EXIT_SUCCESS;
}
