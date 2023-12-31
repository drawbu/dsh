#include <stdlib.h>

#include "shell.h"
#include "args.h"
#include "builtins.h"

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

void input_set(shell_t *shell, char *input, size_t len)
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
    in->args = args_get(input);
}
