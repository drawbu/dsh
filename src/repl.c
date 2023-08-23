#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "repl.h"
#include "status.h"

static
shell_t *shell_init(void)
{
    shell_t *shell = malloc(sizeof(*shell));

    if (shell == NULL)
        return NULL;
    shell->input = malloc(sizeof(*(shell->input)));
    if (shell->input == NULL) {
        free(shell);
        return NULL;
    }
    shell->is_running = true;
    return shell;
}

static
void shell_free(shell_t *shell)
{
    free(shell->input->input);
    free(shell->input);
    free(shell);
}

int repl_prompt(shell_t *shell)
{
    static size_t offset = 0;
    input_t *input = shell->input;

    printf("> ");
    input->len = getline(&(input->input), &offset, stdin);
    if (input->len == (size_t)-1 || strcmp(input->input, "exit") == 0) {
        shell->is_running = false;
        return 0;
    }
    input->input[input->len - 1] = '\0';
    printf("input: '%s'\nlen:  %lu\n", input->input, input->len);
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