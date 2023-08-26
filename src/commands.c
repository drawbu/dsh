#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "shell.h"
#include "args.h"
#include "builtins.h"
#include "debug.h"

int execute_cmd(shell_t *shell)
{
    args_t *args = shell->input->args;

    if (args == NULL || args->argc < 1)
        return EXIT_SUCCESS;
    for (uint32_t i = 0; i < BUILTINS_COUNT; i++) {
        if (strcmp(BUILTINS_TABLE[i].name, args->argv[0]) == 0) {
            DEBUG("Builtin cmd: %s", BUILTINS_TABLE[i].name);
            return (*BUILTINS_TABLE[i].func)(shell);
        }
    }
    fprintf(stderr, "%s: not found\n", args->argv[0]);
    return EXIT_FAILURE;
}
