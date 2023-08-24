#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "args.h"

static
args_t *args_init(void)
{
    args_t *args = malloc(sizeof(*args));

    if (args == NULL)
        return NULL;
    *args = (args_t){
        .argc = 0,
        .argv = NULL,
    };
    return args;
}

void args_free(args_t *args)
{
    if (args == NULL)
        return;
    if (args->argv != NULL) {
        for (uint32_t i = 0; i < args->argc; i++)
            if (args->argv[i] != NULL)
                free(args->argv[i]);
        free(args->argv);
    }
    free(args);
}

static
uint32_t get_argc(parser_t *parser)
{
    uint32_t size = 0;
    char *token = NULL;

    parser_reset_ptr(parser);
    do {
        size++;
        token = parser_next_token(parser);
        if (token == NULL)
            free(token);
    } while (token != NULL);
    parser_reset_ptr(parser);
    return size - 1;
}

static
void set_argv(parser_t *parser, args_t *args)
{
    args->argc = get_argc(parser);
    args->argv = malloc(sizeof(*(args->argv)) * args->argc);
    if (args->argv == NULL)
        return;
    parser_reset_ptr(parser);
    for (uint32_t i = 0; i < args->argc; i++)
        args->argv[i] = parser_next_token(parser);
}

args_t *get_args(char *input)
{
    args_t *args = NULL;
    parser_t *parser = NULL;

    if (input == NULL)
        return NULL;
    args = args_init();
    if (args == NULL)
        return NULL;
    parser = parser_init(input);
    if (parser == NULL) {
        args_free(args);
        return NULL;
    }
    set_argv(parser, args);
    return args;
}
