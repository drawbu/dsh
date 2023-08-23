#ifndef ARGS_H
    #define ARGS_H

    #include <stdlib.h>

typedef struct {
    char **argv;
    size_t argc;
} args_t;

args_t *get_args(char *input);

#endif /* ARGS_H */