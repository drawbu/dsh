#include "args.h"
#include "shell.h"

int builtin_exit(shell_t *shell, args_t *args)
{
    if (args != NULL && args->argc > 1)
        return 1;
    shell->is_running = false;
    return 0;
}
