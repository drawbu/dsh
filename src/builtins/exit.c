#include <stdlib.h>

#include "args.h"
#include "shell.h"

int builtin_exit(shell_t *shell)
{
    args_t *args = shell->input->args;

    if (args != NULL && args->argc > 1)
        return EXIT_FAILURE;
    shell->is_running = false;
    return EXIT_FAILURE;
}
