#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "shell.h"
#include "args.h"
#include "status.h"
#include "env.h"
#include "debug.h"
#include "builtins.h"

shell_t *shell_init(char **envp)
{
    shell_t *shell = malloc(sizeof(*shell));

    if (shell == NULL)
        return NULL;
    *shell = (shell_t){
        .input = input_init(),
        .env = env_init(envp),
        .is_running = true,
    };
    if (shell->input == NULL || shell->env == NULL) {
        shell_free(shell);
        return NULL;
    }
    return shell;
}

void shell_free(shell_t *shell)
{
    if (shell == NULL)
        return;
    input_free(shell->input);
    env_free(shell->env);
    free(shell);
}

static DEBUG_USED
void debug_shell(shell_t *shell)
{
    input_t *in = shell->input;
    args_t *args = in->args;

    DEBUG("input text: `%s`", in->input);
    DEBUG("input len:  %lu", in->len);
    if (args != NULL)
        for (uint32_t i = 0; i < args->argc; i++)
            DEBUG("argv[%d]: `%s`", i, args->argv[i]);
}

static
int shell_prompt(shell_t *shell)
{
    static size_t offset = 0;
    size_t len = 0;
    char *input = NULL;

    show_prompt();
    len = getline(&input, &offset, stdin);
    if (len == (size_t)-1) {
        free(input);
        return builtin_exit(shell, NULL);
    }
    input_set(shell, input, len);
    debug_shell(shell);
    return input_process(shell);
}

status_t shell_run(char **envp)
{
    shell_t *shell = shell_init(envp);

    if (shell == NULL)
        return FAILURE;
    while (shell->is_running) {
        shell_prompt(shell);
    }
    shell_free(shell);
    return SUCCESS;
}
