#ifndef ARGS_H
    #define ARGS_H

    #include <stdlib.h>

typedef struct {
    char **argv;
    size_t argc;
} args_t;

typedef struct {
    char *input;
    char *ptr;
} parser_t;

args_t *get_args(char *input);
parser_t *parser_init(char *input);
void parser_free(parser_t *parser);
char *parser_next_token(parser_t *parser);

#endif /* ARGS_H */