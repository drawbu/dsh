#ifndef BUILTINS_H
    #define BUILTINS_H

    #include <stdint.h>

    #include "args.h"
    #include "shell.h"

    #define BUILTINS_COUNT ((uint32_t)2)

int builtin_exit(shell_t *shell);
int builtin_echo(shell_t *shell);

typedef struct {
    const char *name;
    int (*func)(shell_t *);
} builtin_t;

static
const builtin_t BUILTINS_TABLE[BUILTINS_COUNT] = {
    {"exit", &builtin_exit},
    {"echo", &builtin_echo},
};

#endif /* BUILTINS_H */
