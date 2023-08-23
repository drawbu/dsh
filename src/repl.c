#include <stdlib.h>
#include <stdio.h>

#include "repl.h"
#include "status.h"

static
shell_t *shell_init(void)
{
    shell_t *shell = malloc(sizeof(*shell));

    if (shell == NULL)
        return NULL;
    shell->is_running = true;
    return shell;
}

static
void shell_free(shell_t *shell)
{
    free(shell);
}

int repl_prompt(shell_t *shell)
{
    printf(">\n");
    shell->is_running = false;
    return 0;
}

status_t repl_run(void)
{
    shell_t *shell = shell_init();

    if (shell == NULL)
        return FAILURE;
    while (shell->is_running) {
        repl_prompt(shell);
    }
    shell_free(shell);
    return SUCCESS;
}