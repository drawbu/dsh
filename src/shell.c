#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "shell.h"
#include "args.h"
#include "status.h"
#include "env.h"
#include "debug.h"

static
input_t *input_init(void)
{
    input_t *input = malloc(sizeof(*input));

    if (input == NULL)
        return NULL;
    *input = (input_t){
        .input = NULL,
        .len = 0,
        .args = NULL,
    };
    return input;
}

static
void input_free(input_t *input)
{
    if (input == NULL)
        return;
    if (input->input != NULL)
        free(input->input);
    if (input->args != NULL)
        args_free(input->args);
    free(input);
}

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
void debug_shell(input_t *input DEBUG_USED, args_t *args DEBUG_USED)
{
    DEBUG("input text: `%s`", input->input);
    DEBUG("input len:  %lu", input->len);
    for (uint32_t i = 0; i < args->argc; i++)
        DEBUG("argv[%d]: `%s`", i, args->argv[i]);
}

static
void set_input(shell_t *shell, char *input, size_t len)
{
    input_t *in = shell->input;
    args_t *args = in->args;

    if (args != NULL)
        args_free(args);
    if (in->input != NULL)
        free(in->input);
    *in = (input_t){
        .input = input,
        .len = len,
        .args = NULL,
    };
    if (in->len > 0 && in->input[in->len - 1] == '\n')
        in->input[--(in->len)] = '\0';
    in->args = get_args(input);
}

int process_input(shell_t *shell)
{
    input_t *input = shell->input;
    args_t *args = input->args;

    debug_shell(input, args);
    if (strcmp(args->argv[0], "exit") == 0)
        shell->is_running = false;
    return 0;
}

static
int shell_prompt(shell_t *shell)
{
    static size_t offset = 0;
    size_t len = 0;
    char *input = NULL;

    printf("> ");
    len = getline(&input, &offset, stdin);
    if (len == (size_t)-1) {
        free(input);
        shell->is_running = false;
        return 0;
    }
    set_input(shell, input, len);
    return process_input(shell);
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
